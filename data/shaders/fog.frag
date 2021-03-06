varying float fog_coord;

void mix_fog(inout vec4 color)
{
	// Fog will interpolate with this color
	const vec4 FOG_COLOR = vec4(0.0, 0.0, 0.0, 1.0);

	// Exponential curve constant, based on the fixed pipeline fog
	const float FOG_FACTOR = -1.20; // aprox. the same as -(0.83 / log(2))
	//TODO: Simple use gl_FragCoord.z
	float f = exp2(FOG_FACTOR * fog_coord);
	color = mix(FOG_COLOR, color, f);
}
