#pragma once

#include"Common.h"
#include"DataManager.h"

//ScanMannager��Ƴ�Ϊһ������ģʽ����������ֻ��һ��ɨ��ģ��
class ScanMannager
{
public:
	void Scan(const string& path);
	void StartScan()
	{
		while (1)
		{
			Scan("D:\\��ѧ\\dir");
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	static ScanMannager* CreateIntance()
	{
		static ScanMannager scanmgr;
		static std::thread thd(&StartScan, &scanmgr);
		thd.detach();

		return &scanmgr;
	}
private:
	ScanMannager()
	{
		//_datamgr.Init();
	}

	ScanMannager(const ScanMannager&);

	//DataManager _datamgr;
	//vector<string> netrys;
};
