#pragma once

enum SERVERTYPE 
{
	SERVER_PLANT = 0x00,	// 平台服务器
	SERVER_LOG   ,			// 日志服务器
	SERVER_UPDATE,			// 升级服务器
};

//----- 消息类型-----

// 心跳
#define PROTOCOL_BEEP				0x00000000

/************************************************************************/
/* 终端端接入                                                  */
/************************************************************************/
#define PROTOCOL_CLIENT_JION		0x00000001

#define CMD_CLIENT_JION				0x00000001		// 客户端接入请求
#define CMD_CLIENT_JION_RES			0xFF000001		// 客户端接入请求应答

/************************************************************************/
/* 服务接入/服务同步													*/
/************************************************************************/
#define PROTOCOL_SERVICE_JION		0x00000002

#define CMD_SERVICE_JION			0x00000001		// 服务接入请求
#define CMD_SERVICE_JION_RES		0xFF000001		// 服务接入请求应答
#define CMD_SERVICE_LOGOUT			0x00000002		// 服务登出
#define CMD_SERVICE_LOGOUT_RES		0xFF000002		// 服务登陆应答

/************************************************************************/
/* 用户协议															*/
/************************************************************************/
#define PROTOCOL_CLIENT_USER		0x00000010

#define CMD_CLIENT_LOGIN			0x00000001		// 客户端登陆请求
#define CMD_CLIENT_LOGIN_RES		0xFF000001		// 客户端登陆请求应答
#define CMD_CLIENT_LOGOUT			0x00000002		// 客户端退出请求
#define CMD_CLIENT_LOGOUT_RES		0xFF000002		// 客户端退出请求应答
#define CMD_USER_INFOR				0x00000003		// 获取用户信息请求
#define CMD_USER_INFOR_RES			0xFF000003		// 获取用户信息请求应答
#define CMD_USER_GROUP_CHANGE		0x00000004		// 切换用户组
#define CMD_USER_GROUP_CHANGE_RES	0x00000004		// 切换用户组应答
#define CMD_USER_INFOR_CHANGE		0x00000005		// 用户信息更新
#define CMD_USER_INFOR_CHANGER_RES  0xFF000005		// 用户信息更新应答
#define CMD_USER_INFOR_SYN			0x00000006		// 用户信息同步到服务器
#define CMD_USER_INFOR_SYN_RES		0xFF000006		// 用户信息同步到服务器更新应答

/************************************************************************/
/* 策略协议															*/
/************************************************************************/
#define PROTOCOL_POLICY				0x0000011

#define CMD_GET_POLICY				0x0000001		// 获取策略请求
#define CMD_GET_POLICY_RES			0xFF00001		// 获取策略请求应答
#define CMD_CHANGE_POLICY			0x0000002		// 策略更新
#define CMD_CHANGE_POLICY_RES		0xFF00002		// 策略更新应答

/************************************************************************/
/* 终端权限协议														*/
/************************************************************************/
#define PROTOCOL_AUTHORITY			0x0000012

#define CMD_GET_AUTHORITY			0x0000001		// 获取终端用户权限
#define CMD_GET_AUTHORITY_RES		0xFF00001		// 获取终端用户权限应答
#define CMD_CHANGE_AUTHORITY		0x0000002		// 权限更新
#define CMD_CHANGE_AUTHORITY_RES	0xFF00002		// 更新应答

/************************************************************************/
/* 客户端远程命令协议													*/
/************************************************************************/
#define PROTOCOL_CLIENT_REMOTE_CMD	0x0000013

#define CMD_UNINSTALL				0x0000001		// 远程卸载
#define CMD_UNINSTALL_RES			0xFF00001		// 终端卸载应答
#define CMD_GET_CLIENT_INFOR		0x0000002		// 获取终端信息
#define CMD_CLIENT_INFOR			0x0000003		// 终端信息
#define CMD_UPDATE					0x0000004		// 终端升级
#define CMD_UPDATE_RES				0xFF00004		// 终端升级应答

/************************************************************************/
/* 日志协议															*/
/************************************************************************/
#define PROTOCOL_LOG				0x0000014

#define CMD_CLINET_SYS				0x0000001		// 终端产品日志
#define CMD_CLINET_USER				0xFF00001		// 终端用户日志
#define CMD_SERVER_SYS				0x0000002		// 服务器系统日志

/************************************************************************/
/* 密钥协议															*/
/************************************************************************/
#define PROTOCOL_ENCRYPT_KEY		0x0000021

#define CMD_GET_ENCRYPT_KEY			0x0000001		// 获取加密密钥请求
#define CMD_GET_ENCRYPT_KEY_RES		0xFF00001		// 获取加密密钥请求应答
#define CMD_ENCRYPT_KEY_CHANGER     0x0000002		// 密钥更新
#define CMD_ENCRYPT_KEY_CHANGER_RES 0xFF00002		// 密钥更新应答



