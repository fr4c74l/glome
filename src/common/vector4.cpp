#include <iostream>

#include "vector4.hpp"

const Vector4 Vector4::ORIGIN = Vector4(0.0f, 0.0f, 0.0f, -1.0f);

std::ostream&
operator<<(std::ostream& o, const Vector4& v)
{
  o << v.v[0];
  for(size_t j = 1; j < 4; ++j)
    {
      o << ", " << v.v[j];
    }
  o << '\n';
  return o;
}
