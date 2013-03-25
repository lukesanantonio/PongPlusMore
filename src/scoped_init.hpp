/*!
 * \file scoped_init.hpp
 * \brief File containing the RAII style functor adapter.
 */
#ifndef ULTIMATE_PONG_SCOPED_INIT_HPP
#define ULTIMATE_PONG_SCOPED_INIT_HPP
namespace pong
{
  /*!
   * \brief Wrapper of a pair of init-uninit functors used to make them
   * exception-safe.
   *
   * As in, a RAII style class adapting two functors. One for initialization
   * of any given resource, the other for uninitialization of the resource.
   *
   * \tparam Functor1 The type of the first functor to call on construction.
   * \tparam Functor2 The type of the second functor to call on destruction.
   */
  template <class Functor1, class Functor2>
  struct scoped_init
  {
  public:
    /*!
     * \brief Runs the first argument, then stores the second for later. The
     * functors are required to be CopyConstructable, otherwise a compile
     * error will surface as a long template error (I assume).
     *
     * \param[in] func1 Called immediately, then discarded. Any thrown
     * exceptions will be caught and rethrown.
     * \param[in] func2 Stored, called on destruction. Should not throw,
     * otherwise the destructor might throw, which most will not want.
     *
     * \note All arguments of these functor's operator() are encouraged, *erm*,
     * required, to be binded before being passed in, that is because both
     * functors are called without arguments.
     */
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
    
    /*!
     * \brief Calls scoped_init::func2_ of the `this` object. Exceptions, if
     * any, *will be thrown,* to avoid this, catch exceptions in the function
     * itself.
     */
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
    
    /*!
     * \brief Copy constructor deleted, as to guarantee that the pair of
     * functions will each be called only once.
     */
    scoped_init(const scoped_init&) = delete;
    /*!
     * \brief Move constructor deleted, as to guarantee that the pair of
     * functions will each be called only once.
     */
    scoped_init(scoped_init&&) = delete;
    
    /*!
     * \brief Copy assignment operator deleted, as to guarantee that the pair
     * of functions will each be called only once.
     */
    scoped_init& operator=(const scoped_init&) = delete;
    /*!
     * \brief Move assignment operator deleted, as to guarantee that the pair
     * of functions will each be called only once.
     */
    scoped_init& operator=(scoped_init&&) = delete;
  private:
    /*!
     * \brief The assumed to be uninitialization functor set during
     * construction and run during destruction.
     */
    Functor2 func2_;
  };
};
#endif
