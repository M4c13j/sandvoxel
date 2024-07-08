#include "raylib.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
    // Initialize raylib ======================
    InitWindow(1200, 700, "Raylib + ImGui Example");
    SetTargetFPS(60);

    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Imgui init =============================
    // GLFWwindow* window = glfwCreateWindow(600, 400, "Dawg", NULL, NULL);
    GLFWwindow* window = (GLFWwindow*) GetWindowHandle();
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop ==============================
    while (!WindowShouldClose() && !glfwWindowShouldClose(window))
    {
        // Poll and handle user inputs
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a window called "Hello, world!" and append into it.
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text."); // Display some text
        ImGui::End();

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {
            ImGui::Begin("Hello, world!");
            ImGui::Text("This is some useful text.");
            ImGui::Text("Pufcio królik.");
            ImGui::End();
        }

        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("This is a raylib window with ImGui!", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Fps: %d", GetFPS()), 10, 30, 20, DARKGRAY);

        glfwMakeContextCurrent(window);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Rendering
        EndDrawing();
        glfwSwapBuffers(window);

    }
    string dawg;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    CloseWindow();

    return 0;
}
