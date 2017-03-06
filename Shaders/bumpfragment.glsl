# version 150 core

uniform sampler2D diffuseTex ;
uniform sampler2D bumpTex ; // New !

uniform vec3 cameraPos ;
uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform float lightRadius ;
//test light2
uniform vec4 lightColour2 ;
uniform vec3 lightPos2 ;
uniform float lightRadius2 ;

in Vertex {
    vec3 colour ;
    vec2 texCoord ;
    vec3 normal ;
    vec3 tangent ; // New !
    vec3 binormal ; // New !
    vec3 worldPos ;
} IN ;

out vec4 gl_FragColor ;

void main ( void ) {
vec4 diffuse = texture ( diffuseTex , IN.texCoord );
// New !
mat3 TBN = mat3 ( IN.tangent , IN.binormal , IN.normal );
// New !
vec3 normal = normalize ( TBN * ( texture ( bumpTex ,
IN.texCoord ).rgb * 2.0 - 1.0));

//light 1-------------------------------------
vec3 incident = normalize ( lightPos - IN.worldPos );
float lambert = max (0.0 , dot ( incident , normal )); // Different !

float dist = length ( lightPos - IN.worldPos );
float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

vec3 viewDir = normalize ( cameraPos - IN.worldPos );
vec3 halfDir = normalize ( incident + viewDir );

float rFactor = max (0.0 , dot ( halfDir , normal )); // Different !
float sFactor = pow ( rFactor , 33.0 );

vec3 colour = ( diffuse.rgb * lightColour.rgb );
colour += ( lightColour.rgb * sFactor ) * 0.33;
//light 2-------------------------------------
vec3 incident2 = normalize ( lightPos2 - IN.worldPos );
float lambert2 = max (0.0 , dot ( incident2 , normal )); // Different !

float dist2 = length ( lightPos2 - IN.worldPos );
float atten2 = 1.0 - clamp ( dist2 / lightRadius2 , 0.0 , 1.0);

vec3 viewDir2 = normalize ( cameraPos - IN.worldPos );
vec3 halfDir2 = normalize ( incident2 + viewDir2 );

float rFactor2 = max (0.0 , dot ( halfDir2 , normal )); // Different !
float sFactor2 = pow ( rFactor2 , 33.0 );

vec3 colour2 = ( diffuse.rgb * lightColour2.rgb );
colour2 += ( lightColour2.rgb * sFactor2 ) * 0.33;





//light 2 end-------------------------------------

gl_FragColor = vec4 ( colour * atten * lambert , diffuse.a );

gl_FragColor += vec4 ( colour2 * atten2 * lambert2 , diffuse.a );

gl_FragColor.rgb += ( diffuse.rgb * lightColour.rgb ) * 0.1;

gl_FragColor.rgb += ( diffuse.rgb * lightColour.rgb ) * 0.1;
}