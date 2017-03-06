#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
#include "../../nclgl/OBJmesh.h"
#include "PumpEmitter.h"
#include "TextMesh.h"
#include "time.h"

//for sceneNodes
//#include "island.h"
//#include "../../nclgl/SceneNode.h"


//shadow
#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	void	DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void SetTrigger(bool tri) { trigger = tri; }
	//void SetLightPosition(Vector3 p) { terlight_pos = p; };
	bool p_activated;
protected:
	//monster
	void DrawMesh();
	void DrawFloor();
	void DrawShadowScene();
	void DrawCombinedScene();

	//light
	//Vector3 terlight_pos;

	MD5FileData * hellData;
	MD5Node * hellNode;

	Shader * sceneShader;
	Shader * shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;
	//skybox
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	//monster shader
	Shader* monShader;
	//text shader
	Shader* textShader;
	//particle shader
	Shader* ParticleShader;

	//monster position
	Vector3* mon_pos;
	float monRot=0;
	bool monster_count = FALSE;

	//text
	Font* basicFont;	//A font! a basic one...
	void ShowFrameRate();
	//FPS
	int frames = 0;
	double starttime = 0;
	bool first = TRUE;
	float fps = 0.0f;
	GLuint FinalFps;
	GameTimer gt;
	GLuint calFPS();
	//Particle
	void SetShaderParticleSize(float f);	//And a new setter
	void DrawParticle();
	PumpEmitter* emitter;	//A single particle emitter
	PumpEmitter* emit_bubb;
	PumpEmitter* emit_bubb2;

	ParticleEmitter* emit_spark_y;
	ParticleEmitter* emit_spark_r;
	ParticleEmitter* emit_flame;
	ParticleEmitter* emit_smoke;
	GLuint explosion_count=0;
	bool trigger = FALSE;
	


	//sun
	//Shader* pointlightShader;
	//OBJMesh* sphere;
	//void DrawSun();


	HeightMap* heightMap;
	Mesh* quad;

	Light* light;
	Light* light_water;
	Light* light_monster;
	Camera* camera;
	Matrix4 shadProjMatrix;
	//floor
	Mesh * floor;

	GLuint cubeMap;
	float waterRotate;
	float expTime;
};
