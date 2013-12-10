#include "openfoamcellindex.h"

#include <cassert>
#include <iostream>

#include "trace.h"

ribi::foam::CellIndex::CellIndex(const int index)
  : m_index(index)
{
  #ifndef NDEBUG
  Test();
  #endif

  assert(m_index >= 0
    && "A CellIndex must be zero or a positive value");
}

#ifndef NDEBUG
void ribi::foam::CellIndex::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::foam::CellIndex::Test");
  CellIndex a(2);
  CellIndex b(2);
  CellIndex c(3);
  assert(a == a);
  assert(a == b);
  assert(b == a);
  assert(b == b);
  assert(b != c);
  assert(c != b);
  assert(c == c);

  TRACE("Finished ribi::foam::CellIndex::Test successfully");

}
#endif

std::ostream& ribi::foam::operator<<(std::ostream& os, const CellIndex& face_index)
{
  os << face_index.Get();
  return os;
}

std::istream& ribi::foam::operator>>(std::istream& is, CellIndex& face_index)
{
  is >> face_index.m_index;
  return is;
}

bool ribi::foam::operator==(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return lhs.Get() == rhs.Get();
}

bool ribi::foam::operator!=(const CellIndex& lhs, const CellIndex& rhs) noexcept
{
  return !(lhs == rhs);
}
