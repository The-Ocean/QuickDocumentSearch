#include"ScanManager.h"
#include"Common.h"

void ScanMannager::Scan(const string& path)
{
	//ɨ����бȶԣ��ȶ��ļ�ϵͳ�����ݿ�
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
			//�����У����ݿ�û�С�>��������
			//_datamgr.InsertDoc(path, *localit);
			DataManager::GetInstance()->InsertDoc(path, *localit);
			++localit;
		}
		else if (*localit > *dbit)
		{
			//����û�У�������->ɾ������
			DataManager::GetInstance()->DeleteDoc(path, *dbit);
			++dbit;
		}
		else
		{
			//���������
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end())
	{
		//��������
		DataManager::GetInstance()->InsertDoc(path, *localit);
		++localit;
	}
	while (dbit != dbset.end())
	{
		//ɾ������
		DataManager::GetInstance()->DeleteDoc(path, *dbit);
		++dbit;
	}
	//�ݹ�ɨ��Ա���Ŀ¼����
	for (const auto& subdirs : localdirs)
	{
		string subpath = path;
		subpath += '/';
		subpath += subdirs;
		Scan(subpath);
	}
}