#include "pch.h"
#include "DataTableMgr.h"
// #include "StringTable.h"

DataTableMgr::DataTableMgr()
{
}

DataTableMgr::~DataTableMgr()
{
	Release();
}

void DataTableMgr::Init()
{
	Release();
}

void DataTableMgr::Release()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}
