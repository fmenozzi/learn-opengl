#ifndef VAO_HPP_
#define VAO_HPP_

#include <glad/glad.h>

class VAO {
private:
    GLuint m_VAO;

    // No copying or assignment
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

public:
    VAO()  { glGenVertexArrays(1, &m_VAO);    }
    ~VAO() { glDeleteVertexArrays(1, &m_VAO); }

    void bind() {
        glBindVertexArray(m_VAO);
    }

    void unbind() {
        glBindVertexArray(0);
    }
};

#endif
