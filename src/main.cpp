#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <luggagemaker/ui/ui.hpp>

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int argc, char* argv[])
{
    if (!glfwInit()) return 1;

    // TODO: Lista rzeczy do zrobienia
    // TODO: Dodać edycję przedmiotów w liście wyjściowej typu usunięcie, zmniejszenie ilości
    // TODO: Dodać dynamiczną zmianę wagi przedmiotów od sprecyzowania rozmiaru/marki
    // TODO: Dodać opcję zapisywania zestawów i przeglądania
    // TODO: Dodać ogólny profil użytkownika, który spamiętuje zestaw przedmiotów/preferencje
    // TODO: Popracować nad stabilnością aplikacji

#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    GLFWwindow* window = glfwCreateWindow(1600, 1000, "LuggageMaker", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    static const ImWchar polskie_zakresy[] =
    {
        0x0020, 0x00FF,
        0x0100, 0x017F,
        0
    };

    io.Fonts->AddFontFromFileTTF("/System/Library/Fonts/Supplemental/Arial.ttf", 16.0f, nullptr, polskie_zakresy);
    ApplyModernDarkTheme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    constexpr auto clear_color = ImVec4(0.08f, 0.09f, 0.10f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderPackingAppUI();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    //Punta Arenas - cold ex.
    //Dubaj - hot ex.
    //Bracelona - medium ex.


    return 0;
}