#ifndef _DebugClass_
#define _DebugClass_

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include "imgui/imgui.h"
#include "../imgui/imgui-SFML.h"
#include "Log.h"

class Debug {
private:
    static bool initiated;
    static bool running;
    static bool open;

protected:
    Debug() {}
    ~Debug() {}

public:
    static void startStop(sf::RenderTarget& window)
    {
        if (!initiated)
        {
            ImGui::SFML::Init(window);
        }

        initiated = true;
        running = !running;
    }

    static void processEvent(sf::Event event)
    {
        if (canRun())
        {
            ImGui::SFML::ProcessEvent(event);
        }
    }

    static void update(sf::RenderWindow& window, float dt)
    {
        if (canRun())
        {
            ImGui::SFML::Update(window, dt / 1000.0);
        }
    }

    static void render(sf::RenderTarget& window)
    {
        if (canRun())
        {
            ImGui::SFML::Render(window);
        }
    }

    static void shutdown()
    {
        if (initiated)
        {
            ImGui::SFML::Shutdown();
        }
    }

    static void test()
    {
        if (canRun())
        {
            static bool show_app_log = false;

            if (show_app_log)
            {
                Log::ShowLog(&show_app_log);
            }

            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_MenuBar;
            ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin("Obsidian2D", &Debug::open, window_flags))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    ImGui::MenuItem("Log", NULL, &show_app_log);
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::End();
        }
    }

    static bool canRun()
    {
        return initiated && running;
    }
};

bool Debug::initiated = false;
bool Debug::running = false;
bool Debug::open = false;
#endif //_DebugClass_
