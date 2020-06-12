#pragma once
#include "MiniginPCH.h"

//Inspired by Gammeplay Prog Elite framework

class DataBase
{
public:
	DataBase() = default;
	virtual ~DataBase() = default;
};

template <typename T>
class Data : public DataBase
{
public:
	Data(T data) : m_Data{ data } {};

	T GetData() { return m_Data; }

	void SetData(T data) { m_Data = data; }

private:
	T m_Data;
};

class Blackboard final
{
public:
	Blackboard() = default;
	~Blackboard()
	{
		for (auto data : m_pData)
		{
			if (data.second)
			{
				delete data.second;
				data.second = nullptr;
			}
		}
		m_pData.clear();
	};

	template <typename T>
	bool AddData(const std::string& id, T data);

	template <typename T>
	bool GetData(const std::string& id, T& data);

	template <typename T>
	bool SetData(const std::string& id, T data);

private:
	std::map<std::string, DataBase*> m_pData;
};

template<typename T>
inline bool Blackboard::AddData(const std::string& id, T data)
{
	auto temp = m_pData.find(id);
	if (temp == m_pData.end())
	{
		m_pData[id] = new Data<T>(data);
		return true;
	}
	
	std::printf("Blackboard::AddData() : Already in blackboard\n");
	return false;
}

template<typename T>
inline bool Blackboard::GetData(const std::string& id, T& data)
{
	auto temp = m_pData.find(id);
	if (temp != m_pData.end())
	{
		Data<T>* storeddata = dynamic_cast<Data<T>*>((*temp).second);
		if (storeddata)
		{
			data = storeddata->GetData();
			return true;
		}
	}
	
	std::printf("Blackboard::GetData() : Data not found\n");
	return false;
}

template<typename T>
inline bool Blackboard::SetData(const std::string& id, T data)
{
	auto temp = m_pData.find(id);
	if (temp != m_pData.end())
	{
		Data<T>* storeddata = dynamic_cast<Data<T>*>((*temp).second);
		if (storeddata)
		{
			storeddata->SetData(data);
			return true;
		}

	}

	std::printf("Blackboard::SetData() : Data not found");

	return false;
}
