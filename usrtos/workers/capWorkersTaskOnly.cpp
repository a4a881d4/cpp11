#define FUNCLASS capWorkersTaskOnly
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>

using namespace usrtos;

int FUNCLASS::run( void *argv ) {
	struct structThread *my = (struct structThread *)argv;
	task *t = my->workers->m_taskq->pop();
	UsrtCapabilityBearer *bearer;
	if(t != nullptr) {
		my->monitor.run++;
		auto capKey = t->key;
		bearer = my->workers->getBearerByKey(capKey);
		if(bearer != nullptr) {
			bearer->runLP(
				static_cast<void*>
				(my->workers->G2L<char>(t->argv))
				);
			if(t->callbackargv.mode != CBMode::nocallback) {
				bearer = my->workers->getBearerByKey(t->callback);
				if(bearer != nullptr) {
					my->monitor.callback++;
					t->callbackargv.workers = my->workers;
					bearer->runLP(t);
				}
			}
		}
		else {
			std::string bug = std::string("bad cap key: ") + UsrtKey::key2string(capKey);
			my->workers->_SYSLOG.put(bug);
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
		bearer = my->workers->getBearerByKey(my->workers->keeperKey);
		if(bearer != nullptr)
			bearer->runLP(my);
	}
	return 0;
};
#undef FUNCLASS
