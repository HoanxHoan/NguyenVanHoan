#include <stdafx.h>
#include "../stdafx.h" 
#include "Model.h"

Model::Model()
{
    vboId = 0;
    iboId = 0;
    indexCount = 0;
    verticesData = nullptr;
    indices = nullptr;
}

Model::~Model()
{
    if (vboId != 0)
        glDeleteBuffers(1, &vboId);
    if (iboId != 0)
        glDeleteBuffers(1, &iboId);
    delete[] verticesData;
    delete[] indices;
}

bool Model::LoadNFG(const char* filename)
{
    FILE* f;

    if (fopen_s(&f, filename, "rb") != 0)
    {
        std::cout << "Cannot open file.\n";
        return false;
    }

    char temp[256];
    int nrVertices = 0;
    fscanf_s(f, "%s %d", temp, sizeof(temp), &nrVertices); // NrVertices: <num>

    verticesData = new Vertex[nrVertices];

    for (int i = 0; i < nrVertices; i++)
    {
        int index;
        Vertex v;

        fscanf_s(f, "%d.", &index);
        fscanf_s(f, " pos:[%f, %f, %f];", &v.pos.x, &v.pos.y, &v.pos.z);
        fscanf_s(f, " norm:[%f, %f, %f];", &v.normal.x, &v.normal.y, &v.normal.z);
        fscanf_s(f, " binorm:[%f, %f, %f];", &v.binormal.x, &v.binormal.y, &v.binormal.z);
        fscanf_s(f, " tgt:[%f, %f, %f];", &v.tangent.x, &v.tangent.y, &v.tangent.z);
        fscanf_s(f, " uv:[%f, %f];", &v.uv.x, &v.uv.y);

        verticesData[i] = v;
    }

    int nrIndices = 0;
    fscanf_s(f, "%s %d", temp, sizeof(temp), &nrIndices); // NrIndices: <num>

    indices = new unsigned int[nrIndices];

    for (int i = 0; i < nrIndices / 3; i++)
    {
        int idxLine;
        unsigned int a, b, c;

        fscanf_s(f, "%d.", &idxLine);
        fscanf_s(f, " %d, %d, %d", &a, &b, &c);

        indices[i * 3 + 0] = a;
        indices[i * 3 + 1] = b;
        indices[i * 3 + 2] = c;
    }

    fclose(f);

    indexCount = nrIndices;

    // Upload to GPU
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, nrVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    std::cout << "Loaded model with " << nrVertices << " vertices and " << indexCount << " indices.\n";

    return true;
}
void Model::SetAttributes()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)12 + sizeof(Vector3));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)24 + sizeof(Vector3));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)36 + sizeof(Vector3));
}
void Model::unBind() {
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
