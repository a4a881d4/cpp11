#include <vm/opcodes.hpp>
#include <iostream>
using namespace usrtos::vm;
int main() {
	std::cout << asString(Opcode::savegp) << std::endl;
}