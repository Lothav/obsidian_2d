#include "Obsidian2D/Core/Kernel.h"
#include "App/App.h"

int main(int argc, char** argv) {
	//Scoped Bootstrap
	{
		Obsidian2D::Core::Kernel kernel;
		return kernel.start(new App::App());
	}
}

/*
#include <iostream>
#include "gameloop.h"

void printArgs(int argc, char** argv) {
    for(int i = 0; i < argc; i++) {
        std::cout << "arg #" << i << " is " << argv[i] << std::endl;
    }
}

int main(int argc, char** argv) {
    printArgs(argc, argv);
    run();
    return 0;
}
*/
