/*
******************************************

Description: 基本线程池
********************************************
*/

#ifndef _DLP_H_DLPTHREADPOOL_H_
#define _DLP_H_DLPTHREADPOOL_H_

#include "Type.h"
#include "DLPUtility.h"
#include "ISystemLayInterface.h"
#include <set>


class CDLPThreadPool; //声明线程池， 也可理解为任务池

/*
*********************************************
Function :   DLPThreadPoolCallback
DateTime : 
Description :   线程池的工作函数回调
Input : pcsPool 返回所属的线程池对象
Input : TaskData 任务数据库
Input : pDebugInfo 调试使用的信息
Output :
Return :
Note :
*********************************************
*/
typedef void (*DLPThreadPoolCallback)(CDLPThreadPool *pcsPool, void *TaskData, void *pDebugInfo );

class CDLPTaskThread;



/*
********************************************************************
类注释
类名    :    CDLPThreadGlobal
作者    :    
创建时间:    
类描述  :	公共线程管理对象， 管理线程在各个任务线程池的调度
*********************************************************************
*/

class CDLPThreadGlobal
{
private :
	friend class CDLPThreadPool;
	friend class CDLPTaskThread;



	static CDLPThreadGlobal *s_pDefaultGlobal; //默认的公共线程管理对象
	static DLPAtomicInter s_iInitRefs;         //线程池模块被引用的计数

	//缓冲区管理对象
	StruDLPListNode *m_pCache;
	StruDLPListNode *m_vCacheBufferBegin;
	unsigned char *m_vCacheBufferEnd;
	UINT32 m_iCaches;


	StruDLPListNode m_stSchedules;    //等待调度的线程池
	StruDLPListNode m_stGIdleThreads; //空闲的线程
public:
	CDLPMutex m_csMutex;  // m_stSchedules 的同步锁

	std::set<CDLPTaskThread*> m_csThreadQueue; //线程队列
	

	DLPAtomicInter  m_iExitThreads;  //当前已经创建的线程数
	INT16	 m_iWaitQueueSize;      //等待调度的线程池个数
	INT16   m_iGIdleThreads;       //当前空闲线程
	UINT16  m_iGMaxIdleThreads;    //最大允许悠闲的公共线程数	
	UINT16	m_iGMaxThreads;       //最大公共线程数
	UINT16	m_iGThreads;          //当前公共线程数	

	DLPAtomicInter m_iRefs;       //改对象的引用计数

	INT32 m_iDefaultIdleWait;


	CDLPThreadGlobal(void);
	~CDLPThreadGlobal(void);
	
	/*
	 *********************************************
	 Function : Ref
	 DateTime : 
	 Description :  增加引用
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
	 Description :  减少引用
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
	 Description :  由内存池分配 StruDLPListNode 对象
	 Input :  
	 Output : 
	 Return :  失败返回NULL
	 Note :   
	 //********************************************
	 */
	INLINE StruDLPListNode *GetListNode(void);

	/*
	 *********************************************
	 Function : FreeListNode
	 DateTime : 
	 Description : 释放 GetListNode 返回的对象
	 Input :  pNode 被释放的对象
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
	 Description :  把线程池加到等待调度队列
	 Input :  pThread 指定的线程池
	 Input : bFirst 释放优先调度
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
	 Description :  清除线程池的调度任务
	 Input :   pThread 指定的线程池
	 Output : 
	 Return : 返回清除的调度任务数
	 Note :   
	 *********************************************
	 */
	INT Clear( CDLPThreadPool *pThread );

	/*
	 *********************************************
	 Function : ClearSource
	 DateTime : 
	 Description :  释放本对象资源
	 Input :  
	 Output : 
	 Return : 
	 Note :   对象释放时调用
	 *********************************************
	 */
	void ClearSource(void);

public :
	/*
	 *********************************************
	 Function : Global
	 DateTime : 
	 Description :  返回全局性的管理对象
	 Input :  
	 Output : 
	 Return : 失败返回NULL 
	 Note :  只有调用了 CDLPThreadPool::InitModule后才优先
	 *********************************************
	 */
	static CDLPThreadGlobal *Global(void);

