
#include "ISystemLayInterface.h"

#ifdef WINCE

#elif _WIN32
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#elif _LINUX
#include <sys/types.h>
#include <dirent.h>
#include<sys/stat.h>
#endif


/******************************************************************************
����˵����Ŀ¼ö��
******************************************************************************/

//���캯��
CDLPDir::CDLPDir()
{
	m_DLPDir=NULL;

#ifdef WINCE
	ZeroMemory(m_wczDirPath,MAX_PATH);
#endif

}

//��������
CDLPDir::~CDLPDir()
{

}


/********************************************************************************************
  Function		: OpenDir    
  DateTime		: 	
  Description	: ��Ŀ¼
  Input			: const char *czDirPath��Ŀ¼·��
  Output		: NULL
  Return		: �ɹ�����TRUE��ʧ�ܷ���FALSE
  Note			:				// ��ע
********************************************************************************************/
BOOL CDLPDir::OpenDir(const char *czDirPath)
{
	BOOL bOpenDirRet=TRUE;
#ifdef WINCE 

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

#ifdef UNICODE

	int     nResult;

	nResult = MultiByteToWideChar(
		CP_ACP,    
		MB_PRECOMPOSED,
		czDirPath,
		strlen(czDirPath)+1,
		m_wczDirPath,
		ARRAYSIZE(m_wczDirPath));
	if(0 == nResult)
	{
		return false;
	}
#else
	hr = StringCchCopy(m_wczDirPath, ARRAYSIZE(wszDirectory), czDirPath);
	if(FAILED(hr))
	{
		return false;
	}
#endif

	if(GetFileAttributes(m_wczDirPath)==FILE_ATTRIBUTE_DIRECTORY)
	{
		bOpenDirRet=TRUE;
	}
	else
		bOpenDirRet=FALSE;
	
#elif _WIN32

	//�ı䵱ǰĿ¼
	if(_chdir(czDirPath))
	{
		bOpenDirRet=FALSE;
	};

#elif _LINUX

	//��ָ��Ŀ¼
	m_DLPDir =opendir(czDirPath); 
	if(m_DLPDir==NULL)
	{
		bOpenDirRet=FALSE;
	}

#endif

	return bOpenDirRet;
}

