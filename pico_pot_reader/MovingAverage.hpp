#pragma once
#include <array>
#include <cstddef>
#include <type_traits>

template <typename T, std::size_t LENGTH>
class MovingAverage
{
  static_assert(LENGTH > 0, "LENGTH must be > 0");

private:
  using SumType = std::conditional_t<std::is_integral_v<T>, long long, T>;

  SumType sum = 0;
  std::size_t index = 0;
  std::size_t count = 0;
  std::array<T, LENGTH> buffer{};

public:
  T operator()(T v) noexcept
  {
    sum -= buffer[index];
    buffer[index] = v;
    sum += v;

    index = (index + 1) % LENGTH;

    if (count < LENGTH)
      ++count;

    return static_cast<T>(sum / count);
  }
};