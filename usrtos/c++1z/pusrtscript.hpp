#pragma once

#include <workerhelper.hpp>
#include <vm/opcodes.hpp>
namespace usrtos {

class UsrtScript : public vm::EncodeStream {
private:
	UsrtWorkers *m_workers;
	CPBlock::GP gpTask;
	task *pTask;
	U8 *buf;
	size_t len;
	TaskType type;

public:
	UsrtScript(std::string dir, size_t bufSize) {
		len = bufSize;
		m_workers = new UsrtWorkers(dir.c_str());
		reset();
	};
	void push() {
		switch(type) {
			case TaskType::system: m_workers->m_configFifo->push<task>(pTask);
			case TaskType::script: m_workers->tQueue()->insert(pTask);
			default: throw(usrtos_exception("in puserscript: unsupport Task Type"));
		}
	};
	void reset() {
		uuid key = WorkerHelper::cap2key(std::string("capWorkersScript"));
		pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(type)
			-> setKey(key);
		CPBlock::GP& argv = pTask->getArgv();
		buf = m_workers->m_memory->newGP<U8>(argv,len);
		setBuf(buf,len);		
	};
	void newTask() {
		type = TaskType::system;
		reset();
	}
	void newScript() {
		type = TaskType::script;
		auto t = m_workers->tQueue();
		pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(type)
			-> setReady(t->now())
			-> setDeadline(t->after(100))
			-> setValid(t->after(200));
		CPBlock::GP& argv = pTask->getArgv();
		buf = m_workers->m_memory->newGP<U8>(argv,len);
		setBuf(buf,len);		
	};
	void test() {
		vm::OperatorStream os(buf,len);
		vm::JITVisitor visitor;
		vm::Decode d;
		d.run(visitor,os,vm::RunMode::show);
	};
};
};