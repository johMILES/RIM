#ifndef BDCOMMANAGER_H
#define BDCOMMANAGER_H

#if defined(RBDCOM_LIBRARY)
#  define API_BD2COM __declspec(dllexport)
#else
#  define API_BD2COM __declspec(dllimport)
#endif

#include <windows.h>
#include <time.h>

extern "C"
{

#pragma pack(push,4)

/*!
 * @brief 北斗串口模块初始化
 *        打开串口，加载协议
 * @param Dev  模块描述文件绝对路径
 * @return 初始化错误码
 */
#define BD_ERROR_INIT_SUCCESS   0   //串口打开成功
#define BD_ERROR_INIT_PORT      -1  //串口打开指定端口失败
#define BD_ERROR_INIT_DEV       -2  //配置文件错误
#define BD_ERROR_INIT_OPENED    -3  //端口已被打开

API_BD2COM int __stdcall BDCom_Init(char *Dev);


/*!
 * @brief 释放资源
 * @return 错误码
 */
#define BD_ERROR_FREE_NoError                   0
#define BD_ERROR_FREE_DeviceNotFoundError       1
#define BD_ERROR_FREE_PermissionError           2
#define BD_ERROR_FREE_OpenError                 3
#define BD_ERROR_FREE_ParityError               4
#define BD_ERROR_FREE_FramingError              5
#define BD_ERROR_FREE_BreakConditionError       6
#define BD_ERROR_FREE_WriteError                7
#define BD_ERROR_FREE_ReadError                 8
#define BD_ERROR_FREE_ResourceError             9
#define BD_ERROR_FREE_UnsupportedOperationError 10
#define BD_ERROR_FREE_UnknownError              11
#define BD_ERROR_FREE_TimeoutError              12
#define BD_ERROR_FREE_NotOpenError              13

API_BD2COM int __stdcall BDCom_Free();



/*!
 * @brief 获取动态库的描述信息
 * @return 描述信息字符串
 */
API_BD2COM char* __stdcall BDCom_LibName();



/*!
 * @brief 发送命令
 * @param CmdType 命令类型
 * @param data 数据内容
 * @param size 数据大小
 * @return 错误码
 */
API_BD2COM int __stdcall BDCom_SendCmd(int CmdType,char*data,int size);

/*!
 * @brief 绑定回调函数
 * @param 回调函数
 */
typedef void (__stdcall * BDCom_DATA_CALLBACK) (int CmdType,char*data,int size);
API_BD2COM bool __stdcall BDCom_BindRecv(BDCom_DATA_CALLBACK DataCallBack);



/*!
 * @brief 切换北斗接口
 * @return 接口代码
 * @note 1,2,3,4,...
 */
API_BD2COM int __stdcall BDCom_SwitchBDInter(int interNum);
}

#endif // BDCOMMANAGER_H
