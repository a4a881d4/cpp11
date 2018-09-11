#pragma once

#include <workerhelper.hpp>
#include "utask.hpp"
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

	CPBlock::GP newTask(std::string key, size_t argSize) {
		CPBlock::GP gpTask;
		auto t = m_workers->tQueue();
		task *pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(1)
			-> setKey(UsrtKey::string2key(key))
			-> setReady(t->now())
			-> setDeadline(t->after(100))
			-> setValid(t->after(200));
		CPBlock::GP& argv = pTask->getArgv();
		char *a = m_workers->m_memory->newGP<char>(argv,argSize);
		return gpTask;
	};

	void setCallBack( CPBlock::GP& gp
					, std::string cbkey
					, int mode
					, int cnt
					, int delay
					, CPBlock::GP& cbgp
					) {
		auto t = m_workers->tQueue();
		task *pTask = t->tm->getMem()->GP2LP<task>(gp);
		if(pTask == nullptr) {
			std::cerr << "task memory gp to lp failure: in setCallBack" << std::endl;
			return;
		}
		pTask->setCallBack(UsrtKey::string2key(cbkey))
			 ->getCallBackArgv()
			 ->setMode(static_cast<CBMode>(mode))
			 ->setCnt(cnt)
			 ->setDelay(delay)
			 ->setGP(cbgp);
	};

	void emitTask(CPBlock::GP& gp) {
		auto t = m_workers->tQueue();
		task *pTask = t->tm->getMem()->GP2LP<task>(gp);
		if(pTask == nullptr) {
			std::cerr << "task memory gp to lp failure: in emitTask" << std::endl;
			return;
		}
		std::string ks = UserHelper::dumpMem(static_cast<void*>(pTask),sizeof(task));
			std::cout << " emit " << std::endl 
					  << ks << std::endl;
		ks = UserHelper::dumpMem(static_cast<void*>(&gp),sizeof(CPBlock::GP));
			std::cout << " gp " << std::endl 
					  << ks << std::endl;
		if(!t->insert(pTask))
			std::cerr << "emit task failure" << std::endl;
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

	void allocMulti(CPBlock::GP& gpTask, size_t n) {
		auto t = new utask(m_workers);
		m_workers->m_memory->newGP<CPBlock::GP>(t->attach(gpTask)->eximultiGP(),n);
	};

	void setMulti(CPBlock::GP& gpTask, CPBlock::GP& other, size_t i) {
		auto t = new utask(m_workers);
		CPBlock::GP *multi = m_workers->G2L<CPBlock::GP>(t->attach(gpTask)->eximultiGP());
		multi[i] = other;
	};

	void setIntArgv(CPBlock::GP& gpTask, int k) {
		auto t = new utask(m_workers);
		int *a = m_workers->G2L<int>(m_workers->G2L<task>(gpTask)->getArgv());
		*a = k;
	};
};
};