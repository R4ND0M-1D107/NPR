#pragma once
#include <pgr.h>
#include "Shader.h"

class Mesh
{
private:
	int objectNAttribsPerVertex;
	int objectNVertices;
	int objectNTriangles;
	float* objectVertices;
	unsigned* objectTriangles;
public:
	Mesh(std::string file);
	Mesh(int objectNAttribsPerVertex, int objectNVertices, int objectNTriangles, float objectVertices[], unsigned objectTriangles[]);
	~Mesh();
	void setup(Shader* shader);
	GLuint vertexBufferObject;   // identifier for the vertex buffer object
	GLuint elementBufferObject;  // identifier for the element buffer object
	GLuint vertexArrayObject;    // identifier for the vertex array object
	unsigned int numTriangles;         // number of triangles in the mesh
};