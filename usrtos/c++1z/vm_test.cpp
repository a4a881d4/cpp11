#include <vm/opcodes.hpp>
#include <iostream>
using namespace usrtos::vm;
int main() {
	vector<U8> v;
	for(int i=0;i<256;i++)
		v.push_back(static_cast<U8>(i));
	OperatorStream is(v);
	auto r = TGet<U8,U16,U32,U64,UUID>(is).tget();
	std::cout << std::get<0>(r) << " "  
			  << std::get<1>(r) << " "  
			  << std::get<2>(r) << " "  
			  << std::get<3>(r) << " "  
			  << std::get<4>(r) << " "  
			  << std::endl;
}