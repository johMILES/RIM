﻿/*!
 *  @brief     JSON Key值定义
 *  @details   定义客-服传输结果中，json数据中key字段的内容。
 *             使用了QT5的QMetaEnum对结构体进行解析，获取对应的字符信息。
 *  @file      jsonkey.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.18
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef JSONKEY_H
#define JSONKEY_H

#include <QObject>

class JsonKey : public QObject
{
    Q_OBJECT
public:
    explicit JsonKey(QObject *parent = 0);

    static QString key(int value);

    enum KeyId
    {
        Uuid,
        Type,
        RType,
        Command,
        SubCmd,
        Data,
        NickName,
        Pass,
        LoginType,
        AccountId,
        SignName,
        Sexual,
        Birth,
        Address,
        Email,
        Phone,
        Remark,
        Status,
        SearchContent,
        OperateType,
        AddType,
        OperateInfo,
        Result,
        OperateId,
        GroupId,
        OldGroupId,
        GroupName,
        IsDefault,
        Users,
        Groups,
        OtherSideId,
        Time,
        FromId,
        TextId,
        FileName,
        Size,
        LocalFileName,
        MD5,
        Control,
        Id,
        SystemIcon,
        Index,
        IconId,
        ChatUuid,
        ChatRoomId,
        ChatId,
        NotifyLevel,
        Label,
        Desc,
        SearchVisible,
        ValidateAble,
        Question,
        Answer,
        ChatName,
        Encryption,                          /*!< 是否加密 */
        Compress                             /*!< 是否压缩 */
    };
    Q_FLAG(KeyId)

};

#endif // JSONKEY_H
