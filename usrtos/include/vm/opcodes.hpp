#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "BasicTypes.h"
#include <vector>
#include <tuple>

using namespace boost::uuids;
#include <gp.hpp>

namespace usrtos{ namespace vm{
typedef uuid UUID;

enum opnum { 
	  zero = 0
	, unary
	, binary
	, ternary 
};

enum optype { 
	  sz08
	, sz16
	, sz32
	, sz64
};

enum opcode {
	  setseg = 0x02 ///< binary op1 -> segnum<sz08>, op2 -> uuid<sz08> 
	, allocm = 0x10 ///< ternary op1 -> segnum<sz08>, op2 -> regnum<sz08> 
					///< op3 -> size<sz08,sz16,sz32,sz64>
	, savegp = 0x20 ///< gp
					///< ternary op1(address) -> regnum<sz08>, op2 -> offset<sz08,sz16> 
					///< op3(content) -> regnum<sz08>
					///< binary op1(address) -> regnum<sz08>, op2(content) -> regnum<sz08>
	, savelp = 0x21 ///< lp	same as above			
	, savevl = 0x22 ///< vl	same as above			
	, loadgp = 0x30 ///< gp
					///< ternary op1(address) -> regnum<sz08>, op2 -> offset<sz08,sz16> 
					///< op3(dest) -> regnum<sz08>
					///< binary op1(address) -> regnum<sz08>, op2(dest) -> regnum<sz08>
	, loadlp = 0x31 ///< lp	same as above			
	, loadvl = 0x32 ///< vl	same as above			
	, immeid = 0x40 ///< gp.id binary op1 -> regnum<sz08>, op2 -> uuid<sz08>
	, immeos = 0x41 ///< gp.offset binary op1 -> regnum<sz08>, op2 -> offset<sz08,sz16,sz32,sz64>
	, immesz = 0x42 ///< gp.objsize binary op1 -> regnum<sz08>, op2 -> size<sz08,sz16,sz32,sz64>
	, immevl = 0x43 ///< value binary op1 -> regnum<sz08>, op2 -> size<sz08,sz16,sz32,sz64>
	, immelp = 0x44 ///< lp binary op1 -> regnum<sz08>, op2 -> void *<sz64>
	, selfrd = 0x50 ///< unary op1 -> regnum<sz08>
					///< binary op1 -> regnum<sz08>, op2 -> offset<sz08,sz16,sz32,sz64>
	, selfwr = 0x51 ///< unary op1 -> regnum<sz08>
					///< binary op1 -> regnum<sz08>, op2 -> offset<sz08,sz16,sz32,sz64>
	, selfst = 0x52 ///< unary op1 -> regnum<sz08>
					///< binary op1 -> regnum<sz08>, op2 -> offset<sz08,sz16,sz32,sz64>
	, selfgl = 0x53 ///< unary op1 -> regnum<sz08>
	, selflg = 0x54 ///< unary op1 -> regnum<sz08>
	, pushof = 0x60 ///< unary op1 -> regnum<sz08>
	, callcp = 0x70 ///< binary op1(argv) -> regnum<sz08>, op2 -> uuid<sz08>
	, movegp = 0x80 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
	, movelp = 0x81 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
	, movevl = 0x82 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
	, movesz = 0x83 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
};

#define BINARY(A,B) \
	{	\
		A op1; \
		B op2; \
		op1 = is.get<A>(); \
		op2 = is.get<B>(); \
		visitor.name(op1,op2); \
	};

#define NULLARY(A) {};
#define UNARY(A) {};
#define TERNARY(A,B,C) {};

#define ENUM_CONTROL_OPERATORS(visitOp)

#define ENUM_PARAMETRIC_OPERATORS(visitOp)

#define ENUM_NONCONTROL_NONPARAMETRIC_OPERATORS(visitOp) \
	visitOp(0x01,nop,"nop",NULLARY(none)) \
	\
	visitOp(0x02,setseg,"set_segment",BINARY(U8,UUID),std::tuple<U8,UUID>) \
	\
	visitOp(0x10,allocm,"alloc_memory",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	\
	visitOp(0x20,savegp,"save_globe_pointer",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	visitOp(0x21,savelp,"save_local_pointer",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	visitOp(0x22,savevl,"save_value",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	\
	visitOp(0x30,loadgp,"load_globe_pointer",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	visitOp(0x31,loadlp,"load_local_pointer",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	visitOp(0x32,loadvl,"load_value",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	\
	visitOp(0x40,immeid,"set_gp.id",BINARY(U8,UUID),std::tuple<U8,UUID>) \
	visitOp(0x41,immeos,"set_gp.offset",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x42,immesz,"set_gp.objsize",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x43,immevl,"set_value",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x44,immelp,"set_local_pointer",BINARY(U8,U64),std::tuple<U8,U64>) \
	\
	visitOp(0x50,selfrd,"self_read",UNARY(U8),std::tuple<U8>) \
	visitOp(0x51,selfwr,"self_write",UNARY(U8),std::tuple<U8>) \
	visitOp(0x52,selfst,"self_set",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x53,selfgl,"self_gp2lp",UNARY(U8),std::tuple<U8>) \
	visitOp(0x54,selflg,"self_lp2gp",UNARY(U8),std::tuple<U8>) \
	\
	visitOp(0x58,selfro,"self_read_with_offset",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x59,selfwo,"self_write_with_offset",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x5a,selfso,"self_set_with_offset",TERNARY(U8,U8,ANYTYPE)) \
	\
	visitOp(0x60,pushof,"push_offset_tofifo",UNARY(U8),std::tuple<U8>) \
	\
	visitOp(0x70,callcp,"call_capability",BINARY(U8,UUID),std::tuple<U8,UUID>) \
	\
	visitOp(0x80,movegp,"move_gp_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x81,movelp,"move_lp_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x82,movevl,"move_vl_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x83,movesz,"move_sz_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x84,moveal,"move_all_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) 
	
		
#define ENUM_NONCONTROL_OPERATORS(visitOp) \
	ENUM_PARAMETRIC_OPERATORS(visitOp) \
	ENUM_NONCONTROL_NONPARAMETRIC_OPERATORS(visitOp)

#define ENUM_OPERATORS(visitOp) \
	ENUM_NONCONTROL_OPERATORS(visitOp) \
	ENUM_CONTROL_OPERATORS(visitOp)

enum class Opcode : U8 {
	#define VISIT_OPCODE(opcode,name,...) name = opcode,
	ENUM_OPERATORS(VISIT_OPCODE)
	#undef VISIT_OPCODE
	undefined = 0x00
};	

inline const char* asString(Opcode c) {
	switch(c) {
		#define VISIT_OPCODE(opcode,name,str,...) case Opcode::name: return str;
		ENUM_OPERATORS(VISIT_OPCODE)
		#undef VISIT_OPCODE
		default: return "undefined";
	}
};

enum class segment : U8 {
	  task = 0x0
	, temp = 0x1
	, log0 = 0x2
	, log1
	, log2
	, log3
	, log4
	, log5
};

struct OperatorStream {
	OperatorStream(const std::vector<U8>& codeBytes)
	: nextByte(codeBytes.data()), end(codeBytes.data()+codeBytes.size()) {}

	operator bool() const { return nextByte < end; }

	U8* advance(size_t n) {
		U8* r = nextByte;
		nextByte += n;
		if(nextByte > end) {
			std::cout << "Decoder Stream overflow" << std::endl;
		}
		return r;
	};

private:

	const U8* nextByte;
	const U8* end;
};

struct VMContext {
	VMContext(UsrtWorkers& w) {
		workers = &w;
		rfile.setSeg(segment::task, w->getMemKey("taskq"))
			 .setSeg(segment::temp, w->getMemKey("memory"))
			 .setSeg(segment::log0, w->getMemKey("log0"))
			 .setSeg(segment::log1, w->getMemKey("log1"))
			 .setSeg(segment::log2, w->getMemKey("log2"))
			 .setSeg(segment::log3, w->getMemKey("log3"))
			 .setSeg(segment::log4, w->getMemKey("log4"))
			 .setSeg(segment::log5, w->getMemKey("log5"))
			 ;
	};
	RegFile<256> rfile;
	UsrtWorkers *workers;
};

struct JITVisitor {
	JITVisitor(VMContext& uctx) : ctx(&uctx) {};
	void nop() {};
	void setseg(U8 s, UUID& id) {
		ctx->rfile.setSeg(s,id);
	};
	void allocm(U8 s, U8 r, size_t sz) {
		if(ctx->rfile.mem[s] == nullptr) {
			ctx->rfile.mem[s] = ctx->workers
				.bindBlockByKey<Layout::UsrtMem>(
					ctx->rfile.seg[i]);
		}
		Reg& R = ctx->rfile.reg[r];
		R.lp = ctx->rfile.mem[s]
			->newGP<char>(R.gp,sz,16);
	};
	void loadgp(U8 ra, U8 rb, size_t offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		char *p = (static_cast<char*>Rb.lp)+offset;
		memcpy(&(Ra.gp),p,sizeof(Ra.gp));
	};
	void loadlp(U8 ra, U8 rb, size_t offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		char *p = (static_cast<char*>Rb.lp)+offset;
		memcpy(&(Ra.lp),p,sizeof(void*));
	};
	void loadvl(U8 ra, U8 rb, size_t offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		char *p = (static_cast<char*>Rb.lp)+offset;
		auto sz = Ra.value.size();
		memcpy(Ra.value.pvalue,p,sz);
	};
	void savegp(U8 ra, U8 rb, size_t offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		char *p = (static_cast<char*>Rb.lp)+offset;
		memcpy(p,&(Ra.gp),sizeof(Ra.gp));
	};
	void savelp(U8 ra, U8 rb, size_t offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		char *p = (static_cast<char*>Rb.lp)+offset;
		memcpy(p,&(Ra.lp),sizeof(void*));
	};
	void savevl(U8 ra, U8 rb, size_t offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		char *p = (static_cast<char*>Rb.lp)+offset;
		auto sz = Ra.value.size();
		memcpy(p,Ra.value.pvalue,sz);
	};
	void immeid(U8 r, UUID id) {
		Reg& R = ctx->rfile.reg[r];
		R.gp.id = id;
	};
	void immeos(U8 r, size_t off) {
		Reg& R = ctx->rfile.reg[r];
		R.gp.offset = off;
	};
	void immesz(U8 r, size_t sz) {
		Reg& R = ctx->rfile.reg[r];
		R.gp.objsize = sz;
	};
	void immevl(U8 r, ANYTYPE v) {
		Reg& R = ctx->rfile.reg[r];
		R.value(v);
	};
	void immelp(U8 r, size_t p) {
		Reg& R = ctx->rfile.reg[r];
		R.lp = static_cast<void*>(p);
	};
	void selfrd(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		auto sz = R.value.size();
		memcpy(R.value.pvalue,R.lp,sz);	
	};
	void selfwr(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		auto sz = R.value.size();
		memcpy(R.lp,R.value.pvalue,sz);	
	};
	void selfst(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		auto pm = new(R.lp) umutex;
		pm->unlock();
	};
	void selfgl(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		R.lp = static_cast<void*>(ctx->workers->G2L<char>(R.gp));
	};
	void selflg(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		ctx->workers->L2G<void>(R.gp,R.lp);
	};
	void selfro(U8 r, size_t off) {
		Reg& R = ctx->rfile.reg[r];
		auto sz = R.value.size();
		memcpy(R.value.pvalue,R.lp+off,sz);	
	};
	void selfwo(U8 r, size_t off) {
		Reg& R = ctx->rfile.reg[r];
		auto sz = R.value.size();
		memcpy(R.lp+off,R.value.pvalue,sz);	
	};
	void selfso(U8 r, size_t off) {
		Reg& R = ctx->rfile.reg[r];
		auto pm = new(R.lp+off) umutex;
		pm->unlock();
	};
	void pushof(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		if(R.gp.id == ctx->rfile.seg[0]) {
			ctx ->workers
				->tQueue()
				->insert(static_cast<task*>(R.lp));
		}
	};
	void callcp(U8 r, UUID capKey) {
		Reg& R = ctx->rfile.reg[r];
		auto bearer = ctx->workers->getBearerByKey(capKey);
		if(bearer != nullptr) {
			bearer->runLP(R.lp);
		}
	};
	void movegp(U8 src, U8 des) {
		Reg& Ra = ctx->rfile.reg[src];
		Reg& Rb = ctx->rfile.reg[des];
		memcpy(&(Rb.gp),&(Ra.gp),sizeof(Ra.gp));
	};
	void movelp(U8 src, U8 des) {
		Reg& Ra = ctx->rfile.reg[src];
		Reg& Rb = ctx->rfile.reg[des];
		Rb.lp = Ra.lp;
	};
	void movevl(U8 src, U8 des) {
		Reg& Ra = ctx->rfile.reg[src];
		Reg& Rb = ctx->rfile.reg[des];
		Rb.value();
		Rb.value(Ra.value);
	};
	void movesz(U8 src, U8 des) {
		Reg& Ra = ctx->rfile.reg[src];
		Reg& Rb = ctx->rfile.reg[des];
		Rb.gp.objsize = Ra.gp.objsize;
	};
	void moveal(U8 src, U8 des) {
		Reg& Ra = ctx->rfile.reg[src];
		Reg& Rb = ctx->rfile.reg[des];
		memcpy(&(Rb.gp),&(Ra.gp),sizeof(Ra.gp));
		Rb.lp = Ra.lp;
		Rb.value();
		Rb.value(Ra.value);
	};

};
struct JIT {

}

}} // namespace