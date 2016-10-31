#version 330

// === Uniform Structures ===  //

struct Light
{
  vec3 position;  // Center coordinates.
  vec3 normal;    // Direction vector.

  vec3 La;   // Ambient component (in [0, 1]).
  vec3 Ld;   // Diffuse component (in [0, 1]).
  vec3 Ls;   // Specular component (in [0, 1]).
};

// === I/O === //

in vec4 f_color;
out vec4 pixel_color;


// === Code === //

void main()
{
  pixel_color = f_color;
}