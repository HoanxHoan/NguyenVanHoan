#include "Overlay.h"
#include "../Globals.h"

void OverlayObject::Draw()
{
    if (!visible) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(objShader->program);
    objTex->Bind();
    int iTextureLoc = glGetUniformLocation(objShader->program, "u_texture");
    glUniform1i(iTextureLoc, 0);

    int alphaLoc = glGetUniformLocation(objShader->program, "u_alpha");
    glUniform1f(alphaLoc, alpha);
    SetMVP();
    GLuint mvpLoc = glGetUniformLocation(objShader->program, "u_mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvpMatrix.m);

    objModel->SetAttributes();
    glDrawElements(GL_TRIANGLES, objModel->indexCount, GL_UNSIGNED_INT, 0);

    objTex->Unbind();
    objModel->unBind();
    glDisable(GL_BLEND);
}
