/*!
 * \file scoped_init.hpp
 * \brief Declaration for our RAII style functor wrapper.
 */
#ifndef ULTIMATE_PONG_SCOPED_INIT_HPP
#define ULTIMATE_PONG_SCOPED_INIT_HPP
namespace pong
{
  /*!
   * \brief Ensures a functor is called at the end of the scope.
   *
   * \tparam FunctorType The type of the functor of which to call when the scope
   * ends. **Must be CopyConstructable**.
   */
  template <class FunctorType>
  struct scoped_init
  {
  public:
    /*!
     * \brief Stores the functor passed in for later.
     *
     * \post scoped_init::func_ == `func` (parameter).
     */
    scoped_init(FunctorType func) : func_(func){}

    /*!
     * \brief Calls scoped_init::func_ and throws any exceptions up the stack.
     *
     * That's right a throwing destructor, the best way to ensure that it will
     * not throw is by ensuring your passed functor doesn't throw.
     *
     * If you don't like that there is a possibility that this destructor
     * throws, you can screw yourself - *Luke*.
     */
    ~scoped_init()
    {
      try
      {
        this->func_();
      }
      catch(...)
      {
        throw;
      }
    }

    /*!
     * \brief Deleted copy constructor.
     */
    scoped_init(const scoped_init&) = delete;
    /*!
     * \brief Deleted move constructor.
     */
    scoped_init(scoped_init&&) = delete;
    /*!
     * \brief Deleted copy assignment operator.
     */
    scoped_init& operator=(const scoped_init&) = delete;
    /*!
     * \brief Deleted move assignment operator.
     */
    scoped_init& operator=(scoped_init&&) = delete;
  private:
    /*!
     * \brief The functor to call on destruction of the `this` object.
     */
    FunctorType func_;
  };
};
#endif
