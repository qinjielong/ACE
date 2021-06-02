/*
******************************************

Description: �����̳߳�
********************************************
*/

#ifndef _DLP_H_DLPTHREADPOOL_H_
#define _DLP_H_DLPTHREADPOOL_H_

#include "Type.h"
#include "DLPUtility.h"
#include "ISystemLayInterface.h"
#include <set>


class CDLPThreadPool; //�����̳߳أ� Ҳ�����Ϊ�����

/*
*********************************************
Function :   DLPThreadPoolCallback
DateTime : 
Description :   �̳߳صĹ��������ص�
Input : pcsPool �����������̳߳ض���
Input : TaskData �������ݿ�
Input : pDebugInfo ����ʹ�õ���Ϣ
Output :
Return :
Note :
*********************************************
*/
typedef void (*DLPThreadPoolCallback)(CDLPThreadPool *pcsPool, void *TaskData, void *pDebugInfo );

class CDLPTaskThread;



/*
********************************************************************
��ע��
����    :    CDLPThreadGlobal
����    :    
����ʱ��:    
������  :	�����̹߳������ �����߳��ڸ��������̳߳صĵ���
*********************************************************************
*/

class CDLPThreadGlobal
{
private :
	friend class CDLPThreadPool;
	friend class CDLPTaskThread;



	static CDLPThreadGlobal *s_pDefaultGlobal; //Ĭ�ϵĹ����̹߳������
	static DLPAtomicInter s_iInitRefs;         //�̳߳�ģ�鱻���õļ���

	//�������������
	StruDLPListNode *m_pCache;
	StruDLPListNode *m_vCacheBufferBegin;
	unsigned char *m_vCacheBufferEnd;
	UINT32 m_iCaches;


	StruDLPListNode m_stSchedules;    //�ȴ����ȵ��̳߳�
	StruDLPListNode m_stGIdleThreads; //���е��߳�
public:
	CDLPMutex m_csMutex;  // m_stSchedules ��ͬ����

	std::set<CDLPTaskThread*> m_csThreadQueue; //�̶߳���
	

	DLPAtomicInter  m_iExitThreads;  //��ǰ�Ѿ��������߳���
	INT16	 m_iWaitQueueSize;      //�ȴ����ȵ��̳߳ظ���
	INT16   m_iGIdleThreads;       //��ǰ�����߳�
	UINT16  m_iGMaxIdleThreads;    //����������еĹ����߳���	
	UINT16	m_iGMaxThreads;       //��󹫹��߳���
	UINT16	m_iGThreads;          //��ǰ�����߳���	

	DLPAtomicInter m_iRefs;       //�Ķ�������ü���

	INT32 m_iDefaultIdleWait;


	CDLPThreadGlobal(void);
	~CDLPThreadGlobal(void);
	
	/*
	 *********************************************
	 Function : Ref
	 DateTime : 
	 Description :  ��������
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	void Ref(void); 

	/*
	 *********************************************
	 Function : Unref
	 DateTime : 
	 Description :  ��������
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	void Unref(void);

	/*
	 *********************************************
	 Function : GetListNode
	 DateTime : 
	 Description :  ���ڴ�ط��� StruDLPListNode ����
	 Input :  
	 Output : 
	 Return :  ʧ�ܷ���NULL
	 Note :   
	 //********************************************
	 */
	INLINE StruDLPListNode *GetListNode(void);

	/*
	 *********************************************
	 Function : FreeListNode
	 DateTime : 
	 Description : �ͷ� GetListNode ���صĶ���
	 Input :  pNode ���ͷŵĶ���
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	INLINE void FreeListNode(StruDLPListNode *pNode);


	/*
	 *********************************************
	 Function : Push
	 DateTime : 
	 Description :  ���̳߳ؼӵ��ȴ����ȶ���
	 Input :  pThread ָ�����̳߳�
	 Input : bFirst �ͷ����ȵ���
	 Output : 
	 Return : TRUE/FALSE
	 Note :   
	 *********************************************
	 */
	BOOL Push( CDLPThreadPool *pThread, BOOL bFirst  );

