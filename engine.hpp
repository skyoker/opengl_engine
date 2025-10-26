#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct Vec2 { float x, y; };
struct Vec3 { float r, g, b; };

struct Engine2D {
    Engine2D(int w = 800, int h = 600, const char* title = "2D Engine");
    ~Engine2D();

    bool isRunning();
    void beginFrame();
    void endFrame();

    // Fast drawing
    void drawTriangle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec3& color);

private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO;

    int width, height;

    GLuint createShaderProgram();
};
