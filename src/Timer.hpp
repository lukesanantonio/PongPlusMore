#pragma once
#include <chrono>
namespace pong
{
  template <class ClockType = std::chrono::high_resolution_clock>
  class Timer
  {
  public:
    template <class DurationType = typename ClockType::duration>
    bool hasBeen(DurationType duration) const;

    void reset();
  private:
    std::chrono::time_point<ClockType> time_before_ =
                                 std::chrono::high_resolution_clock::now();
  };

  template <class ClockType>
  template <class DurationType>
  bool Timer<ClockType>::hasBeen(DurationType duration) const
  {
    return std::chrono::high_resolution_clock::now() -
                 this->time_before_ >= duration ? true : false;
  }
  template <class ClockType>
  void Timer<ClockType>::reset()
  {
    this->time_before_ = std::chrono::high_resolution_clock::now();
  }
}
