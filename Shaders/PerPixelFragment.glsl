# version 150 core

uniform sampler2D diffuseTex ;

uniform vec3 cameraPos ;
uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform float lightRadius ;

in Vertex {
    vec3 colour ;
    vec2 texCoord ;
    vec3 normal ;
    vec3 worldPos ;
    float height ;
} IN ;

out vec4 gl_FragColor ;

//add snow
const vec3 river = vec3(0.1,0.5,0.8);   //Blue
const vec3 snow = vec3(1,1,1);          //white

void main ( void ) {
    vec4 diffuse = texture( diffuseTex , IN.texCoord );

    vec3 incident = normalize( lightPos - IN.worldPos );
    float lambert = max(0.0 , dot ( incident , IN.normal ));
    float dist = length( lightPos - IN.worldPos );
    float atten = 1.0 - clamp( dist/lightRadius , 0.0 , 1.0);
    vec3 viewDir = normalize( cameraPos - IN.worldPos );
    vec3 halfDir = normalize( incident + viewDir );

    //add snow and river===============
    //float rivposition = clamp((IN.height/80.0), 0.4, 1.0); 
	//float snoposition = clamp((IN.height-200.0)/50.0, 0.0, 1.0);
	//vec4 temp = texture(diffuseTex, IN.texCoord);
	//vec3 layer1 = mix(temp.rgb, snow, snoposition); //mix the texture & snow first
	//vec3 layer2 = mix(river, layer1, rivposition);
    //=================================
    float rFactor = max(0.0 , dot(halfDir,IN.normal));
    float sFactor = pow(rFactor,50.0);
    vec3 colour = (diffuse.rgb * lightColour.rgb);
    colour += (lightColour.rgb * sFactor) * 0.33;
    gl_FragColor = vec4( colour * atten * lambert , diffuse.a );
    //gl_FragColor.rgb += layer2;
    gl_FragColor.rgb += ( diffuse.rgb * lightColour.rgb ) * 0.1;


} 