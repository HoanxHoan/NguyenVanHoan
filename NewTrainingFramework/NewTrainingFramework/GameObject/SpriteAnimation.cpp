#include "SpriteAnimation.h"
#include "../Globals.h"
#include <iostream>

SpriteAnimation::SpriteAnimation(Model* model, Shaders* shader, Texture* texture,
    GLint numFrames,GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime)
    : m_pModel(model), m_pShader(shader), m_pTexture(texture),
    m_numFrames(numFrames), m_numActions(numActions),
    m_currentFrame(currentFrame), m_currentAction(currentAction),
    m_frameTime(frameTime), m_currentTime(0.0f)
{
    m_scale = Vector3(200, 200, 1);
}

SpriteAnimation::~SpriteAnimation()
{
}
void SpriteAnimation::SetPosition(Vector3 pos) {
    x = pos.x;
    y = pos.y;
}
void SpriteAnimation::SetScale(Vector3 scale) {
    m_scale = scale;
}
void SpriteAnimation::Update(GLfloat deltaTime)
{
    m_currentTime += deltaTime;
    if (m_currentTime >= m_frameTime)
    {
        m_currentFrame++;
        if (m_currentFrame >= m_numFrames)
            m_currentFrame = 0;

        m_currentTime -= m_frameTime;
    }
}

void SpriteAnimation::Draw()
{
    glUseProgram(m_pShader->program);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_pTexture->Bind();
    int iTextureLoc = glGetUniformLocation(m_pShader->program, "u_texture");
    glUniform1i(iTextureLoc, 0);

    Matrix modelMatrix, scaleMatrix, translationMatrix, mvpMatrix;
    translationMatrix.SetIdentity();
    scaleMatrix.SetIdentity();
    modelMatrix.SetIdentity();
    mvpMatrix.SetIdentity();
    translationMatrix.SetTranslation(x, y, 0.0  );
    scaleMatrix.SetScale(m_scale.x, m_scale.y, m_scale.z);
    modelMatrix = scaleMatrix * translationMatrix;

    mvpMatrix = modelMatrix * Camera::GetInstance()->viewMatrix * Camera::GetInstance()->projMatrix;

    GLuint mvpLoc = glGetUniformLocation(m_pShader->program, "u_matMVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (GLfloat*)mvpMatrix.m);

    // Set other uniforms
    glUniform1f(glGetUniformLocation(m_pShader->program, "u_alpha"), 1.0f);
    glUniform1f(glGetUniformLocation(m_pShader->program, "u_currentFrame"), m_currentFrame);
    glUniform1f(glGetUniformLocation(m_pShader->program, "u_numFrames"), m_numFrames);
    glUniform1f(glGetUniformLocation(m_pShader->program, "u_currentAction"), m_currentAction);
    glUniform1f(glGetUniformLocation(m_pShader->program, "u_numActions"), m_numActions);

    m_pModel->SetAttributes();
    glDrawElements(GL_TRIANGLES, m_pModel->indexCount, GL_UNSIGNED_INT, 0);

    m_pTexture->Unbind();
    m_pModel->unBind();
}
