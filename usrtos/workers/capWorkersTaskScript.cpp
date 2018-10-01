#define FUNCLASS capWorkersTaskScript
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <vm/opcodes.hpp>

using namespace usrtos;
static void handleTask(UsrtWorkers *w, task *t, structThread *my) {
	auto capKey = t->key;
	UsrtCapabilityBearer *bearer = w->getBearerByKey(capKey);
	if(bearer != nullptr) {
		try{
			bearer->runLP(
				static_cast<void*>
				(w->G2L<char>(t->argv))
				);
		} catch(usrtos_exception& e) {
			std::cout << "In handleTask:" << e.what() << std::endl;
			std::cout << dumpTask(t) << std::endl;
		}
		if(t->callbackargv.mode != CBMode::nocallback) {
			bearer = w->getBearerByKey(t->callback);
			if(bearer != nullptr) {
				my->monitor.callback++;
				t->callbackargv.workers = w;
				bearer->runLP(t);
			}
		}
	}
	else {
		std::string bug = std::string("bad cap key: ") + UsrtKey::key2string(capKey);
		my->workers->_SYSLOG.put(bug);
	}
};
static void handleScript(UsrtWorkers *w, task *t, structThread *my) {
	my->monitor.script++;
	vm::VMContext   *vmCtx   = new struct vm::VMContext(*w);
	vm::JITVisitor	*visitor = new struct vm::JITVisitor(*vmCtx);
	try {
		U8 *code = w->G2L<U8>(t->argv);
		vm::OperatorStream os(code,t->argv.objsize);
		vm::Decode d;
		d.run(*visitor,os,vm::RunMode::run);
	} catch(usrtos_exception& e) {
		std::cout << "In handleScript:" << e.what() << std::endl;
	}
};

int FUNCLASS::run( void *argv ) {
	struct structThread *my = (struct structThread *)argv;
	task *t = my->workers->m_taskq->pop();
	if(t != nullptr) {
		my->monitor.run++;
		if( t->ID[0] == (uint64_t)TaskType::script ) {
			handleScript(my->workers, t, my);
		} else {
			handleTask(my->workers, t, my);
		}
	}
	else /*if( my->id==0)*/ {
		my->monitor.keeper++;
		if(!(my->monitor.keeper&0xfffffff)) {
			std::stringstream s1;
			s1 << "In Thread (work cap)" << my->id << std::endl;
			std::string ks = s1.str();
			my->workers->_SYSLOG.put(ks);
		}
		my->state=KEEPER;
		UsrtCapabilityBearer *bearer = my->workers->getBearerByKey(my->workers->keeperKey);
		if(bearer != nullptr)
			bearer->runLP(my);
	}
	return 0;
};