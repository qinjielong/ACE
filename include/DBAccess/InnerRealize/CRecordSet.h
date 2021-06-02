#ifndef CRECORDSET_DEF_H
#define CRECORDSET_DEF_H

// 头文件
#include "CConnection.h"
// OuterInterface头文件
#include "../OuterInterface/IDBAccessModule.h"

/**************************************************************************************************

Description : 
数据集对象类
**************************************************************************************************/
namespace	DBAccessModule
{

	class CRecordSet:public IRecordSet
	{
	public:
		CRecordSet(void);
		virtual ~CRecordSet(void);

	public:
		// OuterInterface之间继承IRecordSet类 [4/26/2010 liujs]

	public:
		// 内部接口的定义
		// 查询接口
		virtual	IRecordSet*		QuerySql(const char*  szSql) = 0;
	};

}
#endif // CRECORDSET_DEF_H

