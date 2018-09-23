#include <tuple>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

using namespace boost::preprocessor;

#define ENUM_NONCONTROL_NONPARAMETRIC_OPERATORS(visitOp) \
	visitOp(0x01,nop,"nop",NULLARY(none)) \
	\
	visitOp(0x02,setseg,"set_segment",BINARY(char,int),std::tuple<char,int>)

#define TESTSEQ (char)(int)
#define BINARY(a,b) (a)(b)
#define NULLARY(a) (a)
#define DECLEAR(r,data,s) (s& BOOST_PP_CAT(data,r))
#define VM_PUT(r,data,s) put<s>(BOOST_PP_CAT(data,r));
#define VISIT_OPCODE(code,name,str,types,...) \
	void name( \
		BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(DECLEAR,op_,types)) \
		) { \
		put<U8>(code); \
		BOOST_PP_SEQ_FOR_EACH(VM_PUT,op_,types) \
	}

ENUM_NONCONTROL_NONPARAMETRIC_OPERATORS(VISIT_OPCODE)

#undef VISIT_OPCODE

void init( \
	BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(DECLEAR,op,TESTSEQ)) \
	) {
	BOOST_PP_SEQ_FOR_EACH(DECLEAR,op_,BOOST_PP_SEQ_NIL)

};