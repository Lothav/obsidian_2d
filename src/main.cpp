#include "Obsidian2D/Core/Kernel.h"
#include "App/App.h"


int main(int argc, char** argv) {
	//Scoped Bootstrap
	{
		Obsidian2D::Core::Kernel kernel;
		return kernel.start(new App::App());
	}
}
