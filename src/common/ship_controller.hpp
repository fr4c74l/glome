#pragma once

#include "ship_stats.hpp"

class Ship;

class ShipController
{
public:
	ShipController();

	inline void rotate_h(float rx)
	{
		h_req = rx / 100.0;
	}

	inline void rotate_v(float ry)
	{
		v_req = -ry / 100.0;
	}

	inline void move_v(float y)
	{
		speed_v = y * stats->max_speed_vertical;
	}

	inline void move_h(float x)
	{
		speed_h = x * stats->max_speed_horizontal;
	}

	inline void move(float a)
	{
		accel = a * stats->max_accel_forward;
	}

	inline void move_s(float a)
	{
		speed_s = a * stats->max_speed_spin;
	}

	inline void shoot(bool s)
	{
		shot = s;
	}

private:
	friend class Ship;

	/** Shot */
	int shot_count;
	bool canon_shot_last;
	int heat;
	bool shot;

	/** Movement */
	ShipStats *stats;
	float v_req, h_req;
	float v_tilt, h_tilt;
	float accel;
	float speed, speed_v, speed_h, speed_s;
};

