//=============================================================================
/**
*  @file: PlantManagerTask.h
*
*  $Id:   PlantManagerTask.h 2014-8-15
*  
*  ƽ̨�����߳�
*  1. ������ʱ�����û�״̬
*  2. ͬ����Ȩ����ʱ��ͳ��
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
		// �̺߳���
		virtual int svc();

	private:
		// �����û�״̬
		void UpdateUserState();
		// ����ϵͳʹ��ʱ��
		void UpdateSystemUseTime();
		// �ر�����
		void CloseNet();
		// �Ƚ�ʱ�� 0:��� 1���� -1:С��
		int CompareTime(const string & ft,const string & st);

	private:
		//static UKeyEpass _uKey;
	};

	typedef ACE_Singleton<PlantManagerTask,ACE_Thread_Mutex> Sgl_PlantManagerTask;

}