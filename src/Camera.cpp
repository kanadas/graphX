#include "Camera.h"

#include <GLFW/glfw3.h>
#include "Input.h"

Camera::Camera(Camera::Projection projection, float left, float right, float bottom, float top, float near, float far)
    : projection(projection)
{
    aspectRatio = (right - left) / (top - bottom);
    zoomLevel = far - near;
    setProjection(projection, left, right, bottom, top, near, far);
    recalculateViewMatrix();
}

Camera::Camera(Projection projection, float aspectRatio)
    : projection(projection)
    , aspectRatio(aspectRatio)
{
    zoomLevel = 1.0f;
    setProjection(projection, -aspectRatio, aspectRatio, -1, 1, 1, 2);
    recalculateViewMatrix();
}

void Camera::setProjection(Camera::Projection projection, float left, float right, float bottom, float top, float near, float far)
{
    switch (projection) {
    case Ortographic:
        projectionMatrix = mat4::Ortho(left, right, bottom, top, near, far);
        break;
    case Perspective:
        projectionMatrix = mat4::Frumstumf(left, right, bottom, top, near, far);
        break;
    }
}

void Camera::recalculateViewMatrix()
{
    viewMatrix = mat4::Rotation(vec3(0, 0, 1), -rotation) * mat4::Translation(position * -1);
}

void Camera::onUpdate(float ts)
{
    if (Input::isKeyPressed(GLFW_KEY_A)) {
        position.arr[0] -= translationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_S)) {
        position.arr[1] -= translationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_D)) {
        position.arr[1] += translationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_F)) {
        position.arr[0] += translationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_Z)) {
        position.arr[2] -= translationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_X)) {
        position.arr[2] += translationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_Q)) {
        rotation -= rotationSpeed * ts;
    }
    if (Input::isKeyPressed(GLFW_KEY_R)) {
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
}

bool Camera::onMouseScrolled(MouseScrolledEvent& e)
{
    zoomLevel -= e.getYOffset() * zoomSpeed;
    zoomLevel = std::min(std::max(zoomLevel, minZoom), maxZoom);
    setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel, zoomLevel, 2 * zoomLevel);
    return false;
}

bool Camera::onWindowResized(WindowResizeEvent& e)
{
    aspectRatio = (float)e.getWidth() / (float)e.getHeight();
    setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel, zoomLevel, 2 * zoomLevel);
    return false;
}
