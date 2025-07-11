#pragma once

#include "../Utilities/utilities.h" // dùng Matrix, Vector3
#include "../Shaders.h"
class Camera
{

private:
    static Camera* instance;

    Camera();
public:
    ~Camera();
    // Delete copy constructor and assignment operator
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    // Singleton access
    static Camera* GetInstance();
    static void Destroy();
    // Camera parameters
    float nearPlane;
    float farPlane;
    float fov;      
    float speed;

    // Camera position and orientation
    Vector3 position;
    Vector3 target;
    Vector3 up;
    // Matrices
    Matrix viewMatrix;
    Matrix projMatrix;

    // Functions
    void UpdateViewMatrix();
    void UpdateProjMatrix(float aspectRatio);
    Matrix GetWorldCameraMatrix();
    void MoveForward(float deltaTime);
    void MoveBackward(float deltaTime);
    void MoveRight(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveUp(float deltaTime);
    void MoveDown(float deltaTime);
    void RotateLeft(float deltaTime);
    void RotateRight(float deltaTime);
    void RotateAroundY(float angle);
    void RotateAroundX(float angle);
    void Rotateup(float deltaTime);
    void Rotatedown(float deltaTime);
    void testRotateY(float deltaTime);
    void testRotateX(float deltaTime);
    // Setters
    void SetNearFar(float nearP, float farP);
    void SetFOV(float fovDeg);
    void SetSpeed(float s);
    void SetPosition( Vector3& pos);
    void SetTarget( Vector3& tar);
    void SetUp( Vector3& u);
};
