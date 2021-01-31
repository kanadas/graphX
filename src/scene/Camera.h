#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "utils/linalg.h"
#include "Event.h"

class Camera {
public:
    enum Projection {
        Ortographic,
        Perspective
    };
    Camera(Projection projection, float aspectRatio);
    Camera(Projection projection);
    vec3 getPosition() { return position; }
    void setPosition(vec3 pos)
    {
        position = pos;
        recalculateViewMatrix();
    }
    rot3 getRotation() { return rotation; }
    void setRotation(rot3 rotation)
    {
        this->rotation = rotation;
        recalculateViewMatrix();
    }
    mat4 getProjectionMatrix() { return projectionMatrix; }
    mat4 getViewMatrix() { return viewMatrix; }
    mat4 getViewProjectionMatrix() { return viewProjectionMatrix; }

    void onUpdate(float ts);
    void onEvent(Event& e);

    float getAspectRatio() const { return aspectRatio; }

    float getZoomLevel() const { return zoomLevel; }
    void getZoomLevel(float level) { zoomLevel = level; }

private:
    Projection projection;
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 viewProjectionMatrix;

    vec3 position = { 0.0f, 0.0f, 0.0f };
    rot3 rotation = {{ 1.0f, 0.0f, 0.0f }, 0.0f};

    float aspectRatio;
    float zoomLevel = 1.0f;
    float translationSpeed = 5.0f;
    float rotationSpeed = 2.0f;
    float mouseRotationSpeed = 0.5f;
    float zoomSpeed = 0.25f;
    const float minZoom = 0.25f;
    const float maxZoom = 10.0f;
    const float defaultNear = 1.0f;
    const float defaultFar = 3.0f;
    float lastMouseX, lastMouseY;

    Camera(Projection projection, float left, float right, float bottom, float top, float near, float far);
    void setProjection(Projection projection, float left, float right, float bottom, float top, float near, float far);
    void setProjection(Projection projection, float left, float right, float bottom, float top);

    void recalculateViewMatrix();
    void updateRotation(vec3 axis, float delta_angle);

    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onWindowResized(WindowResizeEvent& e);
    bool onKey(KeyEvent& e);
};

#endif // __CAMERA_H_
