#pragma once

#include <workerhelper.hpp>

namespace usrtos {
class utask {
private:
	CPBlock::GP m_gp;
	task *pTask;
	UsrtWorkers *m_workers;
	utask *next;
public:
	utask(UsrtWorkers *w) : m_workers(w) {};
	utask *attach(CPBlock::GP& gpTask) {
		m_gp = gpTask;
		auto t = m_workers->tQueue();
		pTask = t->tm->getMem()->GP2LP<task>(gpTask);
		return this;
	};
	utask *newTask(uuid key, CPBlock::GP& gpTask) {
		auto t = m_workers->tQueue();
		pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(1)
			-> setKey(key);
		m_gp = gpTask;
		return this;
	};
	
	template<typename T>
	T* getArgv(size_t N) {
		return m_workers
			->m_memory
			->newGP<T>(pTask->getArgv(),N);
	};
	
	void emitTask() {
		auto t = m_workers->tQueue();
		if(!t->insert(pTask))
			std::cerr << "emit task failure" << std::endl;
	};
	
	utask* setTime(   utime_t ready
					, utime_t deadline
					, utime_t valid
					) {
		pTask-> setReady(ready)
			 -> setDeadline(deadline)
			 -> setValid(valid);
		return this;
	};

	utask* immediately() {
		auto t = m_workers->tQueue();
		pTask-> setReady(t->now())
			 -> setDeadline(t->after(100))
			 -> setValid(t->after(200));
		return this;
	};

	utask* setCallBack( uuid cbkey
					, int mode
					, int cnt
					, int delay
					, CPBlock::GP& cbgp
					) {
		
		pTask->setCallBack(cbkey)
			 ->getCallBackArgv()
			 ->setMode(static_cast<CBMode>(mode))
			 ->setCnt(cnt)
			 ->setDelay(delay)
			 ->setGP(cbgp);

		return this;
	};

	CPBlock::GP& eximultiGP() {
		return pTask->getCallBackArgv()->gp;
	};
	
	CPBlock::GP allocDepend(int cnt) {
		CPBlock::GP gp;
		auto t = m_workers->m_memory->newGP<taskdependen>(gp);
		auto pm = new(&(t->depend_mutex)) umutex;
		pm->unlock();
		t->count = cnt;
		t->me = m_gp;
		cout << "depend: " << endl;
		cout << *t << endl;
		cout << endl;
		return gp; 
	};

	string dumpTask() {
		stringstream s1;
		s1 << pTask->key << " " << pTask->ID << endl
		   << "noE: " << pTask->noE << " "
		   << "noL: " << pTask->noL << " "
		   << "valid: " << pTask->valid
		   << endl 
		   << "callback" << endl
		   << *(pTask->getCallBackArgv()) << " "
		   << endl
		   << "GP" << endl
		   << "id: " << m_gp.id  << " "
		   << "off: " << m_gp.offset << " "
		   << "sz: " << m_gp.objsize << " "
		   << endl;

		return s1.str();  
	};
}; // class utask
}; // namespace usrtos

/*
callback chain
A->B->C->D->E
agp->callback_argv->gp
*/