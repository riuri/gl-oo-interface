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
in vec2 f_uv;

in vec3 barycentric;

out vec4 pixel_color;

// === Light Sources === //
const int max_num_lights = 8;
uniform int lighting = 0;  // Boolean.

uniform int num_lights = 1;                 // Number of light sources.
uniform int light_switch[max_num_lights];   // Array of light source states (on/off).

uniform vec3 La = vec3(0.1);                // Ambient light component.
uniform LightSource light[max_num_lights];  // Array of light sources.

// === Texture === //
uniform sampler2D color_map;
uniform sampler2D normal_map;

// === Material === //
uniform Material material;

// === Code === //

void main()
{
  if (lighting == 0)  // off.
  {
    if (barycentric.x < 0.02 || barycentric.y < 0.02 || barycentric.z < 0.02)  // Edge.
    {
      pixel_color = vec4(1.0) - texture(color_map, f_uv);
    }
    else
    {
      pixel_color = texture(color_map, f_uv);
    }
  }
  else  // on.
  {
    vec3 Ka = material.Ka;
    vec3 Kd = texture(color_map, f_uv).xyz;
    vec3 Ks = material.Ks;

    // Fragment data and light sources are in camera coordinates.
    vec3 I = Ka*La;
    
    if (barycentric.x < 0.02 || barycentric.y < 0.02 || barycentric.z < 0.02)  // Edge.
    {
      I = vec3(1.0) - Kd;
    }
    else  // Interior of a triangle.
    {
      vec3 n = f_normal.xyz;

      for (int i = 0; i < num_lights; i++)
      {
        if (light_switch[i] == 0)  // Off!
          continue;

        vec3 l  = normalize(light[i].pos - f_position.xyz);  // Unit vector from fragment to light source.
        vec3 r  = -reflect(l, n);                            // Reflection of light ray on fragment.
        vec3 f = normalize(-f_position.xyz);                 // Unit vector from fragment to camera (origin).
        float d =    length(light[i].pos - f_position.xyz);  // Distance from fragment to light source.
        float alpha = light[i].alpha;

        vec3 Id = light[i].Ld * max(dot(n, l), 0);              // Diffuse component.
        vec3 Is = light[i].Ls * pow(max(dot(r, f), 0), alpha);  // Specular component. TODO: shininess.

        I += (Kd*Id + Ks*Is);
      }
    }

    pixel_color = vec4(I, 1.0);
  }
}