﻿/*!
 *  @brief     应用层协议文件
 *  @details   【1】定义客户端与服务器端通信请求/回复结构体，请求以XXRequest定义，回复以XXResponse定义，
 *             【2】客户端和服务器端的协议文件保持一致!!
 *  @file      protocoldata.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      201810131:wey:添加文本信息发送
 */
#ifndef PROTOCOLDATA_H
#define PROTOCOLDATA_H

#include <QString>
#include <QList>

/*!
 * @brief 应用层数据传输结构
 * @details  数据格式传输分为请求和响应
 *
 * @p Request请求内容
 * @code
 * {
 *    'type'    :   MsgType,
 *    'cmd'     :   MsgCommand,
 *    'data'    :
 *                   {
 *                      'key':'value'
 *                   }
 * }
 * @endcode
 *
 * @p Response响应信息内容
 * @code
 * {
 *     'type'   :   MsgType,
 *     'cmd'    :   MsgCommand,
 *     'status' :   value,                          //若是操作性质,返回操作成功与否标志,部分不会返回data段
 *     'data'   :
 *                  {
 *
 *                      'key'   :  value
 *                  }
 * }
 * @endcode
 */
namespace ProtocolType {

/*!
 *  @brief 客户端类型
 *  @details (可用于以后扩展)
 */
enum ClientType
{
    CLIENT_TYPE_UNKONWN,      /*!< 类型不明 */
    CLIENT_TYPE_PC,           /*!< 客户端之间通信 */
};

/*!
 *  @brief  消息类型
 *  @details 应用层消息的类型，将不同的操作划分为四大类，不同的消息类型，对应的格式存在着一定的区别。
 */
enum MsgType
{
    MSG_TYPE_INVALID,  /*!< 无意义 */
    MSG_CONTROL,       /*!< 控制命令, */
    MSG_TEXT,          /*!< 文本信息 */
    MSG_FILE           /*!< 文件信息 */
};

/*!
 *  @brief  命令类型
 *  @details 根据不同的MstType，命令的表示范围是不一致的;
 *  \parblock
 *  若 \link MsgType \endlink 为 \link MSG_CONTROL \endlink 时，命令的范围在0x00-0x99. \n
 *  若 \link MsgType \endlink 为 \link MSG_TEXT \endlink 时，命令的范围在0xA0-0xAF. \n
 *  若 \link MsgType \endlink 为 \link MSG_IMAGE \endlink 时，命令的范围在0xB0-0xBF. \n
 *  若 \link MsgType \endlink 为 \link MSG_FILE \endlink 时，命令的范围在0xC0-0xCF. \n
 *  \endparblock
 *  @see MsgType
 */
enum MsgCommand
{
/****MsgType为MSG_CONTROL时以下字段有效******/
    MSG_Command_INVALID,                               /*!< 无意义 */
    MSG_USER_REGISTER = 0x01,                          /*!< 用户注册 */
    MSG_USER_LOGIN,                                    /*!< 用户登录 */
    MSG_USER_LOGOUT,                                   /*!< 用户退出 */
    MSG_USER_VIEW_INFO,                                /*!< 用户基本信息 */
    MSG_USER_UPDATE_INFO,                              /*!< 用户更新信息 */
    MSG_USER_STATE,                                    /*!< 用户状态 */

    MSG_RELATION_SEARCH = 0x11,                        /*!< 查找好友 */
    MSG_REALTION_ADD,                                  /*!< 添加好友 */
    MSG_RELATION_OPERATE,                              /*!< 好友操作 (参照 @link OperateFriend @endlink )FRIEND_APPLY  */
    MSG_RELATION_GROUPING_FRIEND,                      /*!< 分组联系人操作 */
    MSG_RELATION_VIEW_INFO,                            /*!< 查看好友信息 */
    MSG_RELATION_LIST,                                 /*!< 请好友列表/更新好友列表 */

    MSG_GROUP_SEARCH = 0x31,                           /*!< 查找群 */
    MSG_GROUP_CREATE,                                  /*!< 创建群 */
    MSG_GROUP_JOIN,                                    /*!< 加入群 */
    MSG_GROUP_EXIT,                                    /*!< 退出群 */
    MSG_GROUP_REMOVE,                                  /*!< 移除群 */
    MSG_GROUP_DISSOLUTION,                             /*!< 解散群 */
    MSG_GROUP_VIEW_INFO,                               /*!< 查看群信息 */
    MSG_GROUP_UPDATE_INFO,                             /*!< 更新群信息 */

