#include "..\..\include\Shading\Mesh.h"
#include <algorithm>

Mesh::Mesh(std::string file)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( file, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	

	objectNAttribsPerVertex = 14;
	objectNVertices = scene->mMeshes[0]->mNumVertices;
	objectNTriangles = scene->mMeshes[0]->mNumFaces;

	float* newObjectVertices = new float[objectNVertices * objectNAttribsPerVertex];
	for (int i = 0; i < objectNVertices; i++)
	{
		aiVector3D position = scene->mMeshes[0]->mVertices[i];
		aiVector3D normal = scene->mMeshes[0]->mNormals[i];
		aiVector3D uv = scene->mMeshes[0]->mTextureCoords[0][i];
		aiVector3D tangent = scene->mMeshes[0]->mTangents[i];
		aiVector3D bitangent = scene->mMeshes[0]->mBitangents[i];
		newObjectVertices[i*14+0] = position.x;
		newObjectVertices[i*14+1] = position.y;
		newObjectVertices[i*14+2] = position.z;
		newObjectVertices[i*14+3] = normal.x;
		newObjectVertices[i*14+4] = normal.y;
		newObjectVertices[i*14+5] = normal.z;
		newObjectVertices[i*14+6] = uv.x;
		newObjectVertices[i*14+7] = uv.y;
		newObjectVertices[i*14+8] = tangent.x;
		newObjectVertices[i*14+9] = tangent.y;
		newObjectVertices[i*14+10] = tangent.z;
		newObjectVertices[i*14+11] = bitangent.x;
		newObjectVertices[i*14+12] = bitangent.y;
		newObjectVertices[i*14+13] = bitangent.z;
	}

	unsigned int* newObjectTriangles = new unsigned int[3 * objectNTriangles];
	for (int i = 0; i < objectNTriangles; i++)
	{
		newObjectTriangles[i * 3 + 0] = scene->mMeshes[0]->mFaces[i].mIndices[0];
		newObjectTriangles[i * 3 + 1] = scene->mMeshes[0]->mFaces[i].mIndices[1];
		newObjectTriangles[i * 3 + 2] = scene->mMeshes[0]->mFaces[i].mIndices[2];
	}

	objectTriangles = newObjectTriangles;
	objectVertices = newObjectVertices;
}

Mesh::Mesh(int _objectNAttribsPerVertex, int _objectNVertices, int _objectNTriangles, float _objectVertices[], unsigned _objectTriangles[])
{
	objectNAttribsPerVertex = _objectNAttribsPerVertex;
	objectNVertices = _objectNVertices;
	objectNTriangles = _objectNTriangles;
	objectVertices = _objectVertices;
	objectTriangles = _objectTriangles;
}

void Mesh::setup(Shader* shader)
{
	//vertex positions
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * objectNVertices * objectNAttribsPerVertex, objectVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CHECK_GL_ERROR();

	// buffer for triangle indices - ELEMENT_ARRAY
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * objectNTriangles * 3, objectTriangles, GL_STATIC_DRAW);  // 3-indices per triangle
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	CHECK_GL_ERROR();

	// VAO
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	CHECK_GL_ERROR();

	// vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	if (shader->locations.count("position"))
	{
		glEnableVertexAttribArray(shader->locations["position"]);
		glVertexAttribPointer(shader->locations["position"], 3, GL_FLOAT, GL_FALSE, objectNAttribsPerVertex * sizeof(float), (void*)(0));  // [xyz][nx,ny,nz][s,t]
		CHECK_GL_ERROR();
	}

	// triangle indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

	CHECK_GL_ERROR();

	//vertex normals
	if (shader->locations.count("normal"))
	{
		glEnableVertexAttribArray(shader->locations["normal"]);
		glVertexAttribPointer(shader->locations["normal"], 3, GL_FLOAT, GL_FALSE, objectNAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));  // [xyz][nx,ny,nz][s,t]
		CHECK_GL_ERROR();
	}

	//vertex uv
	if (shader->locations.count("uv"))
	{
		glEnableVertexAttribArray(shader->locations["uv"]);
		glVertexAttribPointer(shader->locations["uv"], 2, GL_FLOAT, GL_FALSE, objectNAttribsPerVertex * sizeof(float), (void*)(6 * sizeof(float)));
		CHECK_GL_ERROR();
	}

	//tangent
	if (shader->locations.count("tangent"))
	{
		glEnableVertexAttribArray(shader->locations["tangent"]);
		CHECK_GL_ERROR();
		glVertexAttribPointer(shader->locations["tangent"], 3, GL_FLOAT, GL_FALSE, objectNAttribsPerVertex * sizeof(float), (void*)(8 * sizeof(float)));
		CHECK_GL_ERROR();
	}

	//bitangent
	if (shader->locations.count("bitangent"))
	{
		glEnableVertexAttribArray(shader->locations["bitangent"]);
		CHECK_GL_ERROR();
		glVertexAttribPointer(shader->locations["bitangent"], 3, GL_FLOAT, GL_FALSE, objectNAttribsPerVertex * sizeof(float), (void*)(11 * sizeof(float)));
		CHECK_GL_ERROR();
	}

	glBindVertexArray(0);
	CHECK_GL_ERROR();

	numTriangles = objectNTriangles;
}
