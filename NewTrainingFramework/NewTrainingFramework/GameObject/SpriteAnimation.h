#pragma once
#include <memory>
#include "Model.h"
#include "../Shaders.h"
#include "Textures.h"
#include "../Utilities/utilities.h"
#include "Camera.h" 
class SpriteAnimation
{
public:
    SpriteAnimation(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame,GLint numActions, GLint currentAction, GLfloat frameTime);

    ~SpriteAnimation();

    void Update(GLfloat deltaTime);
    void Draw();
    void SetPosition(Vector3 pos);
    void SetScale(Vector3 scale);
    void SetCurrentAction(GLint action) { m_currentAction = action; }
    GLint GetCurrentAction() const { return m_currentAction; }

    void SetFrameTime(GLfloat frameTime) { m_frameTime = frameTime; }
    GLfloat GetFrameTime() const { return m_frameTime; }

    void SetCurrentFrame(GLint frame) { m_currentFrame = frame; }
    GLint GetCurrentFrame() const { return m_currentFrame; }

    void SetNumFrames(GLint numFrames) { m_numFrames = numFrames; }
    GLint GetNumFrames() const { return m_numFrames; }

    void SetNumActions(GLint numActions) { m_numActions = numActions; }
    GLint GetNumActions() const { return m_numActions; }

    void SetShader(Shaders* shader) { m_pShader = shader; }
    Shaders* GetShader() const { return m_pShader; }

    void SetTexture(Texture* texture) { m_pTexture = texture; }
    Texture* GetTexture() const { return m_pTexture; }

    void SetModel(Model* model) { m_pModel = model; }
    Model* GetModel() const { return m_pModel; }
    int x, y;

private:
    Model* m_pModel;
    Shaders* m_pShader;
    Texture* m_pTexture;

    GLint m_numFrames;
    GLint m_numActions;
    GLint m_currentFrame;
    GLint m_currentAction;

    GLfloat m_frameTime;
    GLfloat m_currentTime;

    Vector3 m_scale;
};