	/*
	 *********************************************
	 Function : Create
	 DateTime : 
	 Description :  创建 CDLPThreadGlobal 对象
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
	 Description :  释放本对象
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
	Description :  设定整个线程池使用的最大公共线程数量， 如果不指定
	线程数据将随线程池的实际用量增加。
	改值不包括独占模式线程的数据量

	Input :  iNums 最大线程数
	Output : 
	Return : 
	Note :   0 表示不设定
	*********************************************
	*/
	void SetMaxGlobalThread(  UINT16 iNums = 32 );

	/*
	*********************************************
	Function : GetMaxGlobalThread
	DateTime : 
	Description :  返回公共线程池最大允许多少个空闲线程在等待
	Input :  
	Output : 
	Return : 返回 SetMaxGlobalThread 的设定值
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
	Description :  返回当前公共线程池的线程个数
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
	Description :  设定公共线程池最大允许多少个空闲线程在等待, 默认 32 
	Input :  iNums 最大空闲数
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
	Description :  返回公共线程池最大允许多少个空闲线程在等待
	Input :  
	Output : 
	Return : 返回 SetMaxIdleThreads 的设定值
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
	Description :  返回当前空闲的线程数
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
	Description :  设定默认空闲检测的值
	Input :  iMSec 检测值， 单位毫秒
	表示对非独占模式 线程池超过该时间没任务，线程池的线程将回收到公共线程池，
	当线程池有任务时再从公共线程池从新获取线程
	<0 表示永久等待
	默认值 500 毫秒
	Output : 
	Return : 
	Note :   
	*********************************************
	*/
	void  SetDefaultIdleWait( INT32 iMSec = 500 );

};




