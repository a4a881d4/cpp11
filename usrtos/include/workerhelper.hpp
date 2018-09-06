#pragma once
#include <usrtworker.hpp>

using namespace usrtos;

struct WorkerHelper {

	static void pushTask(UsrtWorkers *w, CPBlock::GP& t) {
		UsrtWorkers::UsrtTask *q = w->tQueue();
		task *pTask = w->G2L<task>(t);
		if(pTask != nullptr) {
			CPBlock::GP gp;
			task *vgp = q->tm->newGP<task>(gp);
			memcpy( vgp, pTask, sizeof(task) );
			q->insert(vgp);
		}
	};

};