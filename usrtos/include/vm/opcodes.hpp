#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "BasicTypes.h"
#include <gp.hpp>

using namespace boost::uuids

namespace usrtos {
namespace vm {

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

enum opcode = {
	  setseg = 0x00 ///< binary op1 -> segnum<sz08>, op2 -> uuid<sz08> 
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
	, pushxx = 0x60 ///< binary op1 -> regnum<sz08>, op2 -> segnum<sz08>
	, callcp = 0x70 ///< binary op1(argv) -> regnum<sz08>, op2 -> uuid<sz08>
	, movegp = 0x80 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
	, movelp = 0x81 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
	, movevl = 0x82 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
	, movesz = 0x83 ///< binary op1 -> regA<sz08>, op2 -> regB<sz08>
};

enum segment = {
	  task = 0x0
	, temp = 0x1
	, log0 = 0x2
	, log1
	, log2
	, log3
	, log4
	, log5
};

template<int c>
class opfunc {

}
};
};