    MSG_GROUPING_OPERATE = 0x51,                       /*!< 联系人、群分组操作，参见 @link OperateGrouping @endlink  */

    MSG_OTHER_HEAT = 0x41,                             /*!< 心跳报 */

/****MsgType为MSG_TEXT时以下字段有效******/
    MSG_TEXT_TEXT = 0xA1,                              /*!< 聊天信息内容 */
    MSG_TEXT_SHAKE = 0xA2,                             /*!< 窗口抖动 */
    MSG_TEXT_APPLY = 0xA3,                             /*!< 文本确认消息 */

/****MsgType为MSG_FILE时以下字段有效******/
    MSG_FILE_CONTROL = 0xB1,                           /*!< 传输控制命令 */
    MSG_FILE_REQUEST = 0xB2,                           /*!< 传输请求 */
    MSG_FILE_DATA = 0xB3                               /*!< 传输正文 */
};

/*!
 *  @brief  通用操作结果类型
 *  @details 用于操作只有正确或错误两种类型的结果
 */
enum MsgOperateResponse
{
    STATUS_SUCCESS = 0x00,      /*!< 操作成功 */
    STATUS_FAILE                /*!< 操作失败 */
};

/*!
 *  @brief  联系人性别
 */
enum Sexual
{
    MAN,            /*!< 男性 */
    WOMAN           /*!< 女性 */
};

/*!
 *  @brief  用户在线状态
 *  @details 标明了联系人在线状态，用户设置 @link STATUS_ONLINE @endlink 、 @link STATUS_BUSY @endlink 、 @link STATUS_AWAY @endlink 之后，与其关联的联系人是可以看见其在线状态。
 *          若设置@link STATUS_OFFLINE @endlink 、 @link STATUS_INVISIBLE @endlink 状态，与其关联的联系人无法明确其真实的状态。
 */
enum OnlineStatus
{
    STATUS_OFFLINE = 0,      /*!< 离线状态 */
    STATUS_ONLINE,           /*!< 在线状态 */
    STATUS_HIDE,             /*!< 隐身状态 */
    STATUS_BUSY,             /*!< 忙碌状态 */
    STATUS_AWAY,             /*!< 离开状态 */
    STATUS_NoDisturb         /*!< 勿扰状态 */
};

/*!
 *  @brief 登陆结果
 *  @details 接收用户登陆请求操作结果
 *  @parblock
 *    [1]:A 登陆信息  服务器;\n
 *    [2]:服务器 验证并发回结果信息 A \n
 *    [3]:服务器 查找历史聊天信息、请求信息 A \n
 *  @endparblock
 *
 */
enum ResponseLogin
{
    LOGIN_SUCCESS = 0,              /*!< 登陆成功 */
    LOGIN_FAILED,                   /*!< 登陆失败 */
    LOGIN_UNREGISTERED,             /*!< 用户未注册 */
    LOGIN_PASS_ERROR,               /*!< 密码错误 */
    LOGIN_USER_LOGINED,             /*!< 用户已经登陆 */
    LOGIN_USER_CANCEL_LOGIN,        /*!< 取消登陆 */
    LOGIN_SERVER_REFUSED,           /*!< 服务器拒绝响应 */
    LOGIN_SERVER_NOT_RESP           /*!< 服务器未响应 */
};

/*!
  @brief 注册结果信息
  @parblock
    [1]:A 发送ResponseRegister 服务器;\n
    [2]:服务器 发送账号   A \n
   @endparblock
*/
enum ResponseRegister
{
    REGISTER_SUCCESS = 0,           /*!< 注册成功 */
    REGISTER_FAILED,                /*!< 注册失败 */
    REGISTER_SERVER_REFUSED         /*!< 服务器未响应 */
};

/*!
  @brief 好友操作类型
  @parblock
   A想添加B为好友:\n
   [1]:A 发送MSG_RELATION_SEARCH  服务器;         查询好友信息  \n
   [2]:服务器 发送ResponseAddFriend  A;           查询结果  \n
   [3]:A 发送 MSG_REALTION_ADD 服务器;            好友请求 \n
   [4]:服务器 发送ResponseAddFriend A;            好友请求确认 \n
   [5]:服务器 发送MSG_RELATION_REQUEST B;         好友请求(B在线直接发送；B未在线，缓存请求等B登陆) \n
   [6]:B 发送OperateFriend 服务器;                请求结果 \n
   [7]:服务器 发送MSG_RELATION_REQUEST A;         转发请求结果 \n
  @endparblock
*/
enum OperateFriend
{
    FRIEND_APPLY,                /*!< 好友请求 */
    FRIEND_DELETE,               /*!< 删除好友 */
    FRIEND_MOVE                  /*!< 移动好友 */
};

/*!
 *  @brief  好友操作结果
 *  @details 需要注意的是，此类型是在 @link OperateFriend @endlink 为 @link FRIEND_APPLY @endlink 有意义。
 */
enum ResponseFriendApply
{
    FRIEND_REQUEST,            /*!< 请求 */
    FRIEND_AGREE,              /*!< 同意请求 */
    FRIEND_REFUSE              /*!< 拒绝请求 */
};

/*!
 *  @brief 删除好友结果信息
 */
enum ResponseDeleteFriend
{
    DELETE_SUCCESS,             /*!< 删除成功 */
    DELETE_FAILED               /*!< 删除失败 */
};

/*!
 *  @brief  添加好友结果信息
 *  @details 在用户发送查找请求后，对发送的请求进行反馈。
 */
enum ResponseAddFriend
{
    FIND_FRIEND_NOT_FOUND,        /*!< 未找到好友 */
    FIND_FRIEND_FOUND,            /*!< 找到好友 */
    FIND_FRIEND_FAILED,           /*!< 查找失败 */
    ADD_FRIEND_SENDED,            /*!< 请求发送成功, @note 若对方在线，将请求投递至对方；若对方离线，则缓存请求，待上线后投递。 */
    ADD_FRIEND_SENDED_FAILED      /*!< 请求发送失败 */
};

/*!
 *  @brief  发送/接收信息结果
 */
enum ResponseOpoerateMsg
{
    SEND_MSG_SUCCESS,               /*!< 发送消息成功 */
    SEND_TEXT_MSG_FAILED,           /*!< 发送文本信息失败 */
    SEND_IMAGE_MSG_FAILED,          /*!< 发送图片信息失败 */
    SEND_FILE_MSG_FAILED,           /*!< 发送文件信息失败 */

