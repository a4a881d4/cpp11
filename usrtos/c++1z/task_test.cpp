#include <usrtkey.hpp>
#include <task.hpp>
#include <iostream>

using namespace usrtos;
int main() {
	std::cout << " script: " << sizeof(struct script) << std::endl;
	std::cout << " timed: " << sizeof(struct timed) << std::endl;
	std::cout << " captask: " << sizeof(struct captask) << std::endl;
	std::cout << " task: " << sizeof(struct task) << std::endl;
	std::cout << " _callback_argv_t: " << sizeof(_callback_argv_t) << std::endl;
}