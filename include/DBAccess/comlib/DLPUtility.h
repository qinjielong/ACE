/*
******************************************

Description: 一些公用的功能函数和模块 Utility公用
********************************************
*/

#ifndef _DLP_H_DLPUTILITY_H_
#define _DLP_H_DLPUTILITY_H_

#include "Type.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include "ISystemLayInterface.h"

#ifdef _LINUX
#include <stdarg.h>
#endif

/*
*********************************************************************
*
*@brief :  链表操作
*
*********************************************************************
*/
template<class T>
struct _StruDLPList
{
	struct _StruDLPList *next; 
	struct _StruDLPList *prev;
	T Data;

	_StruDLPList(void)
	{
		Init();
	}
	~_StruDLPList(void)
	{
		Init();
	}

	BOOL Empty(void) const
	{
		return (next==this && prev==this);
	}

	INLINE void Init(void)
	{
		next = this;
		prev = next;		
	}

	INLINE void Unlink(void)
	{
		if( !Empty() )
		{                     
			next->prev = prev;
			prev->next = next;
			next = this;
			prev = this;
		}
	}



	/*
	*********************************************
	Function : RemoveFront
	DateTime : 
	Description :  移除第一个链表
	Input :  head 链表头
	Output : 
	Return : 链表为空返回 NULL
	Note :   
	*********************************************
	*/
	static INLINE struct _StruDLPList<T> *RemoveFront( struct _StruDLPList<T> *head )
	{
		if( head->Empty() )
		{
			return NULL;
		}
		struct _StruDLPList<T> *pRet = head->next;
		pRet->Unlink();
		return pRet;
	}

	/*
	*********************************************
	Function : Add
	DateTime : 
	Description :  添加数据到 prev 和 next 之间
	Input :  
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	static INLINE void Add( struct _StruDLPList<T> *pnew,
		struct _StruDLPList<T> *prev,
		struct _StruDLPList<T> *next)
	{
		next->prev = pnew;
		pnew->next = next;
		pnew->prev = prev;
		prev->next = pnew;
	}

	/*
	*********************************************
	Function : AddInsert
	DateTime : 
	Description :   添加数据到 prev 和 next 之间
	Input :  
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	static INLINE void AddInsert(struct _StruDLPList<T> *pnew,
		struct _StruDLPList<T> *prev,
		struct _StruDLPList<T> *next)
	{
		Add(pnew, prev, next);
	}


	/**
	*@brief 把新节点pnew 加到链表head的开始位置
	*/
	static INLINE void AddFirst(struct _StruDLPList<T> *pnew, struct _StruDLPList<T> *head)
	{
		Add(pnew, head, head->next);
	}

	/**
	*@brief 把新节点pnew 加到链表head的结尾
	*/	
	static INLINE void AddTail(struct _StruDLPList<T> *pnew, struct _StruDLPList<T> *head)
	{
		Add(pnew, head->prev, head);
	}

	static INLINE void Swap( struct _StruDLPList<T> &stSrcHead, 
							struct _StruDLPList<T> &stDestHead )
	{
		struct _StruDLPList<T> stTemp;
		stTemp.Init();

		if( !stSrcHead.Empty() )
		{
			stTemp.prev = stSrcHead.prev;
			stTemp.next = stSrcHead.next;
		}
		
		if( stDestHead.Empty() )
		{
			stSrcHead.Init();
		}
		else
		{
			stSrcHead.next = stDestHead.next;
			stSrcHead.next->prev = &stSrcHead;
			stSrcHead.prev = stDestHead.prev;
			stSrcHead.prev->next = &stSrcHead;
		}

		if( stTemp.Empty() )
		{
			stDestHead.Init();
		}
		else
		{
			stDestHead.next = stTemp.next;
			stDestHead.next->prev = &stDestHead;
			stDestHead.prev = stTemp.prev;
			stDestHead.prev->next = &stDestHead;
		}

	}


}; //end _StruDLPList



typedef _StruDLPList<void*> StruDLPListNode;


template<class T>
class CDLPListMemCache
{


public :
	typedef _StruDLPList<T> StruListNode;

