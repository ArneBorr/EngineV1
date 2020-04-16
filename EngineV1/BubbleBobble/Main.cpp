#include "stdafx.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
//#include <vld.h>
#endif

#include "SDL.h"
#include "MainGame.h"

int main(int, char* []) {
	MainGame engine{};
	engine.Run();
	return 0;
}