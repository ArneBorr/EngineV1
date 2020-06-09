#include "MiniginPCH.h"
#include "Script.h"

Script::Script(const std::string& name)
	: m_Name{ name }
{
}

void Script::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc.allocate_attribute("Name", m_Name.c_str()));
}
