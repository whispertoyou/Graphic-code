#pragma once
#include "../../nclgl/scenenode.h"
#include "../../nclgl/OBJMesh.h"

class island : public SceneNode {
public:
	island(void);
	~island(void) {};
	virtual void Update(float msec);

	static void CreateSphere() {
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh("../../Meshes/sphere.obj");
		isl = m;
	}
	static void DeleteSphere() { delete isl; }
protected:
	static Mesh* isl;
	SceneNode* planet;
	SceneNode* hmap;
	SceneNode* watTex;
	SceneNode* Mon;
};
