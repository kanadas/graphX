#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "linalg.h"
#include "Event.h"
class Camera {
public:
    enum Projection {
        Ortographic,
        Perspective
    };
    Camera(Projection projection, float left, float right, float bottom, float top, float near, float far);
    Camera(Projection projection, float aspectRatio);
    Camera(Projection projection);
    void setProjection(Projection projection, float left, float right, float bottom, float top, float near, float far);
    void setProjection(Camera::Projection projection, float left, float right, float bottom, float top);
    vec3 getPosition() { return position; }
    void setPosition(vec3 pos)
    {
        position = pos;
        recalculateViewMatrix();
    }
    float getRotation() { return rotation; }
    void setRotation(float rot)
    {
        rotation = rot;
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
    float rotation = 0.0f; //TODO quaternion

    float aspectRatio;
    float zoomLevel = 1.0f;
    float translationSpeed = 5.0f;
    float rotationSpeed = 1.0f;
    float zoomSpeed = 0.25f;
    const float minZoom = 0.25f;
    const float maxZoom = 10.0f;
    const float defaultNear = 1.0f;
    const float defaultFar = 10.0f;

    void recalculateViewMatrix();

    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onWindowResized(WindowResizeEvent& e);
    bool onKey(KeyEvent& e);
};

#endif // __CAMERA_H_
