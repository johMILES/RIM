#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>

#include "abstractchatwidget.h"

class Document:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY sendText)

public:
    explicit Document(QObject *parent = nullptr):QObject(parent){}

    void setSendTextText(const QString &text);
    void setUi(AbstractChatWidget *ui);

public slots:
    void receiveText(const QString &r_path);

signals:
    void sendText(const QString &text);

private:
    void displayMessage(const QString &r_path);
    QString s_text;
    QString recieve_text;
    AbstractChatWidget *chatWidget;
};

#endif//DOCUMENT_H
