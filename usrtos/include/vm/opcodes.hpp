#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <vm/BasicTypes.h>
#include <vm/regs.hpp>
#include <vector>
#include <tuple>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>



using namespace boost::uuids;
#include <gp.hpp>
#include <usrtworker.hpp>

namespace usrtos{ namespace vm{

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

#define ENUM_PARAMETRIC_OPERATORS(visitOp)

#define ENUM_NONCONTROL_NONPARAMETRIC_OPERATORS(visitOp) \
	visitOp(0x02,setseg,"set_segment",BINARY(U8,UUID),std::tuple<U8,UUID>) \
	\
	visitOp(0x10,allocm,"alloc_memory",TERNARY(U8,U8,ANYTYPE),std::tuple<U8,U8,ANYTYPE>) \
	visitOp(0x11,clearm,"clear_memory",UNARY(U8),std::tuple<U8>) \
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
	visitOp(0x44,immelp,"set_local_pointer",BINARY(U8,size_t),std::tuple<U8,size_t>) \
	\
	visitOp(0x50,selfrd,"self_read",UNARY(U8),std::tuple<U8>) \
	visitOp(0x51,selfwr,"self_write",UNARY(U8),std::tuple<U8>) \
	visitOp(0x52,selfst,"self_set",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x53,selfgl,"self_gp2lp",UNARY(U8),std::tuple<U8>) \
	visitOp(0x54,selflg,"self_lp2gp",UNARY(U8),std::tuple<U8>) \
	visitOp(0x55,selfsc,"self_conv_time_sys2cpu",UNARY(U8),std::tuple<U8>) \
	\
	visitOp(0x58,selfro,"self_read_with_offset",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x59,selfwo,"self_write_with_offset",BINARY(U8,ANYTYPE),std::tuple<U8,ANYTYPE>) \
	visitOp(0x5a,selfso,"self_set_with_offset",TERNARY(U8,U8,ANYTYPE)) \
	\
	visitOp(0x60,pushof,"push_offset_tofifo",UNARY(U8),std::tuple<U8>) \
	\
	visitOp(0x70,callsy,"call_sys_capability",BINARY(U8,UUID),std::tuple<U8,UUID>) \
	\
	visitOp(0x71,callcp,"call_capability",BINARY(U8,UUID),std::tuple<U8,UUID>) \
	\
	visitOp(0x80,movegp,"move_gp_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x81,movelp,"move_lp_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x82,movevl,"move_vl_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x83,movesz,"move_sz_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) \
	visitOp(0x84,moveal,"move_all_fromAtoB",BINARY(U8,U8),std::tuple<U8,U8>) 

#define ENUM_CONTROL_OPERATORS(visitOp) \
	visitOp(0x01,nop,"nop",,) \
	\
	visitOp(0xf0,ret,"return",,)	
		
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



struct OperatorStream {
	OperatorStream() {};
	OperatorStream(const U8* codeBytes, size_t len)
	: nextByte((U8*)codeBytes), end(codeBytes+len) {}

	operator bool() const { return nextByte < end; }

	void setBuf(const U8* codeBytes, size_t len) {
		nextByte = (U8*)codeBytes;
		end = codeBytes+len;
	};

	U8* advance(size_t n) {
		U8* r = nextByte;
		nextByte += n;
		if(nextByte > end) {
			std::cout << "Decoder Stream overflow" << std::endl;
		}
		return r;
	};

	template<typename T>
	T* get() {
		return (T*)(advance(sizeof(T)));
	};
	
	template<typename T>
	T* get(T& a);
	template<typename T>
	OperatorStream& put(T& a);
	
	std::tuple<> tget() {
		return std::tuple<>();
	};
	
	void mput() {};
	template<typename T0, typename... Other>
	void mput(T0& a, Other& ... o) {
		put<T0>(a);
		mput(o...);
	};

	void mget() {};
	template<typename T0, typename... Other>
	void mget(T0& a, Other& ... o) {
		get<T0>(a);
		mget(o...);
	};
	
