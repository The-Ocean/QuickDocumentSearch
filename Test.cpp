#include"Common.h"
#include"DataManager.h"
#include"ScanManager.h"

void TestDirectoryList()
{
	vector<string> dirs;
	vector<string> files;
	DirectoryList("D:\\��ѧ", dirs, files);
	for (const auto& e : dirs)
	{
		cout << e << endl;
	}

	for (const auto& e : files)
	{
		cout << e << endl;
	}

}

void TestSqlite()
{
	SqliteManager sq;
	sq.Open("test.db");

	string createtb_sql = "create table tb_doc(id integer primary key autoincrement, doc_path text, doc_name text)";
	sq.ExecuteSql(createtb_sql);


	string insert_sql = "insert into tb_doc(doc_path, doc_name) values('����·��', '�����ļ�')";
	sq.ExecuteSql(insert_sql);

	insert_sql = "insert into tb_doc(doc_path, doc_name) values('stl\', 'vector.h')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc(doc_path, doc_name) values('stl\', 'list.h')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc(doc_path, doc_name) values('stl\', 'queue.h')";
	sq.ExecuteSql(insert_sql);

	string query_sql = "select * from TB_NAME where doc_path = 'stl'";
	int row, col;
	char** ppRet;
	sq.GetTable(query_sql, row, col, ppRet);
	//for (int i = 1; i < row; ++i)
	//{
	//	for (int j = 0; j < col; ++j)
	//	{
	//		cout << ppRet[i*col + j] << " ";
	//	}
	//	cout << endl;
	//}

	sqlite3_free_table(ppRet);

	AutoGetTable agt(sq, query_sql, row, col, ppRet);
	for (int i = 1; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << ppRet[i*col + j] << " ";
		}
		cout << endl;
	}

}

void TestSearch()
{
	//ScanMannager scanmger;
	//scanmger.Scan("D:\\Visual Studio 2017\\C����ѧϰ");

	ScanMannager::CreateIntance();

	//DataManager datamgr;
	//datamgr.Init();

	vector<std::pair<string, string>> docinfos;
	string key;
	cout << "==========================================================" << endl;
	cout << "������Ҫ�����Ĺؼ��֣�";

	while (std::cin >> key)
	{
		docinfos.clear();
		DataManager::GetInstance()->Search(key, docinfos);
		printf("%-30s %-30s\n", "����", "·��");
		for (const auto& e : docinfos)
		{
			//cout << e.first << "  " << e.second << endl;
			//printf("%-30s %-30s\n", e.first.c_str(), e.second.c_str());
			string prefix, highlight, suffix;
			const string& name = e.first;
			const string& path = e.second;
			DataManager::GetInstance()->SplitHighlight(name, key, prefix, highlight, suffix);

			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix;

			//����30�ո񲢶���
			for (size_t i = name.size(); i < 30; i++)
			{
				cout << " ";
			}


			printf("%-30s\n", path.c_str());
		}
		cout << "==========================================================" << endl;
		cout << "������Ҫ�����Ĺؼ��֣�";
	}
}

void TestScanManager()
{
	//ScanMannager scanmger;
	////scanmger.Scan("D:/start");
	//scanmger.Scan("D:\\��ѧ");


}

void TestPinyin()
{
	string allspell = ChineseConvertPinYinAllSpell("ƴ�� ��ð� pinyin test ������");
	string initials = ChineseConvertPinYinInitials("ƴ�� ��ð� pinyin test ������");
	cout << allspell << endl;
	cout << initials << endl;
}


void TestHighlight()
{
	//ʹ��GBK���뺺���������ֽ�
	{
		//ColourPrintf("��Ҫ��\n");
		//1.����key�����������key
		string key = "����������";
		string str = "����������666666666";
		size_t pos = str.find(key);
		string prefix, suffix;
		prefix = str.substr(0, pos);
		suffix = str.substr(pos + key.size(), string::npos);
		cout << prefix;
		ColourPrintf(key.c_str());
		cout << suffix << endl;
	}

	{
		//2.key��ƴ����������Ӧ�ĺ���

		//�����Ϊ��������룬���㷨�����
		/*string key = "wolail";
		string str = "1����������666666666";*/
		//���㷨��2���ַ�ȥȡ������ǰ�������һ���ַ������֣�������
		//�������޸�:
		//�����ASCII�ַ�����һλ��������Ǳ�ʾ�Ǳ���ĺ��֣���ô��ȡ����

		string key = "wolail";
		string str = "s1334da����������666666666";
		string prefix, suffix;
		string str_py = ChineseConvertPinYinAllSpell(str);
		string key_py = ChineseConvertPinYinAllSpell(key);
		size_t pos = str_py.find(key_py);
		if (pos == string::npos)
		{
			cout << "ƴ����ƥ��" << endl;
		}
		else
		{
			size_t key_strat = pos;
			size_t key_end = pos + key_py.size();

			size_t str_i = 0, py_i = 0;
			while (py_i < key_strat)
			{
				if (str[str_i] >= 0 && str[str_i] <= 127)
				{
					++str_i;
					++py_i;
				}
				else
				{
					char chinese[3] = { '\0' };
					chinese[0] = str[str_i];
					chinese[1] = str[str_i + 1];
					str_i += 2;

					string py_str = ChineseConvertPinYinAllSpell(chinese);
					py_i += py_str.size();
				}

			}
			prefix = str.substr(0, str_i);

			size_t str_j = str_i, py_j = py_i;

			while (py_j < key_end)
			{
				if (str[str_j] >= 0 && str[str_j] <= 127)
				{
					++str_j;
					++py_j;
				}
				else
				{
					char chinese[3] = { '\0' };
					chinese[0] = str[str_j];
					chinese[1] = str[str_j + 1];
					str_j += 2;

					string py_str = ChineseConvertPinYinAllSpell(chinese);
					py_j += py_str.size();
				}

			}
			key = str.substr(str_i, str_j - str_i);
			suffix = str.substr(str_j, string::npos);

			cout << prefix;
			ColourPrintf(key.c_str());
			cout << suffix << endl;
		}
	}

	{
		//3.key��ƴ������ĸ��������Ӧ�ĺ���
		string key = "llzl";
		string str = "����������666666666";
		string prefix, suffix;

	}
}

int main()
{
	//TestDirectoryList();
	//TestSqlite();
	//TestScanManager();
	TestSearch();
	//TestPinyin();
	//TestHighlight();


	return 0;

}