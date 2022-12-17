/// @author @lotkey Chris McVickar
#pragma once

#include <mutex>
#include <utility> // std::forward, std::move

namespace transmitter {
template<class T>
/// Class to wrap an object and a mutex to guard it
class Guarded {
public:
  Guarded(Guarded<T> const&) = default;
  Guarded(Guarded<T>&&) = default;
  Guarded<T>& operator=(Guarded<T> const&) = default;
  Guarded<T>& operator=(Guarded<T>&&) = default;
  virtual ~Guarded() = default;

  /// Construct the wrapped object with the provided arguments
  template<class... Args>
  Guarded(Args&&... args);
  /// Copy assignment for the wrapped object
  Guarded<T>& operator=(T const&);
  /// Move assignment for the wrapped object
  Guarded<T>& operator=(T&&);

  /// Lock the object's mutex
  void lock();
  /// Unlock the object's mutex
  void unlock();

  /// Returns a copy of the wrapped object
  operator T() const;
  /// Returns a reference to the wrapped object
  T& get();
  /// Returns a const reference to the wrapped object
  T const& get() const;
  /// Returns a reference to the wrapped object
  operator T&();
  /// Returns a const reference to the wrapped object
  operator T const&() const;
  /// Returns a reference to the wrapped object
  T& operator*();
  /// Returns a const reference to the wrapped object
  T const& operator*() const;
  /// Returns a pointer to the wrapped object
  T* operator->();
  /// Returns a const pointer to the wrapped object
  T const* operator->() const;

private:
  std::mutex m_mtx;
  T m_t;
};

template<class T>
template<class... Args>
Guarded<T>::Guarded(Args&&... args) : m_t(std::forward<Args&&...>(args)...)
{}

template<class T>
Guarded<T>& Guarded<T>::operator=(T const& t)
{
  m_t = t;
  return *this;
}

template<class T>
Guarded<T>& Guarded<T>::operator=(T&& t)
{
  m_t = std::move(t);
  return *this;
}

template<class T>
Guarded<T>::operator T() const
{
  return m_t;
}

template<class T>
Guarded<T>::operator T&()
{
  return m_t;
}

template<class T>
Guarded<T>::operator T const&() const
{
  return m_t;
}

template<class T>
T& Guarded<T>::operator*()
{
  return m_t;
}

template<class T>
T const& Guarded<T>::operator*() const
{
  return m_t;
}

template<class T>
T* Guarded<T>::operator->()
{
  return &m_t;
}

template<class T>
T const* Guarded<T>::operator->() const
{
  return &m_t;
}

template<class T>
void Guarded<T>::lock()
{
  m_mtx.lock();
}

template<class T>
void Guarded<T>::unlock()
{
  m_mtx.unlock();
}

template<class T>
T& Guarded<T>::get()
{
  return m_t;
}

template<class T>
T const& Guarded<T>::get() const
{
  return m_t;
}
} // namespace transmitter