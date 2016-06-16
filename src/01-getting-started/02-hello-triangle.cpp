#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

#define GLSL(src) "#version 150 core\n" #src

#define GLERR do {\
    GLuint glerr;\
    while ((glerr = glGetError()) != GL_NO_ERROR)\
        fprintf(stderr, "%s:%d glGetError() = 0x%04x\n", __FILE__, __LINE__, glerr);\
} while (0)



const GLuint WIDTH  = 800;
const GLuint HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Getting Started - Hello Triangle", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        cleanup(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);

    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLint status;
    GLchar infolog[512];

    // Vertex shader
    const GLchar* vertex_source = GLSL(
        in vec3 position;

        void main() {
            gl_Position = vec4(position, 1.0);
        }
    );

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infolog);
        fprintf(stderr, "Failed to compile vertex shader\n");
        fprintf(stderr, "%s\n", infolog);
        cleanup(EXIT_FAILURE);
    }

    // Fragment shader
    const GLchar* fragment_source = GLSL(
        out vec4 color;

        void main() {
            color = vec4(1.0, 0.5f, 0.2f, 1.0f);
        }
    );
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, infolog);
        fprintf(stderr, "Failed to compile fragment shader\n");
        fprintf(stderr, "%s\n", infolog);
        cleanup(EXIT_FAILURE);
    }

    // Create shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader_program, 512, nullptr, infolog);
        fprintf(stderr, "Failed to link shader program\n");
        fprintf(stderr, "%s\n", infolog);
        cleanup(EXIT_FAILURE);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(shader_program);

    // Position attribute
    GLint pos_attrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (GLvoid*)0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    cleanup(EXIT_SUCCESS);
}