	/*
	 *********************************************
	 Function : Clear
	 DateTime :
	 Description :  ����̳߳صĵ�������
	 Input :   pThread ָ�����̳߳�
	 Output : 
	 Return : ��������ĵ���������
	 Note :   
	 *********************************************
	 */
	INT Clear( CDLPThreadPool *pThread );

	/*
	 *********************************************
	 Function : ClearSource
	 DateTime : 
	 Description :  �ͷű�������Դ
	 Input :  
	 Output : 
	 Return : 
	 Note :   �����ͷ�ʱ����
	 *********************************************
	 */
	void ClearSource(void);

public :
	/*
	 *********************************************
	 Function : Global
	 DateTime : 
	 Description :  ����ȫ���ԵĹ������
	 Input :  
	 Output : 
	 Return : ʧ�ܷ���NULL 
	 Note :  ֻ�е����� CDLPThreadPool::InitModule�������
	 *********************************************
	 */
	static CDLPThreadGlobal *Global(void);

	/*
	 *********************************************
	 Function : Create
	 DateTime : 
	 Description :  ���� CDLPThreadGlobal ����
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	static CDLPThreadGlobal *Create(void);


	/*
	 *********************************************
	 Function : Release
	 DateTime : 
	 Description :  �ͷű�����
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	void Release(void)
	{
		ClearSource();
		Unref();
	}


	/*
	*********************************************
	Function : SetMaxGlobalThread
	DateTime : 
	Description :  �趨�����̳߳�ʹ�õ���󹫹��߳������� �����ָ��
	�߳����ݽ����̳߳ص�ʵ���������ӡ�
	��ֵ��������ռģʽ�̵߳�������

	Input :  iNums ����߳���
	Output : 
	Return : 
	Note :   0 ��ʾ���趨
	*********************************************
	*/
	void SetMaxGlobalThread(  UINT16 iNums = 32 );

	/*
	*********************************************
	Function : GetMaxGlobalThread
	DateTime : 
	Description :  ���ع����̳߳����������ٸ������߳��ڵȴ�
	Input :  
	Output : 
	Return : ���� SetMaxGlobalThread ���趨ֵ
	Note :   
	*********************************************
	*/
	INLINE  UINT16  GetMaxGlobalThread(void) const
	{
		return m_iGMaxThreads;
	}

	/*
	*********************************************
	Function : GetGlobalThreads
	DateTime : 
	Description :  ���ص�ǰ�����̳߳ص��̸߳���
	Input :  
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	INLINE UINT16 GetGlobalThreads(void) const
	{
		return m_iGThreads;
	}



	/*
	*********************************************
	Function :  SetMaxIdleThreads
	DateTime : 
	Description :  �趨�����̳߳����������ٸ������߳��ڵȴ�, Ĭ�� 32 
	Input :  iNums ��������
	Output :
	Return :
	Note :
	*********************************************
	*/
	void   SetMaxIdleThreads( UINT16 iNums = 32);

	
	/*
	*********************************************
	Function : GetMaxIdleThreads
	DateTime : 
	Description :  ���ع����̳߳����������ٸ������߳��ڵȴ�
	Input :  
	Output : 
	Return : ���� SetMaxIdleThreads ���趨ֵ
	Note :   
	*********************************************
	*/
	INLINE UINT16  GetMaxIdleThreads(void) const
	{
		return m_iGMaxIdleThreads;
	}


	/*
	*********************************************
	Function : GetIdleThreads
	DateTime : 
	Description :  ���ص�ǰ���е��߳���
	Input :  
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	INLINE UINT16 GetIdleThreads(void) const
	{
		return m_iGIdleThreads;
	}

	
	/*
	*********************************************
	Function : SetMaxIdleWait
	DateTime : 
	Description :  �趨Ĭ�Ͽ��м���ֵ
	Input :  iMSec ���ֵ�� ��λ����
	��ʾ�ԷǶ�ռģʽ �̳߳س�����ʱ��û�����̳߳ص��߳̽����յ������̳߳أ�
	���̳߳�������ʱ�ٴӹ����̳߳ش��»�ȡ�߳�
	<0 ��ʾ���õȴ�
	Ĭ��ֵ 500 ����
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	void  SetDefaultIdleWait( INT32 iMSec = 500 );

};




/*
********************************************************************
��ע��
����    :    CDLPThreadPool
����    :    
����ʱ��:    
������  :		�̳߳أ� Ҳ�����Ϊ�����
*********************************************************************
*/

class CDLPThreadPool
{

public:
	friend class CDLPTaskThread;

