#ifndef CRECORDSET_DEF_H
#define CRECORDSET_DEF_H

// ͷ�ļ�
#include "CConnection.h"
// OuterInterfaceͷ�ļ�
#include "../OuterInterface/IDBAccessModule.h"

/**************************************************************************************************

Description : 
���ݼ�������
**************************************************************************************************/
namespace	DBAccessModule
{

	class CRecordSet:public IRecordSet
	{
	public:
		CRecordSet(void);
		virtual ~CRecordSet(void);

	public:
		// OuterInterface֮��̳�IRecordSet�� [4/26/2010 liujs]

	public:
		// �ڲ��ӿڵĶ���
		// ��ѯ�ӿ�
		virtual	IRecordSet*		QuerySql(const char*  szSql) = 0;
	};

}
#endif // CRECORDSET_DEF_H

