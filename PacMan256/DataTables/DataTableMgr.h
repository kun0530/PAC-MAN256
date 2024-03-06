#pragma once
#include "Singleton.h"
#include "DataTable.h"

class DataTableMgr : public Singleton<DataTableMgr>
{
	friend Singleton<DataTableMgr>;
private:
	DataTableMgr();
	~DataTableMgr();

	std::unordered_map<DataTable::Types, DataTable*> tables;

public:
	void Init();
	void Release();

	template<typename T>
	T* Get(DataTable::Types type);
};

template<typename T>
inline T* DataTableMgr::Get(DataTable::Types type)
{
	auto find = tables.find(type);
	if (find == tables.end())
	{
		return nullptr;
	}
	return dynamic_cast<T*>(find->second);
}

#define DT_MGR (DataTableMgr::Instance())
#define STRING_TABLE (DT_MGR.Get<StringTable>(DataTable::Types::String))
#define ZOMBIE_TABLE (DT_MGR.Get<ZombieTable>(DataTable::Types::Zombie))