	/*
	 *********************************************
	 Function : CDLPThreadPool
	 DateTime : 
	 Description :  �����̳߳�
	 Input :  pThreadGlobal �����Ĺ����̹߳����ߣ� �����ָ����ʹ�� CDLPThreadGlobal::Global Ϊ������
	 Output : 
	 Return : 
	 Note :   ÿ���̳߳ر�������ĳ�������̹߳�����
	 *********************************************
	 */
	CDLPThreadPool( CDLPThreadGlobal *pThreadGlobal = NULL);
	~CDLPThreadPool(void);

	//���ش���Ŷ���
	static const INT RSUCCESS = 0;   //�ɹ�
	static const INT EUNKNOWN = -1;  //δ֪����
	static const INT EFLOWOUT = -2;  //�����޶���Χ���
	static const INT ENMEMORY = -3;  //û���ڴ�
	static const INT EINVALID = -4; //��������


	/*
	 *********************************************
	 Function : GetThreadGlobal
	 DateTime : 
	 Description :  ���ع������̵߳Ĺ�����
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	CDLPThreadGlobal &GetThreadGlobal(void);



	/*
	*********************************************
	Function : SetIdleWait
	DateTime : 
	Description :  �趨���߳̿��м���ֵ
	Input :  iMSec �趨ֵ, �ο� CDLPThreadGlobal::SetDefaultIdleWait
	Output : 
	Return : 
	Note :   ������趨�� ���̳������̹߳����ߵ�Ĭ��ֵ, �ο�CDLPThreadGlobal::SetDefaultIdleWait
	*********************************************
	*/
	void SetIdleWait( INT32 iMSec );



	/*
	*********************************************
	Function : Init
	DateTime : 
	Description :  ��ʼ���̳߳�
	Input : fnOnCallback �ص�����
	Input : iThreadCounts ָ�� �̳߳ز���ʹ�õ��̸߳���
	Input : bExclusive ָ���߳��Ƿ��Ƕ�ռ��ʽ
	*��ռ���ڱ��̳߳��ͷ�ǰ�����ͷ�������
	�ȴ��̶߳����ڸ��߳�����
	Output : 
	Return :  TRUE/FALSE
	Note :   ֻ�г�ʼ����ſ���ʹ��
	*********************************************
	*/

	BOOL Init( DLPThreadPoolCallback fnOnCallback, 
		UINT16 iThreadCounts = 1, 
		BOOL bExclusive = FALSE);

	/*
	*********************************************
	Function : Uinit
	DateTime : 
	Description :  ֹͣ�̳߳�
	Input :  bWaitJobsComplete �Ƿ�ȴ�ִ����ȴ�������
	TRUE �ȴ�ִ�������еĴ�������
	FALSE ��ִ�еȴ�����, �ȴ����񽫱��趨�Ļص��ͷ�, �ο�SetFreedTaskDataFunction
	����TRUE/FALSE �ĺ�������ȵ��������߳��˳��ŷ���
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	void Uninit(BOOL bJobsComplete = FALSE );


	/*
	*********************************************
	Function : IsInit
	DateTime : 
	Description :  �����ͷ��Ѿ�����
	Input :  
	Output : 
	Return : TRUE/FALSE
	Note :   
	*********************************************
	*/
	BOOL IsInit(void);

