#ifndef MODULE_HEADER_INCLUDED
#define MODULE_HEADER_INCLUDED

#include <mutex>
#include <Arduino.h>

namespace PTS {

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
    INVALID = 0b00,
    ACTIVE  = 0b01,
    PASSED  = 0b10,
    FAILED  = 0b11,
  };

  explicit Module(const char *const name)
    : name_(name), state_(INVALID), state_lock_(), task_handle_(nullptr), handle_lock_()
  { }
  virtual ~Module() = default;

  Module(const Module&) = delete;
  Module& operator=(const Module&) = delete;

  [[nodiscard]] ModuleState getState() const { return state_; }
  [[nodiscard]] const char *const getName() const { return name_; }

  ModuleState passState() const {
    std::lock_guard<std::mutex> lock(state_lock_);
    switch (state_) {
    case INVALID: state_ = ACTIVE; break;
    case ACTIVE: state_ = PASSED; break;
    default: break;
    }
    return state_;
  }

  ModuleState failState() const {
    std::lock_guard<std::mutex> lock(state_lock_);
    switch (state_) {
    case ACTIVE: state_ = FAILED; break;
    default: break;
    }
    return state_;
  }

  // to be used as setup function, since some components might need additional
  // initialization after main setup call
  virtual void begin() const { }

  // thread worker function that gets executed as start() is called
  virtual void threadFunc() const { }

  // creates a new thread from the object running the threadFunc
  // @returns a handle to the new thread
  void start() const {
    std::lock_guard<std::mutex> lock(handle_lock_);
    xTaskCreate(
      [](void *obj) constexpr { static_cast<decltype(this)>(obj)->threadFunc(); }, // wrapper lambda
      name_,
      STACK_DEPTH,
      const_cast<Module*>(this), // removing const qualifyer as the API mandates void*
      PRIORITY,
      &task_handle_
    );
  }

  void suspend() const {
    std::lock_guard<std::mutex> lock(handle_lock_);
    if (task_handle_) {
      vTaskSuspend(task_handle_);
    }
  }

  void resume() const {
    std::lock_guard<std::mutex> lock(handle_lock_);
    if (task_handle_) {
      vTaskResume(task_handle_);
    }
  }

  void destroy() const {
    std::lock_guard<std::mutex> lock(handle_lock_);
    if (task_handle_) {
      vTaskDelete(task_handle_); task_handle_ = nullptr;
    }
  }

 private:
  const char *const name_;
  mutable ModuleState state_;
  mutable std::mutex state_lock_;
  mutable TaskHandle_t task_handle_;
  mutable std::mutex handle_lock_;
}; /* class Module */

}; /* namespace PTS */

#endif /* MODULE_HEADER_INCLUDED */