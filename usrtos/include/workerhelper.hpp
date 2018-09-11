#pragma once
#include <usrtworker.hpp>
#include <userhelper.hpp>

using namespace usrtos;

struct WorkerHelper {

	static void pushTask(UsrtWorkers *w, CPBlock::GP& t) { // must copy
		UsrtWorkers::UsrtTask *q = w->tQueue();
		task *pTask = w->G2L<task>(t);
		if(t.id == q->tm->getMem()->getKey()) {
			q->insert(pTask);
			return;
		};
		if(pTask != nullptr) {
			CPBlock::GP gp;
			task *vgp = q->tm->newGP<task>(gp);
			memcpy( vgp, pTask, sizeof(task) );
			q->insert(vgp);
		}
	};

	static task *newConfigTask(UsrtWorkers *w, CPBlock::GP& t) { // in m_configFifo
		task *pTask = w
			->m_configFifo
			->newGP<task>(t);

		if(pTask != nullptr) {
			memset(pTask,0,sizeof(task));
		}
		return pTask;
	};

	static task *newUserTask(UsrtWorkers *w, CPBlock::GP& t) { // in taskq->tm
		task *pTask = w
			->tQueue()
			->tm
			->newGP<task>(t);

		if(pTask != nullptr) {
			memset(pTask,0,sizeof(task));
		}
		return pTask;
	};

	static task *copyUserTask(UsrtWorkers *w, CPBlock::GP& t, task& src) { // in taskq->tm
		task *pTask = w
			->tQueue()
			->tm
			->newGP<task>(t);

		if(pTask != nullptr) {
			memcpy(pTask,&src,sizeof(task));
		}
		return pTask;
	};

	static task *newConfig(UsrtWorkers *w, std::string capName) {
		CPBlock::GP mem;
		task *pTask = newConfigTask(w,mem)
			->setID(0)
			->setKey(cap2key(std::string("capWorkers")+capName))
			;
		return pTask;
	};

	static uuid meta2uuid(const CapabilityMeta& m) {
		return UserHelper::meta2uuid(m);
	};

	static uuid cap2key(std::string capname) {
		return UserHelper::cap2key(capname);
	};
};