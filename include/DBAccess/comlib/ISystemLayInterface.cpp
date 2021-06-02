// ISystemLayInterface.cpp : 定义控制台应用程序的入口点。
//



#include "ISystemLayInterface.h"

#ifdef _WIN32
#include <direct.h>
#include <Mmsystem.h>
#include <windows.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>


#ifdef WINCE
#pragma once
#include <Windows.h>

#elif _WIN32

#pragma once
#include <intrin.h>

#else

static CDLPMutex s_csAtomicInter;

#endif


// 对iVal 进行递增操作， 返回增加后的值
EXPORT_API long AtomicInterInc( DLPAtomicInter &iVal )
{
#ifdef _WIN32
		return InterlockedIncrement( &iVal );
#else
		CDLPAutoMutex locker( &s_csAtomicInter);
		iVal++;
		return iVal;
		
#endif

}
// 对iVal 进行递减操作， 返回递减后的值
EXPORT_API long AtomicInterDec( DLPAtomicInter &iVal )
{

#ifdef _WIN32
	return InterlockedDecrement( &iVal );
#else
	CDLPAutoMutex locker( &s_csAtomicInter);
	iVal--;
	return iVal;

#endif
}
//比较iVal 释放 以iOldVal 相等， 如果相等，把 iVal 设定为 iNewVal, 并返回TRUE， 否则返回FALSE
EXPORT_API BOOL AtomicInterCompareExchange(DLPAtomicInter &iVal, 
										   const long iOldVal, const long iNewVal)
{

#ifdef _WIN32
	return  (InterlockedCompareExchange( &iVal,iNewVal, iOldVal) == iOldVal ? TRUE :FALSE );
#else
	CDLPAutoMutex locker( &s_csAtomicInter);
	if( iVal == iOldVal )
	{
		iVal = iNewVal;
		return TRUE;
	}
	return FALSE;
#endif
}


EXPORT_API long AtomicInterSet(DLPAtomicInter &iVal, const long iNewVal )
{
#ifdef _WIN32
	return InterlockedExchange( &iVal, iNewVal);
#else
	CDLPAutoMutex locker( &s_csAtomicInter);
	long i = iVal;
	iVal = iNewVal;
	return i;
#endif
}


EXPORT_API long AtomicInterAnd(DLPAtomicInter &iVal, const long iFlag )
{
#ifdef _WIN32
	return _InterlockedAnd( &iVal, iFlag);
#else
	CDLPAutoMutex locker( &s_csAtomicInter);
	long i = iVal;
	iVal &= iFlag;
	return i;
#endif
}

EXPORT_API long AtomicInterOr(DLPAtomicInter &iVal, const long iFlag )
{
#ifdef _WIN32
	return _InterlockedOr( &iVal, iFlag);
#else
	CDLPAutoMutex locker( &s_csAtomicInter);
	long i = iVal;
	iVal |= iFlag;
	return i;
#endif
}


#ifdef _WIN32 
# define DIR_DELIMITERS   '\\'
#else
# define DIR_DELIMITERS   '/'
#endif


CDLPString DLPGetApplicationPath(void)
{
#define DLP_MAX_PATH 300
	char czBuffer[DLP_MAX_PATH];
	std::string strAppPath = "./";
	bzero(czBuffer, sizeof(czBuffer)); 
#ifdef _WIN32
	// 获取执行程序文件路径
	::GetModuleFileNameA(NULL,  czBuffer, DLP_MAX_PATH-1);
	strAppPath = (char*) czBuffer;    
#else
	int n;
	n = readlink("/proc/self/exe" , czBuffer , DLP_MAX_PATH-1);    
	if( n > 0 )
	{            
		strAppPath = czBuffer;        
	} 
	czBuffer[ DLP_MAX_PATH-1] = '\0';
#endif
	CDLPString strRet = strAppPath.substr(0,strAppPath.rfind(DIR_DELIMITERS) );
	DLPPathParser(strRet);
	return strRet;	// 不存在返回当前路径

}

void DLPPathParser( CDLPString &strPath)
{
	std::string strTemp;
	char chrDelimiter = DIR_DELIMITERS;
	INT iCnts = 0;
	INT iLen = strPath.length();
	int i = 0;
	for( ; i<iLen; i++)
	{   
		if( strPath[i]=='\\' || strPath[i]=='/' )
		{
			if( iCnts==0 )
			{
				strTemp += chrDelimiter;
			}
			iCnts++;
		}
		else
		{
			iCnts = 0;
			strTemp += strPath[i];
		}
	}

	if( i && strPath[i-1]!=chrDelimiter )
	{
		strTemp += chrDelimiter;
	}
	strPath = strTemp;
}


static BOOL _CreateDir( const char *czPath )
{
#ifdef WINCE
	return (_mkdir( czPath,NULL ) == 0) ;
#elif _WIN32
	return (_mkdir( czPath ) == 0) ;
#else
	return (mkdir( czPath, 777 ) == 0) ;
#endif
}

BOOL DLPTestAndCreateDir( const char *czPath)
{
	std::string strTemp = czPath;
	int i = strTemp.length();
	if( i>0 && strTemp[i-1] == DIR_DELIMITERS )
	{
		strTemp = strTemp.substr( 0, i-1 );
	}

#ifdef _WIN32
	struct _stat stBuf;
	if( 0 == ::_stat(strTemp.c_str(), &stBuf) )
	{
		return stBuf.st_mode&_S_IFDIR ? TRUE : FALSE;
	}
#else
	struct stat stBuf;
	if( 0== stat(czPath, &stBuf) )
	{
		return S_ISDIR(stBuf.st_mode) ? TRUE : FALSE;
	}
#endif
	std::string strParentDir = strTemp.substr( 0, strTemp.rfind(DIR_DELIMITERS) );
	if( !strParentDir.empty() )
	{
		if( DLPTestAndCreateDir( strParentDir.c_str() ) )
		{
			return _CreateDir(strTemp.c_str() );
		}
	}  

	return FALSE;
}
















