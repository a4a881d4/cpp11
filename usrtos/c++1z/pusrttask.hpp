#pragma once

#include <workerhelper.hpp>

namespace usrtos {

class UsrtTaskAPI {
private:
	UsrtWorkers *m_workers;
public:
	UsrtTaskAPI(std::string dir) {
		m_workers = new UsrtWorkers(dir.c_str());
	};
	
	std::string getKey(std::string capName) {
		uuid key = WorkerHelper::cap2key(std::string("cap") + capName);
		std::string ks = UsrtKey::key2string(key);
		return ks;
	};

	void HelloWorld() {
		CPBlock::GP gpTask;
		auto t = m_workers->tQueue();
		task *pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			->setKey(WorkerHelper::cap2key(std::string("capWorkersHelloWorld")))
			-> setReady(t->now())
			-> setDeadline(t->after(10))
			-> setValid(t->after(20));
		if(!t->insert(pTask))
			std::cerr << "insert HelloWorld failure" << std::endl;
	};

	void byKeyInt(std::string key, int k) {
		CPBlock::GP gpTask;
		task *pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(UsrtKey::string2key(key));
		CPBlock::GP& argv = pTask->getArgv();
		int *a = m_workers->m_memory->newGP<int>(argv);
		*a = k;
		m_workers->m_configFifo->push<task>(pTask);
	};
	
	void byKeyKey(std::string key, std::string k) {
		CPBlock::GP gpTask;
		task *pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(UsrtKey::string2key(key));
		CPBlock::GP& argv = pTask->getArgv();
		uuid *a = m_workers->m_memory->newGP<uuid>(argv);
		*a = UsrtKey::string2key(k);
		m_workers->m_configFifo->push<task>(pTask);
	};
	void byKeyStr(std::string key, std::string k) {
		CPBlock::GP gpTask;
		task *pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(UsrtKey::string2key(key));
		CPBlock::GP& argv = pTask->getArgv();
		char *a = m_workers->m_memory->newGP<char>(argv,k.size()+16);
		strcpy(a,k.c_str());
		m_workers->m_configFifo->push<task>(pTask);
	};
	void byKey(std::string key) {
		CPBlock::GP gpTask;
		task *pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(UsrtKey::string2key(key));
		m_workers->m_configFifo->push<task>(pTask);
	};
};
};