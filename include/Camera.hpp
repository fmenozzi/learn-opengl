#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

static const GLfloat YAW         = -90.0f;
static const GLfloat PITCH       =   0.0f;
static const GLfloat SPEED       =   3.0f;
static const GLfloat SENSITIVITY =   0.25f;
static const GLfloat ZOOM        =  45.0f;

class Camera {
private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    GLfloat m_Yaw;
    GLfloat m_Pitch;

    GLfloat m_MovementSpeed;
    GLfloat m_MouseSensitivity;
    GLfloat m_Zoom;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           GLfloat yaw = YAW, 
           GLfloat pitch = PITCH) 
        
        :  m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
           m_MovementSpeed(SPEED), 
           m_MouseSensitivity(SENSITIVITY), 
           m_Zoom(ZOOM) {

        m_Position = position;
        m_WorldUp  = up;
        m_Yaw      = yaw;
        m_Pitch    = pitch;

        updateCameraVectors();
    }

    Camera(GLfloat xpos, 
           GLfloat ypos, 
           GLfloat zpos, 
           GLfloat xup, 
           GLfloat yup, 
           GLfloat zup, 
           GLfloat yaw, 
           GLfloat pitch) 
        
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
          m_MovementSpeed(SPEED), 
          m_MouseSensitivity(SENSITIVITY), 
          m_Zoom(ZOOM) {
        
        m_Position = glm::vec3(xpos, ypos, zpos);
        m_WorldUp  = glm::vec3(xup, yup, zup);
        m_Yaw      = yaw;
        m_Pitch    = pitch;

        updateCameraVectors();
    }

    glm::mat4 getViewMatrix();

    void processKeyboard(CameraMovement direction, GLfloat dt);
    void processMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrian_pitch = true);
    void processScroll(GLfloat yoffset);

    GLfloat getZoom() {
        return m_Zoom;
    }

private:
    void updateCameraVectors();
};

#endif
