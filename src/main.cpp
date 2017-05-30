#define APP_DEBUG 1

#include <iostream>
#include <memory>

#include "Obsidian/Core/Engine.h"
#include "Obsidian/Util/Logger.h"
#include "Obsidian/Util/Time.h"


std::unique_ptr<Obsidian::Core::Engine> bootstrap()
{
    Obsidian::Util::Logger::setPrefix([]() {
        std::string prefix = "[";
        
        prefix += std::to_string(Obsidian::Util::Time::getCurrentTime());
        prefix += "] ";

        return prefix;
    });

    if(LIKELY(true) && APP_DEBUG) {
        Obsidian::Util::Logger::write("Bootstraping");
    }

    std::unique_ptr<Obsidian::Core::Engine> core = std::make_unique<Obsidian::Core::Engine>();

    return core;
}

int main(int argc, char** argv) {

    std::unique_ptr<Obsidian::Core::Engine> app = bootstrap();

    app->registerRenderer();
    
    return 0;
}
