#ifndef EUCLIDEANVECTOR_H
#define EUCLIDEANVECTOR_H
#include <cassert>
#include <utility>

//A Euclidean vector
template <class T>
struct EuclideanVector
{
  EuclideanVector(const T& any_x, const T& any_y)
    : x { any_x }, y { any_y }
  {

  }
  EuclideanVector(const std::pair<T,T>& p)
    : x { p.first }, y { p.second }
  {

  }
  EuclideanVector(const EuclideanVector<T>& rhs)
    : x { rhs.x }, y { rhs.y }
  {

  }
  EuclideanVector& operator+=(const EuclideanVector& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  EuclideanVector& operator*=(const double f)
  {
    x*=f;
    y*=f;
    return *this;
  }

  T x;
  T y;
};

template <class T>
bool operator==(const EuclideanVector<T>& lhs, const EuclideanVector<T>& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <class T>
bool operator!=(const EuclideanVector<T>& lhs, const EuclideanVector<T>& rhs)
{
  return !(lhs == rhs);
}

template <class T>
const EuclideanVector<T> operator+(const EuclideanVector<T>& lhs, const EuclideanVector<T>& rhs)
{
  return EuclideanVector<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <class T, class U, class V>
const EuclideanVector<T> operator/(const EuclideanVector<U>& lhs, const V& rhs)
{
  #ifndef NDEBUG
  { const int x = int {}; assert(x == 0); }
  { const double x = double {}; assert(x == 0.0); }
  #endif
  return EuclideanVector<T>(lhs.x / rhs, lhs.y / rhs);
}



#endif // EUCLIDEANVECTOR_H