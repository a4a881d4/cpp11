#pragma once

#include <cpblock.hpp>

namespace usrtos {
enum CBMode { nocallback = 0, noarg, abss, me, obj, extwait, extmulti };
typedef struct __callback_argv {
	enum CBMode mode;
	int delay;
	int cnt;
	int argv;
	void *workers;
	CPBlock::GP gp;
	
	__callback_argv *setMode(enum CBMode m) {
		mode = m;
		return this;
	};
	
	__callback_argv *setDelay(int d) {
		delay = d;
		return this;
	};
	
	__callback_argv *setCnt(int c) {
		cnt = c;
		return this;
	};

	CPBlock::GP& getArgv() {
		return gp;
	};

} _callback_argv_t;

struct task {
	int64 ID;
	uuid key;
	CPBlock::GP argv;
	uuid callback;
	_callback_argv_t callbackargv;
	utime_t noE;
	utime_t noL;
	utime_t valid;

	task *setID(int64 id) {
		ID = id;
		return this;
	};

	task *setKey(uuid k) {
		key = k;
		return this;
	};

	task *setArgv(CPBlock::GP& a) {
		memcpy(&argv,&a,sizeof(CPBlock::GP));
		return this;
	};
	
	CPBlock::GP& getArgv() {
		return argv;
	};
	
	task *setCallBack(uuid k) {
		callback = k;
		return this;
	};
	
	task *setReady(utime_t noe) {
		noE = noe;
		return this;
	};

	task *setDeadline(utime_t nol) {
		noL = nol;
		return this;
	};

	task *setValid(utime_t v) {
		valid = v;
		return this;
	};

	_callback_argv_t *getCallBackArgv() {
		return &callbackargv;
	};
};

struct taskdependen {
	CPBlock::GP me;
	umutex depend_mutex;
	int count;
};
};