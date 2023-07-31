#ifndef types_ticks_h
#define types_ticks_h

#include "../ticks.h"

// Fixed-period ticks with native storage.
template <typename ToRep> using milliticks = ticks<ToRep, std::milli>;
template <typename ToRep> using microticks = ticks<ToRep, std::micro>;
template <typename ToRep> using nanoticks  = ticks<ToRep, std::nano>;

// Fixed-storage ticks with native period.
template <typename ToPer> using uticks16 = ticks<uint16_t, ToPer>;
template <typename ToPer> using uticks32 = ticks<uint32_t, ToPer>;
template <typename ToPer> using uticks64 = ticks<uint64_t, ToPer>;
template <typename ToPer> using fticks32 = ticks<float,    ToPer>;
template <typename ToPer> using fticks64 = ticks<double,   ToPer>;

#endif // types_ticks_h
