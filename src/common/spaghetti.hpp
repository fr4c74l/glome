#pragma once

#include "object.hpp"

#define SPAGHETTI_COUNT 100 // Number of cubic Bézier curves

class Spaghetti: public Object
{
public:
  Spaghetti();

  void draw(const Shader& s);
  void update();
private:
  Matrix4 velo;

  Vector4 bezier[(SPAGHETTI_COUNT * 3)+2];
  union {
    struct {
      GLuint ibo, vbo;
    };
    GLuint bufobjs[2];
  };
};
