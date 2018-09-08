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

	static task *newConfig(UsrtWorkers *w, std::string capName) {
		CPBlock::GP mem;
		task *pTask = newConfigTask(w,mem)
			->setID(0)
			->setKey(cap2key(std::string("capWorkers")+capName))
			;
		return pTask;
	};

	static uuid meta2uuid(const CapabilityMeta& m) {
		sha1 sha;
		UsrtKey::strn2sha1((char *)(&m),sha,32+32+40);
		string s1 = UsrtKey::sha2string(sha);
		sha1Set(m.sha1,s1.c_str());
		uuid id = UsrtKey::str2key(s1);
		return id;
	};

	static uuid cap2key(std::string capname) {
		struct CapabilityMeta keeper_meta;
		memset(&keeper_meta,0,sizeof(struct CapabilityMeta));
		strncpy((char *)keeper_meta.name,capname.c_str(),31);
		strncpy((char *)keeper_meta.type,"USRTOS",32);
		keeper_meta.version = sha1str(USRTOSVERSION);
		uuid key = WorkerHelper::meta2uuid(keeper_meta);
		return key;
	};
};