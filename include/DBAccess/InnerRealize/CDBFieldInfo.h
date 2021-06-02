#ifndef CDBFIELDINFO_DEF_H
#define CDBFIELDINFO_DEF_H

#include "DbAccessDataDef.h"

namespace	DBAccessModule
{

	// �ֶ����ͣ�ʹ��ODBC API�������ݿ������ʱ��AddNew PutCollect Updateʱ���ã����洢�ֶε�����
	// ����Ϊ���ַ�����������������
	enum EnumFieldType
	{
		FT_UnKnown = -1,			// δ֪����
		FT_Integer = 0,			// 0������
		FT_String,				// 1���ַ���(Float ,double,�����ݵ�)
		FT_Binary,				// 2�������ƴ��������
		FT_DateTime				// 3��ʱ������
	};

	// �����ƴ��������ֵ��СΪ 100M
	// char ��ʾһ���ֽڣ�1024 * 1 = 1KB
	// 1024 * 1(KB) = 1 MB
	// 1024 * 1024 = 1MB   1024 * 1024* 10*10 = 100M
	const		INT			coniMaxBinaryValueSize = 10 * 10 * 1024 *1024 ;				


	/**************************************************************************************************
	Description 	: ���ڴ洢ODBC API�������ݿ��ֶβ�����ʱ��ģ��ֶ���Ϣ�Ĵ洢
	LINUXʱ���ã�WINDOWS��ADBC���߱���Ѿ�������				  
	**************************************************************************************************/
	class CDBFieldInfo
	{
	public:
		CDBFieldInfo(void);
		virtual ~CDBFieldInfo(void);
		// ��ʾ����ն���
		void	Clear();

	protected:
		// �ֶ�����(Ҳ����������)
		string			m_strFieldName;
		// �ֶ�����
		EnumFieldType	m_eFieldType;
		// �е�����
		INT				m_iColumnIndex;

	public:

		// ���á���ȡ�ֶ�����
		inline		void	SetFieldName(const char* szFiledName)
		{
			m_strFieldName	=	string(szFiledName);
		}
		inline		string	GetFieldName()
		{
			return	m_strFieldName;
		}

		// ���á���ȡ�ֶ�����
		inline		void	SetFieldType(const EnumFieldType eFtType)
		{
			m_eFieldType = eFtType;
		}
		inline		EnumFieldType	GetFieldType()
		{
			return	m_eFieldType;
		}

		// ���ã���ȡ�е�����
		inline		void	SetColumnIndex(const INT iColumnIndex)
		{
			m_iColumnIndex = iColumnIndex;
		}
		inline		INT		GetColumnIndex()
		{
			return	m_iColumnIndex;
		}

	public:
		// ����SQLBindParameter��ʱ��ģ���������,SQLBindParameter������Ӧ�����һ������
		INT				m_icpValue;

	public:
		// ����ֵ��ʱ���õ�
		// ��Ӧ m_strValue��ֵ�Ļ�ȡ
		char			m_szRefValue[FIELD_DATA_LEN];
		// ��Ӧ��m_iValue��ֵ�Ļ�ȡ
		INT				m_iRefValue;

		//protected:
	public:
		// �ֶζ�Ӧ��ֵ(ѡ��һ����)

		// ����
		INT				m_iValue;

		// �ַ��ͣ�string , float ,double ,���ڵȣ�
		string			m_strValue;

		// �����ƶ���ֵ
		INT				m_iBinaryValueLen;
		char*			m_pValue;

		// oracle ʱ���ֶ�
		TIMESTAMP_STRUCT	m_stDateTime;

	public:
		// ��ȡ�����ö�Ӧ�����ε�ֵ
		inline		void	SetIntegerValue(const INT	iValue)
		{
			m_iValue = iValue;
		}
		inline		INT		GetIntegerValue()
		{
			return	m_iValue;
		}
		
		// ����ʱ���ֶε�ֵ
		inline		void	SetDateTimeValue(const INT iYear,const INT iMonth,const INT iDay,const INT iHour,const INT iMinute,const INT iSecond)
		{
			m_stDateTime.year = iYear;
			m_stDateTime.month = iMonth;
			m_stDateTime.day = iDay;
			m_stDateTime.hour = iHour;
			m_stDateTime.minute = iMinute;
			m_stDateTime.second = iSecond;
		}

		// �ַ��ͣ�string , float ,double ,���ڵȣ�
		// ��ȡ�����ö�Ӧ���ַ������͵�ֵ
		inline		void	SetStringValue(const char* szValue)
		{
			m_strValue = string(szValue);
		}
		inline		string	GetStringValue()
		{
			return	m_strValue;
		}

		// �����ƶ���ֵ
		// ��ȡ�����ö����ƶ����С��
		inline		void	SetBinaryValueLen(const		INT	iBinaryValueLen)
		{
			m_iBinaryValueLen = iBinaryValueLen;
		}
		inline		INT	GetBinaryValueLen()
		{
			return	m_iBinaryValueLen;
		}

		// ��ȡ�����ƶ������ݺ����ö����ƶ�������
		BOOL	SetBinaryValue(const void* pSourceValue,const INT	iSourceLen);
		// ��ȡ����
		BOOL	GetBinaryValue(void* pSourceValue,INT& iGetLen);
	};

	// �洢�ֶε��б�����ADDNEW �� �޸�ֵ��ʱ�����ڴ����
	typedef	vector<CDBFieldInfo*>				DBFieldInfoVector;
	typedef	vector<CDBFieldInfo*>::iterator		DBFieldInfoVectorIterator;



	// ����һ�����ַ�����д���࣬���transformʹ�ã�linux�汾����
	class ToUpper{
	public: 
		char operator()(char val){   
			return toupper( val ); 
		} 
	}; 

}

#endif // CDBFIELDINFO_DEF_H

