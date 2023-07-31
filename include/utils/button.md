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

## The fancy stuff

Buttons support extra features as template arguments for minimal overhead. These two are the callback types and delay policy. Let's walk through what these two means if you wanted to use them.

### Delay policies

Delay policies are here for one main reason: for debouncing your circuit in software. Since you might miss hardware implementation or have limited resources to do so, a simple delay might be enough for a shell-proof design. It's the first template parameters, the default value being the `NoDelayPolicy` struct. This simply adds no delay, with zero overhead. For your implementation, simply declare a struct with a single `static constexpr void delay()` method, this will be called once in `update()`, and this declaration makes sure to have minimal overhead.

### Callback return type & arguments

Since callback types are templated, you can tailor them to your needs. The second template argument for the `Button` class is the desired return type, the default being `void`. Any additional parameters after this will determine the argument list for the callbacks, which can be passed in the `update()` method. Beware that all callbacks must share the same declaration.

## Example for a Button object with specific template arguments:

```cpp
#include "utils/button.h"

// A custom delay struct, yours should look alike (templates for params and constexpr)
template<uint32_t millis>
struct ArduinoDelayPolicy {
  static constexpr void delay() { ::delay(millis); }
};

Button<ArduinoDelayPolicy<100>, int, float, double> button(GPIO_PIN_NUM);
//     ^                  ^     ^    ^-- these are the callback arguments
//     \                  \     \-- this is the return type of the callback
//     \                  \-- the delay will be set to 100 ms
//     \-- this will be the delay policy

void setup() {
  button.begin();

// callback with lambda expression, correct paramteres and return type:
  button.onRising( [](float, double) -> int { /* Code to execute when button is pressed. */ } );
}

void loop() {
// preiodically called update() for correct behaviour:
  int x = button.update(1.41f, 3.14);
}
```