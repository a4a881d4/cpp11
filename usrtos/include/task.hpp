#pragma once

#include <cpblock.hpp>

namespace usrtos {
enum CBMode { 
	  nocallback = 0 ///< no callback
	, noarg			 ///< no callback args
	, abss			 ///< delay callback->delay us then exec immediately
	, me 			 ///< relative ref delay callback->delay us
	, obj 			 ///< add callback->delay us
	, extwait 		 ///< dec the related task, if cnt == 0, wake up it
	, extmulti 		 ///< emit multi task
};

typedef struct __callback_argv {
	enum CBMode mode;
	int delay;		 ///< delay us
	int cnt;		 ///< callback count, if count>0, emit callback task 
	int argv;
	void *workers; 	 ///< point to the workers
	CPBlock::GP gp;	 ///< callback task gp
} _callback_argv_t;

class CallBackArgv : public __callback_argv {
public:
	CallBackArgv *setMode(enum CBMode m) {
		mode = m;
		return this;
	};
	
	CallBackArgv *setDelay(int d) {
		delay = d;
		return this;
	};
	
	CallBackArgv *setCnt(int c) {
		cnt = c;
		return this;
	};
	CallBackArgv *setGP(CPBlock::GP& t) {
		gp = t;
		return this;
	};
	CPBlock::GP& getGP() {
		return gp;
	};
};
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

	CallBackArgv *getCallBackArgv() {
		return (CallBackArgv *)&callbackargv;
	};
};

struct taskdependen {
	CPBlock::GP me;
	umutex depend_mutex;
	int count;
};
};