    RECV_FILE_SUCCESS,              /*!< 接收文件成功 */
    RECV_FILE_FAILED,               /*!< 接收文件失败 */
};

/*!
    @brief 更新用户信息结果
    @parblock
    举例：小明想更新信息 \n
    [1]:A 修改基本信息 服务器 \n
    [2]:服务器 修改结果  A \n
    @endparblock
*/
enum ResponseUpdateUser
{
    UPDATE_USER_SUCCESS,      /*!< 信息更新成功 */
    UPDATE_USER_FAILED        /*!< 信息更新失败 */
};

/*!
 *  @brief  联系人信息操作
 */
enum OperateContact
{
    UPDATE_USER_DETAIL,        /*!< 更新用户信息 */
    REQUEST_CONTACT_DETAIL      /*!< 请求联系人详细信息 */
};

/*!
 *  @brief 网络协议头
 */
class MsgPacket
{
public:
    MsgPacket();
    virtual ~MsgPacket();

    bool isAutoDelete;          /*!< 是否将数据转换成json后自动删除，默认为true。 */

    MsgType msgType;            /*!< 数据类型 @link MSG_TYPE @endlink */
    MsgCommand msgCommand;      /*!< 命令类型 @link MSG_COMMAND @endlink */
};

/************************登  陆**********************/
/*!
 *  @brief  用户描述信息
 *  @details 定义用户详细信息，用于保存个人信息、好友详细信息等。
 */
struct UserBaseInfo
{
    QString uuid;                  /*!< 数据库数据ID */
    QString accountId;             /*!< 登陆账号 */
    QString nickName;              /*!< 昵称,必填 */
    QString signName;              /*!< 签名,可为空 */
    Sexual  sexual;                /*!< 性别 */
    QString birthday;              /*!< 生日 */
    QString address;               /*!< 地址,可为空 */
    QString email;                 /*!< 邮箱,可为空 */
    QString phoneNumber;           /*!< 电话,可为空 */
    QString remark;                /*!< 备注,可为空 */
    unsigned short face;           /*!< 头像信息(0表示为自定义，大于0表示系统头像) */
    QString customImgId;           /*!< 头像信息(face为0时有效) */
};

/*!
 *  @brief  登陆请求
 */
class LoginRequest : public MsgPacket
{
public:
    LoginRequest();

