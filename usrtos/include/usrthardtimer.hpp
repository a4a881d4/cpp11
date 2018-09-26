#pragma once

#include <usrttype.hpp>

namespace usrtos {
class UsrtHardTimer {
  private: 
    utime_t end;
    utime t;
      
  public:
    UsrtHardTimer( int time ) {
      end = t.now() + t.fromns(time);
    };
    
    int expired() { return end < t.now(); };
    
    utime_t date() { return t.now(); };
};
}; //namespace usrtos