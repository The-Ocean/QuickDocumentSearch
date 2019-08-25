#pragma once

#include"Common.h"
#include"DataManager.h"

//ScanMannager设计成为一个单例模式，整个程序只有一个扫描模块
class ScanMannager
{
public:
	void Scan(const string& path);
	void StartScan()
	{
		while (1)
		{
			Scan("D:\\大学\\dir");
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
