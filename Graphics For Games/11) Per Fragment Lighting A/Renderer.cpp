#include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	camera = new Camera(0.0f, 0.0f, Vector3(
		RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_HEIGHT * HEIGHTMAP_Z));
	
	heightMap = new HeightMap("../../Textures/8terrain2.raw");
	currentShader = new Shader("../../Shaders/bumpvertex.glsl",
		"../../Shaders/bumpfragment.glsl");
	heightMap -> SetTexture(SOIL_load_OGL_texture(
		"../../Textures/BarrenReds.JPG", SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//tutorial 12
	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/Barren RedsDOT3.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	
	if (!currentShader->LinkProgram() ||
		!heightMap->GetTexture() || !heightMap->GetBumpMap()) {
		return;
	}
	SetTextureRepeating(heightMap -> GetTexture(), true);
	SetTextureRepeating(heightMap -> GetBumpMap(), true);
	//light 2
	li2 = new Light(Vector3(2000.0f,
		500.0f, 20000.0f),
		Vector4(0, 1, 0, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.1f);
	
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f),
				500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
				Vector4(0, 0, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.1f);
	

	/*light = new Light(Vector3(0.0f,
		500.0f, 0.0f),
		Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);*/


	projMatrix = Matrix4::Perspective(50.0f, 50000.0f,
			(float)width / (float)height, 45.0f);
	
	glEnable(GL_DEPTH_TEST);
	init = true;
}
Renderer ::~Renderer(void) {
	delete  camera;
	delete  heightMap;
	delete  light;
	delete  li2;
}

void Renderer::UpdateScene(float msec) {
	camera -> UpdateCamera(msec);
	viewMatrix = camera -> BuildViewMatrix();
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	glUseProgram(currentShader -> GetProgram());
	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),
		"bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader -> GetProgram(),
		"cameraPos"), 1, (float *)& camera -> GetPosition());
	
	UpdateShaderMatrices();
	//SetShaderLight(*li2);
	SetShaderLight(*light/*,*li2*/);
	
	
	heightMap -> Draw();
	
	glUseProgram(0);
	
	SwapBuffers();
}