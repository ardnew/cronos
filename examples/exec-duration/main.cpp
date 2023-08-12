// Based on the example for std::chrono::steady_clock::now here:
//   https://en.cppreference.com/w/cpp/chrono/steady_clock/now

#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <ticks.hpp>

// Measure the time it takes to iteratively allocate:
//  {repeat} × std::vector<int>'s with {length} elements
static constexpr auto length = 1'000ULL;   //   1K × int elements
static constexpr auto repeat = 100'000ULL; // 100K × vectors

// Writing to a global potentially has side-effects, so the compiler won't try
// to optimize away the work being measured.
volatile int sink;

int main() {
  // Record start time in native ticks
  auto start = ticker<>::now();
  for (auto i = 0ULL; i < repeat; ++i) {
    // do some work
    std::vector<int> v(length, 42);
    sink = std::accumulate(v.begin(), v.end(), 0u);
  }
  // Record end time in native ticks
  auto end = ticker<>::now();

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
            << std::setw(9) << pctDiff.count() << " % of 1s\n"
            << std::setw(9) << msDiff.count() << " ms\n";
}
