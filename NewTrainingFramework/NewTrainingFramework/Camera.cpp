#include "GameObject/Camera.h"
#include <cmath>
#define DEG2RAD 0.0174532925199432957f
Camera* Camera::instance = nullptr;
Camera::Camera()
{
    nearPlane = 0.1f;
    farPlane = 100.0f;
    fov = 45.0f;
    speed = 0.3f;
    target = Vector3(0.0f, 0.0f, 0.0f);
    position = Vector3(0.0f, 0.0f, 1.0f);
    up = Vector3(0.0f, 1.0f, 0.0f);
    UpdateViewMatrix();
    //UpdateProjMatrix(4.0f / 3.0f); 
    UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);

}

Camera::~Camera()
{
}
Camera* Camera::GetInstance()
{
    if (!instance)
        instance = new Camera();
    return instance;
}

void Camera::Destroy()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}
Matrix Camera::GetWorldCameraMatrix()
{
    Vector3 zaxis = (target - position).Normalize(); // Forward
    Vector3 xaxis = up.Cross(zaxis).Normalize();     // Right
    Vector3 yaxis = zaxis.Cross(xaxis);              // Up

    Matrix R, T;
    R.SetIdentity();
    T.SetIdentity();

    // R - rotation matrix
    R.m[0][0] = xaxis.x;
    R.m[0][1] = xaxis.y;
    R.m[0][2] = xaxis.z;

    R.m[1][0] = yaxis.x;
    R.m[1][1] = yaxis.y;
    R.m[1][2] = yaxis.z;

    R.m[2][0] = zaxis.x;
    R.m[2][1] = zaxis.y;
    R.m[2][2] = zaxis.z;

    // T - translation matrix
    T.SetTranslation(-position.x, -position.y, -position.z);

    // WorldCamera = R * T
    return R * T;
}
void Camera::UpdateViewMatrix()
{
    Vector3 zaxis = (position - target).Normalize();
    Vector3 xaxis = up.Cross(zaxis).Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis);

    viewMatrix.SetIdentity();

    viewMatrix.m[0][0] = xaxis.x;
    viewMatrix.m[0][1] = yaxis.x;
    viewMatrix.m[0][2] = zaxis.x;
    viewMatrix.m[0][3] = 0;

    viewMatrix.m[1][0] = xaxis.y;
    viewMatrix.m[1][1] = yaxis.y;
    viewMatrix.m[1][2] = zaxis.y;
    viewMatrix.m[1][3] = 0;

    viewMatrix.m[2][0] = xaxis.z;
    viewMatrix.m[2][1] = yaxis.z;
    viewMatrix.m[2][2] = zaxis.z;
    viewMatrix.m[2][3] = 0;

    viewMatrix.m[3][0] = -xaxis.Dot(position);
    viewMatrix.m[3][1] = -yaxis.Dot(position);
    viewMatrix.m[3][2] = -zaxis.Dot(position);
    viewMatrix.m[3][3] = 1;
}


void Camera::UpdateProjMatrix(float aspectRatio)
{
    projMatrix.SetPerspective(fov * DEG2RAD, aspectRatio, nearPlane, farPlane);
}
void Camera::UpdateOrthographic(float left, float right, float bottom, float top)
{
    projMatrix.SetOrthographic(left, right, bottom, top, nearPlane, farPlane);
}
void Camera::SetNearFar(float nearP, float farP)
{
    nearPlane = nearP;
    farPlane = farP;
}

void Camera::SetFOV(float fovDeg)
{
    fov = fovDeg;
}

void Camera::SetSpeed(float s)
{
    speed = s;
}
void Camera::SetPosition( Vector3& pos)
{
    position = pos;
}

void Camera::SetTarget( Vector3& tar)
{
    target = tar;
}

void Camera::SetUp(Vector3& u)
{
    up = u;
}
void Camera::MoveForward(float deltaTime)
{
    Vector3 viewDir = (target - position).Normalize();
    Vector3 deltaMove = viewDir * speed * deltaTime;
    position += deltaMove;
    target += deltaMove;
    UpdateViewMatrix();
}

void Camera::MoveBackward(float deltaTime)
{
    Vector3 viewDir = (target - position).Normalize();
    Vector3 deltaMove = viewDir * speed * deltaTime;
    position -= deltaMove;
    target -= deltaMove;
    UpdateViewMatrix();
}

