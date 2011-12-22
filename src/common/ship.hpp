#pragma once

#include "engine.hpp"
#include "mesh.hpp"
#include "ship_controller.hpp"

class Ship : public Updatable, public Drawable
{
public:
	Ship(MeshTypes type = HUNTER);
	~Ship();
	void draw(const Shader& s);
	void draw(const Shader& s,Matrix4 cam, Matrix4 proj);
	void update();
	void load_guns(MeshTypes type, fpos_t gun_position_infile);
	void set_controller(ship_controller* pctrl);

protected:
	Mesh* mesh;
	ShipController* ctrl;
	Matrix4 r_canon, l_canon;
	ship_gun gun_l;
	ship_gun gun_r;
	uint16_t nguns;
	Engine* engine;
};

