#pragma once
#include <log.hpp>
#include <logs.hpp>
#include <sstream>

namespace usrtos {

class LogLevel {
private:
	int m_level;
	usrtlog *m_log;
public:
	LogLevel(){};
	LogLevel(logs& m, int l) : m_level(l) {
		std::stringstream ln;
		ln << "log" << l;
		m_log = m[ln.str()];
	};
	void operator()(const char * format, ...) {
		if(m_log) {
			va_list args;
			va_start (args, format);
			m_log->printf(format, args);
			va_end (args);
		}
		else {
			std::cerr << "miss log " << m_level << std::endl;
		}
	};
	void put(std::string& s) {
		m_log->put(s);
	};
};
}; // namespace usrtos
