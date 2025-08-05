#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class World {
private:
    static const int width = 161, height = 161;
    int private_grid[width][height];
    int grid[width][height];
    std::vector<float> vertices;
    GLuint VBO, VAO;
    GLuint shaderProgram;
    
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 ourColor;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
            ourColor = aColor;
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

public:
    const float sandSize = 0.01f;
    float currentColor[3] = {1.0f, 1.0f, 0.0f}; // Yellow

    World() {
        // Initialize grids
        for(int i = 0; i < width-1; i++)
            for (int j = 0; j < height-1; j++) {
                private_grid[i][j] = 0;
                grid[i][j] = 0;
            }

        // Initialize OpenGL buffers and shaders
        initGL();
    }

    void initGL() {
        // Create and compile shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Create shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Clean up shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Create buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    void placeSand(float x, float y) {
        int gridX = (int)((x + 1.0f) * width / 2.0f);
        int gridY = (int)((1.0f - y) * height / 2.0f);
        
        if(gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
            grid[gridX][gridY] = 1;
            updateVertices();
        }
    }

    void updateVertices() {
        vertices.clear();
        for(int i = 0; i < width; i++) {
            for(int j = 0; j < height; j++) {
                if(grid[i][j] == 1) {
                    float x = (float)i / width * 2.0f - 1.0f;
                    float y = (float)j / height * 2.0f - 1.0f;
                    
                    // Add vertex data (position + color)
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(currentColor[0]);
                    vertices.push_back(currentColor[1]);
                    vertices.push_back(currentColor[2]);
                }
            }
        }
    }

    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_POINTS, 0, vertices.size() / 5);
    }

    void update() {
        // Similar physics logic as before
        for(int i = 0; i < width-1; i++) {
            for(int j = 0; j < height-1; j++) {
                if(private_grid[i][j] == 1) {
                    if(private_grid[i][j+1] == 0) {
                        grid[i][j] = 0;
                        grid[i][j+1] = 1;
                    }
                    // ... rest of the physics logic
                }
            }
        }
        updateVertices();
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Falling Sand Simulation", NULL, NULL);
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

    World world;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            // Convert screen coordinates to OpenGL coordinates
            float x = (2.0f * xpos) / 800.0f - 1.0f;
            float y = 1.0f - (2.0f * ypos) / 600.0f;
            
            world.placeSand(x, y);
        }

        world.update();
        world.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}