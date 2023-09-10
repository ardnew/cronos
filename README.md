## cronos
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
|C++|`std::chrono`|`std::chrono::steady_clock`|`int64_t`|Microseconds[^1]|

[^1]: Actual resolution may be less. The LSBs of the 64-bit system tick count will be padded with `0` if the system does not support microsecond resolution.
