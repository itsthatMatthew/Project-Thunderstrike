#ifndef BUZZER_MODULE_HEADER_INCLUDED
#define BUZZER_MODULE_HEADER_INCLUDED

#include "module.h"

namespace PTS {

template<uint32_t BUZZER_TONE>
class BuzzerModule : public Module<> {
 public:
  explicit BuzzerModule(const char *const name, const uint8_t pin) : Module(name), pin_(pin) { }

  void begin() const override {
    pinMode(pin_, OUTPUT);
  }

  void threadFunc() const override {
    digitalWrite(pin_, HIGH);
    delayMicroseconds(1000000 / BUZZER_TONE);
    digitalWrite(pin_, LOW);
    delayMicroseconds(1000000 / BUZZER_TONE);
  }
 private:
  const uint8_t pin_;
}; // class Buzzer

}; // namespace PTS

#endif /* BUZZER_MODULE_HEADER_INCLUDED */