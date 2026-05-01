#pragma once

#include <stdint.h>

template <size_t N>
class SMA {
  public:
    SMA() : _sum(0), _index(0), _count(0) {
      for (size_t i = 0; i < N; ++i) _buffer[i] = 0;
    }

    uint16_t process(uint16_t newValue) {
      _sum -= _buffer[_index];
      _buffer[_index] = newValue;
      _sum += newValue;

      _index = (_index + 1) % N;
      if (_count < N) {
        _count++;
      }

      return (uint16_t)(_sum / _count);
    }

  private:
    uint16_t _buffer[N];
    uint32_t _sum;
    size_t _index;
    size_t _count;
};
