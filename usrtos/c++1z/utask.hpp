#pragma once

#include <workerhelper.hpp>

namespace usrtos {
class utask {
private:
	CPBlock::GP *pgpTask;
	task *pTask;
	UsrtWorkers *m_workers;
	utask *next;
public:
	utask(UsrtWorkers *w) : m_workers(w) {};
	utask *attach(CPBlock::GP& gpTask) {
		pgpTask = &gpTask;
		auto t = m_workers->tQueue();
		pTask = t->tm->getMem()->GP2LP<task>(gpTask);
		return this;
	};
	utask *newTask(uuid key, CPBlock::GP& gpTask) {
		auto t = m_workers->tQueue();
		pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(1)
			-> setKey(key);
		pgpTask = &gpTask;
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
	
}; // class pytask
}; // namespace usrtos

/*
callback chain
A->B->C->D->E
agp->callback_argv->gp
*/