	template<typename T0, typename... Other>
	std::tuple<T0,Other ...> tget(T0& a, Other& ... o) {
		get<T0>(a);
		return std::tuple_cat(make_tuple(a),tget(o...));
	};

private:

	U8* nextByte;
	const U8* end;
};

template<typename T>
T* OperatorStream::get(T& a) {
	a = *(T*)(advance(sizeof(T)));
	return &a;
};
template<typename T>
OperatorStream& OperatorStream::put(T& a) {
	memcpy(advance(sizeof(T)),&a,sizeof(T));
	return *this;
};
template<>
ANYTYPE* OperatorStream::get<ANYTYPE>(ANYTYPE& a) {
	a.get<OperatorStream>(*this);
	return &a;
};
template<> 
OperatorStream& OperatorStream::put<ANYTYPE>(ANYTYPE& a) {
	a.put<OperatorStream>(*this);
	return *this;
};


struct VMContext {
	VMContext(UsrtWorkers& w) {
		workers = &w;
		rfile.setSeg(segment::task, workers->getMemKey("taskq"))
			 .setSeg(segment::temp, workers->getMemKey("memory"))
			 .setSeg(segment::log0, workers->getMemKey("log0"))
			 .setSeg(segment::log1, workers->getMemKey("log1"))
			 .setSeg(segment::log2, workers->getMemKey("log2"))
			 .setSeg(segment::log3, workers->getMemKey("log3"))
			 .setSeg(segment::log4, workers->getMemKey("log4"))
			 .setSeg(segment::log5, workers->getMemKey("log5"))
			 ;
	};
	RegFile<256> rfile;
	UsrtWorkers *workers;
};

struct VMOffset {
	size_t offset;
	VMOffset(ANYTYPE& a) {
		offset = a.toOffset();
	};
	void *add(void *lp) {
		char *p = (static_cast<char*>(lp))+offset;
		return static_cast<void*>(p);
	};
};

struct JITVisitor {
	JITVisitor() {};
	JITVisitor(VMContext& uctx) : ctx(&uctx) {};
	void nop(U8 a) {};
	void setseg(U8 s, UUID id) {
		ctx->rfile.setSeg((segment)s,std::move(id));
	};
	void allocm(U8 s, U8 r, ANYTYPE& sz) {
		if(ctx->rfile.mem[s] == nullptr) {
			ctx->rfile.mem[s] = ctx->workers
				->bindBlockByKey<Layout::UsrtMem>(
					ctx->rfile.seg[s]);
			// ctx->rfile.dump(); 
		}
		Reg& R = ctx->rfile.reg[r];
		size_t ssize = sz;
		if(sz > 65536)
			std::cout << "alloc size:" << ssize << std::endl;
		R.lp = ctx->rfile.mem[s]
			->newGP<char>(R.gp,ssize,16);
	};
	void clearm(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		if(R.gp.objsize > 65536){
			CPBlock::GP tempgp;
			ctx->workers->L2G<char>(tempgp,(char*)R.lp);
			std::cout << "id:" << R.gp.id << " t:" << tempgp.id << std::endl;
			std::cout << "of:" << R.gp.offset << " t:" << tempgp.offset << std::endl;
			std::cout << "sz:" << R.gp.objsize << " t:" << tempgp.objsize << std::endl;
			std::cout << "clear size:" << R.gp.objsize << std::endl;
		}
		memset(R.lp, 0, R.gp.objsize);
	};
	void loadgp(U8 ra, U8 rb, VMOffset offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		void *p = offset.add(Rb.lp);
		memcpy(&(Ra.gp),p,sizeof(Ra.gp));
	};
	void loadlp(U8 ra, U8 rb, VMOffset offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		void *p = offset.add(Rb.lp);
		memcpy(&(Ra.lp),p,sizeof(void*));
	};
	void loadvl(U8 ra, U8 rb, VMOffset offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		void *p = offset.add(Rb.lp);
		auto sz = Ra.value.size();
		memcpy(Ra.value.pvalue,p,sz);
	};
	void savegp(U8 ra, U8 rb, VMOffset offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		void *p = offset.add(Rb.lp);
		memcpy(p,&(Ra.gp),sizeof(Ra.gp));
	};
	void savelp(U8 ra, U8 rb, VMOffset offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		void *p = offset.add(Rb.lp);
		memcpy(p,&(Ra.lp),sizeof(void*));
	};
	void savevl(U8 ra, U8 rb, VMOffset offset) {
		Reg& Ra = ctx->rfile.reg[ra];
		Reg& Rb = ctx->rfile.reg[rb];
		void *p = offset.add(Rb.lp);
		auto sz = Ra.value.size();
		memcpy(p,Ra.value.pvalue,sz);
	};
	void immeid(U8 r, UUID id) {
		Reg& R = ctx->rfile.reg[r];
		R.gp.id = id;
	};
	void immeos(U8 r, ANYTYPE& off) {
		Reg& R = ctx->rfile.reg[r];
		VMOffset o(off);
		R.gp.offset = o.offset;
	};
	void immesz(U8 r, ANYTYPE& sz) {
		Reg& R = ctx->rfile.reg[r];
		VMOffset o(sz);
		R.gp.objsize = o.offset;
	};
	void immevl(U8 r, ANYTYPE& v) {
		Reg& R = ctx->rfile.reg[r];
		R.value(v);
	};
	void immelp(U8 r, size_t p) {
		Reg& R = ctx->rfile.reg[r];
		R.lp = (void*)(p);
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
	void selfst(U8 r, U8 t) {
		Reg& R = ctx->rfile.reg[r];
		if(t == 0) {
			auto pm = new(R.lp) umutex;
			pm->unlock();
		}
	};
	void selfgl(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		R.lp = static_cast<void*>(ctx->workers->G2L<char>(R.gp));
	};
	void selflg(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		ctx->workers->L2G<char>(
			R.gp,static_cast<char*>(R.lp));
	};
	void selfsc(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		// check type U64
		timing::time_t& t= *(timing::time_t*)R.value.pvalue;
		t = ctx->workers->m_c2s.toCpu(t);
	};
	void selfcs(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		// check type U64
		timing::time_t& t= *(timing::time_t*)R.value.pvalue;
		t = ctx->workers->m_c2s.toSys(t);
	};
	void selfro(U8 r, VMOffset offset) {
		Reg& R = ctx->rfile.reg[r];
		auto sz = R.value.size();
		memcpy(R.value.pvalue,offset.add(R.lp),sz);	
	};
	void selfwo(U8 r, VMOffset offset) {
		Reg& R = ctx->rfile.reg[r];
		auto sz = R.value.size();
		memcpy(offset.add(R.lp),R.value.pvalue,sz);	
	};
	void selfso(U8 r, U8 t, VMOffset offset) {
		Reg& R = ctx->rfile.reg[r];
		if(t == 0) {
			auto pm = new(offset.add(R.lp)) umutex;
			pm->unlock();
		}
	};
	void pushof(U8 r) {
		Reg& R = ctx->rfile.reg[r];
		if(R.gp.id == ctx->rfile.seg[0]) {
			ctx ->workers
				->tQueue()
				->insert(static_cast<task*>(R.lp));
			// ctx ->workers
			//     ->tQueue()
			//     ->dumpTask(*(task*)R.lp);
		}
	};
	void callcp(U8 r, UUID capKey) {
		Reg& R = ctx->rfile.reg[r];
		auto bearer = ctx->workers->getBearerByKey(capKey);
		if(bearer != nullptr) {
			bearer->runLP(R.lp);
		}
	};
	void callsy(U8 r, UUID capKey) {
		Reg& R = ctx->rfile.reg[r];
		auto bearer = ctx->workers->getBearerByKey(capKey);
		if(bearer == nullptr) {
			if(ctx->workers->setCap(capKey))
				bearer = ctx->workers->getBearerByKey(capKey);
		}
		if(bearer != nullptr) {
			struct mainWorkerCTX mCtx;
			mCtx.workers = ctx->workers;
			mCtx.argv = R.lp;
			bearer->runLP(&mCtx);
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
private:
	VMContext* ctx;
};

#define TERNARY(a,b,c) (a)(b)(c)
#define BINARY(a,b) (a)(b)
#define NULLARY(a) (a)
#define UNARY(a) (a)

struct EncodeStream : OperatorStream {
	EncodeStream() {};
	EncodeStream(U8* buf,size_t len) : OperatorStream(buf,len) {};
	#define DECLEARAA(r,data,s) (s&& BOOST_PP_CAT(data,r))
	#define DECLEAR(r,data,s) (s BOOST_PP_CAT(data,r))
	#define VM_PUT(r,data,s) put<s>(BOOST_PP_CAT(data,r));
	#define PARAM(r,data,s) (std::move(BOOST_PP_CAT(data,r)))
	#define VISIT_OPCODE(code,name,str,types,...) \
	void name( BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(DECLEARAA,op_,types)) ) { \
		Opcode c = Opcode::name; \
		put<Opcode>(c); \
		BOOST_PP_SEQ_FOR_EACH(VM_PUT,op_,types) \
	}; \
	void p_##name( BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(DECLEAR,op_,types)) ) {\
			name( BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(PARAM,op_,types)) ); \
		};
	ENUM_NONCONTROL_OPERATORS(VISIT_OPCODE)

