﻿/*!
 *  @brief     快捷键设置界面
 *  @details   设置软件的需要使用的快捷键
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.24
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef SETKEYSEQUENCEDIALOG_H
#define SETKEYSEQUENCEDIALOG_H

#include <QDialog>
#include <QProcess>
class QVBoxLayout;
class QGroupBox;
class QLabel;
class QTableWidget;
class QHBoxLayout;
class QSpacerItem;
class QRadioButton;
class QMenuBar;
class QToolBar;
class QStatusBar;
class QTimer;

class SetKeySequenceDialog : public QDialog
{
    Q_OBJECT
public:
    SetKeySequenceDialog(QWidget *parent);
    ~SetKeySequenceDialog();

private:
    void initWidget();
    void loadSetting();
    void appendTableWidgetItem(QTableWidget *table,int nums=1);

private slots:
    void slot_RadioButton_Enter(bool);
    void slot_RadioButton_CtrlEnter(bool);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QTableWidget *tblw_KeyList;
    QHBoxLayout *horizontalLayout;
    QPushButton *butt_ResetDefaultKey;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QVBoxLayout *verticalLayout;
    QRadioButton *rdbutt_Ctrl_Enter;
    QRadioButton *rdbutt_Enter;
    QString m_tempKeyString;

    void setSendMsgShortCut(int );
    void updateSenMsgRadioButton(QString);
};

#endif // SETKEYSEQUENCEDIALOG_H
