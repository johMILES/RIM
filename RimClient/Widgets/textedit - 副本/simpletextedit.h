﻿/*!
 *  @brief     简单文本编辑区
 *  @details   可设置输入的
 *  @file      simpletextedit.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.22
 *  @warning
 *  @copyright GNU Public License.
 */
#ifndef SIMPLETEXTEDIT_H
#define SIMPLETEXTEDIT_H

#include "basetextedit.h"

class SimpleTextEdit : public BaseTextEdit
{
    Q_OBJECT
public:
    SimpleTextEdit(QWidget * parent = 0 );
    ~SimpleTextEdit();


};

#endif // SIMPLETEXTEDIT_H
