#include "DLPUtility.h"
#include <assert.h>
using namespace std;



/*
*********************************************************************
*
*@brief : 字符串操作功能
*
*********************************************************************
*/


#define MAX_STRING_LENGHT (100L<<20) //最大字符串最大


namespace DLP
{

	class CTokenizer
	{
	protected:
		INT m_iOffset;
		const std::string m_strTemp;
		std::string m_strToken;
		std::string m_strDelimiters;
	public:
#define DEFAULT_DELIMITERS " "

		CTokenizer(const std::string& str)
			: m_iOffset(0), m_strTemp(str)
			, m_strDelimiters(DEFAULT_DELIMITERS)
		{

		}

		CTokenizer(const std::string& str, const std::string& delimiters)
			: m_iOffset(0),
			m_strTemp(str),
			m_strDelimiters(delimiters)
		{

		}

		// 查找下一个标识
		bool NextToken(void)
		{
			return NextToken(m_strDelimiters);
		}

		bool NextToken(const std::string& delimiters)
		{
			// 查找下一个标识起始字符位置
			UINT i = m_strTemp.find_first_not_of(delimiters, m_iOffset);
			if (i == std::string::npos) {
				m_iOffset = m_strTemp.length();
				return false;
			}

			// 查找标识结束位置
			UINT j = m_strTemp.find_first_of(delimiters, i);
			if (j == std::string::npos) {
				m_strToken = m_strTemp.substr(i);
				m_iOffset = m_strTemp.length();
				return true;
			}

			m_strToken = m_strTemp.substr(i, j - i);
			m_iOffset = j;
			return true;
		}

		// 获取标识字符串
		const std::string GetToken(void) const
		{
			return m_strToken;
		}

		// 重置标识位置
		void Reset(void)
		{
			m_iOffset = 0;
		}


	};

} // end namespace DLP


void DLPStrUtil::Split( std::vector<CDLPString> &csResult, const CDLPString& str, const CDLPString& delimiters)
{
	DLP::CTokenizer csTemp(str, delimiters);
	csResult.clear();

	while( csTemp.NextToken( ) )
	{
		csResult.push_back( csTemp.GetToken() );
	}




}

CDLPString DLPStrUtil::TrimLeft(const CDLPString& str) 
{
	std::string t = str;
	t.erase(0, t.find_first_not_of(" \t\n\r"));
	return t;
};


CDLPString DLPStrUtil::TrimRight(const CDLPString& str) 
{
	std::string t = str;
	t.erase(t.find_last_not_of(" \t\n\r") + 1);
	return t;
};


CDLPString DLPStrUtil::Trim(const CDLPString& str) 
{
	std::string t = str;
	t.erase(0, t.find_first_not_of(" \t\n\r"));
	t.erase(t.find_last_not_of(" \t\n\r") + 1);
	return t;
};

// ----------------------------------------------------------------------
// 函数名: DLPStrUtil::ToLower
// 作者: 
// 描述: 转换为小写字符串
// 返回值: [string] 输出字符串
// 参数: 
//   [const string& str] 输入字符串
// ----------------------------------------------------------------------
CDLPString DLPStrUtil::ToLower(const CDLPString& str) 
{
	std::string t = str;
	std::transform(t.begin(), t.end(), t.begin(), ::tolower);
	return t;
};


CDLPString DLPStrUtil::ToUpper(const CDLPString& str) 
{
	string t = str;
	std::transform(t.begin(), t.end(), t.begin(), ::toupper);
	return t;
};



BOOL DLPStrUtil::VFormat(CDLPString &strOString,const char *czFormat, va_list ap )
{
	char sBuffer[256];
	va_list apStart;
	char *pBuffer;
	int n, size = 256; 

	/*const char *str;
	va_start(apStart, czFormat);
	str = va_arg(apStart, const char *);
	if (str != NULL)
	{
		fputs(str, (FILE *)c);
		str = va_arg(ap, const char *);
	}
	va_end(ap);*/

	strOString.clear();

	//apStart = ap;       
	pBuffer = sBuffer;
	while (pBuffer) 
	{           
		//ap = apStart;
		n = vsnprintf (pBuffer, size, czFormat, ap);          
		if (n > -1 && n < size )
		{
			//成功格式化
			//pBuffer[n] = '\0';
			strOString =  pBuffer;
			if( pBuffer!=sBuffer )
			{      
				::free(pBuffer);
			}
			return TRUE;
		}
		if( pBuffer!=sBuffer )
		{      
			::free(pBuffer);
		}
		pBuffer = NULL;
		size *= 2;
		if( size>MAX_STRING_LENGHT )
		{
			assert(0);
			return FALSE;
		}
		
		pBuffer = (char*) ::malloc(size);
		assert(pBuffer!=NULL);
	}
	if( pBuffer && pBuffer!=sBuffer  )
	{
		::free(pBuffer);
	}
	return FALSE;
}



BOOL DLPStrUtil::Format(CDLPString &strOString, const char* czFormat, ...)
{
	BOOL bRet;
	va_list ap;
	va_start(ap, czFormat);
	bRet = DLPStrUtil::VFormat(strOString, czFormat, ap);
	va_end(ap);
	return  bRet;

}

BOOL DLPStrUtil::AppendWithFormat(CDLPString &strIOString, const char* czFormat, ...)
{
	BOOL bRet; 
	CDLPString strTemp;
	va_list ap;
	va_start(ap, czFormat);
	bRet = DLPStrUtil::VFormat(strTemp, czFormat, ap);
	va_end(ap);

	if( bRet )
	{
		strIOString += strTemp;
	}
	return bRet;
}
