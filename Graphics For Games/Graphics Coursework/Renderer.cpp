#include "Renderer.h"
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	camera = new Camera();
	heightMap = new HeightMap("Textures/terrain.raw");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(Vector3((RAW_HEIGHT * HEIGHTMAP_X)-35000.0f ,
		17000.0f, (RAW_HEIGHT * HEIGHTMAP_Z) - 50000.0f));

	camera->SetYaw(30);
	//camera->SetPosition(Vector3(-4.22017e+37,5.34359e+15,4.35824e-34));
	
	//light for terrain & monster
	//light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 10000.0f,
	//	(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f))
	//	,/*Vector4(0.9f, 0.3f, 0.4f, 1)*/Vector4(1.f, 1.f, 1.f, 1), (RAW_WIDTH * HEIGHTMAP_X)/*/2.0f*/);
	////light->SetPosition(terlight_pos);
	light = new Light(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.f), 15000.f, 10000), 
		Vector4(1, 1, 1, 1), 120000.f);
	//light for water
	light_water = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f,
		(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(0.9f, 0.6f, 0.6f, 1),
		(RAW_WIDTH * HEIGHTMAP_X));


	reflectShader = new Shader("../../Shaders/PerPixelVertex.glsl",
		"../../Shaders/reflectFragment.glsl");
	skyboxShader = new Shader("../../Shaders/skyboxVertex.glsl",
		"../../Shaders/skyboxFragment.glsl");
	textShader = new Shader("../../Shaders/TexturedVertex.glsl",
		"../../Shaders/TexturedFragment.glsl");
	lightShader = new Shader("../../Shaders/shadowscenevert_hmap.glsl",
		"../../Shaders/shadowscenefrag_hmap.glsl");
	ParticleShader = new Shader("../../Shaders/vertex.glsl",
		"../../Shaders/fragment.glsl",
		"../../Shaders/geometry.glsl");
	/*pointlightShader = new Shader("../../Shaders/SceneVertex.glsl",
		"../../Shaders/SceneFragment.glsl");*/


	if (!reflectShader->LinkProgram()) {
		return;
	}
	if (!lightShader->LinkProgram() ) {
		return;
	}
	if (
		!skyboxShader->LinkProgram() ) {
		return;
	}
	if ( !textShader->LinkProgram()) {
		return;
	}
	if (!ParticleShader->LinkProgram()) {
		return;
	}










	quad->SetTexture(SOIL_load_OGL_texture("Textures/icywater.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture(SOIL_load_OGL_texture(
		"Textures/BarrenReds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/Barren RedsDOT3.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(
		"Textures/icyhell_lf.jpg", "Textures/icyhell_rt.jpg",
		"Textures/icyhell_up.jpg", "Textures/icyhell_dn.jpg",
		"Textures/icyhell_ft.jpg", "Textures/icyhell_bk.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	//TEXT part=======================================
	basicFont = new Font(SOIL_load_OGL_texture("../../Textures/tahoma.tga", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
		SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	//TEXT part end=======================================



	//test monster ==========================================
	hellData = new MD5FileData("../../Meshes/hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim("../../Meshes/idle2.md5anim");
	hellNode->PlayAnim("../../Meshes/idle2.md5anim");
	
	sceneShader = new Shader("../../Shaders/shadowscenevert.glsl",
		"../../Shaders/shadowscenefrag.glsl");
	//sceneShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	shadowShader = new Shader("../../Shaders/shadowVert.glsl", 
		"../../Shaders/shadowFrag.glsl");

	//test monster end=======================================
	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() ||
		!heightMap->GetBumpMap() || !sceneShader->LinkProgram() || 
		!shadowShader->LinkProgram()) {
		return;
	}
	//monster binds data======================================
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//monster binds data end==================================
	//floor==================================
	floor = Mesh::GenerateQuad();
	floor->SetTexture(SOIL_load_OGL_texture("../../Textures/brick.tga"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	floor->SetBumpMap(SOIL_load_OGL_texture("../../Textures/brickDOT3.tga"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//floor  end=============================

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	//Particle===============================
	emitter = new PumpEmitter();
	emitter->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"/particle.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	emit_bubb = new PumpEmitter();
	emit_bubb->SetTexture(SOIL_load_OGL_texture("Textures/bubble.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	emit_bubb2 = new PumpEmitter();
	emit_bubb2->SetTexture(SOIL_load_OGL_texture("Textures/bubble.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	
	
	emit_spark_y = new ParticleEmitter();
	emit_spark_y->SetTexture(SOIL_load_OGL_texture("Textures/spark_y.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	emit_spark_r = new ParticleEmitter();
	emit_spark_r->SetTexture(SOIL_load_OGL_texture("Textures/spark_r.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	emit_flame = new ParticleEmitter();
	emit_flame->SetTexture(SOIL_load_OGL_texture("Textures/flame.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	emit_smoke = new ParticleEmitter();
	emit_smoke->SetTexture(SOIL_load_OGL_texture("Textures/smoke.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
	//Particle end===========================
	//Sun ===================================
	/*sphere = new OBJMesh();
	if (!sphere->LoadOBJMesh("../../Meshes/ico.obj")) {
		return;
	}*/
	//Sun end================================
	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 200000.0f,
		(float)width / (float)height, 45.0f);
	
	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	p_activated = false;
	expTime = 0.f;
}
Renderer ::~Renderer(void) {
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete textShader;
	delete sceneShader;
	delete shadowShader;
	//delete pointlightShader;

	delete light;
	delete light_water;
	//===monster part
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete hellData;
	delete hellNode;

	//===moster end
	//floor
	delete floor;
	//text
	delete basicFont;
	//particle
	delete emitter;
	delete emit_bubb;
	delete emit_bubb2;
	//explosion
	delete emit_spark_y;
	delete emit_spark_r;
	delete emit_flame;
	delete emit_smoke;
	//sun
	//delete sphere;

	currentShader = 0;
}
void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	light->UpdateLightPos(msec);

	waterRotate += msec / 1000.0f;
	//moster-----
	
	hellNode->Update(msec);
	//calculate the FPS
	FinalFps = calFPS();
	//particle
	emitter->Update(msec);
	emit_bubb->Update(msec);
	emit_bubb2->Update(msec);
	//explosion
	

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M)) {
		p_activated = !p_activated;
		expTime = gt.GetMS();
	}

	if (p_activated) {
		emit_spark_y->Update(msec);
		emit_spark_r->Update(msec);
		emit_flame->Update(msec);
		emit_smoke->Update(msec);
		if (gt.GetMS() - expTime > 100.f) {
			p_activated = false;
			delete emit_spark_r;
			delete emit_spark_y;
			delete emit_flame;
			delete emit_smoke;
			emit_spark_y = new ParticleEmitter();
			emit_spark_y->SetTexture(SOIL_load_OGL_texture("Textures/spark_y.jpg",
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
			emit_spark_r = new ParticleEmitter();
			emit_spark_r->SetTexture(SOIL_load_OGL_texture("Textures/spark_r.jpg",
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
			emit_flame = new ParticleEmitter();
			emit_flame->SetTexture(SOIL_load_OGL_texture("Textures/flame.png",
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
			emit_smoke = new ParticleEmitter();
			emit_smoke->SetTexture(SOIL_load_OGL_texture("Textures/smoke.png",
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));
		}
	}

}
void Renderer::RenderScene() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	//DrawHeightmap();  moved to draw combinedScene, dont need to be called individually
	DrawWater();
	//monster part--------------
	DrawShadowScene(); 
	DrawCombinedScene(); 
	//sun------------
	//DrawSun();
	//particle
	DrawParticle();
	
	/*if (Window::GetKeyboard()->KeyDown(KEYBOARD_Y)) {
		
	}*/
	//TEXT-----------
	ShowFrameRate();


	SwapBuffers();
}
void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}
void Renderer::DrawHeightmap() {
	//SetCurrentShader(lightShader);
	//SetShaderLight(*light);
	//glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
	//	"cameraPos"), 1, (float *)& camera->GetPosition());

	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
	//	"diffuseTex"), 0);
	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
	//	"bumpTex"), 1);

	//modelMatrix.ToIdentity();
	//textureMatrix.ToIdentity();

	////rotate the heightmap====
	///*modelMatrix = Matrix4::Translation(Vector3((RAW_WIDTH * HEIGHTMAP_X / 2.0f)
	//	, (256 * HEIGHTMAP_Y / 3.0f), (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f))) *
	//	Matrix4::Rotation(waterRotate, Vector3(0.0f, 1, 0.0f)) *
	//	Matrix4::Translation(Vector3(-(RAW_WIDTH * HEIGHTMAP_X / 2.0f)
	//		, -(256 * HEIGHTMAP_Y / 3.0f), -(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)));*/
	////rotate the heightmap end====
	//UpdateShaderMatrices();
	//
	//heightMap->Draw();

	//glUseProgram(0);

	/*glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());*/

	modelMatrix.ToIdentity();
	Matrix4 tempMatrix = shadProjMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "shadProjMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isMon"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	//modelMatrix.ToIdentity();
	//textureMatrix.ToIdentity();

	heightMap->Draw();
}
void Renderer::DrawWater() {
	SetCurrentShader(reflectShader);
	SetShaderLight(*light_water);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f)) /**
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, -1.0f))*/;

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));
	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}
// draw monster shadow
void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));
	shadProjMatrix = biasMatrix *(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	//DrawFloor();
	DrawHeightmap();
	DrawMesh();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	SetCurrentShader(sceneShader);
	SetShaderLight(*light);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();

	UpdateShaderMatrices();

	//DrawFloor();
	DrawHeightmap();
	DrawMesh();

	glUseProgram(0);
}
//draw monster
void Renderer::DrawMesh() {

	modelMatrix.ToIdentity();

	mon_pos = heightMap->GetCoordinate();
	modelMatrix = Matrix4::Translation(Vector3(mon_pos[heightMap->numHiposes - 1])) *
		Matrix4::Scale(Vector3(10.f, 10.f, 10.f)) *
		Matrix4::Rotation(waterRotate * 10, Vector3(0.0f, 1.0f, 0.0f));

	Matrix4 tempMatrix = shadProjMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "shadProjMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isMon"), 1);



	/*modelMatrix = Matrix4::Translation(Vector3((RAW_WIDTH*HEIGHTMAP_X) / 2.f, 100.f, (RAW_HEIGHT*HEIGHTMAP_Z) / 2.f)) *
		Matrix4::Scale(Vector3(100.f, 100.f, 100.f));*/

		//set position of monster
	//UpdateShaderMatrices();
	hellNode->Draw(*this);
	//int c = 1;
	//for (int i = heightMap->numHiposes; i > -1; i--) {
	//	/*if ((abs((int)(mon_pos[heightMap->numHiposes - 1].x - mon_pos[i].x)) > 600
	//		&& abs((int)(mon_pos[heightMap->numHiposes - 1].z - mon_pos[i].z)) > 600))
	//	{*/
	//		int ranX = rand() % 1000 + 200,
	//		ranZ = rand() % 1000 + 200;
	//		cout << c << endl;
	//		cout << mon_pos[i] << endl;
	//		cout << ranX << "," << ranZ << endl;
	//		c++;
	//	//}
	//}
	//if (monster_count == FALSE) {
	//int ranX = 420,
	//	ranZ = 500;
	//for (int i = heightMap->numHiposes; i > heightMap->numHiposes - 12; i--) {
	//	UpdateShaderMatrices();
	//	if (i == heightMap->numHiposes) { //this mean the 
	//		//Vector3 setMonPos = mon_pos[i];
	//		modelMatrix = Matrix4::Translation(Vector3(mon_pos[i - 1])) *
	//			Matrix4::Scale(Vector3(10.f, 10.f, 10.f)) *
	//			Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	//		hellNode->Draw(*this);
	//	}
	//	else if ((mon_pos[heightMap->numHiposes - 1].x) - (mon_pos[i].x) > 600
	//		/*&& abs((int)(mon_pos[heightMap->numHiposes-1].z - mon_pos[i].z))>600*/)
	//	{
	//		modelMatrix = Matrix4::Translation(Vector3(mon_pos[i].x + ranX, mon_pos[i].y, mon_pos[i].z + ranZ)) *
	//			Matrix4::Scale(Vector3(10.f, 10.f, 10.f)) *
	//			Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	//		hellNode->Draw(*this);
	//		ranX += 100;
	//		ranZ += 100;
	//	}
	//	
	//}
	//monster_count = TRUE;
	//}
}
//draw floor
void Renderer::DrawFloor() {
	/*mon_pos = heightMap->GetCoordinate();
	modelMatrix = Matrix4::Translation(Vector3(mon_pos))*
		Matrix4::Rotation(90, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(450, 450, 1));*/
	
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	floor->Draw();
	//glUseProgram(0);
}
//draw text 
void Renderer::ShowFrameRate() {
	SetCurrentShader(textShader);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(currentShader->GetProgram());	//Enable the shader...
	//And turn on texture unit 0
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	
	//Memory Usage

	GLint total_mem_kb = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
		&total_mem_kb);

	GLint cur_avail_mem_kb = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
		&cur_avail_mem_kb);
	
	GLint memoryUsage = (total_mem_kb - cur_avail_mem_kb)/1024;

	//Render function to encapsulate our font rendering!
	DrawText("FPS : "+ to_string(FinalFps), Vector3(0, 0, 0), 16.0f);
	DrawText("Total GPU memory usage: " + to_string(memoryUsage) + "MB", Vector3(0, 16.f, 0), 16.0f);
	DrawText("Press M to explode the monster", Vector3(0, height - 48, 0), 16.0f);
	DrawText("Move the Camera by pressing W, A, S, D", Vector3(0, height - 32, 0), 16.0f);
	DrawText("Move the Light by pressing I, J, K, L, O, p", Vector3(0, height-16, 0), 16.0f);
	DrawText("", Vector3(0, 0, -1000), 64.0f, true);
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_BLEND);
}

//calculate the FPS referto: http://gamedev.stackexchange.com/questions/83159/simple-framerate-counter
GLuint Renderer::calFPS() {

	float timepassed = gt.GetMS();

	if (first)
	{
		frames = 0;
		starttime = timepassed;
		first = FALSE;
		return 0;
	}
	frames++;
	if (timepassed - starttime > 1.0 && frames > 10)
	{
		fps = (double)frames / (timepassed - starttime);
		starttime = timepassed;
		frames = 0;
	}

	//cout << "FPS : " << fps << endl;
	
	return fps * 1000;
}
void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	
	
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);
	
	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 200000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		textureMatrix.ToIdentity();
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	

	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}
//draw particle
void Renderer::DrawParticle() {
	//glClearColor(0, 0, 0, 1);
	SetCurrentShader(ParticleShader);
	//snow
	modelMatrix = Matrix4::Translation(Vector3((RAW_WIDTH * HEIGHTMAP_X / 2.0f),
		80000.0f,
		(RAW_WIDTH * HEIGHTMAP_X / 2.0f))) * Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(180, Vector3(0.0,0.0,1.0));
	textureMatrix.ToIdentity();

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "heightLimit"), 80000.f);


	SetShaderParticleSize(emitter->GetParticleSize());
	emitter->SetParticleSize(500.0f);
	emitter->SetParticleVariance(1.0f);
	emitter->SetLaunchParticles(16.0f);
	emitter->SetParticleLifetime(9000.0f);
	emitter->SetParticleSpeed(0.1f);
	UpdateShaderMatrices();

	emitter->Draw();

	//bubble 1
	modelMatrix = Matrix4::Translation(Vector3((RAW_WIDTH * HEIGHTMAP_X / 2.0f)+200.0f,
		4000.0f,
		(RAW_WIDTH * HEIGHTMAP_X / 2.0f)+200.0f)) * Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(0, Vector3(0.0, 0.0, 1.0));
	textureMatrix.ToIdentity();
	
	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emit_bubb->GetParticleSize());
	emit_bubb->SetParticleSize(300.0f);
	emit_bubb->SetParticleVariance(1.0f);
	emit_bubb->SetLaunchParticles(16.0f);
	emit_bubb->SetParticleLifetime(300.0f);
	emit_bubb->SetParticleSpeed(0.1f);


	UpdateShaderMatrices();

	emit_bubb->Draw();

	//bubble 2
	modelMatrix = Matrix4::Translation(Vector3((RAW_WIDTH * HEIGHTMAP_X / 4.0f)+15000 ,
		4000.0f,
		(RAW_WIDTH * HEIGHTMAP_X / 2.0f) + 16000.0f)) * Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(0, Vector3(0.0, 0.0, 1.0));
	textureMatrix.ToIdentity();

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emit_bubb2->GetParticleSize());
	emit_bubb2->SetParticleSize(300.0f);
	emit_bubb2->SetParticleVariance(1.0f);
	emit_bubb2->SetLaunchParticles(16.0f);
	emit_bubb2->SetParticleLifetime(400.0f);
	emit_bubb2->SetParticleSpeed(0.08f);
	UpdateShaderMatrices();

	emit_bubb2->Draw();
	
	//spark-yellow
	modelMatrix = Matrix4::Translation(mon_pos[heightMap->numHiposes - 1]) *
		Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(0, Vector3(0.0, 0.0, 1.0));
	textureMatrix.ToIdentity();

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emit_spark_y->GetParticleSize());
	emit_spark_y->SetParticleSize(300.0f);
	emit_spark_y->SetParticleVariance(1.0f);
	emit_spark_y->SetLaunchParticles(16.0f);
	emit_spark_y->SetParticleLifetime(400.0f);
	emit_spark_y->SetParticleSpeed(0.08f);
	UpdateShaderMatrices();

	emit_spark_y->Draw();

	//spark-red
	modelMatrix = Matrix4::Translation(mon_pos[heightMap->numHiposes-1]) * 
		Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(0, Vector3(0.0, 0.0, 1.0));
	textureMatrix.ToIdentity();

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emit_spark_r->GetParticleSize());
	emit_spark_r->SetParticleSize(300.0f);
	emit_spark_r->SetParticleVariance(1.0f);
	emit_spark_r->SetLaunchParticles(16.0f);
	emit_spark_r->SetParticleLifetime(400.0f);
	emit_spark_r->SetParticleSpeed(0.08f);
	UpdateShaderMatrices();

	emit_spark_r->Draw();

	//flame
	modelMatrix = Matrix4::Translation(mon_pos[heightMap->numHiposes - 1]) *
		Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(0, Vector3(0.0, 0.0, 1.0));
	textureMatrix.ToIdentity();

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emit_flame->GetParticleSize());
	emit_flame->SetParticleSize(5000.0f);
	emit_flame->SetParticleVariance(1.0f);
	emit_flame->SetLaunchParticles(5.0f);
	emit_flame->SetParticleLifetime(90.0f);
	emit_flame->SetParticleSpeed(0.12f);
	UpdateShaderMatrices();

	emit_flame->Draw();

	//smoke
	modelMatrix = Matrix4::Translation(mon_pos[heightMap->numHiposes - 1]) *
		Matrix4::Scale(Vector3(100.f, 100.f, 100.f))*
		Matrix4::Rotation(0, Vector3(0.0, 0.0, 1.0));
	textureMatrix.ToIdentity();

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(emit_smoke->GetParticleSize());
	emit_smoke->SetParticleSize(8000.0f);
	emit_smoke->SetParticleVariance(1.0f);
	emit_smoke->SetLaunchParticles(5.0f);
	emit_smoke->SetParticleLifetime(180.0f);
	emit_smoke->SetParticleSpeed(0.5f);
	UpdateShaderMatrices();

	emit_smoke->Draw();
	

	glUseProgram(0);
}
void Renderer::SetShaderParticleSize(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}
//draw sun
//void Renderer::DrawSun() {
//	SetCurrentShader(pointlightShader);
//
//	glClearColor(0, 0, 0, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glBlendFunc(GL_ONE, GL_ONE);
//	glUniformMatrix4fv(
//		glGetUniformLocation(currentShader->GetProgram(),
//			"modelMatrix"), 1, false, (float *)& modelMatrix.values);
//
//	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
//		"nodeColour"), 1, 0);
//
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
//		"useTexture"), 0);
//	
//
//	Vector3 translate = Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500,
//		(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f));
//
//	Matrix4 pushMatrix = Matrix4::Translation(translate);
//	Matrix4 popMatrix = Matrix4::Translation(-translate);
//
//	UpdateShaderMatrices();
//	sphere->Draw();
//}
//explosion
