#ifndef stdint_ticks_h
#define stdint_ticks_h

#include "ticker.h"

using msecu32_t = std::chrono::duration<uint32_t, std::milli>;
using mseci32_t = std::chrono::duration<int32_t, std::milli>;
using usecu64_t = std::chrono::duration<uint64_t, std::micro>;
using useci64_t = std::chrono::duration<int64_t, std::micro>;
using nsecu64_t = std::chrono::duration<uint64_t, std::nano>;
using nseci64_t = std::chrono::duration<int64_t, std::nano>;

static constexpr auto msecu32() -> ticker<msecu32_t>::duration {
  return ticker<msecu32_t>::uptime();
}
static constexpr auto mseci32() -> ticker<mseci32_t>::duration {
  return ticker<mseci32_t>::uptime();
}
static constexpr auto usecu64() -> ticker<usecu64_t>::duration {
  return ticker<usecu64_t>::uptime();
}
static constexpr auto useci64() -> ticker<useci64_t>::duration {
  return ticker<useci64_t>::uptime();
}
static constexpr auto nsecu64() -> ticker<nsecu64_t>::duration {
  return ticker<nsecu64_t>::uptime();
}
static constexpr auto nseci64() -> ticker<nseci64_t>::duration {
  return ticker<nseci64_t>::uptime();
}

#endif // stdint_ticks_h
