#include "Obsidian/Core/Engine.h"
#include "App/App.h"


int main(int argc, char** argv) {
    //Bootstrap
    {
        Obsidian::Core::Engine engine;
        return engine.start(new App::App());
    }
}
