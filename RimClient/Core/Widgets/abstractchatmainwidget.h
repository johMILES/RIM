﻿/*!
 *  @brief     聊天信息记录抽象窗口
 *  @details   抽象了个人、群聊窗口中聊天信息内容显示窗口特点
 *  @file      abstractchatmainwidget.h
 *  @author    LYS
 *  @version   1.0
 *  @date      2018.05.10
 *  @warning   嵌入html显示聊天信息。
 *  @copyright NanJing RenGu.
 *  @note
 *      20180511:LYS:抽象化聊天窗口未完成
 *      20180823:LYS:历史记录窗口关闭后收回对应内存
 */
#ifndef ABSTRACTCHATMAINWIDGET_H
#define ABSTRACTCHATMAINWIDGET_H

#include <QWidget>

#include "observer.h"
#include "../protocol/datastruct.h"
#include "../protocol/protocoldata.h"

struct ChatInfoUnit;
class RToolButton;
class QWebChannel;
class TransferFileItem;
using namespace ProtocolType;

class AbstractChatMainWidgetPrivate;

class AbstractChatMainWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AbstractChatMainWidget)
public:
    enum MsgTarget
    {
        SEND,   //发出信息
        RECV    //收到信息
    };
    enum MsgReadState
    {
        READYREAD = 0x00,     /*!< 信息已读 == 阅办 */
        WATISEND,      /*!< 信息等待发送  */
        SENDING,       /*!< 信息正在发送中  */
        SENDERROR,     /*!< 信息发送失败 */
        SENDED,        /*!< 信息已发送 */
        REPLY,         /*!< 信息已回执 */
        READYDEAL,      /*!< 信息已处理（业务相关）*/
        RECEVIED ,       /*! <信息已接受 */
    };
    enum TimeFormat
    {
        TIME,       //时间
        DATETIME    //日期+时间
    };
    enum ChatType
    {
        C2C,        //单聊模式
        GROUP       //群聊模式
    };
    enum RightTabType
    {
        MsgRecord,  //消息记录
        SendFile,   //发送文件
        RecvFile    //接收文件
    };
    explicit AbstractChatMainWidget(QWidget *parent = 0);
    ~AbstractChatMainWidget();
    void onMessage(MessageType type);
    void setUserInfo(const SimpleUserInfo &info);
#ifdef __LOCAL_CONTACT__
    void setOuterNetConfig(const ParameterSettings::OuterNetConfig &config);
#endif
    void inserHtml(QUrl &htmlUrl);
    void setChatType(ChatType type);
    void playVoiceMessage(QString audioName);
    void updateMsgRecord();
    void setNextDateTime(QDateTime);
    void setChatChannel(QWebChannel *channel);
    void openTargetFile(QString filePath);
    void openTargetFolder(QString filePath);
    void setMsgState(QString serialNo);
    void setScrollFlag();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void shakeWindow();
    void closeWindow();
    void initFinished();

public slots:

private slots:
    void finishLoadHTML(bool);
    void sendShakeWindow(bool);
    void screenShotReady(bool);
    void enterSend();
    void dealDropFile(QString);
    void sendMsg(bool flag=true);
    void sendMsg(QString str);
    void sendImg();
    void setSideVisible(bool flag=false);

    /**********字体处理**********/
    void respFontArea(bool);
    void setMsgShowMode(int);
    void setInputAreaFont(const QFont &font);
    void setInputAreaColor(const QColor &color);

    /**********录音处理**********/
    void respShowAudioArea(bool);
    void prepareSendAudio();
    void preapreCancelAudio();

    void showQueryRecord(const ChatInfoUnit &);
    void showMoreQueryRecord(const ChatInfoUnit & ,bool hasNext=false);
    void recvTextChatMsg(const TextRequest &msg);
    void recvVoiceChatMsg(const QString &msg);
    void respCloseRightSideTab(int);
    void respHistoryRecord(bool flag);

    void slot_RecvRUDpData(QByteArray data);
    void sendTargetFiles(bool);
    void showQuickOrderWidget(bool);
    void sendQuickOrde();
    void openQuickOrdePanel();
    void updateTransFileTab();
    void cancelTransfer(QString);
    void updateTransFileStatus(FileTransProgress);
    void updateMsgStatus(ushort serialNo);

    void showAllHistoryRecord(const ChatInfoUnitList &);

private:
    AbstractChatMainWidgetPrivate * d_ptr;
    void setFontIconFilePath();
    void closeRightSideTab(RightTabType);
    void showRightSideTab(RightTabType);
    TransferFileItem *appendTransferFile(QString &, TransType transType);
    void appendMsgRecord(const TextRequest &recvMsg,MsgTarget source = RECV);
    void appendMsgRecord(const ChatInfoUnit &unitMsg, MsgTarget source = RECV);
    void prependMsgRecord(const TextRequest &recvMsg,MsgTarget source = RECV);
    void prependMsgRecord(const ChatInfoUnit &unitMsg, MsgTarget source = RECV);
    void appendVoiceMsg(QString recordFileName,MsgTarget source = RECV);
    void prependVoiceMsg(QString recordFileName,MsgTarget source = RECV);
    void appendImageMsg(const ChatInfoUnit &unitMsg, MsgTarget source = RECV);
    void prependImageMsg(const ChatInfoUnit &unitMsg, MsgTarget source = RECV);
    void appendChatNotice(QString content,NoticeType type = NONOTICE);
    void prependChatNotice(QString content,NoticeType type = NONOTICE);
    void appendChatTimeNote(QDateTime content,TimeFormat format = TIME);
    void prependChatTimeNote(QDateTime content,TimeFormat format = TIME);
    void sendMsgState2Server(QString otherId, QString state, QString serialNo);
    void updateRecvMsgState(int state, QString serialNo);
};

#endif // ABSTRACTCHATMAINWIDGET_H
