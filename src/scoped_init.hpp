#ifndef ULTIMATE_PONG_SCOPED_INIT_HPP
#define ULTIMATE_PONG_SCOPED_INIT_HPP
namespace pong
{
  template <class Functor1, class Functor2>
  struct scoped_init
  {
  public:
    scoped_init(Functor1 func1, Functor2 func2) : func2_(func2)
    {
      try
      {
        func1();
      }
      catch(...)
      {
        throw;
      }
    }
    ~scoped_init()
    {
      try
      {
        this->func2_();
      }
      catch(...)
      {
        throw;
      }
    }
    scoped_init(const scoped_init&) = delete;
    scoped_init(scoped_init&&) = delete;
    scoped_init& operator=(const scoped_init&) = delete;
    scoped_init& operator=(scoped_init&&) = delete;
  private:
    Functor2 func2_;
  };
};
#endif
