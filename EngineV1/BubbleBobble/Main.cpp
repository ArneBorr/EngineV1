#include "stdafx.h"

#if _DEBUG
#include <vld.h>
#endif

#include "SDL.h"
#include "MainGame.h"


int main(int, char* []) {

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1652);
#endif

	MainGame engine{};
	engine.Run();
	return 0;
}