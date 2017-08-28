#ifndef _DebugClass_
#define _DebugClass_

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

class Debug {
private:
    static bool initiated;
    static bool running;

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

    static void update(sf::RenderWindow& window, long dt)
    {
        if (canRun())
        {
            ImGui::SFML::Update(window, dt);
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
            ImGui::ShowTestWindow();
        }
    }

    static bool canRun()
    {
        return initiated && running;
    }
};

bool Debug::initiated = false;
bool Debug::running = false;
#endif //_DebugClass_
