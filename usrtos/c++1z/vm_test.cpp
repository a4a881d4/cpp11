#include <iostream>
#include <vm/opcodes.hpp>
#include <usrtkey.hpp>

using namespace usrtos::vm;
int main() {
	U8 v[256];
	for(int i=0;i<256;i++)
		v[i] = static_cast<U8>(i);
	OperatorStream is(v,256);
	U8 a0;
	U16 a1;
	U32 a2;
	U64 a3;
	UUID a4;
	auto r = is.tget<U8,U16,U32,U64,UUID>(a0,a1,a2,a3,a4);
	std::cout << std::get<0>(r) << " "  
			  << std::get<1>(r) << " "  
			  << std::get<2>(r) << " "  
			  << std::get<3>(r) << " "  
			  << std::get<4>(r) << " "  
			  << std::endl;
	a0 += 1;
	a1 += 1;
	a2 += 1;
	a3 += 1;
	a4 = usrtos::UsrtKey::usrtosNS();
	is.mput<UUID,U64,U32,U16,U8>(a4,a3,a2,a1,a0);
	ANYTYPE a5;
	a5(a3);
	is.mput<ANYTYPE>(a5);
	for(int i=0;i<64+16;i++) {
		std::cout << hex << (int)v[i] << " ";
		if((i&15)==15)
			std::cout << std::endl;
	}
	OperatorStream os(v,256);
	os.mget<U8,U16,U32,U64,UUID>(a0,a1,a2,a3,a4);
	std::cout << " os 0 " << std::endl;
	os.mget<UUID,U64,U32,U16,U8>(a4,a3,a2,a1,a0);
	std::cout << " os 1 " << std::endl;
	a5();
	std::cout << " os 2 " << std::endl;
	os.mget<ANYTYPE>(a5);
	std::cout << " os 3 " << std::endl;
	std::cout << *((U64*)a5.pvalue) << std::endl;
}



