#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define PI 3.14159653589738
 
enum class DE { Unknown, Hyprland, Sway, GNOME, KDE, XFCE };

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
    void drawRect(const float width, const float height, const Vec3& color, const Vec2& tlpos);
    void drawCircle(float radius, const Vec2& center, int points, Vec3 color);




private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO;

    int width, height;

    GLuint createShaderProgram();
    Vec2 addVec2pos(const Vec2 pos1, const Vec2 pos2);
    Vec2 addVec2posx(const Vec2 pos, float tobeadded);
    Vec2 addVec2posy(const Vec2 pos, float tobeadded);
};  