    QString accountId;              /*!< 登陆账号 */
    QString password;               /*!< 账号密码，采用密码加密传输 */
    OnlineStatus  status;           /*!< 用户选择的在线状态 */
};

/*!
 *  @brief  登陆响应
 */
class LoginResponse : public MsgPacket
{
public:
    LoginResponse();

    UserBaseInfo baseInfo;          /*!< 登陆用户的个人信息，参见 @link UserBaseInfo @endlink */
};

/************************注  册**********************/
/*!
 *  @brief  注册请求
 */
class RegistRequest : public MsgPacket
{
public:
    RegistRequest();

    QString nickName;       /*!< 用户昵称 */
    QString password;       /*!< 账户密码 */
};

/*!
 *  @brief  注册响应
 */
class RegistResponse : public MsgPacket
{
public:
    RegistResponse();

    QString accountId;       /*!< 服务器分配的ID，在登陆时使用此ID作为用户名 */
};

/************************更新基本信息**********************/
/*!
 *  @brief  更新基本信息请求
 */
class UpdateBaseInfoRequest : public MsgPacket
{
public:
    UpdateBaseInfoRequest();
    OperateContact requestType;          /*!< 请求操作类型 */
    UserBaseInfo baseInfo;               /*!< 请求更新的基本信息 */
};

/*!
 *  @brief  基本信息响应
 */
class UpdateBaseInfoResponse : public MsgPacket
{
public:
    UpdateBaseInfoResponse();
    OperateContact reponseType;         /*!< 回复操作类型 */
    UserBaseInfo baseInfo;              /*!< 更新后的基本信息 */
};

/***********************查询好友**********************/
/*!
 *  @brief  简单用户信息
 *  @details 该信息描述了用户的最基础信息，可用于更新用户基本的描述信息、获取好友列表
 */
struct SimpleUserInfo
{
    QString accountId;           /*!< 登陆账号 */
    QString nickName;            /*!< 昵称 */
    QString signName;            /*!< 签名 */
    unsigned short face;         /*!< 头像信息(0表示为自定义，大于0表示系统头像) */
    QString customImgId;         /*!< 头像信息(face为0时有效) */
    QString remarks;             /*!< 备注名称 */
    OnlineStatus status;         /*!< 在线状态 */
};

/*!
 *  @brief 分组操作类型
 *  @details 区分操作分组、联系人、请求的类型
 */
enum SearchType
{
    SearchPerson,       /*!< 联系人操作 */
    SearchGroup         /*!< 分组操作 */
};

/*!
 *  @brief  查找好友请求
 */
class SearchFriendRequest : public MsgPacket
{
public:
    SearchFriendRequest();
    SearchType stype;                  /*!< 查询类型 */
    QString accountOrNickName;         /*!< 对应 @link SearchType @endlink 的AccountId或NickName */
};

/*!
 *  @brief  查找好友响应
 *  @details 返回查找的结果列表
 */
class SearchFriendResponse : public MsgPacket
{
public:
    SearchFriendResponse();

