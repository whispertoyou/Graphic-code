#version 150 core

uniform sampler2D diffuseTex;

uniform float heightLimit;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
	vec3 worldPos;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	vec4 colour = IN.colour* texture(diffuseTex, IN.texCoord);
	if(IN.worldPos.y < heightLimit - 10000.f) {
		gl_FragColor = colour;
	}
}