	#undef VISIT_OPCODE
	
	#define VISIT_OPCODE(code,name,str,...) \
	void name() { \
		Opcode c = Opcode::name; \
		put<Opcode>(c); \
	}; \
	void p_##name() { name(); };
	ENUM_CONTROL_OPERATORS(VISIT_OPCODE)

	#undef VISIT_OPCODE	
	#undef VM_PUT
	#undef DECLEARAA
	#undef DECLEAR
	#undef PARAM

	void test(U8 a, U8 b) {
		std::cout << a*b << std::endl;
	};
};

enum RunMode {
	  run = 1
	, show = 2
	, step = 4
};

struct Decode {
	#define STRING(x) #x
	#define DECLEAR(r,data,s) s BOOST_PP_CAT(data,r);
	#define VM_GET(r,data,s) is.get<s>(BOOST_PP_CAT(data,r));
	#define PARAM(r,data,s) (BOOST_PP_CAT(data,r))
	#define SHOWOP(r,data,s) << STRING(s) << "::" << BOOST_PP_CAT(data,r) << " "
	#define VISIT_OPCODE(code,name,str,types,...) \
		case Opcode::name: {\
			BOOST_PP_SEQ_FOR_EACH(DECLEAR,op_,types) \
			BOOST_PP_SEQ_FOR_EACH(VM_GET,op_,types) \
			if(show&RunMode::show) { \
				std::cout << str << ": " \
				BOOST_PP_SEQ_FOR_EACH(SHOWOP,op_,types) \
				<< std::endl; \
			} \
			if(show&RunMode::run) {\
				v.name(\
					BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(PARAM,op_,types)) \
					); \
			}\
			return true; \
		}
	bool once(JITVisitor& v, OperatorStream& is, int show = RunMode::show) {
		if(is) {
			Opcode c;
			is.get<Opcode>(c);
			switch(c) {
			ENUM_NONCONTROL_OPERATORS(VISIT_OPCODE)
			case Opcode::nop: return true;
			case Opcode::ret: return false;
			default:
				return false;
			}
		} else {
			return false;
		}
	#undef VISIT_OPCODE
	#undef SHOWOP
	#undef PARAM
	#undef VM_GET
	#undef DECLEAR
	#undef STRING

	};
	void run(JITVisitor& v, OperatorStream& is, int show = RunMode::show) {
		while(once(v,is,show));
	};
};
#undef UNARY
#undef NULLARY
#undef BINARY
#undef TERNARY

}} // namespace