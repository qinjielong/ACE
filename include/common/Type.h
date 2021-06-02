#ifndef _DLP_TYPE_H
#define _DLP_TYPE_H

#include <string>
#include <vector>
#include <map>
#define _LINUX 1
#undef _WIN32

#ifdef WINCE
#include <windows.h>
#include <winsock2.h>		
#pragma   comment(lib,"Ws2.lib")
#elif _WIN32

// 消除sprintf 等函数在VS 2008 平台下的告警
#pragma warning(disable:4996)			// vs strcpy,sprintf....	
#pragma warning(disable:4786)			// stl map
// linux    数据库相关文件

#include <winsock2.h>		
#pragma   comment(lib,"Ws2_32.lib")
#include <windows.h>

#elif _LINUX

#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>

#endif

/******************************************************************************/
/********************************数据类型定义********************************* ***/
/******************************************************************************/

#ifdef _WIN32

typedef 	unsigned char		BYTE;
typedef		signed	 char		INT8;
typedef 	BYTE				UINT8;
typedef 	int					INT;
typedef		unsigned int		UINT;
typedef 	short				INT16;
typedef 	unsigned short		UINT16;
typedef 	__int32				INT32;
typedef 	unsigned __int32	UINT32;

typedef 	unsigned short		WORD;
typedef 	unsigned long		DWORD;

#define		ERR32				(INT32)

typedef		void *				HANDLE;


typedef 	__int64				INT64;
typedef 	unsigned __int64	UINT64;


#ifndef		BOOL 
#define		BOOL	INT
#endif
#ifndef		FALSE
#define		TRUE	(INT)0x1
#define		FALSE	(INT)0
#endif

#define EXPORT_API     extern "C" __declspec(dllexport) 
#define IMPORT_API     extern "C" __declspec(dllimport)
#define EXPORT_CLASS     __declspec(dllexport) 
#define IMPORT_CLASS     __declspec(dllimport)

#define INLINE    	inline

#elif _LINUX
// linux
//typedef short int           int16_t;
//typedef	unsigned short int  uint16_t;
//
//typedef int                 int32_t;
//typedef unsigned int		uint32_t;
//
//# if __WORDSIZE == 64
//typedef long int            int64_t;
//typedef unsigned long int	uint64_t;
//# else
//typedef long long int       int64_t;
//typedef unsigned long long	uint64_t;
//#endif
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

typedef     long            LONG;
typedef		signed	 char	INT8;
typedef 	unsigned char	BYTE;
typedef 	char			CHAR;
typedef 	BYTE			UINT8;
typedef 	int				INT;
typedef 	unsigned int	UINT;
typedef 	int16_t			INT16;
typedef 	uint16_t		UINT16;
typedef    	int32_t			INT32;
typedef   	uint32_t		UINT32;

typedef		unsigned short	WORD;
typedef		unsigned long	DWORD;

typedef    	int64_t			INT64;
typedef   	uint64_t		UINT64;

typedef		void *			HANDLE;

typedef		void			VOID;

#ifndef CALLBACK
#define CALLBACK
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef BOOL 
#define BOOL 	INT
#endif
#ifndef		FALSE
#define TRUE	1
#define FALSE	0
#endif

#define EXPORT_API
#define IMPORT_API 

#define INLINE	inline

#endif

/******************************************************************************/
/********************************导出导入函数宏定义 *********************** ***/
/******************************************************************************/
#ifdef API_EXPORT
#define DLP_API EXPORT_API
#define DLP_CLASS  EXPORT_CLASS

#elif API_IMPORT
#define DLP_API IMPORT_API
#define DLP_CLASS   IMPORT_CLASS

#else
#define DLP_API 
#define DLP_CLASS  
#endif


/******************************************************************************/
/********************************数据大小比较宏定义 *********************** ***/
/******************************************************************************/

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef ROUND
# include <math.h>
# define ROUND(f) (INT)( floor( (f) + 0.5) )
#endif

/******************************************************************************/
/********************************休眠定义 *********************** ***/
/******************************************************************************/
#ifdef _WIN32

#define MSLEEP(x)			Sleep(x)
#define DLPAtomicInter		volatile LONG

#define bzero(pMem, iSize )  memset( pMem, 0, iSize)

#define strncasecmp(str1, str2, n)  _strnicmp(str1,str2, n)
#define strcasecmp(str1,str2 )   _stricmp( str1,str2 )
#define DLP_SNPRINTF(xbuf, xbufsize , xfmt, ... ) _snprintf_s(xbuf,xbufsize, _TRUNCATE,xfmt, __VA_ARDLP__)

#elif _LINUX

#define MSLEEP(x)  usleep(1000L*(x))
#define DLPAtomicInter		volatile LONG
#define DLP_SNPRINTF  snprintf


#endif




//========国际化 格式化字符串宏
#define _DLPTX(x)   x

//===========极大值定义
#ifndef MIN_INT16
#define MIN_INT16 (-0x7fff - 1)
#endif

#ifndef MAX_INT16
#define MAX_INT16 0x7fff
#endif


#ifndef MAX_UINT16 
#define MAX_UINT16 0xFFFF
#endif

#ifndef MIN_INT32
#define MIN_INT32 (-0x7fffffff - 1)
#endif

#ifndef MAX_INT32
#define MAX_INT32  0x7fffffff
#endif


#ifndef MAX_UINT32 
#define MAX_UINT32 0xFFFFFFFF
#endif

#ifndef MIN_INT64
#define MIN_INT64  (-0x7fffffffffffffffLL - 1)
#endif

#ifndef MAX_INT64
#define MAX_INT64  0x7fffffffffffffffLL
#endif

#ifndef MAX_UINT64
#define MAX_UINT64  0xFFFFFFFFFFFFFFFFLL
#endif



#define DLP_VOLATILE volatile

//==========断言定义
#define DLP_ASSERT assert
#define DLP_ASSERT_RET_VAL( x, val) do{ DLP_ASSERT((x)); if(!(x)) return val; }while(0)
#define DLP_ASSERT_RET(x) do{ DLP_ASSERT((x)); if(!(x)) return; }while(0)


#define DLP_ASSERT_EXIT(x, val ) do{ \
	if(!(x))  { \
	DLP_ASSERT((x)); \
	printf("Assert Exit %s %d\n", __FILE__, __LINE__); \
	_exit(val); \
	} }while(0)

//=============计算数组大小
#ifndef ARRARY_SIZE
#define ARRARY_SIZE(x)  (sizeof(x)/sizeof((x)[0]))
#endif

//==========生成 32 位 标签
#define DLP_MAKE_TAG( c1, c2, c3, c4 )  ( c1 << 24 | c2 << 16 | c3 << 8 | c4 )

//个数化打印定义
#define PUINT64 "llu"
#define PINT64  "lld"
#define PINT32 "ld"
#define PUINT32 "lu"

#ifndef KBYTES
#define KBYTES (1024L)
#endif

#ifndef MBYTES
#define MBYTES (1L<<20)
#endif

#ifndef GBYTES
#define GBYTES (1L<<30)
#endif

#define	CDLPString std::string


//增加一个基础类
class CDLPObject
{
public :
	CDLPObject(void)
	{

	}
	virtual ~CDLPObject(void)
	{

	}
};

#endif //_DLP_TYPE_H
