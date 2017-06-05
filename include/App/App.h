#ifndef _APP_APP_
#define _APP_APP_
    #define APP_DEBUG 1

#include "Obsidian2D/Core/App.h"

namespace App {
    class App: public Obsidian2D::Core::App
    {
    private:
    protected:
    public:
        App()
        {
            //https://harrylovescode.gitbooks.io/vulkan-api/content/chap05/chap05-linux.html
            //renderer->draw(Util::Shapes::Square(10, 10, Util::Color::White));
        }
    };
}

#endif //_APP_APP_