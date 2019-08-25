#pragma once
#include<string>

#include"Common.h"

// SqliteManager�Ƕ�Sqlite�Ľӿڽ���һ��򵥵ķ�װ�� 
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
	sqlite3* _db;       // ���ݿ���� 
};

//����ļ����㣺RAII
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
//���ݹ���ģ��
#define DB_NAME "doc.db"
#define TB_NAME "tb_doc"

//Ϊ�˷�����м�����������Ƴɵ���ģʽ
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
	//���������ݿ�
	void GetDoc(const string& path, std::set<string>& dbset);
	//����ĳ��path�µ��������ĵ�
	void InsertDoc(const string& path, const string& name);
	//��������
	void DeleteDoc(const string& path, const string& name);
	//ɾ������
	void Search(const string& key, vector<std::pair<string, string>>& docinfos);
	//��������
	void SplitHighlight(const string& str, const string& key, string& prefix, string& highlight, string& suffix);
	//��������

private:
	DataManager()
	{}

	SqliteManager _dbmgr;
	std::mutex _mtx;
};

