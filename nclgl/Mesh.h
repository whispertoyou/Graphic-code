#pragma once

#include "../../nclgl/OGLRenderer.h"
#include <vector>
#include <iterator>

enum MeshBuffer
{
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, 
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER,
	MAX_BUFFER
};
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	static Mesh* GenerateTriangle();
	//Depth and Transparency
	static Mesh* GenerateQuad();
	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }
	//tutorial 12
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }
	// test get the cur pos of HM
	//vector<Vector3> GetCoordinate() { return hiPoses; }
	Vector3* GetCoordinate() { return hiPoses; }
	//void SetCoordinate(Vector3 tex) { curPos = tex; }

	int numHiposes;
protected:
	void BufferData();
	GLuint texture;
	Vector2* textureCoords;
	GLuint numVertices;
	Vector3* vertices;
	//tutorial8
	GLuint numIndices;
	unsigned int* indices;
	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	Vector4* colours;
	//tutorial 11
	void GenerateNormals();
	Vector3 * normals;
	//tutorial 12
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
							const Vector3 &c, const Vector2 &ta,
							const Vector2 &tb, const Vector2 &tc);
	Vector3* tangents;
	GLuint bumpTexture;

	//Extra-drawing text
	GLuint type;

	//test get highest position
	Vector3 highestPos{ 0.0f, 2000.0f, 0.0f };
	//vector<Vector3> hiPoses;
	Vector3* hiPoses;
};