void Camera::MoveRight(float deltaTime)
{
    Vector3 viewDir = (target - position).Normalize();
    Vector3 right = viewDir.Cross(up).Normalize();
    Vector3 deltaMove = right * speed * deltaTime;
    position -= deltaMove;
    target -= deltaMove;
    UpdateViewMatrix();
}

void Camera::MoveLeft(float deltaTime)
{
    Vector3 viewDir = (target - position).Normalize();
    Vector3 right = viewDir.Cross(up).Normalize();
    Vector3 deltaMove = right * speed * deltaTime;
    position += deltaMove;
    target += deltaMove;
    UpdateViewMatrix();
}
void Camera::MoveUp(float deltaTime)
{
    Vector3 deltaMove = up.Normalize() * speed * deltaTime;
    position += deltaMove;
    target += deltaMove;

    UpdateViewMatrix(); 
}

void Camera::MoveDown(float deltaTime)
{
    Vector3 deltaMove = up.Normalize() * speed * deltaTime;
    position -= deltaMove;
    target -= deltaMove;

    UpdateViewMatrix(); 
}
void Camera::RotateLeft(float deltaTime)
{
    RotateAroundY(deltaTime * speed);
}

void Camera::RotateRight(float deltaTime)
{
    RotateAroundY(-deltaTime * speed);
}
void Camera::Rotateup(float deltaTime)
{
    RotateAroundX(-deltaTime * speed);
}

void Camera::Rotatedown(float deltaTime)
{
    RotateAroundX(+deltaTime * speed);
}

void Camera::RotateAroundY(float angle)
{

    Vector3 viewDir = target - position;
    float distance = viewDir.Length();
    viewDir.Normalize();
    Matrix rotY;
    rotY.SetRotationAngleAxis(angle, up.x, up.y, up.z);

    Vector4 localTarget(viewDir.x * distance, viewDir.y * distance, viewDir.z * distance, 0);

    Vector4 rotatedTarget = localTarget * rotY;

    target = position + Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

  
    UpdateViewMatrix();
}
void Camera::RotateAroundX(float angle)
{
    Vector3 viewDir = target - position;
    float distance = viewDir.Length();
    viewDir.Normalize();
    Matrix rotX;
    rotX.SetRotationAngleAxis(angle, 1, 0, 0);

    Vector4 localTarget(viewDir.x * distance, viewDir.y * distance, viewDir.z * distance, 0);

    Vector4 rotatedTarget = localTarget * rotX;

    target = position + Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

    UpdateViewMatrix();
}
void Camera::testRotateY(float deltaTime)
{
    float angle = deltaTime ;
    Vector3 viewDir = target - position;
    float distance = viewDir.Length();
    Vector4 localTarget(0, 0, distance, 1);
    Matrix rotY;
    rotY.SetRotationAngleAxis(angle, up.x, up.y, up.z);
  
    Vector4 localNewTarget = localTarget * rotY;
 

    Vector4 worldNewTarget = localNewTarget * viewMatrix.Inverse();

  
    target = Vector3(worldNewTarget.x, worldNewTarget.y, worldNewTarget.z);

 
    UpdateViewMatrix();
}
void Camera::testRotateX(float deltaTime)
{
    float angle = deltaTime;
    Vector3 viewDir = target - position;
    float distance = viewDir.Length();
    Vector4 localTarget(0, 0, distance, 1);
    Matrix rotX;
    rotX.SetRotationAngleAxis(angle, 1, 0, 0);

    Vector4 localNewTarget = localTarget * rotX;


    Vector4 worldNewTarget = localNewTarget * viewMatrix.Inverse();


    target = Vector3(worldNewTarget.x, worldNewTarget.y, worldNewTarget.z);


    UpdateViewMatrix();
}
void Camera::RotateAroundTarget(float deltaAngle)
{
   
    Vector3 offset = position - target; 
    float radius = offset.Length();

    float currentAngle = atan2(offset.z, offset.x);
  
    float newAngle = currentAngle + deltaAngle;

    //float r = 2 * radius * sin(newAngle);
 
    position.x = target.x + radius * cos(newAngle);
    position.z = target.z + radius * sin(newAngle);


    UpdateViewMatrix();

 
    //printf("Camera pos: (%f, %f, %f)\n", position.x, position.y, position.z);
    //printf("Target: (%f, %f, %f)\n", target.x, target.y, target.z);
}



