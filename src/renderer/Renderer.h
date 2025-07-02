#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../simulation/Interactions.h"




// Vertex Shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec3 uColor;
out vec3 vertexColor;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    vertexColor = uColor;
}
)";
// Fragment Shader
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

// RENDERER 

class Renderer {      
    GLFWwindow* window;
    unsigned int shaderProgram;
    unsigned int VAO, VBO;

    unsigned int compileShader(const char* source, GLenum type) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Vertex shader compilation failed:\n" << infoLog << "\n";
        }
        return shader;
    }

public:
    void init(int width, int height) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, "Renderer", NULL, NULL);
        if (!window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize Glew\n";
            return;
        }

        glViewport(0,0,width,height);

        unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "Shader program linking failed: " << infoLog << "\n";
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glEnable(GL_BLEND);
        glPointSize(5.0);
    }
    
    void drawPolygon(const Polygon& pgon, const glm::vec3& color ) {
        std::vector<PointR2> vertices = pgon.vertices();
        if (vertices.empty()) return;

        glUseProgram(shaderProgram);
        int colorLocation = glGetUniformLocation(shaderProgram, "uColor");
        glUniform3f(colorLocation, color.r, color.g, color.b);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        //If polygon is convex perform usual render but use decomposition otherwise
        if (pgon.isConvex() ) {
            std::vector<float> vertexData;
            for (const auto& vertex : vertices) {
                vertexData.push_back(vertex.x());
                vertexData.push_back(vertex.y());
            }
            glBufferData(GL_ARRAY_BUFFER,vertexData.size()*sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN,0,vertices.size());
            glDrawArrays(GL_LINE_LOOP,0,vertices.size());
        } else {
            std::vector<Polygon> decomposed = ConvexDecomposition::decompose(pgon);
            for (const Polygon& decomposed_piece : decomposed) {
                std::vector<PointR2> decomposed_vertices = decomposed_piece.vertices();
                if (decomposed_vertices.size() < 3) continue;

                std::vector<float> vertexData;
                for (const auto& vertex : decomposed_vertices) {
                    vertexData.push_back(vertex.x());
                    vertexData.push_back(vertex.y());
                }
                glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLE_FAN, 0, decomposed_vertices.size());
            }
            std::vector<float> boundary;
            for (const auto& vertex : vertices) {
                boundary.push_back(vertex.x());
                boundary.push_back(vertex.y());
            }
            glBufferData(GL_ARRAY_BUFFER, boundary.size()*sizeof(float), boundary.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
        }
        glBindVertexArray(0);
        
    }
    
    void drawPoint(const PointR2& point, const glm::vec3& color) {
        double vertexData[] = {
            point.x(),
            point.y()
        };
        glUseProgram(shaderProgram);
        int colorLocation = glGetUniformLocation(shaderProgram, "uColor");
        glUniform3f(colorLocation, color.r, color.g, color.b);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS,0,1);
        glBindVertexArray(0);
    }

    void clear() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void swapBuffers() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    GLFWwindow* getWindow() const {
        return window;
    }
// Cleanup
    ~Renderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }
    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }
};
namespace RendererUtils {
    glm::vec2 screenToWorld(GLFWwindow* window, double x, double y); 
    glm::vec3 randomColor(int i, unsigned int seed);
    // simple utilities for the scene

    //color randomizer
    glm::vec3 randomColor(int i, unsigned int seed) {
        // Simple hash for determinism
        unsigned int SEED = static_cast<unsigned>(seed * 2654435761 % 360);
        float r = static_cast<float>((SEED * 37) % 256) / 255.0f;
        float g = static_cast<float>((SEED * 57) % 256) / 255.0f;
        float b = static_cast<float>((SEED * 97) % 256) / 255.0f;
        
        return glm::vec3(r, g, b);
    }
    // handle conversion of pixels to OpenGL world
    glm::vec2 screenToWorld(GLFWwindow* window, double x, double y) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return glm::vec2(
            2.0 * x / width - 1.0,
            1.0 - 2.0 * y / height
        );
    }
    
}
