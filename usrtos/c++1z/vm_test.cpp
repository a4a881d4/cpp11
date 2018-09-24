#include <iostream>
#include <vm/opcodes.hpp>
#include <usrtkey.hpp>

using namespace usrtos::vm;
int main() {
	U8 v[256];
	for(int i=0;i<256;i++)
		v[i] = static_cast<U8>(i);
	EncodeStream es(v,256);
	ANYTYPE a;
	U8 b = 10;
	U8 a0 = 1;
	U8 a1 = 0;
	U8 a2;
	es.allocm(a0,a1,a(b));
	es.moveal(a0,a1);
	UUID id = usrtos::UsrtKey::usrtosNS();
	es.immeid(a0,id);
	ANYTYPE offset;
	offset.pack(65535);
	es.selfro(a0,offset);
	es.nop();
	es.ret();
	OperatorStream os(v,256);
	JITVisitor visitor;
	Decode d;
	d.run(visitor,os,true);
	// a();
	// os.mget<U8,U8,U8,ANYTYPE>(a0,a1,a2,a);
	
	// std::cout << static_cast<int>(a0) << " "
	// 		  << static_cast<int>(a1) << " "
	// 		  << static_cast<int>(a2) << " "
	// 		  << static_cast<int>(*(U8*)a.pvalue) << " "
	//  		  << std::endl;

	// U8 a0;
	// U16 a1;
	// U32 a2;
	// U64 a3;
	// UUID a4;
	// auto r = is.tget<U8,U16,U32,U64,UUID>(a0,a1,a2,a3,a4);
	// std::cout << std::get<0>(r) << " "  
	// 		  << std::get<1>(r) << " "  
	// 		  << std::get<2>(r) << " "  
	// 		  << std::get<3>(r) << " "  
	// 		  << std::get<4>(r) << " "  
	// 		  << std::endl;
	// a0 += 1;
	// a1 += 1;
	// a2 += 1;
	// a3 += 1;
	// a4 = usrtos::UsrtKey::usrtosNS();
	// is.mput<UUID,U64,U32,U16,U8>(a4,a3,a2,a1,a0);
	// ANYTYPE a5;
	// a5(a3);
	// is.mput<ANYTYPE>(a5);
	// for(int i=0;i<64+16;i++) {
	// 	std::cout << hex << (int)v[i] << " ";
	// 	if((i&15)==15)
	// 		std::cout << std::endl;
	// }
	// OperatorStream os(v,256);
	// os.mget<U8,U16,U32,U64,UUID>(a0,a1,a2,a3,a4);
	// std::cout << " os 0 " << std::endl;
	// os.mget<UUID,U64,U32,U16,U8>(a4,a3,a2,a1,a0);
	// std::cout << " os 1 " << std::endl;
	// a5();
	// std::cout << " os 2 " << std::endl;
	// os.mget<ANYTYPE>(a5);
	// std::cout << " os 3 " << std::endl;
	// std::cout << *((U64*)a5.pvalue) << std::endl;
}



