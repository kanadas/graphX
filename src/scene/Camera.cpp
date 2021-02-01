#include "Camera.h"

#include <GLFW/glfw3.h>
#include "Input.h"
#include "utils/Log.h"
#include <imgui/imgui.h>

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
    lastMouseX = Input::GetMouseX();
    lastMouseY = Input::GetMouseY();
}

Camera::Camera(Projection projection)
    : Camera(projection, Input::GetWindowWidth() / Input::GetWindowHeight())
{
    lastMouseX = Input::GetMouseX();
    lastMouseY = Input::GetMouseY();
}

void Camera::setProjection(Camera::Projection projection, float left, float right, float bottom, float top, float near, float far)
{
    //TRACE("Projection {} ({}, {}, {}, {}, {}, {})", projection == Ortographic ? "Orto" : "Perspect", left, right, bottom, top, near, far);
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
    viewMatrix = rotation.inverse().toMatrix() * mat4::Translation(position * -1);
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::updateRotation(vec3 axis, float delta_angle)
{
    rot3 nrot(rotation * axis, delta_angle);
    rotation = rotation.compose(nrot);
}

void Camera::onUpdate(float ts)
{
    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        position = position + (rotation * vec3(translationSpeed * ts, 0, 0) * -1);
    }
    if (Input::IsKeyPressed(GLFW_KEY_Q)) {
        position = position + (rotation * vec3(0, translationSpeed * ts, 0) * -1);
    }
    if (Input::IsKeyPressed(GLFW_KEY_E)) {
        position = position + (rotation * vec3(0, translationSpeed * ts, 0));
    }
    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        position = position + (rotation * vec3(translationSpeed * ts, 0, 0));
    }
    if (Input::IsKeyPressed(GLFW_KEY_W)) {
        position = position + (rotation * vec3(0, 0, translationSpeed * ts) * -1);
    }
    if (Input::IsKeyPressed(GLFW_KEY_S)) {
        position = position + (rotation * vec3(0, 0, translationSpeed * ts));
    }
    if (Input::IsKeyPressed(GLFW_KEY_F)) {
        zoomLevel += ts * zoomSpeed;
        zoomLevel = std::min(std::max(zoomLevel, minZoom), maxZoom);
        setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
    }
    if (Input::IsKeyPressed(GLFW_KEY_R)) {
        zoomLevel -= ts * zoomSpeed;
        zoomLevel = std::min(std::max(zoomLevel, minZoom), maxZoom);
        setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
    }
    if (Input::IsKeyPressed(GLFW_KEY_L)) {
        updateRotation(vec3(0,1,0), -rotationSpeed * ts);
    }
    if (Input::IsKeyPressed(GLFW_KEY_J)) {
        updateRotation(vec3(0,1,0), rotationSpeed * ts);
    }
    if (Input::IsKeyPressed(GLFW_KEY_K)) {
        updateRotation(vec3(1,0,0), -rotationSpeed * ts);
    }
    if (Input::IsKeyPressed(GLFW_KEY_I)) {
        updateRotation(vec3(1,0,0), rotationSpeed * ts);
    }
    if (Input::IsKeyPressed(GLFW_KEY_U)) {
        updateRotation(vec3(0,0,1), -rotationSpeed * ts);
    }
    if (Input::IsKeyPressed(GLFW_KEY_O)) {
        updateRotation(vec3(0,0,1), rotationSpeed * ts);
    }

    float mouseX = Input::GetMouseX();
    float mouseY = Input::GetMouseY();
    float dX = lastMouseX - mouseX;
    float dY = lastMouseY - mouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    //if (!Input::IsImGuiWindowHovered() && (dX != 0 || dY != 0)) {
    if (!ImGui::GetIO().WantCaptureMouse && (dX != 0 || dY != 0)) {
        dX = dX * 2 * aspectRatio * zoomLevel / Input::GetWindowWidth();
        dY = dY * 2 * zoomLevel / Input::GetWindowHeight();
        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ) {
            position = position + (rotation * vec3(dX, -dY, 0));
        } else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            updateRotation(vec3(dY, dX, 0), rotationSpeed * ts);
        }
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
    if (!ImGui::GetIO().WantCaptureMouse) {
        zoomLevel -= e.getYOffset() * zoomSpeed;
        zoomLevel = std::min(std::max(zoomLevel, minZoom), maxZoom);
        setProjection(projection, -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
    }
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
