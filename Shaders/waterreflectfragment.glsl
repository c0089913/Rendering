#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D dudvTex;
uniform samplerCube cubeTex;
uniform sampler2D bumpTex;

uniform vec3 cameraPos;
uniform float speed;

const float strength = 0.01;

in Vertex {
vec2 texCoord;
vec3 normal;
vec3 tangent;
vec3 binormal;
vec3 worldPos;
} IN;

out vec4 fragColour[2];
void main(void) {

mat3 TBN = mat3(normalize(IN.tangent),
normalize(IN.binormal),
normalize(IN.normal ));

//vec3 normal = texture2D(bumpTex , IN.texCoord ).rgb* 2.0 - 1.0;
vec3 normal = vec3(0,0,1.0);
normal = normalize(TBN * normalize(normal ));

vec2 dudv1 = (texture(dudvTex, vec2(IN.texCoord.x + speed, IN.texCoord.y)).xy*2.0 -1.0);
vec2 dudv2 = (texture(dudvTex, vec2(-IN.texCoord.x , IN.texCoord.y+ speed)).xy*2.0 -1.0);
vec2 dudv = dudv1+dudv2;

vec4 diffuse = texture(diffuseTex , IN.texCoord + dudv * strength );

vec3 viewDir = normalize(cameraPos - IN.worldPos );

vec3 reflectDir = reflect(-viewDir ,normalize(IN.normal ));
vec4 reflectTex = texture(cubeTex ,reflectDir + vec3(dudv * strength,0.0) );

fragColour[0] = reflectTex * 0.7 + (diffuse * 0.3 );
fragColour [1] = vec4(normal.xyz * 0.5 + 0.5 ,1.0);
}