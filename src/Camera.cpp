#include <Camera.hpp>

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_Position,
                       m_Position + m_Front,
                       m_Up);
}

void Camera::processKeyboard(CameraMovement direction, GLfloat dt) {
    GLfloat velocity = m_MovementSpeed * dt;

    if (direction == CameraMovement::FORWARD)
        m_Position += m_Front * velocity;
    if (direction == CameraMovement::BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == CameraMovement::LEFT)
        m_Position -= m_Right * velocity;
    if (direction == CameraMovement::RIGHT)
        m_Position += m_Right * velocity;
}

void Camera::processMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrain_pitch) {
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw   += xoffset;
    m_Pitch += yoffset;

    if (constrain_pitch) {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processScroll(GLfloat yoffset) {
    if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
        m_Zoom -= yoffset;
    if (m_Zoom <= 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom >= 45.0f)
        m_Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));
    front.y = std::sin(glm::radians(m_Pitch));
    front.z = std::sin(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}
