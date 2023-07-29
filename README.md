## ticks
###### Convert period and type representing a system's native tick count

### Usage
Converting the tick representation for a supported system is done via template parameters:

```c++
include <ticks.h>

// Declare an alias of ticks with the desired period and type:
using milliu32 = ticks<uint32_t, std::chrono::milli>;

// Or use the native period with a different type:
using ticksi64 = ticks<int64_t>;

// Or change the period of the native type 
//  (Note that this will not magically adjust tick frequency, just its resolution)
using twoShakesOfALambsTail = std::duration<float, 
    std::ratio_multiply<std::ratio<7, 10>, std::chrono::seconds::period>;
        // (2 × (7/11 × 1_sec)) <≈> (2 × (0.636364F_sec)) <≈> 1.272727F_sec
```

With your types defined, retrieving tick counts is done via constructor and the
count method:

```c++
auto  = milliu32();
auto  = ticksi64();
auto  = twoShakesOfALambsTail();

// TODO: proceed with ridiculous example
```

