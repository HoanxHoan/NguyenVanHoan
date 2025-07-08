#include "stdafx.h"
#include "Object.h"
#include <cstdio>
#include <iostream>

Object::Object()
{
    vboId = 0;
    iboId = 0;
    textureID = 0;
    vertexCount = 0;
    indexCount = 0;

    modelMatrix.SetIdentity();
    viewMatrix.SetIdentity();
    projMatrix.SetIdentity();
}

Object::~Object()
{
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &iboId);
    glDeleteTextures(1, &textureID);
}

bool Object::Init(const char* modelFile, const char* textureFile)
{
    if (!LoadModel(modelFile))
        return false;
    if (!LoadTexture(textureFile))
        return false;

    return true;
}

bool Object::LoadModel(const char* filename)
{
    FILE* f;
    if (fopen_s(&f, filename, "rb") != 0)
    {
        std::cout << "Cannot open model file.\n";
        return false;
    }

    char temp[256];
    fscanf_s(f, "%s %d", temp, sizeof(temp), &vertexCount);

    Vertex* verticesData = new Vertex[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
        int index;
        fscanf_s(f, "%d.", &index);
        fscanf_s(f, " pos:[%f, %f, %f];", &verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z);
        fscanf_s(f, " norm:[%f, %f, %f];", &verticesData[i].normal.x, &verticesData[i].normal.y, &verticesData[i].normal.z);
        fscanf_s(f, " binorm:[%f, %f, %f];", &verticesData[i].binormal.x, &verticesData[i].binormal.y, &verticesData[i].binormal.z);
        fscanf_s(f, " tgt:[%f, %f, %f];", &verticesData[i].tangent.x, &verticesData[i].tangent.y, &verticesData[i].tangent.z);
        fscanf_s(f, " uv:[%f, %f];", &verticesData[i].uv.x, &verticesData[i].uv.y);
    }

    fscanf_s(f, "%s %d", temp, sizeof(temp), &indexCount);
    unsigned int* indices = new unsigned int[indexCount];

    for (int i = 0; i < indexCount / 3; i++)
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

    // Upload to GPU
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] verticesData;
    delete[] indices;

    std::cout << "Loaded model with " << vertexCount << " vertices and " << indexCount << " indices.\n";
    return true;
}

bool Object::LoadTexture(const char* filename)
{
    int width, height, bpp;
    char* imageData = LoadTGA(filename, &width, &height, &bpp);

    if (!imageData)
    {
        std::cout << "Failed to load texture.\n";
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] imageData;
    std::cout << "Loaded texture: " << filename << "\n";
    return true;
}

void Object::SetMVP(Matrix model, Matrix view, Matrix proj)
{
    modelMatrix = model;
    viewMatrix = view;
    projMatrix = proj;
}


void Object::Draw(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);

    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int iTextureLoc = glGetUniformLocation(shaderProgram, "u_texture");
    glUniform1i(iTextureLoc, 0);

    // Set MVP
    Matrix mvpMatrix = modelMatrix * viewMatrix * projMatrix;
    GLuint mvpLoc = glGetUniformLocation(shaderProgram, "u_mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvpMatrix.m);

    // Bind Buffers
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

    // Attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, binormal));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // Draw
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
