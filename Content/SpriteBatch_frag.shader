
#version 330 core
in vec2 UV;
in vec4 Color;
  
out vec4 outcolor;

uniform sampler2D tex;

void main()
{
  vec4 temp = texture ( tex, UV );
  outcolor = temp*Color;
} 