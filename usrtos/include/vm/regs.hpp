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

struct Reg {
	_globe_pointer gp;
	U64 value;
	void *lp;
	Reg& setValue(U64 v) {
		value = v;
		return *this;
	};
	Reg& setOffset(size_t o) {
		gp.offset = o;
		return *this;
	};
	Reg& setLP(void *p) {
		lp = p;
		return *this;
	};
	Reg& setGP(_globe_pointer& a) {
		gp = a;
		return *this;
	};
	_globe_pointer& getGP() {
		return gp;
	};
};

template<int SZ>
struct RegFile {
	reg r[SZ];
	uuid seg[SZ];
	RegFile& setSeg(int i, uuid& id) {
		seg[i] = id;
		return *this;
	};
};

};
};