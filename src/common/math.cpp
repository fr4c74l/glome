#include "matrix4.hpp"
#include "math.hpp"

Matrix4
rotation(float angle, float x, float y, float z) {
  const float c = cos(angle);
  const float s = sin(angle);
  const float omc = 1.0 - c;

  return Matrix4
      (x*x*omc+c, x*y*omc-z*s, x*z*omc+y*s, 0.0,
       y*x*omc+z*s, y*y*omc+c, y*z*omc-x*s, 0.0,
       x*z*omc-y*s, y*z*omc+x*s, z*z*omc+c, 0.0,
       0.0, 0.0, 0.0, 1.0);
}

Matrix4
perspective(float fovy, float aspect, float zNear, float zFar)
{
	float f = 1.0f / tan(fovy / 2.0);
	float dif = zNear - zFar;
	return Matrix4(
			f/aspect, 0.0f, 0.0f, 0.0f,
			0.0f, f, 0.0f, 0.0f,
			0.0f, 0.0f, (zFar + zNear) / dif, 2.0 * zFar * zNear / dif,
			0.0f, 0.0f, -1.0f, 0.0f
			);
}

Matrix4
xy_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(c, -s, 0, 0,
                 s, c, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}

Matrix4
xz_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(c, 0, -s, 0,
                 0, 1, 0, 0,
                 s, 0, c, 0,
                 0, 0, 0, 1);
}

Matrix4
yz_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(1, 0, 0, 0,
                 0, c, -s, 0,
                 0, s, c, 0,
                 0, 0, 0, 1);
}

Matrix4
xw_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(c, 0, 0, -s,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 s, 0, 0, c);
}

Matrix4
zw_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(1,0,0,0,
		 0,1,0,0,
		 0,0,c,-s,
		 0,0,s,c);
}

Matrix4
yw_matrix(float angle) {
  const float c = cos(angle);
  const float s = sin(angle);
  return Matrix4(1,0,0,0,
		 0,c,0,-s,
		 0,0,1,0,
		 0,s,0,c);
}
