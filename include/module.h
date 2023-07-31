#ifndef MODULE_HEADER_INCLUDED
#define MODULE_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Base class for modules
// Modules are the core building blocks of the project. Modules' intended
// function is the separation of code based on the manual tasks a player must
// do to finish one.
// Modules are effectively state machines, with at max 2 strikes each.
//
//    │
//    ▼
// INVALID──►ACTIVE
//            │  │
//       ┌────┘  │
//       ▼       │
//   STRIKE_1────┼───►PASSED
//       │       │
//       ▼       │
//   STRIKE_2────┘
//       │
//       ▼
//     FAILED

class Module {
 public:
  enum ModuleState : byte {
    INVALID   = 0b000,
    ACTIVE    = 0b001,
    PASSED    = 0b010,
    STRIKE_1  = 0b101,
    STRIKE_2  = 0b110,
    FAILED    = 0b111,
  };

  explicit Module() : state_(INVALID) { }

  [[nodiscard]] ModuleState getState() const { return state_; }

  ModuleState passState() const {
    switch (state_)
    {
    case INVALID: // --> ACTIVE
    case ACTIVE:  // --> PASSED
      state_ = static_cast<ModuleState>(state_ + 1);
      break;
    
    case STRIKE_1: // --> PASSED
    case STRIKE_2: // --> PASSED
      state_ = PASSED;
      break;
    }

    return state_;
  }

  ModuleState failState() const {
    switch (state_)
    {
    case ACTIVE: // --> STRIKE_1
      state_ = STRIKE_1;
      break;
    
    case STRIKE_1: // --> STRIKE_2
    case STRIKE_2: // --> FAILED
      state_ = static_cast<ModuleState>(state_ + 1);
      break;
    }

    return state_;
  }

  // to be used as setup function, since some components might need additional
  // initialization after main setup call
  virtual void begin() const { }

  // thread worker function that gets executed as start is called
  virtual void threadFunc() const { }

  // creates a new thread from the object running the threadFunc
  // @returns a handle to the new thread
  virtual int start() const { }

  virtual ~Module() { }

 private:
  mutable ModuleState state_;
}; /* class Module */

}; /* namespace PTS */

#endif /* MODULE_HEADER_INCLUDED */