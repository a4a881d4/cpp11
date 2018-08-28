#include <cpblock.hpp>
#include "usrtheap.hpp"
#include "usrttask.hpp"

using namespace usrtos;
#include <sstream>
int main(int argc,char *argv[])
{
	struct CPBlock b;
	
	std::stringstream s;
    s << argv[1];
	b.setFileName(s.str());
	
	if(b.attach(s.str()))
		std::cout << "Attach ok" << std::endl;
	
	if(b.checkHead())
		std::cout << "Check head ok" << std::endl;

	if(b.checkType("block"))
		std::cout << "Check type ok" << std::endl;

	if(b.checkMetaSize<4096>())
		std::cout << "Check meta size ok" << std::endl;

	TaskHeap h(b);
	auto adj = h.adjust();
	std::cout << "1. move " << adj << " from wait to ready " << std::endl;
	for(int i=0;i<129;i++){
		auto p = h.tm->newLP<task>();
		if(p!=nullptr) {
			p->noE=h.after((double)i * 1e6);
			p->noL=h.after((100.-i)*1e6);
			if(h.wait->insert(h.wait->LP2offset(p)) == h.wait->NullOffset()) {
				std::cout << "wait heap full" << h.wait->HeapSize() << std::endl;
				break;
			}

		} else {
			std::cout << "can not alloc memory" << std::endl;
		}
	}
	size_t p;
	int c;
	adj = h.adjust();
	std::cout << "2. move " << adj << " from wait to ready " << std::endl;

/*	if(h.wait->check(h.wait->HeapSize()) == 0) {
		std::cout << "wait heap check success" << std::endl;
		c = 0;
		while((p = h.wait->pop()) != h.wait->NullOffset() && c < 10) {
			h.dumpTaskTime(*(h.wait->Off2LP(p)));
			c++;
		}
	}
*/
	if(h.ready->check(h.ready->HeapSize()) == 0) {
		std::cout << "ready heap check success" << std::endl;
		c = 0;
		while((p = h.ready->pop()) != h.ready->NullOffset() && c < 129*2) {
			h.dumpTaskTime(*(h.ready->Off2LP(p)));
			c++;
		}
	}
	adj = h.adjust();
	std::cout << "3. move " << adj << " from wait to ready " << std::endl;
	
}
