# include "Renderer.h"
Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();
	//currentShader = new Shader(SHADERDIR"MatrixVertex.glsl",
	//		SHADERDIR"colourFragment.glsl");
	//
	//if (!currentShader -> LinkProgram()) {
	//	return;
	//}
	//init = true;
	//SwitchToOrthographic();

	//texture mapping
	triangle -> SetTexture(SOIL_load_OGL_texture("../Textures/brick.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	
		if (!triangle -> GetTexture()) {
		return;
	}
	currentShader = new Shader("../Shaders/TexturedVertex.glsl",
			"../Shaders/texturedfragment.glsl");
	
	if (!currentShader -> LinkProgram()) {
			return;
	
	}
	init = true;
			
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, 1, -1);
	
	filtering = true;
	repeating = false;

}
Renderer ::~Renderer(void) {
	delete triangle;
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f,
		width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT/*texture mapping*/);
	
	glUseProgram(currentShader -> GetProgram());

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),
		" diffuseTex "), 0);
	
	triangle -> Draw();

	//for 3 triangles
	/*glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram()
			, "projMatrix"), 1, false, (float *)& projMatrix);
	
	glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram()
			, "viewMatrix"), 1, false, (float *)& viewMatrix);
	
	for (int i = 0; i < 3; ++i) {
	Vector3 tempPos = position;
	tempPos.z += (i *500.0f);
	tempPos.x -= (i *100.0f);
	tempPos.y -= (i *100.0f);
		
	modelMatrix = Matrix4::Translation(tempPos) *
	Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
	Matrix4::Scale(Vector3(scale, scale, scale));
		
	glUniformMatrix4fv(glGetUniformLocation(
		currentShader -> GetProgram(), "modelMatrix"), 1, false,
		(float *)& modelMatrix);
	triangle -> Draw();
	}*/
	// try to make 4 triangle=============================================================
	//NO.1
	//Vector3 tempPos1 = position;
	////tempPos1.z = 0.0f;
	//tempPos1.x = -200.0f;
	//tempPos1.y = 200.0f;

	//modelMatrix = Matrix4::Translation(tempPos1) *
	//	Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
	//	Matrix4::Scale(Vector3(scale, scale, scale));

	//glUniformMatrix4fv(glGetUniformLocation(
	//	currentShader->GetProgram(), "modelMatrix"), 1, false,
	//	(float *)& modelMatrix);
	//triangle->Draw();
	////NO.2
	//Vector3 tempPos2 = position;
	////tempPos2.z = 0.0f;
	//tempPos2.x = 200.0f;
	//tempPos2.y = 200.0f;

	//modelMatrix = Matrix4::Translation(tempPos2) *
	//	Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
	//	Matrix4::Scale(Vector3(scale, scale, scale));

	//glUniformMatrix4fv(glGetUniformLocation(
	//	currentShader->GetProgram(), "modelMatrix"), 1, false,
	//	(float *)& modelMatrix);
	//triangle->Draw();
	////NO.3
	//Vector3 tempPos3 = position;
	////tempPos2.z = 0.0f;
	//tempPos3.x = -200.0f;
	//tempPos3.y = -200.0f;

	//modelMatrix = Matrix4::Translation(tempPos3) *
	//	Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
	//	Matrix4::Scale(Vector3(scale, scale, scale));

	//glUniformMatrix4fv(glGetUniformLocation(
	//	currentShader->GetProgram(), "modelMatrix"), 1, false,
	//	(float *)& modelMatrix);
	//triangle->Draw();
	////NO.4
	//Vector3 tempPos4 = position;
	////tempPos2.z = 0.0f;
	//tempPos4.x = 200.0f;
	//tempPos4.y = -200.0f;

	//modelMatrix = Matrix4::Translation(tempPos4) *
	//	Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
	//	Matrix4::Scale(Vector3(scale, scale, scale));

	//glUniformMatrix4fv(glGetUniformLocation(
	//	currentShader->GetProgram(), "modelMatrix"), 1, false,
	//	(float *)& modelMatrix);
	//triangle->Draw();
	// try to make 4 triangle=============================================================
	glUseProgram(0);

		 SwapBuffers();
	
}

//for texture mapping
void Renderer::UpdateTextureMatrix(float value) {
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;
}

void Renderer::ToggleRepeating() {
	repeating = !repeating;
	glBindTexture(GL_TEXTURE_2D, triangle -> GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, //x axis
		repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, //y axis
		repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ToggleFiltering() {
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, triangle -> GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		filtering ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}
