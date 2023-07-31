# Buttons

The `button.h` header's `Button` class is a handy abstraction layer for interfacing with physical *two state* buttons. Its inner state helps detecting state transitions, but it's only updated when the `update()` method is called. For this reason, it should be called periodically and *whenever* necessary, at least once in the framework's `loop()` function.

The *inner state* is determined by reading the delegated pin's (must be assigned in the constructor) LOW or HIGH state.

Buttons can be assigned callbacks for their 4 state transitions. These are parts of code that are executed when the `update()` method is called. Callbacks are stored as function pointers, but with C++17 support you can use lambda expressions with zero overhead as well. If a transition does not have a callback assigned, nothing will be done.

Currently one transition may have one callback. The transitions are:
- Released - both before and after updating the state remained LOW
- Pressed  - both before and after updating the state remained HIGH
- Rising   - from befor to after updating the state went from LOW to HIGH
- Falling  - from befor to after updating the state went from HIGH to LOW

Callbacks can be assigned by calling the appropriate function with an `on` prefix and the transition's name (for example: setting a callback to the rising state change can be done by calling the button's `onRising()` method, with the callback as the parameter).

## Example code:

```cpp
#include "utils/button.h"

Button button(GPIO_PIN_NUM);

void on_released_func() { /* Code to execute when button is released. */ }

void setup() {
// button setup:
  button.begin();
// callback with function pointer:
  button.onReleased( on_released_func );
// callback with lambda expression:
  button.onPressed( []() { /* Code to execute when button is pressed. */ } );
// you could also assign callback to button.onRising() and button.onFalling();
}

void loop() {
// preiodically called update() for correct behaviour:
  button.update();
}
```
