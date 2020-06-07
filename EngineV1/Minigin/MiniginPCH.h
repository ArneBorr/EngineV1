#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "GameInfo.h"
#include "GameObjectManager.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

#include "structs.h"
#include "rapidxml.hpp"
#pragma warning(push, 0)   
#include <Box2D.h>
#pragma warning(pop)

static const float M_PI = 3.14159265359f;
static const float M_PPM = 2.f;

char* IntToXMLChar(rapidxml::xml_document<>& doc, int value);
char* FloatToXMLChar(rapidxml::xml_document<>& doc, float value);

template <class T>
inline void Clamp(T& value, T min, T max) 
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}



