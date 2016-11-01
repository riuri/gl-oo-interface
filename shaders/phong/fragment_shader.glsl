#version 330

// === Uniform Structures ===  //

struct Light
{
  vec3 pos;  // Center coordinates.
  vec3 dir;  // Direction vector.

  vec3 La;   // Ambient component (in [0, 1]).
  vec3 Ld;   // Diffuse component (in [0, 1]).
  vec3 Ls;   // Specular component (in [0, 1]).

  float alpha;  // Shininess of specular component.
};

// === I/O === //

// Per-fragment data:
in vec4 f_position;
in vec4 f_normal;
in vec4 f_uv;

out vec4 pixel_color;

// Environment uniforms:
uniform Light light;

// Textures:

// === Code === //

void main()
{
  // TODO: implement phong shading model.
  pixel_color = vec4(1, 0, 0, 0);
}