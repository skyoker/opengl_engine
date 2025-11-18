#include <cmath>
#include <iostream>

#include "engine.hpp"
/*
for wayland DE to work you have to add the following lines to bashrc or zshrc;

windowrulev2 = float,class:^engine2d$
windowrulev2 = size 1000 1000,class:^engine2d$
windowrulev2 = center,class:^engine2d$

replace engine2d with the title one put under
replace the size to desired size everything will 
be vectorized anyway
*/






// shaders

// vertex shader
static const char* vertexShaderSrc = R"( 
#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec3 uColor;
out vec3 vColor;
void main() {
    vColor = uColor;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

// runs ones per pixel inside the shape
static const char* fragmentShaderSrc = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

// constructor
Engine2D::Engine2D(int w, int h, const char* title)
    : width(w), height(h) {

    if (!glfwInit()) { std::cerr << "GLFW init failed\n"; exit(EXIT_FAILURE); }

    glfwWindowHintString(GLFW_WAYLAND_APP_ID, "engine2d"); // only on wayland or hyperland

    window = glfwCreateWindow(width, height, title, NULL, NULL); // creates window obj
    if (!window) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); exit(EXIT_FAILURE); }

    glfwMakeContextCurrent(window); // set context 
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cerr << "GLEW init failed\n"; glfwTerminate(); exit(EXIT_FAILURE); }

    glViewport(0, 0, width, height);

    shaderProgram = createShaderProgram();

    // persistent VAO + VBO for triangles
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW); // 3 vertices * 2 floats

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// destructor
Engine2D::~Engine2D() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
}

// frame control
bool Engine2D::isRunning() { return !glfwWindowShouldClose(window); }
int frames_done = 0;
int current_frame = 0;

void Engine2D::beginFrame() { glClearColor(0.1f, 0.1f, 0.12f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); current_frame = frames_done + 1;}
void Engine2D::endFrame() { glfwSwapBuffers(window); glfwPollEvents(); frames_done =+ 1;}

// fast triangle draw

void Engine2D::drawTriangle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec3& color) {
    float vertices[] = { a.x, a.y, b.x, b.y, c.x, c.y };

    glUseProgram(shaderProgram);
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, color.r, color.g, color.b);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // update existing buffer
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Engine2D::drawRect(float width, float height, const Vec3& color, const Vec2& blpos) {
    // bottom-left (given)
    Vec2 bl = blpos;
    // bottom-right
    Vec2 br = { blpos.x + width, blpos.y };
    // top-left
    Vec2 tl = { blpos.x, blpos.y + height };
    // top-right
    Vec2 tr = { blpos.x + width, blpos.y + height };

    // 2 triangles forming the rectangle
    drawTriangle(tl, bl, tr, color);
    drawTriangle(br, tr, bl, color);
}

void Engine2D::drawCircle(float radius, const Vec2& center, int points, Vec3 color) {
    const double radians_per_triangle = (2.0 * PI) / points; // full circle / # of slices


    for (int i = 0; i < points; i++) {
        // two points on the circle’s edge
        Vec2 p2 = {center.x + radius * static_cast<float>(cos(i * radians_per_triangle)),
                   center.y + radius * static_cast<float>(sin(i * radians_per_triangle))};
        Vec2 p3 = {center.x + radius * static_cast<float>(cos((i + 1) * radians_per_triangle)),
                   center.y + radius * static_cast<float>(sin((i + 1) * radians_per_triangle))};

        drawTriangle(center, p2, p3, color);
    }
}

// shader creation
GLuint Engine2D::createShaderProgram() {

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

// Utils

