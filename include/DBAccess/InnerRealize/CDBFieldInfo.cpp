#include "CDBFieldInfo.h"

using	namespace	DBAccessModule;

CDBFieldInfo::CDBFieldInfo(void)
{
	m_eFieldType = FT_UnKnown;
	m_iValue = coniZero;
	m_strFieldName = constrDefaultString;
	m_iBinaryValueLen = coniZero;
	m_pValue	=	NULL;
	m_iColumnIndex = coniZero;
	memset(m_szRefValue,0x0,FIELD_DATA_LEN);
	m_iRefValue = coniZero;
	m_icpValue = coniZero;
}

CDBFieldInfo::~CDBFieldInfo(void)
{
	m_eFieldType = FT_UnKnown;
	m_iValue = coniZero;
	m_strFieldName = constrDefaultString;
	m_iBinaryValueLen = coniZero;
	m_iColumnIndex = coniZero;

	// �ͷ��ڴ�
	if (m_pValue != NULL)
	{
		delete[] m_pValue;
		m_pValue = NULL;
	}
	// ��Ӧ m_strValue��ֵ�Ļ�ȡ
	memset(m_szRefValue,0x0,FIELD_DATA_LEN);

	// ��Ӧ��m_iValue��ֵ�Ļ�ȡ
	m_iRefValue = coniZero;
	m_icpValue = coniZero;
}


/**************************************************************************************************
  Function		: Clear()    
  DateTime		: 	
  Description	: ����ֶζ���
  Input			: NULL
  Output		: NULL
  Return		: NULL
  Note			: ����ʱ�򣬽�����ʾ�����������
**************************************************************************************************/
void	CDBFieldInfo::Clear()
{
	m_eFieldType = FT_UnKnown;
	m_iValue = coniZero;
	m_strFieldName = constrDefaultString;
	m_iBinaryValueLen = coniZero;
	m_iColumnIndex = coniZero;

	// �ͷ��ڴ�
	if (m_pValue != NULL)
	{
		delete[] m_pValue;
		m_pValue = NULL;
	}

	// ��Ӧ m_strValue��ֵ�Ļ�ȡ
	memset(m_szRefValue,0x0,FIELD_DATA_LEN);

	// ��Ӧ��m_iValue��ֵ�Ļ�ȡ
	m_iRefValue = coniZero;
	m_icpValue = coniZero;
}


/**************************************************************************************************
  Function		: SetBinaryValue(const void* pSourceValue,const INT	iSourceLen)     
  DateTime		: 
  Description	: ���ö����ƶ����С
  Input			: pSourceValue������������    iSourceLen������(��������\0���ַ�����)
  Output		: NULL
  Return		: TRUE:�ɹ�   FALSE��ʧ��
  Note			: ���Ϊ10M������
**************************************************************************************************/
BOOL	CDBFieldInfo::SetBinaryValue(const void* pSourceValue,const INT	iSourceLen)
{
	if (iSourceLen > coniMaxBinaryValueSize)
	{
		return FALSE;
	}

	try
	{
		m_pValue = new char[iSourceLen+1];
		memset(m_pValue,0x0,iSourceLen+1);
		// ��������
		memcpy((void*)m_pValue,pSourceValue,iSourceLen);

		return	TRUE;
	}
	catch(...)
	{
		// ��ֹ�ڴ�������
		return	FALSE;
	}

	return	TRUE;

}

/**************************************************************************************************
  Function		: GetBinaryValue(void* pSourceValue,INT iGetLen)    
  DateTime		: 	
  Description	: ��ȡ����������
  Input			: pSourceValue: Ҫ��ȡ�����ݣ� iGetLen ��Ҫ��ȡ�����ݳ��ȣ�-1:��ʾȫ����ȡ
  Output		: pSourceValue: Ҫ��ȡ�����ݣ� iGetLen ���Ѿ���ȡ�ĳ��ȣ���������\0��
  Return		: TRUE:�ɹ��� FALSE:ʧ��
  Note			: ��ȡ���������ݴ���,pSourceValue�������������ڴ棬Ҫ�Ǵ��ˣ�������
**************************************************************************************************/
BOOL	CDBFieldInfo::GetBinaryValue(void* pSourceValue,INT& iGetLen)
{
	try
	{
		// ȫ������
		if (iGetLen == -1)
		{
			// ����
			memcpy(pSourceValue,m_pValue,m_iBinaryValueLen);

			iGetLen = m_iBinaryValueLen;
		}
		else if(iGetLen >= m_iBinaryValueLen)
		{
			// Ҫ��ı������ȡ��ȫ��
			// ����
			memcpy(pSourceValue,m_pValue,m_iBinaryValueLen);

			iGetLen = m_iBinaryValueLen;
		}
		else
		{
			// ֻ��������
			// ����
			memcpy(pSourceValue,m_pValue,iGetLen);
		}

	}
	catch(...)
	{
		return	FALSE;
	}

	return	TRUE;

}


