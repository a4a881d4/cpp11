#include <findblock.hpp>
#include <log.hpp>

#include <sstream>
#include <set>

using namespace usrtos;

int main(int argc,char *argv[])
{
	FindBlock fb;
	
	std::stringstream s;
    s << argv[1];
	fb.setDir(s.str());
	fb.list();
	fb.dumpHead();
	fb.attach();
	fb.dump();

	logs mylog(*(fb.getMap()));
	std::set<std::string> logName{"log0","log1","log2","log3","log4","log5"};
	for(auto l : logName) {
		if(mylog.has(l)) {
			std::cout << "find " << l << "!" << std::endl;
		}
		else {
			std::cout << "miss " << l << "!" << std::endl;
		}
	} 
}