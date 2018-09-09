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
			-> setID(1)
			-> setKey(WorkerHelper::cap2key(std::string("capExamplesHelloWorld")))
			-> setReady(t->now())
			-> setDeadline(t->after(10))
			-> setValid(t->after(20));
		CPBlock::GP& argv = pTask->getArgv();
		int *a = m_workers->m_memory->newGP<int>(argv);
		*a = 0;
		if(!t->insert(pTask))
			std::cerr << "insert HelloWorld failure" << std::endl;
	};

	void byKeyInt(std::string key, int k) {
		CPBlock::GP gpTask;
		auto t = m_workers->tQueue();
		task *pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(1)
			-> setKey(UsrtKey::string2key(key))
			-> setReady(t->now())
			-> setDeadline(t->after(10))
			-> setValid(t->after(20));
		CPBlock::GP& argv = pTask->getArgv();
		int *a = m_workers->m_memory->newGP<int>(argv);
		*a = k;
		if(!t->insert(pTask))
			std::cerr << "insert by key with Int argv failure" << std::endl;
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
		auto t = m_workers->tQueue();
		task *pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(1)
			-> setKey(UsrtKey::string2key(key))
			-> setReady(t->now())
			-> setDeadline(t->after(10))
			-> setValid(t->after(20));
		CPBlock::GP& argv = pTask->getArgv();
		int alen = k.size()+16;
		char *a = m_workers->m_memory->newGP<char>(argv,alen);
		strncpy(a,k.c_str(),k.size()+1);
		if(!t->insert(pTask))
			std::cerr << "insert by key with String argv failure" << std::endl;
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