#ifndef ticks_h
#define ticks_h

#include <chrono>
#include <cstdint>

struct native {
#if defined(ARDUINO)
#include <Arduino.h>
#define _get (millis) // Arduino ticks are in milliseconds.
  using per = std::milli;
#elif defined(ESP_PLATFORM)
#include <esp_timer.h> // ESP32 (FreeRTOS) ticks are in microseconds.
#define _get (esp_timer_get_time)
  using per = std::micro;
#endif
  // Type dur is a std::chrono::duration represented using the platform's native
  // timer API. The precision and period of all templated ticks classes are
  // limited to dur internally, but they can be accessed and used according to
  // the parameters of ticks they were declared with.
  using rep = decltype(_get());
  using dur = std::chrono::duration<rep, per>;

  template <typename ToRep, typename ToPer>
  static inline constexpr const std::chrono::duration<ToRep, ToPer> &
  at(const dur &d) {
    return std::chrono::duration_cast<std::chrono::duration<ToRep, ToPer>>(d);
  }
  template <typename ToRep, typename ToPer>
  static inline const std::chrono::duration<ToRep, ToPer> &now() {
    return at<ToRep, ToPer>(dur(_get()));
  }

  // Give all generated ticks classes access to all native members.
  template <typename ToRep, typename ToPer> friend struct ticks;
};

// The current platform's native ticks are always in periods of 1× native::per
// represented with type native::rep.
// If the user chooses to not specify template parameters, the default period
// and storage representation from native are used.
// Otherwise, the user can specify a period and representation to use, and the
// appropriate conversion will be performed automatically.
template <typename ToRep = native::rep, typename ToPer = native::per>
struct ticks : std::chrono::duration<ToRep, ToPer> {
  using std::chrono::duration<ToRep, ToPer>::duration;
  using rep = ToRep;
  using per = ToPer;
  inline ticks():
    std::chrono::duration<ToRep, ToPer>(native::now<ToRep, ToPer>()) {}
};

#endif // ticks_h