	CDLPListMemCache( UINT iSize = 10000 )	
		:m_csMutex()
	{
		m_pCacheMem = NULL;
		m_pMemBegin = NULL;
		m_pMemEnd = NULL;

	
		m_pMemBegin = new StruListNode[iSize+1];
		if( m_pMemBegin )
		{
			m_pMemEnd = &m_pMemBegin[iSize];
			for( UINT i=0; i<iSize; i++  )
			{			
				m_pMemBegin[i].next = m_pCacheMem;
				m_pCacheMem =&m_pMemBegin[i];
			}
		}

	}

	~CDLPListMemCache(void)
	{
		m_csMutex.Lock();
		if( m_pMemBegin )
		{
			delete[] m_pMemBegin;
		}
		m_pCacheMem = NULL;
		m_pMemBegin = NULL;
		m_pMemEnd = NULL;
		m_csMutex.Unlock();
	}
	

	StruListNode *Get(void)
	{
		StruListNode *pRet = NULL;
		m_csMutex.Lock();
		if( m_pCacheMem )
		{
			pRet = m_pCacheMem;
			m_pCacheMem = pRet->next;
			pRet->Init();
		}
		else
		{
			pRet = new StruListNode;
			if( pRet )
			{
				pRet->Init();
			}
		}
		m_csMutex.Unlock();
		return pRet;
	}

	void Free( StruListNode *pNode )
	{
		
		m_csMutex.Lock();
		if( pNode>=m_pMemBegin && pNode<m_pMemEnd )
		{
			pNode->Init();
			pNode->next = m_pCacheMem;
			m_pCacheMem = pNode;
		}
		else
		{
			delete pNode;
		}
		m_csMutex.Unlock();


	}
private :
	CDLPMutex m_csMutex;
	StruListNode *m_pCacheMem; //缓冲内存池
	StruListNode *m_pMemBegin;
	StruListNode *m_pMemEnd;
	
};




/*
*********************************************************************
*
*@brief : 字符串操作功能
*
*********************************************************************
*/
namespace  DLPStrUtil
{


	// 删除字符串左边空白
	CDLPString TrimLeft(const CDLPString& str);

	// 删除字符串右边空白
	CDLPString TrimRight(const CDLPString& str);

	// 删除字符串左右两边空白
	CDLPString Trim(const CDLPString& str);

	// 转换为小写字符串
	CDLPString ToLower(const CDLPString& str);

	// 转换为大写字符串
	CDLPString ToUpper(const CDLPString& str); 

	// 分解字符串
	void Split(std::vector<CDLPString> &csResult, const CDLPString& str, const CDLPString& delimiters);


	// 字符串类型转换模板函数
	// 字符串类型转换模板函数
	template<class T> T ToNumber(const CDLPString& str);
	template<class T> T ToHexNumber(const CDLPString& str);
	template<class T> CDLPString ToString(const T value);
	template<class T> CDLPString ToHexString(const T value);



	/*
	*********************************************
	Function : ToNumber
	DateTime : 
	Description : 将十进制字符串转换为数值
	Input :
	Output :
	Return :
	Note :
	*********************************************
	*/
	template<class T> 
	T ToNumber(const CDLPString& str)
	{
		T value;
		std::istringstream iss(str.c_str());
		iss >> value;
		return value;
	}

	/*
	*********************************************
	Function :
	DateTime : 
	Description : 将十六进制字符串转换为数值
	Input :
	Output :
	Return :
	Note :
	*********************************************
	*/
	template<class T> 
	T ToHexNumber(const CDLPString& str) 
	{
		T value;
		std::istringstream iss(str.c_str());
		iss >> std::hex >> value;
		return value;
	}


	template<class T> 
	CDLPString ToString(const T value) 
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}


	template<class T> 
	CDLPString ToHexString(const T value) 
	{
		std::ostringstream oss;
		oss << "0x" << std::hex << value;
		return oss.str();
	}

	BOOL Format(CDLPString &strOString, const char* czFormat, ...);
	BOOL VFormat(CDLPString &strOString,const char *czFormat, va_list ap );
	BOOL AppendWithFormat(CDLPString &strIOString, const char* czFormat, ...);

	template<class T> 
	INT CheckValueRange(T &vValue, const T vMin, const T vMax)
	{
		if( vValue<vMin )
		{
			vValue = vMin;
			return -1;
		}
		else if( vValue > vMax )
		{
			vValue = vMax;
			return 1;
		}
		return 0;
	}

};

#endif //end _DLP_H_DLPUTILITY_H_