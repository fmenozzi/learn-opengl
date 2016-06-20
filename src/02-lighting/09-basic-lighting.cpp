#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <VAO.hpp>
#include <VBO.hpp>
#include <Shader.hpp>
#include <Texture2D.hpp>
#include <Camera.hpp>

#include <cstdio>
#include <cstdlib>

#define GLERR do {\
    GLuint glerr;\
    while ((glerr = glGetError()) != GL_NO_ERROR)\
        fprintf(stderr, "%s:%d glGetError() = 0x%04x\n", __FILE__, __LINE__, glerr);\
} while (0)

const GLuint WIDTH  = 800;
const GLuint HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;

GLfloat xlast = WIDTH  / 2.0f;
GLfloat ylast = HEIGHT / 2.0f;

bool first_mouse = true;

bool keys[1024];

glm::vec3 lamp_pos(1.2f, 1.0f, 2.0f);

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (0 <= key && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow*, double xpos, double ypos) {
    if (first_mouse) {
        xlast = xpos;
        ylast = ypos;

        first_mouse = false;
    }

    GLfloat xoffset = xpos - xlast;
    GLfloat yoffset = ylast - ypos;

    xlast = xpos;
    ylast = ypos;

    camera.processMouse(xoffset, yoffset);
}

void scrollCallback(GLFWwindow*, double, double yoffset) {
    camera.processScroll(yoffset);
}

void move() {
    if (keys[GLFW_KEY_W])
        camera.processKeyboard(CameraMovement::FORWARD, delta_time);
    if (keys[GLFW_KEY_S])
        camera.processKeyboard(CameraMovement::BACKWARD, delta_time);
    if (keys[GLFW_KEY_A])
        camera.processKeyboard(CameraMovement::LEFT, delta_time);
    if (keys[GLFW_KEY_D])
        camera.processKeyboard(CameraMovement::RIGHT, delta_time);
}

void cleanup(int status) {
    glfwTerminate();
    exit(status);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting - Basic Lighting", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        cleanup(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    GLfloat vertices[] = {
	//    vx     vy     vz     nx     ny     nz
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };

    // Shaders
    Shader lighting_shader;
    lighting_shader.attachFromFile("02-lighting/09-basic-lighting-lighting.vert")
                   .attachFromFile("02-lighting/09-basic-lighting-lighting.frag")
                   .link();
    Shader lamp_shader;
    lamp_shader.attachFromFile("02-lighting/09-basic-lighting-lamp.vert")
               .attachFromFile("02-lighting/09-basic-lighting-lamp.frag")
               .link();

    GLint pos_attrib = glGetAttribLocation(lighting_shader.get(), "position");
    GLint nor_attrib = glGetAttribLocation(lighting_shader.get(), "normal");

    // Container
    VAO container_vao;
    VBO vbo;
    vbo.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
    container_vao.bind();
    vbo.vertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)0);
    vbo.vertexAttribPointer(nor_attrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
    container_vao.unbind();

    // Light VAO
    VAO lamp_vao;
    lamp_vao.bind();
    vbo.bind();
    vbo.vertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)0);
    vbo.vertexAttribPointer(nor_attrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
    lamp_vao.unbind();

    lighting_shader.use();
    lighting_shader.bind(lamp_pos, "lightPos");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        move();

        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(camera.getZoom(), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);

        // Lighting shader
        lighting_shader.use();
        lighting_shader.bind(glm::vec3(1.0f, 0.5f, 0.31f), "objectColor");
        lighting_shader.bind(glm::vec3(1.0f, 1.0f, 1.0f),  "lightColor");
        lighting_shader.bind(camera.getPosition(), "viewPos");
        lighting_shader.bind(view, "view");
        lighting_shader.bind(proj, "proj");
        lighting_shader.bind(glm::mat4(), "model");

        // Draw container
        container_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        container_vao.unbind();

        glm::mat4 model;
        model = glm::translate(model, lamp_pos);
        model = glm::scale(model, glm::vec3(0.2f)); // Shrink cube

        // Lamp shader
        lamp_shader.use();
        lamp_shader.bind(view,  "view");
        lamp_shader.bind(proj,  "proj");
        lamp_shader.bind(model, "model");

        // Draw lamp
        lamp_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        lamp_vao.unbind();

        glfwSwapBuffers(window);
    }

    cleanup(EXIT_SUCCESS);
}
