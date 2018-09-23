#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "BasicTypes.h"
#include <layout.hpp>
#include <gp.hpp>

using namespace boost::uuids;

namespace usrtos{ namespace vm{

typedef uuid UUID;

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
			case ValueType::none: return 0;
			case ValueType::i8:
			case ValueType::u8: return 1;
			case ValueType::i16:
			case ValueType::u16: return 2;
			case ValueType::i32:
			case ValueType::u32:
			case ValueType::f32: return 4;
			case ValueType::i64:
			case ValueType::u64:
			case ValueType::f64: return 8;
			case ValueType::v128:
			case ValueType::id: return 16;
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
		if(size() > 8)
			return 0;
		size_t mask = (1LL<<(size()*8)) - 1LL;
		unsigned long long r;
		memcpy(&r,buf,size());
		return r&mask;
	};
	
	ANYTYPE(U8& i) : type(ValueType::u8) , pvalue(&i) {};
	ANYTYPE(U16& i) : type(ValueType::u16) , pvalue((U8*)(&i)) {};
	ANYTYPE(U32& i) : type(ValueType::u32) , pvalue((U8*)(&i)) {};
	ANYTYPE(U64& i) : type(ValueType::u64) , pvalue((U8*)(&i)) {};
	ANYTYPE(UUID& i) : type(ValueType::id) , pvalue((U8*)(&i)) {};
	ANYTYPE() { pvalue = &buf[0]; };
	ANYTYPE& operator()(U64& x) {
		type = ValueType::u64;
		pvalue = (U8*)(&x);
		return *this;
	};
	ANYTYPE& operator()(U32& x) {
		type = ValueType::u32;
		pvalue = (U8*)(&x);
		return *this;
	};
	ANYTYPE& operator()(U16& x) {
		type = ValueType::u16;
		pvalue = (U8*)(&x);
		return *this;
	};
	ANYTYPE& operator()(U8& x) {
		type = ValueType::u8;
		pvalue = &x;
		return *this;
	};
	ANYTYPE& operator()(UUID& x) {
		type = ValueType::id;
		pvalue = (U8*)(&x);
		return *this;
	};
	ANYTYPE& operator()(ANYTYPE& x) {
		type = x.type;
		auto sz = x.size();
		memcpy(pvalue,x.pvalue,sz);
		return *this;
	};
	ANYTYPE& operator()() {
		pvalue = &buf[0];
		return *this;
	};
	ANYTYPE& pack(size_t s) {
		if(s < (1<<8)) type = ValueType::u8;
		else if(s < (1<<16)) type = ValueType::u16;
		else if(s < (1LL<<32)) type = ValueType::u32;
		else type = ValueType::u64;
		pvalue = &buf[0];
		memcpy(pvalue,&s,sizeof(size_t));
		return *this;
	};
};

inline std::ostream& operator<<(std::ostream& os,struct ANYTYPE& h)
{
	auto x = h.toOffset();
	os << x << "(" << h.size() << ")";
	return os;
}
inline std::ostream& operator<<(std::ostream& os,U8& h)
{
	os << (int)h;
	return os;
}
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
	void *addOff(size_t off) {
		char *p = static_cast<char*>(lp);
		return static_cast<void*>(p+off);
	};
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

template<int SZ>
struct RegFile {
	Reg  reg[SZ];
	uuid seg[SZ];
	Layout::UsrtMem *mem[SZ];
	RegFile& setSeg(segment i, uuid&& id) {
		seg[(size_t)i] = id;
		mem[(size_t)i] = nullptr;
		return *this;
	};
};

};
};