    QList<SimpleUserInfo> result;      /*!< 在status为 @link FIND_FRIEND_FOUND @endlink 字段时，表示查找的结果 */
};

/***********************添加好友**********************/
/*!
 *  @brief  添加好友请求
 *  @details 根据 @link SearchFriendResponse @endlink 中的查询结果列表，用户可选择发送添加好友请求。
 */
class AddFriendRequest : public MsgPacket
{
public:
    AddFriendRequest();
    SearchType stype;               /*!< 添加人或群 */
    QString accountId;              /*!< 用户账号 */
    QString operateId;              /*!< 待添加的用户或群ID  */
};

/*!
 *  @brief  添加好友请求响应
 *  @attention 与 @link OperateFriendResponse @endlink 区分
 */
class AddFriendResponse : public MsgPacket
{
public:
    AddFriendResponse();
};

/***********************联系人操作**********************/
/*!
 *  @brief 好友操作请求
 *  @details 依据 @link SearchType @endlink 类型发送请求、移动、删除等操作。
 */
class OperateFriendRequest : public MsgPacket
{
public:
    OperateFriendRequest();
    OperateFriend type;                 /*!< 操作类型 @link OperateFriend @endlink */
    SearchType stype;                   /*!< 操作人或群  */
    int result;                         /*!< @parblock  当type为 @link FRIEND_APPLY @endlink 时，对应 @link ResponseFriendApply @endlink \n
                                                        当type为 @link FRIEND_DELETE @endlink 时，对应 @link ResponseDeleteFriend @endlink
                                        */
    QString accountId;                  /*!< 用户账号 */
    QString operateId;                  /*!< 待操作的好友ID */
};

/*!
 *  @brief 好友操作响应
 *  @details 依据 @link SearchType @endlink 类型发送请求、移动、删除等操作。
 */
class OperateFriendResponse : public MsgPacket
{
public:
    OperateFriendResponse();
    OperateFriend type;                 /*!< 操作类型 @link OperateFriend @endlink */
    SearchType stype;                   /*!< 操作人或群  */
    int result;                         /*!< @parblock  当type为 @link FRIEND_APPLY @endlink 时，对应 @link ResponseFriendApply @endlink \n
                                                        当type为 @link FRIEND_DELETE @endlink 时，对应 @link ResponseDeleteFriend @endlink
                                        */
    QString accountId;                  /*!< 用户账号 */
    SimpleUserInfo requestInfo;         /*!< 当 type 为 @link FRIEND_APPLY @endlink,用于保存待操作联系人基本信息 */
};

/***********************联系人列表操作**********************/
/*!
 *  @brief 分组信息
 *  @details 描述联系人或者群分组的基本信息
 */
struct RGroupData
{
    ~RGroupData();
    QString groupId;                /*!< 分组ID，可用于判断唯一标识  */
    QString groupName;              /*!< 分组名称  */
    bool isDefault;                 /*!< 是否为默认分组  */
    QList<SimpleUserInfo *> users;    /*!< 分组中联系人或群的描述信息  */
};

/*!
 *  @brief 好友列表请求
 */
class FriendListRequest : public MsgPacket
{
public:
    FriendListRequest();
    QString accountId;              /*!< 用户账号  */
};

/*!
 *  @brief 好友列表结果
 */
class FriendListResponse : public MsgPacket
{
public:
    FriendListResponse();
    ~FriendListResponse();
    QString accountId;              /*!< 用户账号  */
    QList<RGroupData *> groups;     /*!< 分组信息  */
};

/***********************分组联系人操作**********************/
/*!
 *  @brief 分组操作类型
 */
enum OperateGroupingFriend
{
    G_Friend_CREATE,          /*!< 创建分组联系人  */
    G_Friend_UPDATE,          /*!< 更新分组联系人(如备注信息等)  */
    G_Friend_MOVE,            /*!< 移动联系人  */
    G_Friend_Delete           /*!< 删除联系人  */
};

/*!
 *  @brief  分组操作请求
 */
class GroupingFriendRequest : public MsgPacket
{
public:
    GroupingFriendRequest();
    OperateGroupingFriend type;     /*!< 参见 @link OperateGroupingFriend @endlink */
    SearchType stype;               /*!< 联系人分组/群分组  */
    QString groupId;                /*!< 分组ID; @attention 当type为 @link G_Friend_Move @endlink 时，表示移动后分组Id */
    QString oldGroupId;             /*!< 当type为 @link G_Friend_Move @endlink 时，表示移动前分组Id  */
    SimpleUserInfo user;            /*!< 联系人基本信息, @link SimpleUserInfo @endlink  */
};

/*!
 *  @brief  分组操作响应
 */
class GroupingFriendResponse : public MsgPacket
{
public:
    GroupingFriendResponse();
    OperateGroupingFriend type;     /*!< 参见 @link OperateGroupingFriend @endlink */
    SearchType stype;               /*!< 联系人分组/群分组  */
    QString groupId;                /*!< 分组ID; @attention 当type为 @link G_Friend_Move @endlink 时，表示移动后分组Id */
    QString oldGroupId;             /*!< 当type为 @link G_Friend_Move @endlink 时，表示移动前分组Id  */
    SimpleUserInfo user;            /*!< 联系人基本信息, @link SimpleUserInfo @endlink  */
};

/**********************分组操作**********************/
/*!
 *  @brief 分组类型
 */
enum GroupingType
{
    GROUPING_FRIEND,     /*!< 联系人分组 */
    GROUPING_GROUP       /*!< 群分组 */
};

/*!
 *  @brief 对于联系人/群分组操作种类
 */
enum OperateGrouping
{
    GROUPING_CREATE,     /*!< 创建分组 */
    GROUPING_RENAME,     /*!< 重命名分组 */
    GROUPING_DELETE,     /*!< 删除分组 */
    GROUPING_SORT,       /*!< 分组排序 */
    GROUPING_REFRESH     /*!< 刷新分组 */
};

/*!
 *  @brief 分组操作请求
 */
class GroupingRequest : public MsgPacket
{
public:
    GroupingRequest();
    QString uuid;           /*!< 联系人uuid */
    QString groupId;        /*!< 分组ID(为 @link GROUPING_CREATE @endlink 时无需填写) */
    GroupingType gtype;     /*!< 分组类型，参见 @link GroupingType @endlink */
    OperateGrouping type;   /*!< 分组操作类型，参见 @link OperateGrouping @endlink */
    QString groupName;      /*!< 分组名称 */

