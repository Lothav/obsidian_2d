#include "Obsidian2D/Core/Engine.h"
#include "App/App.h"


int main(int argc, char** argv) {
	//Scoped Bootstrap
	{
		Obsidian2D::Core::Engine engine;
		return engine.start(new App::App());
	}
}
