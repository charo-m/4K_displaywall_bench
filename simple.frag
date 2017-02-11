#version 440 core

in vec2 v_texcoord;
in vec2 v_bound;
uniform sampler2D u_texture;

out vec4 frag_color;


void main ()
{
  frag_color.rgb = texture2D(u_texture, v_texcoord).rgb;
  //frag_color.rgb = vec3(v_texcoord.xy, 0.0);
  //frag_color.rgb = vec3(0.0, 1.0, 0.0);

  frag_color.a = 1.0;
}
