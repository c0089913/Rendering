#version 330 core
uniform sampler2D diffuseTex;
in Vertex {
vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void) {
vec3 diffuse = texture(diffuseTex , IN.texCoord ).xyz;
 fragColour.rgb = diffuse;
fragColour.a = 1.0;
}