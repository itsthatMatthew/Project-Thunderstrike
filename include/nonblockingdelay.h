#ifndef NONBLOCKINGDELAY_HEADER_INCLUDED
#define NONBLOCKINGDELAY_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Utility class to delay execution in an block of code without blocking general execution.
// Must be init as a global var or a local static to function properly.
class NonBlockingDelay {
 public:
  explicit NonBlockingDelay(size_t delay) : delay_(delay), start_(0), active_(true) { }
  
  // For the object to be placed at the top fo a block with an if(obj) as a guard.
  // @returns true, if the correct amount of time has passed;
  // @returns false otherwise
  operator bool() {
    if (active_) {
      active_ = false;
      start_ = millis();
      return false;
    }
    if (millis() - start_ > delay_) {
      active_ = true;
      return true;
    }
    return false;
  }
      
 private:
  size_t delay_;
  size_t start_;
  bool active_;
};  

}; // namespace PTS

#endif /* NONBLOCKINGDELAY_HEADER_INCLUDED */