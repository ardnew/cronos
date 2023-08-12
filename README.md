## ticks
###### Convert period and type representing a system's native tick count

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
|C++|`chrono`|`std::chrono::steady_clock`|`int64_t`|Microseconds[^1]|

[^1]: Actual resolution may be less. The LSBs of the 64-bit system tick count will be padded with `0` if the system does not support microsecond resolution.
