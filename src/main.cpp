#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "4dmath.hpp"
#include "matrix4.hpp"
#include "minimap.hpp"
#include "meridian.hpp"
#include "ship.hpp"
#include "projectile.hpp"
#include "init_gl.hpp"
#include "udp_server.hpp"
#include "net_world.hpp"
#include "split_world.hpp"
#include "jsinput.hpp"

#include "main.hpp"

using namespace std;
using namespace boost::asio::ip;

const int WIDTH = 800;
const int HEIGHT = 600;

const float FOV = 45.0f;

boost::asio::io_service gIOService;
bool isServer;
bool isClient;
string host;
short port;
World* world;
bool isSplit;

void main_loop()
{
  const int FPS = 60;
  uint64_t frame_count = 0;
  bool running = true;
  Uint32 ticks = SDL_GetTicks();

  while(running) {
    running = world->update();

    world->draw();
    SDL_GL_SwapBuffers();

    // Fix framerate
    {
      // TODO: maybe clk_div is useful here...
      const int period = 1000/60;
      Uint32 now = SDL_GetTicks();
      int delay = period - int(now - ticks);
      if(delay > 0)
        SDL_Delay(delay);
      ticks = now;
    }
    ++frame_count;
  }

  cout << frame_count << " frames rendered." << endl;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  // SDL startup
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) != 0) {
    cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
    return 1;
  }
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_OPENGL /*| SDL_FULLSCREEN*/);
  SDL_WM_SetCaption("Navigna", NULL);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_JoystickEventState(SDL_ENABLE);

  init_gl();

  // OpenGL nonchanging settings
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glLineWidth(1.5f);
  glAlphaFunc(GL_NOTEQUAL, 0.0f);

  // Fancy OpenGL fog
  {
    const float fcolor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fcolor);
  }
  glFogi(GL_FOG_MODE, GL_EXP);
  glFogf(GL_FOG_DENSITY, 0.9f);
  glHint(GL_FOG_HINT, GL_NICEST);
  glFogf(GL_FOG_END, 3.0f);
  glFogf(GL_FOG_START, 2.0f);
  glEnable(GL_FOG);

  // Configure network
  if(argc == 1)
  {
    isServer = isClient = false;
    isSplit = true;
  }
  else if(argc == 4)
  {
    host = argv[2];
    port = atoi(argv[3]);

    isSplit = false;
    isServer = (atoi(argv[1]) == 0);
    isClient = !isServer;
  }

  // Initializations
  Input::Js::initialize(0);
  initialize_meridians();
  if(isSplit)
  {
    SplitWorld::initialize();
    world = new SplitWorld();
  }
  else
  {
    NetWorld::initialize();
    if(isServer)
      Server::initialize(port);
    world = new NetWorld(isClient, host, port);
  }
  MiniMap::initialize();
  Projectile::initialize();

  main_loop();

  SDL_Quit();
}

