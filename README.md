## ticks
###### Convert period and type representing a system's native tick count

## Usage
Converting the tick representation for a supported system is done via template parameters:

```c++
include <ticks.hpp>

// Declare an alias of ticks with the desired period and type:
using milliu32 = ticks<std::chrono::duration<uint32_t, std::chrono::milli>>;

// Or use the native storage type with a different period:
using seconds = ticks<std::chrono::seconds>;

// Or change the period of the native type 
//  (Note that this will not magically adjust tick frequency, just its resolution)
using twoShakesOfALambsTail = ticks<std::chrono::duration<float, 
    std::ratio_multiply<std::ratio<7, 10>, std::chrono::seconds::period>>;
        // (2 × (7/11 × 1_sec)) <≈> (2 × (0.636364F_sec)) <≈> 1.272727F_sec
```

With your types defined, retrieving tick counts is done via static method `count`:

```c++
auto n1 = milliu32::count();
auto n2 = seconds::count();
auto n3 = twoShakesOfALambsTail::count();

// TODO: get rid of this ridiculous example and start over
```