	/*
	*********************************************
	Function : SetFreedTaskDataFunction
	DateTime : 
	Description :  �趨�ͷ��������ݵĻص�����					
	Input :   fnFreeFunc �ص�����
	����趨�� ����������ʱ��û��ִ�е�������󽫵��øú����ͷ�
	Output : 
	Return : 
	Note :   ������趨�� ������ܵ����ڴ����
	*********************************************
	*/
	void SetFreedTaskDataFunction( void (*fnFreeFunc)(void *)  );

	/*
	*********************************************
	Function : SetMaxWaitTask
	DateTime : 
	Description :  �趨���ĵȴ�������
	Input :  iMaxVal �趨��ֵ�� < 0 ��ʾ����Ч
	Output : 
	Return : 
	Note :   �趨��ֵ�� ����ȴ����ȵ�������ڵ��ڸ�ֵ�� �������ʧ�ܣ������� EFLOWOUT
	*********************************************
	*/
	void SetMaxWaitTask( INT32 iMaxVal = -1 );

	/*
	*********************************************
	Function : Task
	DateTime : 
	Description :  �������
	Input :  pTaskData ��������
	Input :  bFirst �ͷżӵ��Ѿ����ڵ�����ǰ�� TRUE �ӵ���ͷ�� ����ִ�У� FALSE �ӵ���β
	Output : 
	Return : �ɹ����� 0�� ������ʾ����
	EFLOWOUT �� �����趨�����ֵ
	ENMEMORY    ��ʾ ��ȡ�ڴ�ʧ��			
	Note :   
	*********************************************
	*/
	INT Task( void *pTaskData, BOOL bFirst = FALSE );

	/*
	*********************************************
	Function : GetWaitTask
	DateTime : 
	Description :  ��ȡ��ǰ�ȴ����ȵ���������
	Input :  
	Output : 
	Return : ��������ֵ
	Note :   
	*********************************************
	*/
	UINT32 GetWaitTask(void);

	/*
	*********************************************
	Function : Clear
	DateTime : 
	Description :  ������еȴ����ȵ�����
	Input :  
	Output : 
	Return : 
	Note :   �ͷŵ����ݽ������趨�Ļص��ͷţ� �ο� SetFreedTaskDataFunction
	*********************************************
	*/
	void Clear(void);

	/*
	*********************************************
	Function : RemoveTask
	DateTime : 
	Description :  �Ƴ�����
	Input :  pTaskData ���Ƴ�������
	Output : 
	Return : �����ܹ��Ƴ��ĸ����� ������-1
	Note :   
	*********************************************
	*/
	INT RemoveTask(void *pTaskData);

	/*
	*********************************************
	Function : IsSelfThread
	DateTime : 
	Description :  �ж��ͷ�Ϊ���̳߳ص��߳� 
	Input :  
	Output : 
	Return : TRUE/FALSE
	Note :   
	*********************************************
	*/
	BOOL IsSelfThread(void);

	/*
	*********************************************
	Function : SetUserData
	DateTime : 
	Description :  �洢�û�����
	Input :  pUserData �û�����, Ĭ��ֵΪ NULL
	Output : 
	Return : 
	Note :  �û�Ҫ�Լ��ͷŸ����ݣ� ע�ⲻҪ�ڴ�й¶ 
	*********************************************
	*/
	void SetUserData( void *pUserData );

	/*
	*********************************************
	Function : GetUserData
	DateTime : 
	Description :  ��ȡ�洢���û����ݣ� �ο�SetUserData
	Input :  
	Output : 
	Return : ���� SetUserData ���趨ֵ
	Note :   
	*********************************************
	*/
	void *GetUserData(void);

	/*
	 *********************************************
	 Function : Disable
	 DateTime : 
	 Description :  ��ֹʹ��
	 Input :  bJobsComplete �Ƿ�ȴ�ִ����ȴ������� �ο� Uninit
	 Output : 
	 Return : 
	 Note :   ������Ѿ����ڵ�����
	 *********************************************
	 */
	void Disable( BOOL bJobsComplete = FALSE );

