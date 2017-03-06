# pragma once

# include "../../nclgl/OGLRenderer.h"

class Renderer : public OGLRenderer {
public:
Renderer(Window & parent);
virtual ~Renderer(void);

virtual void RenderScene();
//texture mapping
void UpdateTextureMatrix(float rotation);
void ToggleRepeating();
void ToggleFiltering();

void SwitchToPerspective();
void SwitchToOrthographic();

inline void SetScale(float s) { scale = s; }
inline void SetRotation(float r) { rotation = r; }
inline void SetPosition(Vector3 p) { position = p; }

protected:
Mesh * triangle;
//texture mapping
bool filtering;
bool repeating;
float scale;
float rotation;
Vector3 position;};
