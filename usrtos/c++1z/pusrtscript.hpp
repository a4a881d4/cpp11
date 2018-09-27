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
public:
	UsrtScript(std::string dir, size_t bufSize) {
		len = bufSize;
		m_workers = new UsrtWorkers(dir.c_str());
		reset();
	};
	void push() {
		m_workers->m_configFifo->push<task>(pTask);
	};
	void reset() {
		uuid key = WorkerHelper::cap2key(std::string("capWorkersScript"));
		pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(key);
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