#include "stdafx.h"

#if _DEBUG
#include <vld.h>
#endif

#include "SDL.h"
#include "MainGame.h"


int main(int, char* []) {

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(22641);
#endif
	try {
		MainGame engine{};
		engine.Run();
	}
	catch (const std::exception & ex) 
	{
		std::cout << ex.what() << "\n";
	}
	catch (const std::string & ex) 
	{
		std::cout << ex << "\n";
	}
	catch (...) 
	{
		std::cout << "Huh\n";
	}

	
	return 0;
}