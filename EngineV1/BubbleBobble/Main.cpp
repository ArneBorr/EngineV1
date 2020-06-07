#include "stdafx.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "MainGame.h"


int main(int, char* []) {

#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(1154);
#endif

	MainGame engine{};
	engine.Run();
	return 0;
}