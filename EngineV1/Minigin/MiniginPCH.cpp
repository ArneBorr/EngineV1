#include "MiniginPCH.h"
#include "imgui.h"

char* BoolToXMLChar(rapidxml::xml_document<>* doc, bool value)
{
	const char* temp;
	if (value)
		temp = "true";
	else 
		temp = "false";

	return doc->allocate_string(temp);
}

char* IntToXMLChar(rapidxml::xml_document<>* doc, int value)
{
	//https://stackoverflow.com/questions/4583409/add-number-double-float-as-attribute-to-rapidxml-node
	char temp[16];
	sprintf_s(temp, "%i", value);
	return doc->allocate_string(temp);
}

char* FloatToXMLChar(rapidxml::xml_document<>* doc, float value)
{
	
	//https://stackoverflow.com/questions/4583409/add-number-double-float-as-attribute-to-rapidxml-node
	char temp[32];
	sprintf_s(temp, "%f", value);
	return doc->allocate_string(temp);
}

//https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#using-imgui-with-stl
static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

//https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#using-imgui-with-stl
bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
{
	using namespace ImGui;
	if (values.empty()) { return false; }
	return Combo(label, currIndex, vector_getter,
		static_cast<void*>(&values), values.size());
};

