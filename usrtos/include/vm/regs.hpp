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
struct ANYTYPE {
	enum class ValueType : U8 {
		  none
		, i8
		, u8
		, i16
		, u16
		, i32
		, u32
		, i64
		, u64
		, f32
		, f64
		, v128
		, id
	};

	U8 buf[16];
	ValueType type;
	U8* pvalue;

	size_t size() {
		switch(type) {
			case none: return 0;
			case  i8:
			case  u8: return 1;
			case i16:
			case u16: return 2;
			case i32:
			case u32:
			case f32: return 4;
			case i64:
			case u64:
			case f64: return 8;
			case v128:
			case  id: return 16;
			default: return 0;
		}
	};

	template<typename ostream>
	void put(ostream& os) {
		U8 *head = os.advance(sizeof(ValueType));
		*head = static_cast<U8>(type);
		auto s = size();
		memcpy(os.advance(s),pvalue,s);
	};
	template<typename istream>
	void get(istream& is) {
		U8 *h = is.advance(1);
		type = static_cast<ValueType>(*h);
		auto s = size();
		memcpy(pvalue,is.advance(s),s);
	};
	size_t toOffset() {
		size_t mask = (1LL<<(size()*8))-1;
		unsigned long long r;
		memcpy(&r,buf,size());
		return r&mask;
	};
	ANYTYPE(U8& i) : type(u8) , pvalue(&i) {};
	ANYTYPE(U16& i) : type(u16) , pvalue(static_cast<U8*>(&i)) {};
	ANYTYPE(U32& i) : type(u32) , pvalue(static_cast<U8*>(&i)) {};
	ANYTYPE(U64& i) : type(u64) , pvalue(static_cast<U8*>(&i)) {};
	ANYTYPE(UUID& i) : type(id) , pvalue(static_cast<U8*>(&i)) {};
	ANYTYPE() { pvalue = &buf[0]; };
	ANYTYPE& operator()(U64& x) {
		type = u64;
		pvalue = static_cast<U8*>(&x);
		return *this;
	};
	ANYTYPE& operator()(U32& x) {
		type = u32;
		pvalue = static_cast<U8*>(&x);
		return *this;
	};
	ANYTYPE& operator()(U16& x) {
		type = u16;
		pvalue = static_cast<U8*>(&x);
		return *this;
	};
	ANYTYPE& operator()(U8& x) {
		type = u8;
		pvalue = &x;
		return *this;
	};
	ANYTYPE& operator()(UUID& x) {
		type = id;
		pvalue = static_cast<U8*>(&x);
		return *this;
	};
	ANYTYPE& operator()(ANYTYPE& x) {
		type = id;
		sz = x.size();
		memcpy(pvalue,x/pvalue,sz);
		return *this;
	};
	ANYTYPE& operator()() {
		pvalue = &buf[0];
		return *this;
	};
};

struct Reg {
	_globe_pointer gp;
	ANYTYPE value;
	void *lp;
	template<typename T>
	Reg& setValue(T v) {
		value(v);
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
	UsrtMem *mem[SZ];
	RegFile& setSeg(int i, uuid& id) {
		seg[i] = id;
		mem[i] = nullptr;
		return *this;
	};
};

};
};