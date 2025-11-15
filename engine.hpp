#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils.hpp"

#define PI 3.14159653589738
 
enum class DE { Unknown, Hyprland, Sway, GNOME, KDE, XFCE };

struct Engine2D {
    Engine2D(int w = 800, int h = 600, const char* title = "2D Engine");
    ~Engine2D();

    bool isRunning();
    void beginFrame();
    void endFrame();

    // Fast drawing
    void drawTriangle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec3& color);
    void drawRect(const float width, const float height, const Vec3& color, const Vec2& tlpos);
    void drawCircle(float radius, const Vec2& center, int points, Vec3 color);
    int width, height;




private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO;


    GLuint createShaderProgram();
};  

