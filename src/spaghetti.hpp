#pragma once

#include "drawable.hpp"

#define SPAGHETTI_COUNT 100 // Number of cubic Bézier curves

class Spaghetti: public Drawable
{
public:
  Spaghetti();

  void draw();
  void update();
private:
  Matrix4 velo;

  Vector4 p[(SPAGHETTI_COUNT * 3)+2];
};