/********************************************************************************************
  Function		: CloseDir    
  DateTime		: 	
  Description	: �ر�Ŀ¼
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
void CDLPDir::CloseDir()
{
#ifdef _WIN32


#elif _LINUX

	if (m_DLPDir!=NULL)
	{
		closedir((DIR *)m_DLPDir);
	}

#endif

}

//��ȡĿ¼�б�
/********************************************************************************************
  Function		: ReadDir    
  DateTime		: 
  Description	: ��ȡĿ¼
  Input			: std::vector<StruDLPFileInfo> &vectFileList��Ŀ¼��Ϣ�б�
  Output		: NULL
  Return		: NULL
  Note			:				// ��ע
********************************************************************************************/
void CDLPDir::ReadDir(std::vector<StruDLPFileInfo> &vectFileList)
{

	StruDLPFileInfo DirInfo;
	vectFileList.clear();
#ifndef WINCE
	struct stat	  FileBuf;
#endif
#ifdef WINCE

	HANDLE hDirectory=NULL;
	WIN32_FIND_DATAW wfd={0};
	hDirectory=FindFirstFile(m_wczDirPath,&wfd);

	if (hDirectory==INVALID_HANDLE_VALUE)
	{
		return;
	}
	do 
	{

		char tmp[MAX_PATH];
		WideCharToMultiByte(CP_ACP,0,wfd.cFileName,256,tmp,256,NULL,NULL);
		DirInfo.strFileName=tmp;

		if((wfd.dwFileAttributes) & FILE_ATTRIBUTE_DIRECTORY)
		{
			DirInfo.iFileType=DLP_FILE_DIRECTORY;
		}
		//�ж��ļ��Ƿ�Ϊ��ͨ�ļ�
		else if ((wfd.dwFileAttributes) & FILE_ATTRIBUTE_NORMAL)
		{
			DirInfo.iFileType=DLP_FILE_COMMON;
		}
		//�ж��ļ��Ƿ�Ϊ�ܵ��ļ�
// 		else if ((wfd.dwFileAttributes) & _S_IFIFO)
// 		{
// 			DirInfo.iFileType=DLP_FILE_FIFO;
// 		}
		//�ļ�Ϊ��������
		else 
		{
			DirInfo.iFileType=DLP_FILE_OTHER;
		}	

		vectFileList.push_back(DirInfo);

	} while (FindNextFile(hDirectory,&wfd));

#elif _WIN32
	
	long handle; 
	struct _finddata_t filestruct;

	//�õ���ǰĿ¼�ĵ�һ����Ŀ¼
	handle=_findfirst("*",&filestruct);
	
	//�жϵ�һ��Ŀ¼��ȡ�Ƿ�ɹ���ʧ���򷵻�
	if(handle==-1)
		{
			return;
		}
	
	//��ȡ��һ��Ŀ¼�����ԣ��浽FileBuf��
	if (stat(filestruct.name,&FileBuf)==0)
	{
		DirInfo.strFileName=filestruct.name;
		
		//�ж��ļ��Ƿ�ΪĿ¼�ļ�
		if((FileBuf.st_mode) & _S_IFDIR)
		{
			DirInfo.iFileType=DLP_FILE_DIRECTORY;
		}
		//�ж��ļ��Ƿ�Ϊ��ͨ�ļ�
		else if ((FileBuf.st_mode) & _S_IFREG)
		{
			DirInfo.iFileType=DLP_FILE_COMMON;
		}
		//�ж��ļ��Ƿ�Ϊ�ܵ��ļ�
		else if ((FileBuf.st_mode) & _S_IFIFO)
		{
			DirInfo.iFileType=DLP_FILE_FIFO;
		}
		//�ļ�Ϊ��������
		else 
		{
			DirInfo.iFileType=DLP_FILE_OTHER;
		}	
		//���ļ���Ϣ�����б���
		vectFileList.push_back(DirInfo);

	}

	//ѭ����ȡĿ¼���ļ�
	 while(!(_findnext(handle,&filestruct))) 
	 	{
			//��ȡ�ļ�����
			if (stat(filestruct.name,&FileBuf)==0)
			{
				DirInfo.strFileName=filestruct.name;
				
				//�ж��ļ��Ƿ�ΪĿ¼�ļ�
				if((FileBuf.st_mode) & _S_IFDIR)
				{
					DirInfo.iFileType=DLP_FILE_DIRECTORY;
				}
				//�ж��ļ��Ƿ�Ϊ��ͨ�ļ�
				else if ((FileBuf.st_mode) & _S_IFREG)
				{
					DirInfo.iFileType=DLP_FILE_COMMON;
				}
				//�ж��ļ��Ƿ�Ϊ�ܵ��ļ�
				else if ((FileBuf.st_mode) & _S_IFIFO)
				{
					DirInfo.iFileType=DLP_FILE_FIFO;
				}
				//�ļ�Ϊ��������
				else 
				{
					DirInfo.iFileType=DLP_FILE_OTHER;
				}	
				//���ļ���Ϣ�����б���
				vectFileList.push_back(DirInfo);
			}
		 }

	 //�ر�Ŀ¼
    	_findclose(handle); 
		

#elif _LINUX

	struct dirent *dirStru;

	//�ж��ļ�����Ƿ�Ϊ��
	if (m_DLPDir!=NULL)
	{
		//ѭ����ȡ�ļ��б�
		while((dirStru=readdir((DIR *)m_DLPDir))!=NULL)
		{
			//��ȡ�ļ�ʧ�ܣ�����
			if(errno != 0)
			{
				return;
			}
			//��ȡ�ļ�����
			if (stat(dirStru->d_name,&FileBuf)==0)
			{
				DirInfo.strFileName=dirStru->d_name;

				//�ж��ļ��Ƿ���Ŀ¼�ļ�
				if(S_ISDIR(FileBuf.st_mode))
				{
					DirInfo.iFileType=DLP_FILE_DIRECTORY;
				}
				//�ж��ļ��Ƿ�����ͨ�ļ�
				else if (S_ISREG(FileBuf.st_mode))
				{
					DirInfo.iFileType=DLP_FILE_COMMON;
				}
				//�ж��ļ��Ƿ�Ϊ�ܵ��ļ�
				else if (S_ISFIFO(FileBuf.st_mode))
				{
					DirInfo.iFileType=DLP_FILE_FIFO;
				}
				//�ļ�Ϊ�����ļ�
				else 
				{
					DirInfo.iFileType=DLP_FILE_OTHER;
				}
				//���ļ���Ϣ�����б�
				vectFileList.push_back(DirInfo);
			}
			
		}
		
	}
	
#endif

}