    //int groupIndex;                       //分组序号
};

/*!
 *  @brief 分组操作响应
 */
class GroupingResponse : public MsgPacket
{
public:
    GroupingResponse();
    QString uuid;             /*!< 联系人uuid */
    GroupingType gtype;       /*!< 分组类型，参见 @link GroupingType @endlink */
    QString groupId;          /*!< 分组ID(为 @link GROUPING_CREATE @endlink 时无需填写) */
    OperateGrouping type;     /*!< 分组操作类型，参见 @link OperateGrouping @endlink */
};

/**********************用户状态变更**********************/
/*!
 *  @brief  用户状态变更
 *  @details 向服务器更新自己的在线状态
 */
class UserStateRequest : public MsgPacket
{
public:
    UserStateRequest();
    QString accountId;              /*!< 用户账号 */
    OnlineStatus onStatus;          /*!< 在线状态 @link OnlineStatus @endlink */
};

/*!
 *  @brief  用户变更响应
 *  @details 1.用于接收本身更新成功与否的状态 @n
 *           2.用于接收朋友状态变更通知 @n
 */
class UserStateResponse : public MsgPacket
{
public:
    UserStateResponse();
    QString accountId;              /*!< 用户账号，若账号等于自身则用于响应自己的操作；若账户为联系人列表中，则表示接收好友状态 */
    OnlineStatus onStatus;          /*!< 在线状态 @link OnlineStatus @endlink */
};

/*********************聊天信息和窗口抖动操作**********************/
/*!
 *  @brief 文本信息类型
 *  @details 消息可分为普通消息、需确认消息、需回执消息等，可进行扩展
 */
enum TextType
{
    TEXT_NORAML,            /*!< 普通消息 */
    TEXT_NEED_CONFIRM,      /*!< 需确认消息，消息到达发送方后弹窗显示，接收者需点击确认按钮才可关闭显示 */
    TEXT_NEED_RECEIPT       /*!< 需发送回执，已读或未读 */
};

/*!
    @brief 文本信息发送
    @details 为了方便存储，聊天的文字和窗口抖动均复用一个请求。默认为聊天信息，
            但可修改msgCommand为窗口抖动
    @note:在ClientA-Server-ClientB交互过程中，为了确保消息的可靠投递，需要使用消息确认机制。 @n
          1.ClientA向Server发送TextRequest,并将消息压入发送消息队列。 @n
          2.Server向ClientA发送TextApply，检查消息队列，将对应的消息从队列中移除。 @n
          3.Server向ClientB发送TextRequest @n
          4.ClientB向Server发送TextApply @n
*/
class TextRequest : public MsgPacket
{
public:
    TextRequest();
    QString textId;             /*!< 消息唯一标识 */
    qint64 timeStamp;           /*!< 发送时间戳 */
    TextType textType;          /*!< 消息类型 @link TextType @endlink */
    bool isEncryption;          /*!< 是否已经加密,暂时只对发送的数据加密 */
    bool isCompress;            /*!< 是否对数据压缩 */
    SearchType type;            /*!< 联系人or群消息 @link SearchType @endlink */
    QString accountId;          /*!< 用户自己ID */
    QString otherSideId;        /*!< 对方账户ID */
    QString sendData;           /*!< 发送数据, @note 可配置是否需要加密以及压缩 */
};

/*!
 *  @brief 消息回执类型
 *  @details 消息发送出去后，用户可能需要根据其状态，如已读、未读、需要确认。
 */
enum TextReplyType
{
    APPLY_SYSTEM,               /*!< 服务器确认接收 */
    APPLY_CONFIRM,              /*!< 确认信息 */
    APPLY_RECEIPT               /*!< 回执消息 */
};

/*!
 *  @brief 消息确认
 */
class TextReply : public MsgPacket
{
public:
    TextReply();
    QString textId;             /*!< 消息唯一标识 */
    TextReplyType applyType;    /*!< 消息回执类型 @link TextApplyType @endlink */
};


/*********************文件操作**********************/
/*!
 *  @brief  文件操作的类型
 *  @details 主要包含聊天过程中图片的收、发，用户头像信息的上传、接收，文件传输的收、发。
 */
enum FileItemType
{
    FILE_ITEM_UNKNOWN,           /*!< 文件类型未知 */
    FILE_ITEM_CHAT_UP,           /*!< 聊天中文件 */
    FILE_ITEM_USER_UP,           /*!< 上传用户头像 */
    FILE_ITEM_CHAT_DOWN,         /*!< 聊天中下载文件 */
    FILE_ITEM_USER_DOWN          /*!< 下载用户头像 */
};

/*!
 *  @brief  文件上传/下载结果
 */
enum FileOperateResult
{
    FILE_DOWN_SUCCESS,                /*!< 文件下载成功 */
    FILE_DOWN_FAILED,                 /*!< 文件下载失败 */
    FILE_DOWN_CANCEL,                 /*!< 取消下载 */
    FILE_Up_SUCCESS,                  /*!< 文件上传成功 */
    FILE_Up_FAILED,                   /*!< 文件上传失败 */
    FILE_Up_CANCEL                    /*!< 取消上传 */
};

/*!
 *  @brief 文件传输控制指令
 */
enum FileTransferControl
{
    T_REQUEST,                 /*!< 传输请求 */
    T_ABLE_SEND,               /*!< 可以发送 */
    T_ABLE_RECEIVE,            /*!< 可以接收 */
    T_SERVER_EXIST,            /*!< 服务器已经存在此文件 */
    T_TRANING,                 /*!< 继续传输 */
    T_PAUSE,                   /*!< 暂停传输 */
    T_CANCEL,                  /*!< 取消传输 */
    T_OVER,                    /*!< 文件传输结束 */
    T_DATA                     /*!< 数据体 */
};

/*!
 * @brief 文件发送描述
 * @details 用于本地建立待发送或接收请求项，由ImageTask或FileTask进行处理 @n
 */
struct FileItemDesc
{
public:
    FileItemDesc(){}
    QString id;                     /*!< 当前文件唯一标识 */
    QString fullPath;               /*!< 文件全路径 */
    size_t fileSize;                /*!< 文件尺寸，用于在接收时开辟对应大小的控件 */
    QString md5;                    /*!< 文件对应的MD5值，用于校验接收数据的正确与否 */
    QString otherSideId;            /*!< 对方用户ID */
    FileItemType itemType;          /*!< 文件操作类型 @line FileItemType @endlink */
};

/*!
 * @brief 文件发送操作
 * @details 客户端向服务器传输文件时，经历以下几步： @n
 *          1.从待发送列表缓冲区中取出一个待发送文件，计算文件的MD5、文件名等基本信息； @n
 *          2.客户端将基本信息发送至服务器，服务器对基本信息进行接收处理准备,并向客户端发送确认接收信息;  @n
 *          2.1 服务器检测当前文件是否已经存在，若存在则直接发送传输结束命令，goto 5；否则goto3; @n
 *          3.客户端接收服务器确认消息后，开始传输当前文件，直至结束; @n
 *          4.服务器在接收完成，并进行MD5校验，将校验的结果传输至客户端; @n
 *          5.若接收成功，则将当前文件移除队列，并进行下一个文件发送准备; @n
 *          6.若接收失败，服务器删除接收的文件信息，客户端则重新发送文件，流程转至1 @n
 * 发送具体流程： @n
 *  1.Client 发送 FileItemRequest 至 Server; @n
 *  2.Server 发送 SimpleItemRequest 至 Client; @n
 *  3.Client 发送 FileDateRequest 至 Server; @n
 *  4.Server 发送 SimpleItemRequest 至 Client; @n
 *
 * 客户端向服务器端请求接收文件时，经历以下几步： @n
 *  1.客户端向服务器发送MD5请求具体的文件信息； @n
 *  2.服务器根据客户端的MD5信息找到对应的文件信息，发送至客户端; @n
 *  3.客户端做接收准备，若同意接收或取消接收，均发送简单控制请求; @n
 *  4.1 若客户端发送同意接收，服务器将文件加入待发送队列，并开始发送数据信息; @n
 *  4.2 若客户端拒绝接收，服务器不做处理; @n
 *  5.若客户端接收完成，则发送接收完成指令，服务器根据此指令将文件从发送队列移除； @n
 * 接收文件流程：@n
 *  1.Client 发送 SimpleFileItemRequest 至 Server; @n
 *  2.Server 发送 FileItemRequest 至 Server; @n
 *  3.Client 发送 SimpleFileItemRequest 至 Server; @n
 *  4.Server 发送 FileDateRequest 至 Client; @n
 *  5.Client 发送 SimpleFileItemRequest 至 Server; @n
 */
class FileItemRequest : public MsgPacket
{
public:
    FileItemRequest();
    FileTransferControl control;    /*!< 传输控制命令 @link FileTransferControl @endlink */
    FileItemType itemType;          /*!< 文件操作类型 @line FileItemType @endlink */
    QString fileName;               /*!< 文件名称 @attention 维护文件真实的信息 */
    size_t size;                    /*!< 文件大小 */
    QString fileId;                 /*!< 文件数据库中唯一标识 */
    QString md5;                    /*!< 文件MD5 @attention 服务器以此文件名作为在磁盘中保存的索引 */
    QString accountId;              /*!< 发送方的ID */
    QString otherId;                /*!< 接收方ID */
};

/*!
 * @brief 文件数据传输请求
 */
class FileDataRequest : public MsgPacket
{
public:
    FileDataRequest();
    FileTransferControl control;    /*!< 传输控制命令 @link FileTransferControl @endlink */
    QString md5;                    /*!< 待传输文件MD5，用于标识判断 */
    size_t index;                   /*!< 数据帧序号索引，默认从0开始，服务器在写文件时根据序号随机读写 */
    QByteArray array;               /*!< 数据正文 */
};

/*!
 *  @brief 简单文件传输控制请求
 *  @details 在简历传输连接后，用于客户端和服务器端传输文件控制命令.
 */
class SimpleFileItemRequest : public MsgPacket
{
public:
    SimpleFileItemRequest();
    FileTransferControl control;    /*!< 传输控制命令 @link FileTransferControl @endlink */
    FileItemType itemType;          /*!< 文件操作类型 @line FileItemType @endlink */
    QString md5;                    /*!< 当前文件唯一标识,以文件的MD5作为索引 @attention 服务器以此文件名作为在磁盘中保存的索引 */
    QString fileId;                 /*!< 文件在数据库中的唯一ID */
};

}

#endif // PROTOCOLDATA_H
