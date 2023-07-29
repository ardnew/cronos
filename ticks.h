#ifndef ticks_h
#define ticks_h

#include <chrono>
#include <cstdint>

class native {
  friend struct ticks;

protected:
#if defined(ARDUINO)
#include <Arduino.h>
  static const auto get = millis;
  using per = std::milli;
#elif defined(ESP_PLATFORM)
#include <esp_timer.h>
  static const auto get = esp_timer_get_time;
  using per = std::micro;
#endif
  // A std::chrono::duration wrapping the platform's native timer API.
  using rep = decltype(get());
  using duration = std::chrono::duration<rep, per>;
  template <typename ToRep, typename ToPer>
  static inline constexpr const std::chrono::duration<ToRep, ToPer> &
  at(const duration &d) {
    return std::chrono::duration_cast<std::chrono::duration<ToRep, ToPer>>(d);
  }
  template <typename ToRep, typename ToPer>
  static inline const std::chrono::duration<ToRep, ToPer> &now() {
    return at<ToRep, ToPer>(duration(get()));
  }
};

// The current platform's native ticks — in periods of 1× native::per
// represented with type native::rep — are translated at compile-time into the
// desired ticks — in periods of 1× ToPer, represented with type ToRep —
// according to the following template parameters.
template <typename ToRep = native::rep, typename ToPer = native::per>
struct ticks : std::chrono::duration<ToRep, ToPer> {
  inline ticks() : std::chrono::duration(native::now<ToRep, ToPer>()) {}
};

#endif // ticks_h
