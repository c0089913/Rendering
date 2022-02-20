#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec4 tangent;

out Vertex {

vec2 texCoord;
vec3 normal;
vec3 tangent; //New! Note , Vec3!
vec3 binormal; //New!
vec3 worldPos;
} OUT;

void main(void) {
mat3 normalMatrix = transpose(inverse(mat3(modelMatrix )));

OUT.texCoord = (textureMatrix * vec4(texCoord , 0.0, 1.0)). xy;

OUT.normal = normalize(normalMatrix * normalize(normal ));

vec4 worldPos = (modelMatrix * vec4(position ,1));

OUT.worldPos = worldPos.xyz;

gl_Position = (projMatrix * viewMatrix) * worldPos;
}