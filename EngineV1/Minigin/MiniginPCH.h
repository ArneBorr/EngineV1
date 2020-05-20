#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameInfo.h"
#include "GameObjectManager.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

#include "structs.h"
#include "rapidxml.hpp"

char* IntToXMLChar(rapidxml::xml_document<>& doc, int value);
char* FloatToXMLChar(rapidxml::xml_document<>& doc, float value);
