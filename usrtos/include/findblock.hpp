#include <cpblock.hpp>
#include <regex>
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include <iostream>

namespace fs = boost::filesystem;

namespace usrtos {
class FindBlock {
private:
	std::string dir;
	std::map<std::string,struct Head*> m_heads;
	std::map<std::string,CPBlock*> m_blocks;
	std::map<std::string,CPBlock*> m_byName;
public:
	FindBlock(){};
	void setDir(std::string d) { dir = d; };
	void list() {
		CPBlock b;
		std::regex expuuid("[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"); 
		fs::path full_path(fs::initial_path<fs::path>());
		full_path = fs::system_complete(fs::path(dir));
		if(fs::is_directory(full_path)) {
			fs::directory_iterator end_iter;
			for(fs::directory_iterator dir_itr(full_path);dir_itr != end_iter;++dir_itr) {
				try {
					if(fs::is_regular_file( dir_itr->status())) {
						std::cmatch what;
						auto fn = dir_itr->path().filename();
						if(std::regex_match(fn.c_str(),what,expuuid)){
							b.setFileName((full_path/fn).string());
							b.headFromFile();
							if(b.checkHead())
								m_heads[fn.string()] = b.m_head;
							else {
								delete b.m_head;
							}
						}
        			}
				}
				catch(const std::exception &ex) {
					std::cout << " 1 " << dir_itr->path().filename() << " " << ex.what() << std::endl;
				}
			}
		}
	};
	
	void attach() {
		fs::path full_path(fs::initial_path<fs::path>());
		full_path = fs::system_complete(fs::path(dir));
		for(auto it = m_heads.begin();it != m_heads.end(); ++it) {
			auto pb = new CPBlock;
			if(!pb->attach((full_path/it->first).string()))
				std::cout << pb->m_head->name << " attach failure" << std::endl;
			else
				m_blocks[it->first] = pb;
		}
	};

	void dumpHead() {
		CPBlock b;
		std::cout << "total: " << m_heads.size() << std::endl;
		for(auto it = m_heads.begin();it != m_heads.end(); ++it) {
			b.m_head = it->second;
			b.dumpHead();
		}
	};

	void dump() {
		std::cout << "total: " << m_blocks.size() << std::endl;
		for(auto it = m_blocks.begin();it != m_blocks.end(); ++it) {
			it->second->dump();
		}
	};
};
}; // namespace usrtos