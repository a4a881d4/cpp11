#pragma once

#include <workerhelper.hpp>

namespace usrtos {

class UsrtConfig {
private:
	UsrtWorkers *m_workers;
public:
	UsrtConfig(std::string dir) {
		m_workers = new UsrtWorkers(dir.c_str());
	};
	
	std::string getWorkerKey(std::string capName) {
		uuid key = WorkerHelper::cap2key(std::string("capWorkers") + capName);
		std::string ks = UsrtKey::key2string(key);
		return ks;
	};

	std::string getKey(std::string capName) {
		uuid key = WorkerHelper::cap2key(capName);
		std::string ks = UsrtKey::key2string(key);
		return ks;
	};

	void HelloWorld() {
		task *pTask = WorkerHelper::newConfig(m_workers,std::string("HelloWorld"));
		m_workers->m_configFifo->push<task>(pTask);
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
		CPBlock::GP& argv = pTask->getArgv();
		char *a = m_workers->m_memory->newGP<char>(argv,16);
		m_workers->m_configFifo->push<task>(pTask);
	};
};
};