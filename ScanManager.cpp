#include"ScanManager.h"
#include"Common.h"

void ScanMannager::Scan(const string& path)
{
	//扫描进行比对，比对文件系统和数据库
	vector<string> localdirs;
	vector<string> localfiles;
	DirectoryList(path, localdirs, localfiles);

	std::set<string> localset;
	localset.insert(localfiles.begin(), localfiles.end());
	localset.insert(localdirs.begin(), localdirs.end());

	std::set<string> dbset;
	//DataManager
	//_datamgr.GetDoc(path, dbbset);
	DataManager::GetInstance()->GetDoc(path, dbset);

	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end())
	{
		if (*localit < *dbit)
		{
			//本地有，数据库没有—>新增数据
			//_datamgr.InsertDoc(path, *localit);
			DataManager::GetInstance()->InsertDoc(path, *localit);
			++localit;
		}
		else if (*localit > *dbit)
		{
			//本地没有，数据有->删除数据
			DataManager::GetInstance()->DeleteDoc(path, *dbit);
			++dbit;
		}
		else
		{
			//不变的数据
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end())
	{
		//新增数据
		DataManager::GetInstance()->InsertDoc(path, *localit);
		++localit;
	}
	while (dbit != dbset.end())
	{
		//删除数据
		DataManager::GetInstance()->DeleteDoc(path, *dbit);
		++dbit;
	}
	//递归扫描对比子目录数据
	for (const auto& subdirs : localdirs)
	{
		string subpath = path;
		subpath += '/';
		subpath += subdirs;
		Scan(subpath);
	}
}