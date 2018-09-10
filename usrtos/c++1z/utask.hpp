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
	utask(UsrtWorkers *w) : m_workers(&w) {};
	utask *newTask(uuid key, CPBlock::GP& gpTask) {
		auto t = m_workers->tQueue();
		pTask = WorkerHelper::newUserTask(m_workers,gpTask)
			-> setID(1)
			-> setKey(key)
			-> setReady(t->now())
			-> setDeadline(t->after(100))
			-> setValid(t->after(200));
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
		if(!t->insert(pTask))
			std::cerr << "emit task failure" << std::endl;
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
	utask *nextTask(uuid key, uuid cbkey) {
		utask *t = new utask(m_workers);
		this->setCallBack(cbkey, CMode::abss, 1, 1)
		t->newTask(key,pTask->callback_argv()->getArgv());

	}
}; // class pytask
}; // namespace usrtos

/*
callback chain
A->B->C->D->E
agp->callback_argv->gp
*/