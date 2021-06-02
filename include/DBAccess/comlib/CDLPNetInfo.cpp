#include "ISystemLayInterface.h"
/**************************************************************************************************

  Description 	: ������Ϣ��ȡ��
**************************************************************************************************/
#ifdef _WIN32
#include "IPHlpApi.h"
#pragma   comment(lib,"iphlpapi.lib")
#elif _LINUX
#include <ctype.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif


CDLPNetInfo::CDLPNetInfo()
{
	m_iNetCounts = 0;
	m_uiLastTime = 0;
	memset(m_stLastNetStat, 0, sizeof(m_stLastNetStat));
	m_pDataBuf = NULL;
	m_iBufLen = 0;
}
CDLPNetInfo::~CDLPNetInfo(void)
{
	if (m_pDataBuf == NULL)
	{
		free(m_pDataBuf);
		m_pDataBuf = NULL;
		m_iBufLen = 0;
	}
}
/**************************************************************************************************
Function	:  ��ȡ��������   
DateTime	: 	
Description	:// �������ܡ����ܵȵ�����
Input		:// �������˵��������ÿ����������
Output	:// �����������˵����
Return	:// ��������ֵ��˵��
Note		:// ��ע
**************************************************************************************************/
INT32	CDLPNetInfo::DLPGetNetCount()
{
	INT32	iNetCount = 0;
#ifdef _WIN32
	PIP_ADAPTER_INFO pIpInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	PIP_ADAPTER_INFO temp = NULL;
	ULONG tbufLen = 0;
	
	if(pIpInfo == NULL)
	{
		return m_iNetCounts;
	}
	if(GetAdaptersInfo(pIpInfo, &tbufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pIpInfo);
		pIpInfo = (PIP_ADAPTER_INFO)malloc(tbufLen);
	}
	if(GetAdaptersInfo(pIpInfo, &tbufLen) == NO_ERROR)
	{
		temp = pIpInfo;
		while(temp)
		{
			temp = temp->Next;
			iNetCount++;
		}

	}
	if(pIpInfo != NULL)
	{
		free(pIpInfo);
		pIpInfo = NULL;
	}
	

#elif _LINUX

	FILE* f = fopen("/proc/net/dev", "r");
	if (!f)
	{
		//���ļ�ʧ��
		return m_iNetCounts;
	}
	char szLine[512];
	fgets(szLine, sizeof(szLine), f);   
	fgets(szLine, sizeof(szLine), f);
	while(fgets(szLine, sizeof(szLine), f))
	{
		char szName[128] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)continue;
		if (szName[nLen - 1] == ':') szName[nLen - 1] = 0;
		if (strcmp(szName, "lo") == 0)continue;
		iNetCount++;
	}
	fclose(f);
	f = NULL;
	
#endif
	m_iNetCounts = iNetCount;
	return m_iNetCounts;
}
/**************************************************************************************************
  Function	:     
  DateTime	: 	
  Description	:// �������ܡ����ܵȵ�����
  Input		:// �������˵��������ÿ����������
  Output	:// �����������˵����
  Return	:// ��������ֵ��˵��
  Note		:// ��ע
**************************************************************************************************/

INT32	CDLPNetInfo::DLPGetNetUsage(StruDLPNETSTAT * pstNetStat,INT32 iBuffLen)
{
	StruDLPNETSTATTable* pstNetTable = NULL;
	StruDLPNETSTATTable* pstLastNetTable = NULL;
	//��Ϊ��ȡ������������ȡ������������������Ļ�������С
	if (m_iNetCounts == 0)
	{
		m_iNetCounts = DLPGetNetCount();
	}
	INT32 iLen = m_iNetCounts*sizeof(StruDLPNETSTATTable)+ sizeof(INT32);
	//�����������ڿգ�������������0���жϻ�������С
	if (pstNetStat == NULL || m_iNetCounts <=0 ||iLen > iBuffLen)
	{
		pstNetStat->iNetNum = m_iNetCounts;
		return	-1;
	}
	pstNetStat->iNetNum = m_iNetCounts;

	//��ȡ��ǰʱ��(����)
	UINT64   uiCurrentTime = DoGetTickCount();
	UINT64	 uiTime = uiCurrentTime - m_uiLastTime;
	m_uiLastTime = uiCurrentTime;
	
	if (uiTime <= 0)
	{
		//�����ǰʱ��С���ϴ�ʱ�䣬ʱ�����������������
	}
#ifdef _WIN32
	PMIB_IFROW pIfRow = NULL;
	PMIB_IFTABLE pIfTable = NULL;
	DWORD	dwSize = 0;
	if (m_pDataBuf == NULL)
	{
		m_iBufLen = m_iNetCounts*sizeof(MIB_IFROW) + sizeof(DWORD);
		m_pDataBuf = (char*)malloc(m_iBufLen);
		if (m_pDataBuf == NULL) 
		{
			return -1;
		}
	}	
	pIfTable = (MIB_IFTABLE *)m_pDataBuf;
	dwSize = m_iBufLen;

	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) 
	{
		free(m_pDataBuf);
		m_iBufLen = dwSize;
		m_pDataBuf = (char*)malloc(m_iBufLen);
		pIfTable = (MIB_IFTABLE *)m_pDataBuf;
		if (pIfTable == NULL) 
		{
			return -1;
		}
	}

	if (GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR) 
	{
		/*if (m_iNetCounts != (INT32) pIfTable->dwNumEntries)
		{
			return	-1;
		}*/
		pstNetStat->iNetNum = m_iNetCounts;
		
		StruDLPNETSTATTable* pstNetTable = NULL;

		for (INT32 i = 0; i < (INT32) pIfTable->dwNumEntries; i++) 
		{
			if (pIfTable->table[i].dwType != MIB_IF_TYPE_ETHERNET)
			{
				continue;
			}
			pstNetTable = (StruDLPNETSTATTable*)&pstNetStat->stNetStatTable[i];
			pIfRow = (MIB_IFROW *) &pIfTable->table[i];
			strcpy(pstNetTable->szName,(char*)pIfRow->bDescr);
			pstNetTable->iRecv = pIfRow->dwInOctets;
			pstNetTable->iTrans = pIfRow->dwOutOctets;

			pstLastNetTable = GetLastNetStat(pstNetTable->szName);	
			if (pstNetTable == NULL)
			{
				return	-1;
			}
			if (pstNetTable->iRecv < pstLastNetTable->iRecv) 
			{
				//���
				pstLastNetTable->iRecv = 0;
			} 

			if (pstNetTable->iTrans < pstLastNetTable->iTrans) 
			{
				//���
				pstLastNetTable->iTrans = 0;
			}

			//����������������
			pstNetTable->dRecvSpeed = (pstNetTable->iRecv - pstLastNetTable->iRecv)/uiTime/1000;
			pstNetTable->dTransSpeed = (pstNetTable->iTrans - pstLastNetTable->iTrans)/uiTime/1000;

			//���汾�λ�ȡ������
			pstLastNetTable->iRecv = pstNetTable->iRecv;
			pstLastNetTable->iTrans = pstNetTable->iTrans;
			pstLastNetTable->dRecvSpeed = pstNetTable->dRecvSpeed;
			pstLastNetTable->dTransSpeed = pstNetTable->dTransSpeed;
		}
	

	}
	
