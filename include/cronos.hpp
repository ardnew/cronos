#ifndef src_cronos_h
#define src_cronos_h

#include <chrono>
#include <functional>
#include <iostream>

#if defined(ESP_PLATFORM)
#include <esp_timer.h>
#elif defined(ARDUINO)
#include <Arduino.h>
#endif

namespace native {
class ticker {
 public:
#if defined(ESP_PLATFORM)
#define fastcode IRAM_ATTR /* __attribute__((section(".iram1.text"))) */
  // ESP32 (FreeRTOS) ticks are in microseconds.
  static constexpr auto fastcode count = esp_timer_get_time;
  using period = std::micro;

#elif defined(ARDUINO)
  // It's likely there are many generic platforms that will have ARDUINO
  // defined. So be sure to check it last (but before the default C++
  // implementation).
#define fastcode
  // Arduino ticks are in milliseconds.
  static constexpr auto fastcode count = millis;
  using period = std::milli;

#else
#define fastcode
  // C++17 allows constexpr with lambda functions (instead of masquerading
  // around as a functor).
  static constexpr auto fastcode count = []() {
    return std::chrono::steady_clock::now().time_since_epoch().count();
  };
  using period = std::chrono::steady_clock::period;

#endif
  using rep = decltype(count());
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<ticker, duration>;

  static constexpr bool is_steady = true;

  // Return the number of ticks since epoch using the target's native
  // representation.
  //
  // For unhandled platforms, the default implementation returns current tick
  // count using std::chrono::steady_clock (C++11).
  static auto fastcode now() noexcept -> time_point {
    return time_point(duration(count()));
  }
};
}  // namespace native

// cronos wraps the highest-precision, monotonic, and stable tick counter
// available for the target device/platform.
//
// It provides methods for converting the native, internal representation of
// ticks to user-defined representations.
//
// Conversions are fully constexpr for compile-time evaluation (C++17).
// cronos implements the C++11 named requirements (C++20 is_clock constraint).
template <class ToDur = native::ticker::duration>
class cronos {
 public:
  using duration = ToDur;
  using rep = typename duration::rep;
  using period = typename duration::period;
  using time_point = std::chrono::time_point<cronos, duration>;
  using duration_c = std::chrono::seconds;

  static constexpr bool is_steady = true;

  // Return current time using the cronos's declared time representation, which
  // is converted from the target's native representation.
  static constexpr auto now() noexcept -> time_point {
    return time_point(std::chrono::duration_cast<duration>(
        native::ticker::duration(native::ticker::count())));
  }

  // Return current time using the given parameterized template representation.
  template <typename Rep, typename Period>
  static constexpr auto now() noexcept -> std::chrono::time_point<
      cronos<std::chrono::duration<Rep, Period>>,
      std::chrono::duration<Rep, Period>> {
    return cronos<std::chrono::duration<Rep, Period>>::now();
  }

  static constexpr auto uptime() noexcept -> duration {
    return now().time_since_epoch();
  }

  template <typename Rep, typename Period>
  static constexpr auto uptime() noexcept -> std::chrono::duration<Rep, Period> {
    return cronos<std::chrono::duration<Rep, Period>>::uptime();
  }

  static constexpr auto ticks() noexcept -> rep {
    return uptime().count();
  }

  template <typename Rep, typename Period>
  static constexpr auto ticks() noexcept -> Rep {
    return cronos<std::chrono::duration<Rep, Period>>::ticks();
  }

  //// POSIX C time_t API

  static constexpr std::time_t to_time_t(const time_point &t = now()) noexcept {
    return std::chrono::duration_cast<duration_c>(t.time_since_epoch()).count();
  }

  static constexpr auto from_time_t(std::time_t t) noexcept -> time_point {
    return time_point(duration_c(t));
  }
};

/// Convenience aliases for common time representations.

using msecu32_t = std::chrono::duration<uint32_t, std::milli>;
using mseci32_t = std::chrono::duration<int32_t, std::milli>;
using usecu64_t = std::chrono::duration<uint64_t, std::micro>;
using useci64_t = std::chrono::duration<int64_t, std::micro>;
using nsecu64_t = std::chrono::duration<uint64_t, std::nano>;
using nseci64_t = std::chrono::duration<int64_t, std::nano>;

static constexpr auto msecu32() -> cronos<msecu32_t>::duration {
  return cronos<msecu32_t>::uptime();
}
static constexpr auto mseci32() -> cronos<mseci32_t>::duration {
  return cronos<mseci32_t>::uptime();
}
static constexpr auto usecu64() -> cronos<usecu64_t>::duration {
  return cronos<usecu64_t>::uptime();
}
static constexpr auto useci64() -> cronos<useci64_t>::duration {
  return cronos<useci64_t>::uptime();
}
static constexpr auto nsecu64() -> cronos<nsecu64_t>::duration {
  return cronos<nsecu64_t>::uptime();
}
static constexpr auto nseci64() -> cronos<nseci64_t>::duration {
  return cronos<nseci64_t>::uptime();
}

#endif // src_cronos_h
