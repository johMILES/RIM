﻿#include "msgparsefactory.h"

#include "json_msgparse.h"

MsgParseFactory::MsgParseFactory():dataParse(nullptr)
{

}

DataParse *MsgParseFactory::getDataParse()
{
#ifdef __LOCAL_CONTACT__
    if(dataParse == nullptr)
        dataParse = new Json_MsgParse();
#else
    if(dataParse == nullptr)
        dataParse = new Json_MsgParse();
#endif

    return dataParse;
}