#elif _LINUX

	FILE *net_dev_fp;

	INT32 i=0;
	char buf[256]={0};
	
	char szNetName[DLP_MAX_NET_NAME_LEN] = {0};
	char stri[10]={0};

	if (!(net_dev_fp = fopen("/proc/net/dev", "r"))) 
	{
		return -1;
	}

	fgets(buf, 255, net_dev_fp);  
	fgets(buf, 255, net_dev_fp);  

	

	pstNetStat->iNetNum = m_iNetCounts;
	
	for (i = 0; i < m_iNetCounts; i++) 
	{
		sprintf(stri,"%d",i);   
		strcpy(szNetName,"eth");
		strcat(szNetName,stri);
		char *s=NULL, *p=NULL;
		//unsigned char *s=NULL, *p=NULL;
		//UINT64 r=0, t=0;
		//UINT64 last_recv=0, last_trans=0;

		pstNetTable = &(pstNetStat->stNetStatTable[i]);
		if (fgets(buf, 255, net_dev_fp) == NULL) 
		{
			break;
		}
		p = buf;
		while (isspace((int) *p))
		{
			p++;
		}
		s = p;

		while (*p && *p != ':') 
		{
			p++;
		}
		if (*p == '\0') 
		{
			continue;
		}
		*p = '\0';

		p++;

		if(strcmp(s, szNetName) != 0)
			continue;

		strcpy(pstNetTable->szName,szNetName);
		pstLastNetTable = GetLastNetStat(s);

		sscanf(p, "%lld  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %lld",
			&pstNetTable->iRecv, &pstNetTable->iTrans);

		pstLastNetTable = GetLastNetStat(pstNetTable->szName);	
		if (pstNetTable->iRecv < pstLastNetTable->iRecv) 
		{
			//���
			pstLastNetTable->iRecv = 0;
		} 

		if (pstNetTable->iTrans < pstLastNetTable->iTrans) 
		{
			//���
			pstLastNetTable->iTrans = 0;
		}

		//����������������
		pstNetTable->dRecvSpeed = (pstNetTable->iRecv - pstLastNetTable->iRecv)/uiTime/1000;
		pstNetTable->dTransSpeed = (pstNetTable->iTrans - pstLastNetTable->iTrans)/uiTime/1000;

		//���汾�λ�ȡ������
		pstLastNetTable->iRecv = pstNetTable->iRecv;
		pstLastNetTable->iTrans = pstNetTable->iTrans;
		pstLastNetTable->dRecvSpeed = pstNetTable->dRecvSpeed;
		pstLastNetTable->dTransSpeed = pstNetTable->dTransSpeed;

	}

	fclose(net_dev_fp);
#endif
	return	0;
}

/**************************************************************************************************
  Function	:     
  DateTime	: 	
  Description	:// �������ܡ����ܵȵ�����
  Input		:// �������˵��������ÿ����������
  Output	:// �����������˵����
  Return	:// ��������ֵ��˵��
  Note		:// ��ע
**************************************************************************************************/

StruDLPNETSTATTable* CDLPNetInfo::GetLastNetStat(const char* szName)
{
	unsigned int i=0;
	char name[256] = {0};
	if (!szName) 
	{
		return 0;
	}

	/* �ҵ���Ӧ��״̬ */
	for (i = 0; i < m_iNetCounts; i++) 
	{
		if (m_stLastNetStat[i].szName && strcmp(m_stLastNetStat[i].szName, szName) == 0) 
		{
			return &m_stLastNetStat[i];
		}
	}

	/* û���ҵ���Ӧ��״̬��������� */
	if (i == m_iNetCounts) 
	{
		for (i = 0; i < m_iNetCounts; i++) 
		{
			if (strcmp(m_stLastNetStat[i].szName, name) == 0) 
			{
				strcpy(m_stLastNetStat[i].szName,szName);
				return &m_stLastNetStat[i];
			}
		}
	}

	return 0;
}
