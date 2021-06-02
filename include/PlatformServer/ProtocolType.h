#pragma once

enum SERVERTYPE 
{
	SERVER_PLANT = 0x00,	// ƽ̨������
	SERVER_LOG   ,			// ��־������
	SERVER_UPDATE,			// ����������
};

//----- ��Ϣ����-----

// ����
#define PROTOCOL_BEEP				0x00000000

/************************************************************************/
/* �ն˶˽���                                                  */
/************************************************************************/
#define PROTOCOL_CLIENT_JION		0x00000001

#define CMD_CLIENT_JION				0x00000001		// �ͻ��˽�������
#define CMD_CLIENT_JION_RES			0xFF000001		// �ͻ��˽�������Ӧ��

/************************************************************************/
/* �������/����ͬ��													*/
/************************************************************************/
#define PROTOCOL_SERVICE_JION		0x00000002

#define CMD_SERVICE_JION			0x00000001		// �����������
#define CMD_SERVICE_JION_RES		0xFF000001		// �����������Ӧ��
#define CMD_SERVICE_LOGOUT			0x00000002		// ����ǳ�
#define CMD_SERVICE_LOGOUT_RES		0xFF000002		// �����½Ӧ��

/************************************************************************/
/* �û�Э��															*/
/************************************************************************/
#define PROTOCOL_CLIENT_USER		0x00000010

#define CMD_CLIENT_LOGIN			0x00000001		// �ͻ��˵�½����
#define CMD_CLIENT_LOGIN_RES		0xFF000001		// �ͻ��˵�½����Ӧ��
#define CMD_CLIENT_LOGOUT			0x00000002		// �ͻ����˳�����
#define CMD_CLIENT_LOGOUT_RES		0xFF000002		// �ͻ����˳�����Ӧ��
#define CMD_USER_INFOR				0x00000003		// ��ȡ�û���Ϣ����
#define CMD_USER_INFOR_RES			0xFF000003		// ��ȡ�û���Ϣ����Ӧ��
#define CMD_USER_GROUP_CHANGE		0x00000004		// �л��û���
#define CMD_USER_GROUP_CHANGE_RES	0x00000004		// �л��û���Ӧ��
#define CMD_USER_INFOR_CHANGE		0x00000005		// �û���Ϣ����
#define CMD_USER_INFOR_CHANGER_RES  0xFF000005		// �û���Ϣ����Ӧ��
#define CMD_USER_INFOR_SYN			0x00000006		// �û���Ϣͬ����������
#define CMD_USER_INFOR_SYN_RES		0xFF000006		// �û���Ϣͬ��������������Ӧ��

/************************************************************************/
/* ����Э��															*/
/************************************************************************/
#define PROTOCOL_POLICY				0x0000011

#define CMD_GET_POLICY				0x0000001		// ��ȡ��������
#define CMD_GET_POLICY_RES			0xFF00001		// ��ȡ��������Ӧ��
#define CMD_CHANGE_POLICY			0x0000002		// ���Ը���
#define CMD_CHANGE_POLICY_RES		0xFF00002		// ���Ը���Ӧ��

/************************************************************************/
/* �ն�Ȩ��Э��														*/
/************************************************************************/
#define PROTOCOL_AUTHORITY			0x0000012

#define CMD_GET_AUTHORITY			0x0000001		// ��ȡ�ն��û�Ȩ��
#define CMD_GET_AUTHORITY_RES		0xFF00001		// ��ȡ�ն��û�Ȩ��Ӧ��
#define CMD_CHANGE_AUTHORITY		0x0000002		// Ȩ�޸���
#define CMD_CHANGE_AUTHORITY_RES	0xFF00002		// ����Ӧ��

/************************************************************************/
/* �ͻ���Զ������Э��													*/
/************************************************************************/
#define PROTOCOL_CLIENT_REMOTE_CMD	0x0000013

#define CMD_UNINSTALL				0x0000001		// Զ��ж��
#define CMD_UNINSTALL_RES			0xFF00001		// �ն�ж��Ӧ��
#define CMD_GET_CLIENT_INFOR		0x0000002		// ��ȡ�ն���Ϣ
#define CMD_CLIENT_INFOR			0x0000003		// �ն���Ϣ
#define CMD_UPDATE					0x0000004		// �ն�����
#define CMD_UPDATE_RES				0xFF00004		// �ն�����Ӧ��

/************************************************************************/
/* ��־Э��															*/
/************************************************************************/
#define PROTOCOL_LOG				0x0000014

#define CMD_CLINET_SYS				0x0000001		// �ն˲�Ʒ��־
#define CMD_CLINET_USER				0xFF00001		// �ն��û���־
#define CMD_SERVER_SYS				0x0000002		// ������ϵͳ��־

/************************************************************************/
/* ��ԿЭ��															*/
/************************************************************************/
#define PROTOCOL_ENCRYPT_KEY		0x0000021

#define CMD_GET_ENCRYPT_KEY			0x0000001		// ��ȡ������Կ����
#define CMD_GET_ENCRYPT_KEY_RES		0xFF00001		// ��ȡ������Կ����Ӧ��
#define CMD_ENCRYPT_KEY_CHANGER     0x0000002		// ��Կ����
#define CMD_ENCRYPT_KEY_CHANGER_RES 0xFF00002		// ��Կ����Ӧ��



