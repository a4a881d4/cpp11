#define FUNCLASS capCallBackLunchTask
#include <capabilityAPI.hpp>
#include <usrtworker.hpp>
#include <workerhelper.hpp>
#include <glog.hpp>
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
			if(pTask == ref) {
				CPBlock::GP gp;
				pTask = WorkerHelper::copyUserTask(w,gp,*pTask);
				pTask->noE = w->tQueue()->after(callback->delay);
				pTask->noL = 0;
				pTask->callbackargv.gp = gp;
				WorkerHelper::pushTask(w,gp);
			} 
			else {
				pTask->noE = w->tQueue()->after(callback->delay);
				pTask->noL = 0;
				WorkerHelper::pushTask(w,callback->gp);
			}
		}
		break;
		case me: {
			pTask = w->G2L<task>(callback->gp);
			if(ref == nullptr) 
				ref=pTask;
			pTask->noE = w->tQueue()->after(ref->noE ,callback->delay);
			// pTask->noL = w->tQueue()->after(ref->noL ,callback->delay);
			WorkerHelper::pushTask(w,callback->gp);
		}
		break;
		case obj: {
			CPBlock::GP gp = callback->gp;
			pTask = w->G2L<task>(gp);
			if(pTask == ref) {
				pTask = WorkerHelper::copyUserTask(w,gp,*pTask);
				pTask->callbackargv.gp = gp;
			}
			pTask->noE += w->tQueue()->clock.micro_type(callback->delay);
			// pTask->noL += w->tQueue()->clock.micro_type(callback->delay);
			WorkerHelper::pushTask(w,gp);
		}
		break;
		case extwait: { // callback gp stored taskdependen gp
			CPBlock::GP *multi = w->G2L<CPBlock::GP>(callback->gp);
			size_t n = callback->gp.objsize/sizeof(CPBlock::GP);
			for(int i = 0;i < n;i++) {
				auto wait = w->G2L<struct taskdependen>(multi[i]);
				int cnt = 0;
				{
					USRT_SCOPED_LOCK(wait->depend_mutex);
					cnt = wait->count;
					wait->count--;
				}
				// stringstream s1;
				// s1 << cnt << " " << *wait;
				// string s = s1.str();
				// w->SYSLOG.put(s);
				if(cnt <= 1) {
					auto p = w->G2L<task>(wait->me);
					w->tQueue()->dumpTask(*p);
					WorkerHelper::pushTask(w,wait->me);
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
	SYSLOG = w->_SYSLOG;
	dealCallBack(w, callback, my );
	return 0;
}
#undef FUNCLASS



