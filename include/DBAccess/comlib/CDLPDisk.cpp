
#include "ISystemLayInterface.h"


/******************************************************************************
����˵��������ö��
******************************************************************************/

/********************************************************************************************
  Function		: DLPGetSysDisk    
  DateTime		: 	
  Description	: ö�ٴ���
  Input			: std::vector<std::string> &vectDiskList�������б�
  Output		: NULL
  Return		: ���̸���
  Note			:				// ��ע
********************************************************************************************/
INT DLPGetSysDisk(std::vector<std::string> &vectDiskList)
{

	INT iDiskNum=0;
	std::string strDiskName;
	vectDiskList.clear();
#ifdef WINCE
	return iDiskNum;
#elif _WIN32

	DWORD dwDriveStrlen=0;
	//ͨ��GetLogicalDriveStrinDLP()������ȡ�����������ַ�����Ϣ���ȡ�
	dwDriveStrlen=GetLogicalDriveStrings(0,NULL);

	//�û�ȡ�ĳ����ڶ�������һ���ַ�������
	// char *szDriveName=new char[dwDriveStrlen];
	//������ĳ��ȸ�Ϊ��2,����GetLogicalDriveStrinDLP��delete[] szDriveName����
	char *szDriveName=new char[dwDriveStrlen * 2];
	memset(szDriveName, 0, dwDriveStrlen * 2);
	char *pDriveName=NULL;

	//ͨ��GetLogicalDriveStrinDLP���ַ�����Ϣ���Ƶ�����������,���б�������������������Ϣ��
	if(GetLogicalDriveStrings(dwDriveStrlen * 2,(LPTSTR)szDriveName))
	{
		pDriveName=szDriveName;
		while(*pDriveName!=NULL)
		{
			//INT DType;
			////�жϴ�������
			//DType = GetDriveType((LPCWSTR)pDriveName);
			//if(DType == DRIVE_FIXED)
			//{
			//	iDiskNum++;
			//	//����������Ϣ�����б�
			//	strDiskName=pDriveName;
			//	pDriveName+=8;
			//	vectDiskList.push_back(strDiskName);
			//}	
			//����������Ϣ�����б�
			iDiskNum++;
			strDiskName=pDriveName;
			pDriveName+=8;
			vectDiskList.push_back(strDiskName);
		}
	}
	else 
	{
		iDiskNum = 0;
	}

	//ɾ���������ַ�������
	if(NULL != szDriveName)
	{
		delete[] szDriveName;
		szDriveName = NULL;
	}

#elif _LINUX

	FILE *fp;
	INT i=0;
	char czDiskName[20];
	char czDiskInfoLine[255]; 
	char *p;
	//�򿪶�ȡ������Ϣ
	if((fp=fopen("/proc/partitions","r"))==NULL) 
	{
		exit(0);
	}
	//��ȡ�ļ���ͷ����
	fgets(czDiskInfoLine,255,fp);
	fgets(czDiskInfoLine,255,fp);
	//�ӵ����п�ʼѭ����ȡ�ļ�
	while (fgets(czDiskInfoLine,255,fp)!=NULL)
	{
		iDiskNum++;	
		//����strtok�ָ��ַ���czDiskInfoLine
		p=strtok(czDiskInfoLine," ");
		i=0;
		do   
		{  
			i++;
			p = strtok(NULL, " "); //NULL��Ϊ���淵�ص�ָ��   
			if (i==3)
			{
				//��ȡ�õĴ������Ƹ�ʽ��
				snprintf(czDiskName, 20, "%s", p);
				//��������Ϣ�����б�
				vectDiskList.push_back(czDiskName);
			}
		}while(p); 
	}
	//�ر��ļ�
	fclose(fp);

#endif

	//���ش��̸���
	return iDiskNum;

}



