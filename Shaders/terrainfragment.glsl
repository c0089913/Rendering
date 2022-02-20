#version 330 core
uniform sampler2D diffuseTex ;
uniform sampler2D stoneTex ;
uniform sampler2D slopeTex ;
uniform sampler2D splatTex ;
uniform sampler2D bumpTex;

in Vertex {
vec2 texCoord ;
vec2 splatCoord;
vec3 normal;
vec3 tangent;
vec3 binormal;
vec3 worldPos;

} IN ;

out vec4 fragColour[2] ;
void main ( void ) {

 mat3 TBN = mat3(normalize(IN.tangent),
 normalize(IN.binormal),
 normalize(IN.normal ));
 
 vec3 normal = texture2D(bumpTex , IN.splatCoord ).rgb* 2.0 - 1.0;
 normal = normalize(TBN * normalize(normal ));

 vec4 stoneColour = texture(stoneTex, IN.texCoord);
 vec4 slopeColour = texture(slopeTex, IN.texCoord);
 vec4 floorColour = texture(diffuseTex, IN.texCoord);
 vec4 splatColour = texture ( splatTex , IN . splatCoord );
 vec4 diffuse = splatColour.r*stoneColour+ splatColour.b*slopeColour + splatColour.g*floorColour;
 diffuse.a = 1.0;
 
 fragColour[0] = diffuse; 
 fragColour[1] =  vec4(normal.xyz * 0.5 + 0.5 ,1.0);
 }
