#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

namespace MessageType
{
/*!
 *  @brief  系统通知消息的类型
 *  @details 程序内部提供了系统级别的通知-订阅模式，此数据类型用于区分广播的类型。订阅者一句类型进行对应的处理。
 */
enum MessageType
{
    MESS_BASEINFO_READY,                 /*!< 基本信息可读 */
    MESS_DATABASEINFO_READY              /*!< 数据库信息可读 */
};

}

#endif // MESSAGETYPE_H
