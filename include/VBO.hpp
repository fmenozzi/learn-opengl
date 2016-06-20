#ifndef VBO_HPP_
#define VBO_HPP_

class VBO {
private:
    GLuint m_VBO;

    // No copying or assignment
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

public:
    VBO()  { glGenBuffers(1, &m_VBO);    }
    ~VBO() { glDeleteBuffers(1, &m_VBO); }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    void bufferData(GLsizei size, const GLvoid* data, GLenum usage) const {
        bind();

        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    void vertexAttribPointer(GLuint index, GLint size, GLenum type,
                             GLboolean normalized, GLsizei stride,
                             const GLvoid* pointer) {
        bind();

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }
};

#endif
