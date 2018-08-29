#pragma once
#include<cpblock.hpp>
#include<usrtmem.hpp>
#include<usrtheap.hpp>
#include<log.hpp>
#include <boost/mpl/size_t.hpp>

namespace usrtos {
class Layout {
public:

	typedef boost::mpl::size_t<0x1000> ADDONPOST;
	typedef boost::mpl::size_t<0x10000> ADDONEND;
	
	typedef log<ADDONPOST::value,ADDONEND::value> UsrtLog;
	typedef Mem<ADDONPOST::value> UsrtMem;
	typedef Fifo<ADDONPOST::value,ADDONEND::value> UsrtFifo;

};
}; // namespace usrtos