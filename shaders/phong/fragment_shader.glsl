#version 330

// === Uniform Structures ===  //

struct LightSource
{
  vec3 pos;  // Center coordinates.
  vec3 dir;  // Direction vector.

  vec3 Ld;  // Diffuse component  (in [0, 1]).
  vec3 Ls;  // Specular component (in [0, 1]).

  float alpha;  // Shininess of specular component.
};

struct Material
{
  vec3 Ka;  // Ambient component (in [0, 1]).
  vec3 Kd;  // Diffuse component (in [0, 1]).
  vec3 Ks;  // Specular component (in [0, 1]).
};

// === I/O === //

// Per-fragment data:
in vec4 f_position;
in vec4 f_normal;
in vec4 f_uv;

out vec4 pixel_color;

// Light Sources:
const int max_num_lights = 8;

uniform vec3 La;                            // Ambient light component.
uniform int num_lights;                     // Number of light sources.
uniform LightSource light[max_num_lights];  // Array of light sources.
uniform int light_switch[max_num_lights];   // Array of light source states (on/off).

// Texture:


// Material:
uniform Material material;


// === Code === //

void main()
{
  // Fragment data and light sources are in camera coordinates.
  vec3 I = La;  // TODO: multiply by Ka.

  // for (int i = 0; i < num_lights; i++)
  // {
  //   I += ;

  // }

  // TODO: implement phong shading model.
  pixel_color = vec4(I, 1.0);
}