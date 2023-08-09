#ifndef ticks_hpp
#define ticks_hpp

#include <chrono>
#include <functional>
#include <iostream>

namespace native {
class ticker {
 public:
#if defined(ESP_PLATFORM)
#include <esp_timer.h>  // ESP32 (FreeRTOS) ticks are in microseconds.
  static constexpr auto ticks = esp_timer_get_time;
  using period = std::micro;

#elif defined(ARDUINO)
  // It's likely there are many generic platforms that will have ARDUINO
  // defined. So be sure to check it last (but before the default C++
  // implementation).
#include <Arduino.h>  // Arduino ticks are in milliseconds.
  static constexpr auto ticks = millis;
  using period = std::milli;

#else
  static constexpr auto ticks = []() {
    return std::chrono::steady_clock::now().time_since_epoch().count();
  };
  using period = std::chrono::steady_clock::period;

#endif
  using rep = decltype(ticks());
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<ticker, duration>;

  static constexpr bool is_steady = true;

  // Return the number of ticks since epoch using the target's native
  // representation.
  //
  // For unhandled platforms, the default implementation returns current tick
  // count using std::chrono::steady_clock (C++11).
  static constexpr auto now() noexcept { return time_point(duration(ticks())); }
};
}  // namespace native

// ticker wraps the highest-precision, monotonic, and stable tick counter
// available for the target device/platform.
//
// It provides methods for converting the native, internal representation of
// ticks to user-defined representations.
//
// Conversions are fully constexpr for compile-time evaluation (C++17).
// ticker implements the C++11 named requirements (C++20 is_clock constraint).
template <class ToDur = native::ticker::duration>
class ticker {
 public:
  using duration = ToDur;
  using rep = typename duration::rep;
  using period = typename duration::period;
  using time_point = std::chrono::time_point<ticker, duration>;
  using duration_c = std::chrono::seconds;

  static constexpr bool is_steady = true;

  // Return current time using the ticker's declared time representation, which
  // is converted from the target's native representation.
  static constexpr auto now() noexcept {
    return time_point(std::chrono::duration_cast<duration>(
        native::ticker::duration(native::ticker::ticks())));
  }

  // Return current time using the given parameterized template representation.
  template <typename Rep, typename Period>
  static constexpr auto now() noexcept {
    return ticker<std::chrono::duration<Rep, Period>>::now();
  }

  //// POSIX C time_t API

  static constexpr std::time_t to_time_t(const time_point &t = now()) noexcept {
    return std::chrono::duration_cast<duration_c>(t.time_since_epoch()).count();
  }

  static constexpr auto from_time_t(std::time_t t) noexcept {
    return time_point(duration_c(t));
  }
};

#endif // ticks_hpp
