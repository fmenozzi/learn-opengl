#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <Shader.hpp>
#include <Texture2D.hpp>

#include <cstdio>
#include <cstdlib>

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
    glfwWindowHint(GLFW_SAMPLES, 4);

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Getting Started - Coordinate Systems", nullptr, nullptr);
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

    glfwSetKeyCallback(window, key_callback);

    GLfloat vertices[] = {
    //    x      y      z      s     t
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    };

	glm::vec3 cubes[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
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

    // Container
    Texture2D texture1;
    texture1.loadFromFile("container.jpg")
            .setWrapST(GL_REPEAT, GL_REPEAT)
            .setMinMagFilter(GL_LINEAR, GL_LINEAR)
            .generateTexture();

    // Awesome face
    Texture2D texture2;
    texture2.loadFromFile("awesomeface.png")
            .setWrapST(GL_REPEAT, GL_REPEAT)
            .setMinMagFilter(GL_LINEAR, GL_LINEAR)
            .generateTexture();

    // Shaders
    Shader shader;
    shader.attachFromFile("01-getting-started/06-coordinate-systems.vert")
          .attachFromFile("01-getting-started/06-coordinate-systems.frag")
          .link();

    // Position attribute
    GLint pos_attrib = glGetAttribLocation(shader.get(), "position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)0);

    // Texture attribute
    GLint tex_attrib = glGetAttribLocation(shader.get(), "texCoord");
    glEnableVertexAttribArray(tex_attrib);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture1.bind(GL_TEXTURE0);
        shader.bind(0, "ourTexture1");

        texture2.bind(GL_TEXTURE1);
        shader.bind(1, "ourTexture2");

        glm::mat4 model;
		model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view;
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);

        shader.bind(view, "view");
        shader.bind(projection, "projection");
        for (GLuint i = 0; i < 10; i++) {
            GLfloat time    = glfwGetTime();
            GLfloat sintime = std::sin(time);

            glm::mat4 model;
            model = glm::translate(model, cubes[i]);
            model = glm::rotate(model, time * glm::radians(20.0f), glm::vec3(1.0f, 0.3f*sintime, 0.5f));

            shader.bind(model, "model");
            shader.activate();

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
    }

    cleanup(EXIT_SUCCESS);
}
