#include "../stdafx.h"
#include "Overlay.h"
#include "../Globals.h"
#include <sstream>
#include<Box2d/Box2D.h>
void Overlay::SetOverlayPosition(GLfloat px, GLfloat py) {
    x = px;
    y = py;

}
void Overlay::SetOverlaySize(GLfloat sx, GLfloat sy) {
    scale.x = sx;
    scale.y = sy;
}
void Overlay::GetgameTime(float t) {
    gameTime = t;
}

void Overlay::Draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(O_Shader->program);

    // === Texture 
    if (O_Texture) {
        O_Texture->Bind();

        int iTextureLoc = glGetUniformLocation(O_Shader->program, "u_texture");
        glUniform1i(iTextureLoc, 0);
    }

    // === Alpha Overlay 
    int alphaLoc = glGetUniformLocation(O_Shader->program, "u_alpha");
    if (alphaLoc != -1)
        glUniform1f(alphaLoc, alpha);

    // === Base Darkness 
    float timeOfDay = fmod(gameTime, 60.0f); 
    float t = timeOfDay / 60.0f; 
    float daylight = 0.5f * (2.0f - cos(6.2831f * t));
    daylight = pow(daylight, 5.0f);  // try( 2.0-5.0)
    float baseDarkness = 0.7f * (1.7f - daylight);

    int baseDarkLoc = glGetUniformLocation(O_Shader->program, "baseDarkness");
    glUniform1f(baseDarkLoc, baseDarkness);

    // === Light Positions ===
    int lightCountLoc = glGetUniformLocation(O_Shader->program, "lightCount");
    glUniform1i(lightCountLoc, (int)lightPositions.size());

    for (int i = 0; i < lightPositions.size(); ++i) {
        std::string lightName = "lightPositions[" + std::to_string(i) + "]";
        GLint loc = glGetUniformLocation(O_Shader->program, lightName.c_str());
        glUniform2f(loc, lightPositions[i].x, lightPositions[i].y);
    }

    // === Other uniforms ===
    glUniform1f(glGetUniformLocation(O_Shader->program, "radius"), radius);
    glUniform1f(glGetUniformLocation(O_Shader->program, "softness"), softness);
    glUniform2f(glGetUniformLocation(O_Shader->program, "screenSize"),
        (float)Globals::screenWidth, (float)Globals::screenHeight);

    // === Model Transform (Overlay = fullscreen quad) ===
    Matrix modelMatrix, scaleMatrix, translationMatrix, mvpMatrix;
    translationMatrix.SetIdentity();
    scaleMatrix.SetIdentity();
    modelMatrix.SetIdentity();
    mvpMatrix.SetIdentity();

    scaleMatrix.SetScale(scale.x, scale.y, 0.0f);
    translationMatrix.SetTranslation(x, y, 0.0f);
    modelMatrix = scaleMatrix * translationMatrix;
    mvpMatrix = modelMatrix * Camera::GetInstance()->viewMatrix * Camera::GetInstance()->projMatrix;

    GLuint mvpLoc = glGetUniformLocation(O_Shader->program, "u_mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvpMatrix.m);

    // === Draw overlay ===
    O_Model->SetAttributes();
    glDrawElements(GL_TRIANGLES, O_Model->indexCount, GL_UNSIGNED_INT, 0);
    O_Model->unBind();

    if (O_Texture) O_Texture->Unbind();
    glDisable(GL_BLEND);
}