## cronos[^1]
###### A std::chrono wrapper for target system's native tick count

This abstraction allows applications to hide the exact data type representation and period of the system's native ticker.
This provides a consistent interface for specifying delays, timeouts, etc., regardless of the target system.

## Usage
This library is provided as a single C++17 header file.

See the [example](examples/exec-duration) for general usage and notes until proper documentation is written.

## Targets
The native system tick capability is used on the following target platforms.
The default C++ API at the bottom of the table is used for all other platforms.

|   |Header|API|Representation|Period|
|:-:|:----:|:-:|:------------:|:----:|
|Arduino|`Arduino.h`|`unsigned long millis()`|`unsigned long`|Milliseconds|
|ESP-IDF|`esp_timer.h`|`int64_t esp_timer_get_time(void)`|`int64_t`|Microseconds|
|C++|`std::chrono`|`std::chrono::steady_clock`|`int64_t`|Microseconds[^2]|

#### Integration
###### Manual<sub>**_A_**</sub>
Fetch repository, add `#include <cronos.h>` to your sources, and modify your compiler's includes — with a [Makefile, for example](examples/exec-duration/Makefile):

```Makefile
CFLAGS += -I/foo/cronos/src -g -Wall
CXXFLAGS += $(CFLAGS) -std=gnu++17
```
###### Manual<sub>**_B_**</sub>
Fetch repository, copy [`src/cronos.h`](src/cronos.h) into your project, add `#include "cronos.h"` to your sources.

###### Arduino
This [library](library.properties) conforms to the [Arduino Library Specification (1.5)](https://arduino.github.io/arduino-cli/0.34/library-specification/). Install using the Library Manager via [IDE](https://github.com/arduino/arduino-ide) (or [GUI](https://github.com/arduino/Arduino)) or with command `arduino-cli lib install cronos` using the [CLI](https://github.com/arduino/arduino-cli).

###### ESP-IDF <sub>**_TODO_**</sub>
- [ ] _Add support files required to implement an ESP-IDF library/component_.

[^1]: Not a typo; an empathetic nod to [Jesús Gris](https://en.wikipedia.org/wiki/Cronos_(film)).
[^2]: Actual resolution may be less. The LSBs of the 64-bit system tick count will be padded with `0` if the system does not support microsecond resolution.
