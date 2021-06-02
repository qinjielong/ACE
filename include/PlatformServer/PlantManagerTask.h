//=============================================================================
/**
*  @file: PlantManagerTask.h
*
*  $Id:   PlantManagerTask.h 2014-8-15
*  
*  平台管理线程
*  1. 更新临时表中用户状态
*  2. 同步授权表中时间统计
*
*  @author Don kvin geng
*/
//=============================================================================

#pragma once

#include <Ace/Task.h>
#include <Ace/Singleton.h>
#include <string>
//#include "UKey/UKeyEpass.h"

using namespace std;

namespace DataLock{

	class PlantManagerTask : public ACE_Task<ACE_MT_SYNCH>
	{
	public:
		PlantManagerTask(void);
		virtual ~PlantManagerTask(void);

	public:
		int open();
		void close();

	protected:
		// 线程函数
		virtual int svc();

	private:
		// 更新用户状态
		void UpdateUserState();
		// 更新系统使用时间
		void UpdateSystemUseTime();
		// 关闭网络
		void CloseNet();
		// 比较时间 0:相等 1大于 -1:小于
		int CompareTime(const string & ft,const string & st);

	private:
		//static UKeyEpass _uKey;
	};

	typedef ACE_Singleton<PlantManagerTask,ACE_Thread_Mutex> Sgl_PlantManagerTask;

}