#include "MiniginPCH.h"

char* BoolToXMLChar(rapidxml::xml_document<>& doc, bool value)
{
	const char* temp;
	if (value)
		temp = "true";
	else 
		temp = "false";

	return doc.allocate_string(temp);
}

char* IntToXMLChar(rapidxml::xml_document<>& doc, int value)
{
	//https://stackoverflow.com/questions/4583409/add-number-double-float-as-attribute-to-rapidxml-node
	char temp[16];
	sprintf_s(temp, "%i", value);
	return doc.allocate_string(temp);
}

char* FloatToXMLChar(rapidxml::xml_document<>& doc, float value)
{
	
	//https://stackoverflow.com/questions/4583409/add-number-double-float-as-attribute-to-rapidxml-node
	char temp[32];
	sprintf_s(temp, "%f", value);
	return doc.allocate_string(temp);
}


