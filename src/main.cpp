#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

class ParticleSystem {
private:
    static const int MAX_PARTICLES = 10000;
    struct Particle {
        float x, y;      
        float vx, vy;    
        float r, g, b;   
        bool active;
    };

    std::vector<Particle> particles;
    std::vector<float> vertices;
    GLuint VBO, VAO;
    GLuint shaderProgram;
    
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 ourColor;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            ourColor = aColor;
            gl_PointSize = 3.0;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 ourColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(ourColor, 1.0);
        }
    )";

    void initGL() {
        
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

       
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

public:
    float currentColor[3] = {1.0f, 0.0f, 0.0f};
    const float GRAVITY = 0.001f;
    const float DAMPING = 0.8f;

    ParticleSystem() {
        particles.reserve(MAX_PARTICLES);
        vertices.reserve(MAX_PARTICLES * 5); // position (2) + color (3)
        initGL();
    }

    void addParticle(float x, float y) {
        if (particles.size() < MAX_PARTICLES) {
            Particle p;
            p.x = x;
            p.y = y;
            p.vx = (rand() % 100 - 50) * 0.0001f; // Small random initial velocity
            p.vy = 0;
            p.r = currentColor[0];
            p.g = currentColor[1];
            p.b = currentColor[2];
            p.active = true;
            particles.push_back(p);
        }
    }

    void update() {
        for (auto& p : particles) {
            if (!p.active) continue;

            
            p.vy -= GRAVITY;

            
            p.x += p.vx;
            p.y += p.vy;

            
            if (p.y < -1.0f) {
                p.y = -1.0f;
                p.vy = -p.vy * DAMPING;
            }
            if (p.x < -1.0f) {
                p.x = -1.0f;
                p.vx = -p.vx * DAMPING;
            }
            if (p.x > 1.0f) {
                p.x = 1.0f;
                p.vx = -p.vx * DAMPING;
            }

            
            for (auto& other : particles) {
                if (&other != &p && other.active) {
                    float dx = other.x - p.x;
                    float dy = other.y - p.y;
                    float dist = std::sqrt(dx*dx + dy*dy);
                    if (dist < 0.02f) {
                        // Simple collision response
                        float nx = dx / dist;
                        float ny = dy / dist;
                        float relativeVel = (p.vx - other.vx) * nx + (p.vy - other.vy) * ny;
                        if (relativeVel > 0) {
                            float j = -(1.0f + DAMPING) * relativeVel / 2.0f;
                            p.vx -= j * nx;
                            p.vy -= j * ny;
                            other.vx += j * nx;
                            other.vy += j * ny;
                        }
                    }
                }
            }
        }
        updateVertices();
    }

    void updateVertices() {
        vertices.clear();
        for (const auto& p : particles) {
            if (p.active) {
                vertices.push_back(p.x);
                vertices.push_back(p.y);
                vertices.push_back(p.r);
                vertices.push_back(p.g);
                vertices.push_back(p.b);
            }
        }
    }

    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glEnable(GL_POINT_SMOOTH);
        glDrawArrays(GL_POINTS, 0, vertices.size() / 5);
    }

    void changeColor(float r, float g, float b) {
        currentColor[0] = r;
        currentColor[1] = g;
        currentColor[2] = b;
    }
};

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Particle System", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    ParticleSystem particles;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            particles.changeColor(1.0f, 0.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            particles.changeColor(0.0f, 1.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            particles.changeColor(0.0f, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
            particles.changeColor(1.0f, 1.0f, 0.0f);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            float x = (2.0f * xpos) / 800.0f - 1.0f;
            float y = 1.0f - (2.0f * ypos) / 600.0f;
            particles.addParticle(x, y);
        }

        particles.update();
        particles.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}