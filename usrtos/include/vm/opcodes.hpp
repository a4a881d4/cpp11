#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <stdint.h>

namespace usrtos {
namespace vm {
typedef uint8_t nop_t;
typedef uint8_t num_t;
typedef uint8_t op_t;
typedef uint8_t regN_t;
typedef uint32_t memSZ_t;
typedef uint64_t memNOff_t;
typedef uint16_t memSOff_t;
typedef boost::uuids::uuid memid_t;

struct _load_imm {  ///< load imm gp address
	memid_t id;
	memNOff_t off;
	memSZ_t objsize;
	nop_t nop[2];
	regN_t reg;
	op_t op;
};

union regAndSize {
	struct _op {
		byte s3z[3];
		regN_t r;
	} subRSZ;
	uint32_t RSZ;
	size_t getSZ() {
		return static_cast<size_t>((RSZ>>8)&0xfff);
	};
	regN_t getRN() {
		return subRSZ.r;
	};
};

struct _alloc_mem { ///< alloc gp mem and save point into reg
	regAndSize sub[7];
	nop_t nop[2];
	num_t num;
	op_t op;
};

union AOffB {
	struct _op {
		memLOff off;
		regN_t rA;
		regN_t rB;
	} subAOB;
	uint32_t AOB;
};

struct _load_gp { ///< load rA + offset gp to rB
	AOffB sub[7];
	nop_t nop[2];
	num_t num;
	op_t op;			
};

};
};