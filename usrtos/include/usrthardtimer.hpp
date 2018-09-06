#pragma once

#include <usrttype.hpp>

namespace usrtos {
class UsrtHardTimer {
  private: 
    utime_t end;
    utime_t getNow() {
      return std::chrono::high_resolution_clock::now();
    };
  public:
    UsrtHardTimer( int time ) {
      end = getNow() + micro_type(time);
    };
    
    int expired() { return end < getNow(); };
    
    utime_t date() { return getNow(); };
};
}; //namespace usrtos