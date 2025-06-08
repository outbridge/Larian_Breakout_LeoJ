#include "pch.h"

#include "States/GameStateManager.h"
#include "States/PlayingState.h"

// Global GLFW window pointer
GLFWwindow* g_Window = nullptr;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // prevent resizing

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_Window = glfwCreateWindow(800, 600, "Breakout", nullptr, nullptr);
    if (!g_Window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(g_Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(g_Window, FramebufferSizeCallback);

    GameStateManager gsm;
    gsm.PushState(std::make_unique<PlayingState>(g_Window));

    while (!glfwWindowShouldClose(g_Window))
    {
        GameState* state = gsm.GetActiveState();
        if (state)
        {
            state->HandleInput();
            state->Update(0.016f);
            state->Render();
        }

        glfwSwapBuffers(g_Window);
        glfwPollEvents();
    }

    glfwDestroyWindow(g_Window);
    glfwTerminate();
    return 0;
}
