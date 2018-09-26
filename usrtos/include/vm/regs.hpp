#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "BasicTypes.h"
#include <layout.hpp>
#include <gp.hpp>
#include <sstream>
#include <iomanip>


using namespace boost::uuids;

namespace usrtos{ namespace vm{

struct UUID : uuid {
	UUID() {};
	UUID(std::string sid) {
		uuid id = lexical_cast<uuid>(sid);
		id.swap(*this);
	};
	UUID(const uuid& id) {
		uuid iid = id;
		iid.swap(*this);
	};
};

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
		, string
	};

	U8 buf[32];
	ValueType type;
	U8* pvalue;
	size_t stringSz;

	size_t size() const {
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
			case ValueType::string: return stringSz;
			default: return 0;
		}
	};

	template<typename ostream>
	void put(ostream& os) {
		U8 *head = os.advance(sizeof(ValueType));
		*head = static_cast<U8>(type);
		auto s = size();
		if(type == ValueType::string) {
			U8 *sz = os.advance(1);
			*sz = static_cast<U8>(stringSz);
		}
		memcpy(os.advance(s),pvalue,s);
	};
	template<typename istream>
	void get(istream& is) {
		U8 *h = is.advance(1);
		type = static_cast<ValueType>(*h);
		if(type == ValueType::string) {
			U8 *t = is.advance(1);
			stringSz = static_cast<size_t>(*t);
		}
		auto s = size();
		memcpy(pvalue,is.advance(s),s);
	};
	size_t toOffset() const {
		if(size() > 8)
			return 0;
		size_t mask = -1LL;
		if(size()<8)
			mask = (1LL<<(size()*8)) - 1LL;
		unsigned long long r;
		memcpy(&r,buf,size());
		return r&mask;
	};
	// ANYTYPE(size_t&& i) {
	// 	pack(i);
	// };
	ANYTYPE(size_t i) {
		pack(i);
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
		if(type == ValueType::string)
			stringSz = x.stringSz;
		pvalue = buf;
		memcpy(pvalue,x.pvalue,sz);
		return *this;
	};
	ANYTYPE& operator()() {
		pvalue = &buf[0];
		return *this;
	};
	void pack(size_t s) {
		if(s < (1<<8)) type = ValueType::u8;
		else if(s < (1<<16)) type = ValueType::u16;
		else if(s < (1LL<<32)) type = ValueType::u32;
		else type = ValueType::u64;
		pvalue = &buf[0];
		memcpy(pvalue,&s,sizeof(size_t));
	};
	void setUUID(UUID& id) {
		(*this)(id);
	};
	void setString(std::string str) {
		type = ValueType::string;
		stringSz = str.length()+1;
		if(stringSz > 32)
			stringSz = 32;
		memcpy(buf,str.c_str(),stringSz-1);
		buf[stringSz-1] = '\0';
		pvalue = buf;
		std::cout << "set to: " << size() << ":" << buf << std::endl;
	};
	operator size_t() const { return toOffset(); }
};

std::ostream& operator<<(std::ostream& os,struct ANYTYPE& h)
{
	if(h.type != ANYTYPE::ValueType::string) 
	{
		std::stringstream s1;
		for(int i=0;i<h.size();i++)
			s1 << std::setfill('0') << std::setw(2) << std::hex << (int)h.buf[i];
		os << s1.str() << "(" << h.size() << ")";
	}
	else {
		os << (const char *)h.buf << "(" << h.size() << ")";
	}
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
	void dump() {
		for(int i=0;i<8;i++) {
			std::cout << i << ": " << seg[i] << " ";
			if(mem[i])
				std::cout << "attach mem";
			else
				std::cout << "no attach";
			std::cout << std::endl;
		}
	};
};

};
};