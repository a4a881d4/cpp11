#include <findblock.hpp>
#include <logs.hpp>

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
	std::string lname(argv[3]);
	if(mylog.has(lname)) {

		auto pl = mylog[lname];

		if(strcmp(argv[2],"dump")==0) {
			while(1) {
				usleep(100000);
				pl->dump();
			}
		}

		if(strcmp(argv[2],"log")==0) {
			int c = 0;
			while(1) {
				usleep(1000000);
				pl->printf("hello world! %d\n",c);
				c++;
			}
		}
	}
}