/*
********************************************************************
类注释
类名    :    CDLPThreadPool
作者    :    
创建时间:    
类描述  :		线程池， 也可理解为任务池
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
	 Description :  构建线程池
	 Input :  pThreadGlobal 所属的公共线程管理者， 如果不指定将使用 CDLPThreadGlobal::Global 为管理者
	 Output : 
	 Return : 
	 Note :   每个线程池必须属于某个公共线程管理者
	 *********************************************
	 */
	CDLPThreadPool( CDLPThreadGlobal *pThreadGlobal = NULL);
	~CDLPThreadPool(void);

	//返回错误号定义
	static const INT RSUCCESS = 0;   //成功
	static const INT EUNKNOWN = -1;  //未知错误
	static const INT EFLOWOUT = -2;  //超出限定范围溢出
	static const INT ENMEMORY = -3;  //没有内存
	static const INT EINVALID = -4; //参数错误


	/*
	 *********************************************
	 Function : GetThreadGlobal
	 DateTime : 
	 Description :  返回管理公共线程的管理者
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
	Description :  设定本线程空闲检测的值
	Input :  iMSec 设定值, 参考 CDLPThreadGlobal::SetDefaultIdleWait
	Output : 
	Return : 
	Note :   如果不设定， 将继承所属线程管理者的默认值, 参考CDLPThreadGlobal::SetDefaultIdleWait
	*********************************************
	*/
	void SetIdleWait( INT32 iMSec );



	/*
	*********************************************
	Function : Init
	DateTime : 
	Description :  初始化线程池
	Input : fnOnCallback 回调函数
	Input : iThreadCounts 指定 线程池并发使用的线程个数
	Input : bExclusive 指定线程是否是独占方式
	*独占表在本线程池释放前无论释放有任在
	等待线程都属于改线程所有
	Output : 
	Return :  TRUE/FALSE
	Note :   只有初始化后才可以使用
	*********************************************
	*/

	BOOL Init( DLPThreadPoolCallback fnOnCallback, 
		UINT16 iThreadCounts = 1, 
		BOOL bExclusive = FALSE);

	/*
	*********************************************
	Function : Uinit
	DateTime : 
	Description :  停止线程池
	Input :  bWaitJobsComplete 是否等待执行完等待的任务
	TRUE 等待执行完所有的存在任务
	FALSE 不执行等待任务, 等待任务将被设定的回调释放, 参考SetFreedTaskDataFunction
	无论TRUE/FALSE 改函数都会等到所属的线程退出才返回
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
	Description :  返回释放已经启动
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
	Description :  设定释放任务数据的回调函数					
	Input :   fnFreeFunc 回调函数
	如果设定， 当对象销毁时，没有执行的任务对象将调用该函数释放
	Output : 
	Return : 
	Note :   按情况设定， 否则可能导致内存溢出
	*********************************************
	*/
	void SetFreedTaskDataFunction( void (*fnFreeFunc)(void *)  );

	/*
	*********************************************
	Function : SetMaxWaitTask
	DateTime : 
	Description :  设定最大的等待任务量
	Input :  iMaxVal 设定的值， < 0 表示不生效
	Output : 
	Return : 
	Note :   设定改值， 如果等待调度的任务大于等于该值， 添加任务将失败，并返回 EFLOWOUT
	*********************************************
	*/
	void SetMaxWaitTask( INT32 iMaxVal = -1 );

	/*
	*********************************************
	Function : Task
	DateTime : 
	Description :  添加任务
	Input :  pTaskData 任务数据
	Input :  bFirst 释放加到已经存在的任务前， TRUE 加到开头， 优先执行， FALSE 加到结尾
	Output : 
	Return : 成功返回 0， 其他表示错误
	EFLOWOUT 表 超过设定的最大值
	ENMEMORY    表示 获取内存失败			
	Note :   
	*********************************************
	*/
	INT Task( void *pTaskData, BOOL bFirst = FALSE );

	/*
	*********************************************
	Function : GetWaitTask
	DateTime : 
	Description :  获取当前等待调度的任务数据
	Input :  
	Output : 
	Return : 返回数据值
	Note :   
	*********************************************
	*/
	UINT32 GetWaitTask(void);

	/*
	*********************************************
	Function : Clear
	DateTime : 
	Description :  清除所有等待调度的任务
	Input :  
	Output : 
	Return : 
	Note :   释放的数据将调用设定的回调释放， 参考 SetFreedTaskDataFunction
	*********************************************
	*/
	void Clear(void);

	/*
	*********************************************
	Function : RemoveTask
	DateTime : 
	Description :  移除任务
	Input :  pTaskData 被移除的数据
	Output : 
	Return : 返回总共移除的个数， 出错返回-1
	Note :   
	*********************************************
	*/
	INT RemoveTask(void *pTaskData);

	/*
	*********************************************
	Function : IsSelfThread
	DateTime : 
	Description :  判断释放为改线程池的线程 
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
	Description :  存储用户数据
	Input :  pUserData 用户数据, 默认值为 NULL
	Output : 
	Return : 
	Note :  用户要自己释放改数据， 注意不要内存泄露 
	*********************************************
	*/
	void SetUserData( void *pUserData );

	/*
	*********************************************
	Function : GetUserData
	DateTime : 
	Description :  获取存储的用户数据， 参考SetUserData
	Input :  
	Output : 
	Return : 返回 SetUserData 的设定值
	Note :   
	*********************************************
	*/
	void *GetUserData(void);

	/*
	 *********************************************
	 Function : Disable
	 DateTime : 
	 Description :  禁止使用
	 Input :  bJobsComplete 是否等待执行完等待的任务 参考 Uninit
	 Output : 
	 Return : 
	 Note :   将清除已经存在的任务
	 *********************************************
	 */
	void Disable( BOOL bJobsComplete = FALSE );

	/*
	 *********************************************
	 Function : Enable
	 DateTime : 
	 Description :  起用, Disable 的反操作
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
	Description :  返回当前是否使能
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
	Description :  初始化线程池模块, 只有调用改接口后才能使用线程池功能
	Input :  
	Output : 
	Return : 
	Note :   如果要使用线程模块， 必须调用该函数
	*********************************************
	*/
	static void InitModule(void);

	/*
	*********************************************
	Function : UninitModule
	DateTime : 
	Description :  释放线程池功能模块， 和InitModule配对使用
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
	 Description :  线程管理者的任务调度入口
	 Input :   pThread 执行本对象
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	BOOL Run(CDLPTaskThread *pThread );


	
	static INT32      s_iGIdleMSecs; //Idle 的时间间隔 毫秒， 当前无用
	static DLPAtomicInter m_iInitRefs; //引用计数器


	
	/*
	********************************************************************
	类注释
	类名    :    CCoreData
	作者    :    
	创建时间:    
	类描述  :		内部数据管理类
	*********************************************************************
	*/
	
	class CCoreData
	{
	public :
		CDLPThreadPool *m_pPool;
		CDLPThreadGlobal *m_pThreadGlobal;  //线程管理者
		DLPThreadPoolCallback m_fnCallback; //线程回调
		void (*m_fnFreeFunc)(void *);  //任务数据对象释放函数
		void *m_pUserData;    //用户数据
		INT32 m_iIdleInterval; //本对象IDLE 时间， 毫秒

		INT32 m_iMaxJobs;			//最大允许等待任务队列
		UINT32 m_iRefs;
		UINT32 m_iWaitTask;			//等待执行的任务数据
		UINT32 m_iCaches;           //本地缓冲个数

		UINT16 m_iPermitThreads;    //最大允许线程数  
		UINT16 m_iNumThreads;		//当前使用的线程数
		UINT16 m_iStatus;			//状态
		UINT16 m_bExclusive;		//是否独占
		UINT16 m_bWaiting;			//释放等待执行完成
		UINT16 m_iIdleThreads;



		

		StruDLPListNode m_stTaskQueue; //任务队列
		StruDLPListNode *m_pTaskCache; //链表结构内存池队列

		CDLPMutex m_csMutex;        //同步对象
		CDLPCondEx m_csCond;        //同步信号

		CDLPCondEx  m_csExitCond;			//等待退出

		
		/*
		 *********************************************
		 Function : CCoreData
		 DateTime : 
		 Description :  
		 Input :  pPool 属于的线程池
		 Input :  pThreadGlobal  pPool 的管理者
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
		 Description :  减少引用
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
		 Description :  增加引用
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
	     Description :  释放在等待的任务数据
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
		 Description :  移除等待的任务
		 Input :  
		 Output : 
		 Return : 返回移除的个数
		 Note :   
		 *********************************************
		 */
		INT RemoveTask(void *pTaskData);

		/*
		 *********************************************
		 Function : FreeTaskNode
		 DateTime : 
		 Description :  GetTaskNode 释放的返回对象
		 Input :  pNode 被释放对象
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
		 Description :  由内存池分配 GetTaskNode 对象
		 Input :  
		 Output : 
		 Return : NULL表示失败
		 Note :   
		 *********************************************
		 */
		StruDLPListNode *GetTaskNode(void);	

		/*
		 *********************************************
		 Function : AddTask
		 DateTime : 
		 Description :  添加任务
		 Input :  pData 任务对象
		 Input : bFirst 释放有效
		 Output : 
		 Return : NULL 表示失败
		 Note :   
		 *********************************************
		 */
		StruDLPListNode * AddTask( void *pData, BOOL bFirst);

	};

	CCoreData *m_pCore; //核心数据

};


