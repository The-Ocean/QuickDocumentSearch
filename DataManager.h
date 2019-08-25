#pragma once
#include<string>

#include"Common.h"

// SqliteManager是对Sqlite的接口进行一层简单的封装。 
class SqliteManager {
public:
	SqliteManager()
		:_db(nullptr)
	{}

	~SqliteManager() {
		Close();
	}
	void Open(const string& path);
	void Close();
	void ExecuteSql(const string& sql);
	void GetTable(const string& sql, int& row, int& col, char**& ppRet);

	SqliteManager(const SqliteManager&) = delete;
	SqliteManager& operator=(const SqliteManager&) = delete;
private:
	/*
	int sqlite3_get_table{
		sqlite3 *db,		//An open database
		const char *zSql,	//SQL to be evaluated
		char ***pazResult,	//Results of the query
		int *pnRow,			//Number of result rows written here
		int *pnColumn,		//Number 0f result columns written here
		char **pzErrmsg		//Error msg written here
	};
	*/
	sqlite3* _db;       // 数据库对象 
};

//引入的技术点：RAII
class AutoGetTable
{
public:
	AutoGetTable(SqliteManager& sm, const string& sql, int& row, int& col, char**& ppRet)
	{
		sm.GetTable(sql, row, col, ppRet);
		_ppRet = ppRet;
	}
	~AutoGetTable()
	{
		sqlite3_free_table(_ppRet);
	}

	AutoGetTable(const AutoGetTable&) = delete;
	AutoGetTable operator=(const AutoGetTable&) = delete;

private:
	char** _ppRet;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//数据管理模块
#define DB_NAME "doc.db"
#define TB_NAME "tb_doc"

//为了方便进行加锁，我们设计成单例模式
class DataManager
{
public:
	static DataManager* GetInstance()
	{
		static DataManager datamger;
		datamger.Init();

		return &datamger;
	}

	void Init();
	//建表，打开数据库
	void GetDoc(const string& path, std::set<string>& dbset);
	//查找某个path下的所有子文档
	void InsertDoc(const string& path, const string& name);
	//插入数据
	void DeleteDoc(const string& path, const string& name);
	//删除数据
	void Search(const string& key, vector<std::pair<string, string>>& docinfos);
	//搜索数据
	void SplitHighlight(const string& str, const string& key, string& prefix, string& highlight, string& suffix);
	//高亮处理

private:
	DataManager()
	{}

	SqliteManager _dbmgr;
	std::mutex _mtx;
};

