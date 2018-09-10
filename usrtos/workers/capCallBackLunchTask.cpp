#define FUNCLASS capCallBackLunchTask
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <workerhelper.hpp>

using namespace usrtos;

static void dealCallBack(UsrtWorkers *w, _callback_argv_t *callback, task *ref )
{
	task *pTask;
	if(	 callback->mode != extwait 
		&& callback->mode != extmulti 
		&& callback->cnt  != -1 ) {
		  callback->cnt--;
		
    if(callback->cnt <= 0)
			return;
	}
	switch( callback->mode ) {
		case nocallback:
			break;
		case noarg: {
			WorkerHelper::pushTask(w,callback->gp);
		}
		break;
		case abss: {
			pTask = w->G2L<task>(callback->gp);
			pTask->noE = w->tQueue()->after(callback->delay);
			pTask->noL = pTask->noE;
			WorkerHelper::pushTask(w,callback->gp);
		}
		break;
		case me: {
			pTask = w->G2L<task>(callback->gp);
			if( ref==nullptr ) 
				ref=pTask;
			pTask->noE = ref->noE + micro_type(callback->delay);
			pTask->noL = ref->noL + micro_type(callback->delay);
			WorkerHelper::pushTask(w,callback->gp);
		}
		break;
		case obj: {
			pTask = w->G2L<task>(callback->gp);
			pTask->noE += micro_type(callback->delay);
			pTask->noL += micro_type(callback->delay);
			WorkerHelper::pushTask(w,callback->gp);
		}
		break;
		case extwait: {
			auto wait = w->G2L<struct taskdependen>(callback->gp);
			size_t n = callback->gp.objsize/sizeof(struct taskdependen);
			for(int i = 0;i < n;i++) {
				int cnt = 0;
				{
					uscoped_lock lock(wait[i].depend_mutex);
					cnt = wait[i].count;
					wait[i].count--;
				}
				if(cnt <= 1) {
					WorkerHelper::pushTask(w,wait[i].me);
				}
			}
		}
		break;
		case extmulti: {
			CPBlock::GP *multi = w->G2L<CPBlock::GP>(callback->gp);
			size_t n = callback->gp.objsize/sizeof(CPBlock::GP);
			
			for(int i = 0;i < n;i++)
				WorkerHelper::pushTask(w,multi[i]);
		}
		break;
		default:
		break;
	}
}
int FUNCLASS::run( void *argv ) {
	task *my = (task *)argv;
	_callback_argv_t *callback = &(my->callbackargv); 
	UsrtWorkers *w = (UsrtWorkers *)callback->workers;
	dealCallBack(w, callback, my );
	return 0;
}