	/*
	 *********************************************
	 Function : Enable
	 DateTime : 
	 Description :  ����, Disable �ķ�����
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	void Enable(void);

	/*
	*********************************************
	Function : IsEnable
	DateTime : 
	Description :  ���ص�ǰ�Ƿ�ʹ��
	Input :  
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	BOOL IsEnable(void);


	

	/*
	*********************************************
	Function : InitModule
	DateTime : 
	Description :  ��ʼ���̳߳�ģ��, ֻ�е��øĽӿں����ʹ���̳߳ع���
	Input :  
	Output : 
	Return : 
	Note :   ���Ҫʹ���߳�ģ�飬 ������øú���
	*********************************************
	*/
	static void InitModule(void);

	/*
	*********************************************
	Function : UninitModule
	DateTime : 
	Description :  �ͷ��̳߳ع���ģ�飬 ��InitModule���ʹ��
	Input :  
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	static void UninitModule(void);

private :
	/*
	 *********************************************
	 Function : Run
	 DateTime : 2012/4/24 9:26
	 Description :  �̹߳����ߵ�����������
	 Input :   pThread ִ�б�����
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	BOOL Run(CDLPTaskThread *pThread );


	
	static INT32      s_iGIdleMSecs; //Idle ��ʱ���� ���룬 ��ǰ����
	static DLPAtomicInter m_iInitRefs; //���ü�����


	
	/*
	********************************************************************
	��ע��
	����    :    CCoreData
	����    :    
	����ʱ��:    
	������  :		�ڲ����ݹ�����
	*********************************************************************
	*/
	
	class CCoreData
	{
	public :
		CDLPThreadPool *m_pPool;
		CDLPThreadGlobal *m_pThreadGlobal;  //�̹߳�����
		DLPThreadPoolCallback m_fnCallback; //�̻߳ص�
		void (*m_fnFreeFunc)(void *);  //�������ݶ����ͷź���
		void *m_pUserData;    //�û�����
		INT32 m_iIdleInterval; //������IDLE ʱ�䣬 ����

		INT32 m_iMaxJobs;			//�������ȴ��������
		UINT32 m_iRefs;
		UINT32 m_iWaitTask;			//�ȴ�ִ�е���������
		UINT32 m_iCaches;           //���ػ������

		UINT16 m_iPermitThreads;    //��������߳���  
		UINT16 m_iNumThreads;		//��ǰʹ�õ��߳���
		UINT16 m_iStatus;			//״̬
		UINT16 m_bExclusive;		//�Ƿ��ռ
		UINT16 m_bWaiting;			//�ͷŵȴ�ִ�����
		UINT16 m_iIdleThreads;



		

		StruDLPListNode m_stTaskQueue; //�������
		StruDLPListNode *m_pTaskCache; //����ṹ�ڴ�ض���

		CDLPMutex m_csMutex;        //ͬ������
		CDLPCondEx m_csCond;        //ͬ���ź�

		CDLPCondEx  m_csExitCond;			//�ȴ��˳�

		
		/*
		 *********************************************
		 Function : CCoreData
		 DateTime : 
		 Description :  
		 Input :  pPool ���ڵ��̳߳�
		 Input :  pThreadGlobal  pPool �Ĺ�����
		 Output : 
		 Return : 
		 Note :   
		 *********************************************
		 */
		CCoreData(CDLPThreadPool *pPool, CDLPThreadGlobal *pThreadGlobal);
		~CCoreData(void);


		/*
		 *********************************************
		 Function : Unref
		 DateTime : 
		 Description :  ��������
		 Input :  
		 Output : 
		 Return : TRUE/FALSE
		 Note :   
		 *********************************************
		 */
		BOOL Unref(void);

		/*
		 *********************************************
		 Function : Ref
		 DateTime : 
		 Description :  ��������
		 Input :  
		 Output : 
		 Return : 
		 Note :   
		 *********************************************
		 */
		void Ref(void);

	    /*
	     *********************************************
	     Function : FreeWaitTask
	     DateTime : 
	     Description :  �ͷ��ڵȴ�����������
	     Input :  
	     Output : 
	     Return : 
	     Note :   
	     *********************************************
	     */
		void FreeWaitTask(void);

