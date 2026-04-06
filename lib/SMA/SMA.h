#include <stdio.h>

template <size_t N>
class SMA {
  public:
    SMA() : m_sum(0), m_index(0) {
      for (int i = 0; i < N; ++i) m_buffer[i] = 0;
    }

    uint16_t process(uint16_t newValue) {
      m_sum -= m_buffer[m_index];
      m_buffer[m_index] = newValue;
      m_sum += newValue;

      m_index = (m_index + 1) % N;

      return (uint16_t)(m_sum / N);
    }

  private:
    uint16_t m_buffer[N];
    uint32_t m_sum;
    int m_index;
};