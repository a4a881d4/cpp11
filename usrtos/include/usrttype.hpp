#pragma once
#include <chrono>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <usrtsync.hpp> 
namespace usrtos {

typedef std::chrono::high_resolution_clock::time_point utime_t;
typedef std::chrono::duration<int, std::micro> micro_type;

typedef long long int64;

inline std::ostream& operator<<(std::ostream& os,const utime_t& h)
{
	auto d = h - std::chrono::high_resolution_clock::now();
	micro_type dd = std::chrono::duration_cast<micro_type>(d);
	os << dd.count();
	return os;
}

typedef boost::interprocess::interprocess_mutex umutex;
typedef boost::interprocess::scoped_lock<umutex> uscoped_lock;

};