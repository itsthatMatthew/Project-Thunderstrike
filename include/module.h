#ifndef MODULE_HEADER_INCLUDED
#define MODULE_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Base class for modules.
//
// Modules are the core building blocks of the project. Modules' intended
// function is the separation of code based on the manual tasks a player must
// do to pass one.
//
// Modules are effectively state machines for robust functioning.
//
//    |
//    v
// INVALID-->ACTIVE
//           |    |
//           v    v
//      FAILED    SOLVED

template<uint32_t STACK_DEPTH = 1024, uint32_t PRIORITY = 1>
class Module {
 public:
  enum ModuleState : uint8_t {
    INVALID   = 0b00,
    ACTIVE    = 0b01,
    PASSED    = 0b10,
    FAILED    = 0b11,
  };

  explicit Module(const char *const name) : name_(name), state_(INVALID), task_handle_(nullptr) { }

  [[nodiscard]] ModuleState getState() const { return state_; }

  ModuleState passState() const {
    switch (state_) {
    case INVALID: state_ = ACTIVE; break;
    case ACTIVE: state_ = PASSED; break;
    }
    return state_;
  }

  ModuleState failState() const {
    switch (state_) {
    case ACTIVE: state_ = FAILED; break;
    }
    return state_;
  }

  // to be used as setup function, since some components might need additional
  // initialization after main setup call
  virtual void begin() const { }

  // thread worker function that gets executed as start() is called
  static constexpr void threadFunc(void *params) { }

  // creates a new thread from the object running the threadFunc
  // @returns a handle to the new thread
  TaskHandle_t start(void *params = nullptr) const {
    xTaskCreate(
      threadFunc,
      name_,
      STACK_DEPTH,
      params,
      PRIORITY,
      &task_handle_
    );
    return task_handle_;
  }

  void suspend() { if (task_handle_) { vTaskSuspend(task_handle_); } }
  void resume() { if (task_handle_) { vTaskResume(task_handle_); } }
  void destroy() const { if (task_handle_) { vTaskDelete(task_handle_); task_handle_ = nullptr; } }

  virtual ~Module() { } // only for heterogeneus collection

 private:
  const char *const name_;
  mutable ModuleState state_;
  mutable TaskHandle_t task_handle_;
}; /* class Module */

}; /* namespace PTS */

#endif /* MODULE_HEADER_INCLUDED */