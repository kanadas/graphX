#include "Camera.h"

#include <GLFW/glfw3.h>
#include "Input.h"
#include "utils/Log.h"

Camera::Camera(Camera::Projection projection, float left, float right, float bottom, float top, float near, float far)
    : projection(projection)
{
    aspectRatio = (right - left) / (top - bottom);
    zoomLevel = far - near;
    TRACE("Creating camera aspect = {}, zoom = {}", aspectRatio, zoomLevel);
    setProjection(projection, left, right, bottom, top, near, far);
    recalculateViewMatrix();
}

Camera::Camera(Projection projection, float aspectRatio)
    : projection(projection)
    , aspectRatio(aspectRatio)
{
    TRACE("Creating camera aspect = {}", aspectRatio);
    zoomLevel = 1.0f;
    setProjection(projection, -aspectRatio, aspectRatio, -1, 1);
    recalculateViewMatrix();
}

Camera::Camera(Projection projection)
    : Camera(projection, Input::GetWindowWidth() / Input::GetWindowHeight())
{
}

void Camera::setProjection(Camera::Projection projection, float left, float right, float bottom, float top, float near, float far)
{
    TRACE("Projection {} ({}, {}, {}, {}, {}, {})", projection == Ortographic ? "Orto" : "Perspect", left, right, bottom, top, near, far);
    switch (projection) {
    case Ortographic:
        projectionMatrix = mat4::Ortho(left, right, bottom, top, near, far);
        break;
    case Perspective:
        projectionMatrix = mat4::Frumstumf(left, right, bottom, top, near, far);
        break;
    }
}

void Camera::setProjection(Camera::Projection projection, float left, float right, float bottom, float top)
{
    setProjection(projection, left, right, bottom, top, defaultNear, defaultFar);
}

void Camera::recalculateViewMatrix()
{
    mat4 transform =  mat4::Translation(position) * mat4::Rotation(vec3(0, 0, 1), rotation);
    viewMatrix = transform.inverseAffineIsometry();
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::onUpdate(float ts)
{
    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        position.arr[0] -= translationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_S)) {
        position.arr[1] -= translationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        position.arr[1] += translationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_F)) {
        position.arr[0] += translationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_Z)) {
        position.arr[2] -= translationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_X)) {
        position.arr[2] += translationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_Q)) {
        rotation -= rotationSpeed * ts;
    }
    if (Input::IsKeyPressed(GLFW_KEY_R)) {
        rotation += rotationSpeed * ts;
    }
    if (rotation > 180) {
        rotation -= 180;
    } else if (rotation < -180) {
        rotation += 180;
    }

    recalculateViewMatrix();
}

void Camera::onEvent(Event& e)
{
    DispatchEvent<MouseScrolledEvent>(e, BIND_EVENT_FN(Camera::onMouseScrolled));
    DispatchEvent<WindowResizeEvent>(e, BIND_EVENT_FN(Camera::onWindowResized));
    DispatchEvent<KeyEvent>(e, BIND_EVENT_FN(Camera::onKey));
}

bool Camera::onMouseScrolled(MouseScrolledEvent& e)
{
    zoomLevel -= e.getYOffset() * zoomSpeed;
    zoomLevel = std::min(std::max(zoomLevel, minZoom), maxZoom);
    setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
    return false;
}

bool Camera::onWindowResized(WindowResizeEvent& e)
{
    aspectRatio = (float)e.getWidth() / (float)e.getHeight();
    TRACE("Resized, ratio = {}", aspectRatio);
    setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
    return false;
}

bool Camera::onKey(KeyEvent& e)
{
    if(e.getKey() == GLFW_KEY_GRAVE_ACCENT && e.getAction() == KeyEvent::Action::PRESS) {
        TRACE("Switching camera");
        if(projection == Projection::Ortographic) {
            projection = Projection::Perspective;
        } else {
            projection = Projection::Ortographic;
        }
        setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
    }
    return false;
}