/*
*********************************************************************
*
*@brief : CObjThreadPool , 对象线程池
*
*********************************************************************
*/

class CObjThreadPool;


/*
 *********************************************
 Function : FuncPtrObjThreadPoolEvent
 DateTime : 
 Description :  对象线程池回调函数
 Input :  pcsPool 对应的线程池
 Input : pTaskData 任务数据
 Output : 
 Return : 
 Note :   
 *********************************************
 */
typedef void (CDLPObject::*FuncPtrObjThreadPoolEvent)(CObjThreadPool *pcsPool, void *pTaskData);






/*
********************************************************************
类注释
类名    :    CObjThreadPool
作者    :    
创建时间:    
类描述  :		对象线程池
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
	 Input :  czName 线程池名称
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
	 Input :  pThreadGlobal 指定线程管理者
	 Input :  czName 线程池名称
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
	 Description :  初始化线程池
	 Input :  pTaskFnOwner 任务回调函数所属的对象
	 Input :  pFnTaskCallback 任务回调函数对象回调函数
	 Input : pcsPool 返回所属的线程池对象
	 Input : TaskData 任务数据库
	 Output : 
	 Return : TRUE/FALSE
	 Note :   
	 *********************************************
	 */
	BOOL Init( CDLPObject *pTaskFnOwner, FuncPtrObjThreadPoolEvent pFnTaskCallback,  
		UINT16 iThreadCounts,  BOOL bExclusive);
private :
	CDLPObject *m_pTaskFnOwner; //任务回调函数所属的对象
	FuncPtrObjThreadPoolEvent m_pFnTaskCallback; //任务回调函数对象回调函数
	CDLPString m_strName; //名称

	/*
	 *********************************************
	 Function : ProxyEntry
	 DateTime : 
	 Description :  CDLPThreadPool 的函数入口, 参考DLPThreadPoolCallback说明
				通过本还是转跳到对象函数
	 Input :  
	 Output : 
	 Return : 
	 Note :   
	 *********************************************
	 */
	static void ProxyEntry( CDLPThreadPool *pcsPool, void *TaskData, void *pDebugInfo);

};

#endif //end _DLP_H_DLPTHREADPOOL_H_
