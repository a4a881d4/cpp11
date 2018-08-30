#pragma once
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

	std::string m_dir;

public:
	typedef std::map<std::string,struct Head*> Heads;
	typedef std::map<std::string,CPBlock*>     Blocks;

	FindBlock(std::string d) { m_dir = d; };

	Heads& list() {
		auto p_heads = new Heads;
		std::regex expuuid("[0-9a-f]{8}-[0-9a-f]{4}-[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$"); 
		fs::path full_path(fs::initial_path<fs::path>());
		full_path = fs::system_complete(fs::path(m_dir));
		if(fs::is_directory(full_path)) {
			fs::directory_iterator end_iter;
			for(fs::directory_iterator dir_itr(full_path);dir_itr != end_iter;++dir_itr) {
				try {
					if(fs::is_regular_file( dir_itr->status())) {
						std::cmatch what;
						auto fn = dir_itr->path().filename();
						if(std::regex_match(fn.c_str(),what,expuuid)){
							struct Head& head = CPBlock::sheadFromFile((full_path/fn).string().c_str());
							if(CPBlock::scheckHead(fn.c_str(),(const struct Head&)head))
								(*p_heads)[fn.string()] = &head;
							else {
								delete &head;
							}
						}
        			}
				}
				catch(const std::exception &ex) {
					std::cout << " 1 " << dir_itr->path().filename() << " " << ex.what() << std::endl;
				}
			}
		}
		return *p_heads;
	};
	
	Blocks& attach(Heads& m_heads) {
		auto p_blocks = new Blocks;
		fs::path full_path(fs::initial_path<fs::path>());
		full_path = fs::system_complete(fs::path(m_dir));
		for(auto it = m_heads.begin();it != m_heads.end(); ++it) {
			auto pb = new CPBlock;
			if(!pb->attach((full_path/it->first).string()))
				std::cout << pb->m_head->name << " attach failure" << std::endl;
			else
				(*p_blocks)[it->first] = pb;
		}
		return *p_blocks;
	};

	void dumpHead(Heads& m_heads) {
		std::cout << "total: " << m_heads.size() << std::endl;
		for(auto it = m_heads.begin();it != m_heads.end(); ++it) {
			const struct Head& m_head = *(it->second);
			CPBlock::sdumpHead(m_head);
		}
	};

	void dump(Blocks& m_blocks) {
		std::cout << "total: " << m_blocks.size() << std::endl;
		for(auto it = m_blocks.begin();it != m_blocks.end(); ++it) {
			it->second->dump();
		}
	};

};
}; // namespace usrtos