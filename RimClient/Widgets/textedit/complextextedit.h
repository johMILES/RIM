﻿/*!
 *  @brief     聊天对话窗口
 *  @details   显示双方的聊天内容，支持显示头像、图片、文字样式、历史记录等
 *  @file      complextextedit.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.22
 *  @warning
 *  @copyright GNU Public License.
 */
#ifndef COMPLEXTEXTEDIT_H
#define COMPLEXTEXTEDIT_H

#include "basetextedit.h"

class ComplexTextEdit : public BaseTextEdit
{
public:
    ComplexTextEdit(QWidget * parent = 0);
    ~ComplexTextEdit();

    void setChatFormat(const QTextCharFormat &format, TextUnit::BaseTextEditType type);
    void setChatFont(const QFont &font, TextUnit::BaseTextEditType type);

    void insertFriendChatText(const TextUnit::InfoUnit record);
    void insertMeChatText(const TextUnit::InfoUnit record);
    void insertTipChatText(const QString tip);

protected:
    void updateChatShow();

private:
    TextUnit::ShowType m_ShowType;
    QTextCharFormat m_Type_Default_Format;
    QTextCharFormat m_Type_UserHead_Friend_Format;
    QTextCharFormat m_Type_UserHead_Me_Format;
    QTextCharFormat m_Type_ChatDetail_Format;
    QTextCharFormat m_Type_RecordTime_Format;
    QTextCharFormat m_Type_Tip_Format;
};

#endif // COMPLEXTEXTEDIT_H
