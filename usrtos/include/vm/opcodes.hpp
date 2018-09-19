#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "BasicTypes.h"

using namespace boost::uuids;
#include <gp.hpp>

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

#define ENUM_CONTROL_OPERATORS(visitOp)

#define ENUM_PARAMETRIC_OPERATORS(visitOp)

#define ENUM_NONCONTROL_NONPARAMETRIC_OPERATORS(visitOp) \
	visitOp(0x01,nop,"nop",NoImm,NULLARY(none)) \
	\
	visitOp(0x02,setseg,"set_segment",BINARY(U8,UUID)) \
	\
	visitOp(0x10,allocm,"alloc_memory",TERNARY(U8,U8,ANYSIZE)) \
	\
	visitOp(0x20,savegp,"save_globe_pointer",TERNARY(U8,U8,ANYSIZE)) \
	visitOp(0x21,savelp,"save_local_pointer",TERNARY(U8,U8,ANYSIZE)) \
	visitOp(0x22,savevl,"save_value",TERNARY(U8,U8,ANYSIZE)) \
	\
	visitOp(0x30,loadgp,"load_globe_pointer",TERNARY(U8,U8,ANYSIZE)) \
	visitOp(0x31,loadlp,"load_local_pointer",TERNARY(U8,U8,ANYSIZE)) \
	visitOp(0x32,loadvl,"load_value",TERNARY(U8,U8,ANYSIZE)) \
	\
	visitOp(0x40,immeid,"set_gp.id",BINARY(U8,UUID)) \
	visitOp(0x41,immeos,"set_gp.offset",BINARY(U8,ANYSIZE)) \
	visitOp(0x42,immesz,"set_gp.objsize",BINARY(U8,ANYSIZE)) \
	visitOp(0x43,immevl,"set_value",BINARY(U8,ANYSIZE)) \
	visitOp(0x44,immelp,"set_local_pointer",BINARY(U8,U64)) \
	\
	visitOp(0x50,selfrd,"self_read",UNARY(U8)) \
	visitOp(0x51,selfwr,"self_write",UNARY(U8)) \
	visitOp(0x52,selfst,"self_set",BINARY(U8,U8)) \
	visitOp(0x53,selfgl,"self_gp2lp",UNARY(U8)) \
	visitOp(0x54,selflg,"self_lp2gp",UNARY(U8)) \
	\
	visitOp(0x58,selfro,"self_read_with_offset",BINARY(U8,ANYSIZE)) \
	visitOp(0x59,selfwo,"self_write_with_offset",BINARY(U8,ANYSIZE)) \
	visitOp(0x5a,selfso,"self_set_with_offset",TERNARY(U8,U8,ANYSIZE)) \
	\
	visitOp(0x60,pushof,"push_offset_tofifo",UNARY(U8)) \
	\
	visitOp(0x70,callcp,"call_capability",BINARY(U8,UUID)) \
	\
	visitOp(0x80,movegp,"move_gp_fromAtoB",BINARY(U8,U8)) \
	visitOp(0x81,movelp,"move_lp_fromAtoB",BINARY(U8,U8)) \
	visitOp(0x82,movevl,"move_vl_fromAtoB",BINARY(U8,U8)) \
	visitOp(0x83,movesz,"move_sz_fromAtoB",BINARY(U8,U8)) \
	visitOp(0x84,moveal,"move_all_fromAtoB",BINARY(U8,U8)) \
	
		
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

enum segment {
	  task = 0x0
	, temp = 0x1
	, log0 = 0x2
	, log1
	, log2
	, log3
	, log4
	, log5
};

}} // namespace