		/*
		 *********************************************
		 Function : RemoveTask
		 DateTime : 
		 Description :  �Ƴ��ȴ�������
		 Input :  
		 Output : 
		 Return : �����Ƴ��ĸ���
		 Note :   
		 *********************************************
		 */
		INT RemoveTask(void *pTaskData);

		/*
		 *********************************************
		 Function : FreeTaskNode
		 DateTime : 
		 Description :  GetTaskNode �ͷŵķ��ض���
		 Input :  pNode ���ͷŶ���
		 Output : 
		 Return : 
		 Note :   
		 *********************************************
		 */
		void FreeTaskNode(StruDLPListNode *pNode);

		/*
		 *********************************************
		 Function : GetTaskNode
		 DateTime : 
		 Description :  ���ڴ�ط��� GetTaskNode ����
		 Input :  
		 Output : 
		 Return : NULL��ʾʧ��
		 Note :   
		 *********************************************
		 */
		StruDLPListNode *GetTaskNode(void);	

		/*
		 *********************************************
		 Function : AddTask
		 DateTime : 
		 Description :  �������
		 Input :  pData �������
		 Input : bFirst �ͷ���Ч
		 Output : 
		 Return : NULL ��ʾʧ��
		 Note :   
		 *********************************************
		 */
		StruDLPListNode * AddTask( void *pData, BOOL bFirst);

	};

	CCoreData *m_pCore; //��������

};


/*
*********************************************************************
*
*@brief : CObjThreadPool , �����̳߳�
*
*********************************************************************
*/

class CObjThreadPool;


/*
 *********************************************
 Function : FuncPtrObjThreadPoolEvent
 DateTime : 
 Description :  �����̳߳ػص�����
 Input :  pcsPool ��Ӧ���̳߳�
 Input : pTaskData ��������
 Output : 
 Return : 
 Note :   
 *********************************************
 */
typedef void (CDLPObject::*FuncPtrObjThreadPoolEvent)(CObjThreadPool *pcsPool, void *pTaskData);






/*
********************************************************************
��ע��
����    :    CObjThreadPool
����    :    
����ʱ��:    
������  :		�����̳߳�
*********************************************************************
*/

class CObjThreadPool : public CDLPThreadPool
{		
public :
	/*
	 *********************************************
	 Function : CObjThreadPool
	 DateTime : 
	 Description :  
	 Input :  czName �̳߳�����
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	CObjThreadPool( const char *czName=NULL );

	/*
	 *********************************************
	 Function : CObjThreadPool
	 DateTime : 
	 Description :  
	 Input :  pThreadGlobal ָ���̹߳�����
	 Input :  czName �̳߳�����
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	CObjThreadPool( CDLPThreadGlobal *pThreadGlobal, const char *czName=NULL );
	virtual ~CObjThreadPool(void);


	/*
	 *********************************************
	 Function : Init
	 DateTime : 
	 Description :  ��ʼ���̳߳�
	 Input :  pTaskFnOwner ����ص����������Ķ���
	 Input :  pFnTaskCallback ����ص���������ص�����
	 Input : pcsPool �����������̳߳ض���
	 Input : TaskData �������ݿ�
	 Output : 
	 Return : TRUE/FALSE
	 Note :   
	 *********************************************
	 */
	BOOL Init( CDLPObject *pTaskFnOwner, FuncPtrObjThreadPoolEvent pFnTaskCallback,  
		UINT16 iThreadCounts,  BOOL bExclusive);
private :
	CDLPObject *m_pTaskFnOwner; //����ص����������Ķ���
	FuncPtrObjThreadPoolEvent m_pFnTaskCallback; //����ص���������ص�����
	CDLPString m_strName; //����

	/*
	 *********************************************
	 Function : ProxyEntry
	 DateTime : 
	 Description :  CDLPThreadPool �ĺ������, �ο�DLPThreadPoolCallback˵��
				ͨ��������ת����������
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	static void ProxyEntry( CDLPThreadPool *pcsPool, void *TaskData, void *pDebugInfo);

};

#endif //end _DLP_H_DLPTHREADPOOL_H_
