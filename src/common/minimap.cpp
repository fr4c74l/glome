#include <cmath>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>

#include "math.hpp"
#include "projectile.hpp"
#include "ship.hpp"
#include "shader.hpp"
#include "meridian.hpp"
#include "world.hpp"
#include "options.hpp"

#include "minimap.hpp"

using namespace std;
using namespace Options;

static Shader map_projection;
static Shader hud;

static GLuint tex_minimap;
static GLuint tex_object;

static GLint uniform_is_dot;
static GLint uniform_camera;

static GLuint square_vbo;

void
MiniMap::draw(int wstart, World* world, const Matrix4& center)
{
  const int t = 160;
  const int b = 10;
  const int l = 10;
  const int r = 160;

  // Calculate field of vision
  const float cx = 0.0f;
  const float cy = 0.0f;

  const float dx = 0.5f * width * sinf(FOV) / height;

  const float ppx0 = cx - dx;
  const float ppy = cy + cosf(asinf(dx));
  const float ppx1 = cx + dx;


  // Change to HUD display mode.
  glViewport(wstart + l, b, r, t);
  hud.enable();

  // Draw 2D green background.
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex_minimap);

  glVertexAttrib3f(hud.colorAttr(), 0.06f, 0.64f, 0.12f);

  glBindBuffer(GL_ARRAY_BUFFER, square_vbo);
  glVertexAttribPointer(hud.posAttr(), 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	{
		// Draw field of vision
		float pos[2*3] = {
				ppx0, ppy,
				cy, cy,
				ppx1, ppy
		};

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	  glDisable(GL_TEXTURE_2D);
	  glVertexAttribPointer(hud.posAttr(), 2, GL_FLOAT, GL_FALSE, 0, pos);
	  glDrawArrays(GL_LINE_STRIP, 0, 3);
	}

	{
		// Draw ship object
		const float a = 0.07;
		float pos[2*4] = {
				0, a,
				a, -a,
				0, (-a * 0.5f),
			  -a, -a
		};

		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		glVertexAttrib3f(hud.colorAttr(), 1.0f, 1.0f, 1.0f);
	  glVertexAttribPointer(hud.posAttr(), 2, GL_FLOAT, GL_FALSE, 0, pos);
	  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

  // Draw objects
  map_projection.enable();
  Matrix4 cam = yz_matrix(M_PI / 2) * center;
  cam.loadTo(uniform_camera);

  // Draw shots
  glUniform1i(uniform_is_dot, 0);
  Projectile::draw_in_minimap();
  draw_meridians(map_projection);

  glUniform1i(uniform_is_dot, 1);
  glBindTexture(GL_TEXTURE_2D, tex_object);
  world->fill_minimap();
  glBindTexture(GL_TEXTURE_2D, 0);

  // Disable 2D
  glDisable(GL_TEXTURE_2D);
  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, width, height);    
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void MiniMap::draw_dot(const Object& obj)
{
	map_projection.setTransform(obj.transformation());

	glVertexAttrib3f(map_projection.colorAttr(), 1.0f, 0.0f, 0.0f);
  glBindBuffer(GL_ARRAY_BUFFER, square_vbo);
  glVertexAttribPointer(map_projection.posAttr(), 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
	
void
create_circle_texture(int w, float p, int a0, int a1, GLuint& tex)
{
	struct elem {
		unsigned char l;
		unsigned char a;
	};
	int i, j;
	float cx, cy, d, tex_r, tex_r_lim;

	elem* texture = (elem*)malloc(w * w * sizeof(elem));

	cx = cy = (w - 1) / 2.;
	tex_r = w / 2.;
	tex_r_lim = tex_r * p;

	for(i = 0; i < w; ++i)
	{
		for(j = 0; j < w; ++j)
		{
			float x;
			d = sqrt(((i - cx) * (i - cx)) + ((j - cy) * (j - cy)));
			texture[(i * w) + j].l = 255u;
			texture[(i * w) + j + 1].a = (d > tex_r) ? a0 : ((d < tex_r_lim) ? a1 : ( (x = (tex_r - d) / (tex_r - tex_r_lim), x*x) * a1 ));
		}
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, w, w, 0,
			GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, (GLvoid*)texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(texture);
}

void
MiniMap::initialize()
{
	create_circle_texture(256, 0.9, 0, 142, tex_minimap);
	create_circle_texture(16, 0.8, 0, 255, tex_object);

	glGenBuffers(1, &square_vbo);
	{
		float v[] = {
				-1.0f, -1.0f,
				1.0f, -1.0f,
				-1.0f, 1.0f,
				1.0f, 1.0f
		};
		glBindBuffer(GL_ARRAY_BUFFER, square_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
	}

#include "minimap.vertex.glsl.hpp"
#include "minimap.fragment.glsl.hpp"
	map_projection.setup_shader(minimap_vertex_glsl, minimap_vertex_glsl_len, minimap_fragment_glsl, minimap_fragment_glsl_len);

	uniform_is_dot = glGetUniformLocation(map_projection.program(), "is_dot");
	uniform_camera = glGetUniformLocation(map_projection.program(), "camera");

#include "hud.vertex.glsl.hpp"
	hud.setup_shader(hud_vertex_glsl, hud_vertex_glsl_len, minimap_fragment_glsl, minimap_fragment_glsl_len);
}

