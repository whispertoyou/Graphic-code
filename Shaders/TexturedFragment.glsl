#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	//float height;
} IN;

out vec4 gl_FragColor;

//const vec3 river = vec3(0.1,0.5,0.8);     //Blue
//const vec3 snow = vec3(1,1,1);  //white


void main(void){
	//float rivposition = clamp((IN.height/80.0), 0.4, 1.0); 
	//float snoposition = clamp((IN.height-200.0)/50.0, 0.0, 1.0);
	//vec4 temp = texture(diffuseTex, IN.texCoord);
	//vec3 layer1 = mix(temp.rgb, snow, snoposition); //mix the texture & snow first
	//vec3 layer2 = mix(river, layer1, rivposition);

	//gl_FragColor.rgb = layer2;
	//gl_FragColor = mix(texture(diffuseTex, IN.texCoord), vec4(1,1,1,1), IN.height/200.0);
	gl_FragColor = texture(diffuseTex, IN.texCoord);
}