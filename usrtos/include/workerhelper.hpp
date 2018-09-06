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