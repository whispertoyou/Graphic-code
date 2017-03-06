#include "island.h"


Mesh* island::isl = NULL;

island::island(void) {
	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );
	planet = new SceneNode(isl, Vector4(1, 1, 1, 1));
	planet->SetModelScale(Vector3(100,100,100));
	planet->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(planet);

	hmap = new SceneNode(isl, Vector4(1, 1, 1, 1));
	hmap->SetModelScale(Vector3(100, 100, 100));
	hmap->SetTransform(Matrix4::Translation(Vector3(0.0f,0.0f, 0.0f)));
	hmap->AddChild(hmap);

}

void island::Update(float msec) {
	transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));

	hmap->SetTransform(hmap->GetTransform() *
		Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));

	SceneNode::Update(msec);
}