#pragma once

#include "matrix4.hpp"

// Rotations from viewpoint

// This one is almost like glRotate, but in radians, and the vector must be normalized.
inline Matrix4 rotation(float angle, float x, float y, float z) {
  const float c = cos(angle);
  const float s = sin(angle);
  const float omc = 1.0 - c;

  return Matrix4
      (x*x*omc+c, x*y*omc-z*s, x*z*omc+y*s, 0.0,
       y*x*omc+z*s, y*y*omc+c, y*z*omc-x*s, 0.0,
       x*z*omc-y*s, y*z*omc+x*s, z*z*omc+c, 0.0,
       0.0, 0.0, 0.0, 1.0);
}

inline Matrix4 xy_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(c, -s, 0, 0,
                 s, c, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}

inline Matrix4 xz_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(c, 0, -s, 0,
                 0, 1, 0, 0,
                 s, 0, c, 0,
                 0, 0, 0, 1);
}

inline Matrix4 yz_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(1, 0, 0, 0,
                 0, c, -s, 0,
                 0, s, c, 0,
                 0, 0, 0, 1);
}

// "Translations" from viewpoint
inline Matrix4 xw_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(c, 0, 0, -s,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 s, 0, 0, c);
}

inline Matrix4 zw_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(1,0,0,0,
		 0,1,0,0,
		 0,0,c,-s,
		 0,0,s,c);
}

inline Matrix4 yw_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(1,0,0,0,
		 0,c,0,-s,
		 0,0,1,0,
		 0,s,0,c);
}