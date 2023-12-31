// Based on the example for std::chrono::steady_clock::now here:
//   https://en.cppreference.com/w/cpp/chrono/steady_clock/now

// Expected output from building and running this example:
//
//   |  $ cd examples/exec-duration
//   |  $ make
//   | g++ -I../../src -g -Wall -std=gnu++17   -c -o main.o main.cpp
//   | g++  -o exec-duration main.o
//   |  $ ./exec-duration
//   | Time to iteratively allocate 100000 vectors of 1000 ints:
//   |                  115.626 % of 1s
//   |                     1156 ms
//   | Look, no long-winded call to narrow-cast:
//   |         1212134307664108 ns
//   |               1212134307 ms

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <cronos.h>

// Measure the time it takes to iteratively allocate:
//  {repeat} × std::vector<int>'s with {length} elements
static constexpr auto length = 1'000ULL;   //   1K × int elements
static constexpr auto repeat = 100'000ULL; // 100K × vectors

// Writing to a global potentially has side-effects, so the compiler won't try
// to optimize away the work being measured.
volatile int sink;

int main() {
  // Record start time in native ticks
  auto start = cronos<>::now();
  for (auto i = 0ULL; i < repeat; ++i) {
    // do some work
    std::vector<int> v(length, 42);
    sink = std::accumulate(v.begin(), v.end(), 0u);
  }
  // Record end time in native ticks
  auto end = cronos<>::now();

  // Now we can express the time elapsed in any arbitrary unit of time and data
  // type we prefer.
  //
  // (Note: the standard librray duration is always defined as a ratio of 1s.)

  // This example uses a double precision float to represent time elapsed in
  // percentage of 1 second (e.g., 100% = 1s, 50% = 0.5s, 0.25% = 2.5ms).
  //
  // You can always convert to floating point without a cast:
  std::chrono::duration<double, std::ratio<1, 100>> pctDiff = end - start;
  // Otherwise, you must cast if the destination loses precision:
  auto msDiff =
      std::chrono::duration_cast<std::chrono::duration<uint32_t, std::milli>>(
          end - start);
  std::cout << "Time to iteratively allocate " << repeat << " vectors of "
            << length << " ints:\n"
            << std::setw(24) << pctDiff.count() << " % of 1s\n"
            << std::setw(24) << msDiff.count() << " ms\n";

  // Instead of calling duration_cast manually, you can also give the same
  // template arguments to any type of tick's now() or ticks() methods:
  using nanosecs = cronos<std::chrono::nanoseconds>;

  std::cout << "Look, no long-winded call to narrow-cast:\n"
            << std::setw(24) << nanosecs::ticks() << " ns\n"
            << std::setw(24) << nanosecs::ticks<int, std::milli>() << " ms\n";
}
