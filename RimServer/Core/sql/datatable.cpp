﻿#include "datatable.h"

namespace DataTable {

User::User():
    table("user"),id("ID"),account("ACCOUNT"),
    password("PASSWORD"),nickName("NICKNAME"),signName("SIGNNAME"),
    gender("GENDER"),birthDay("BIRTHDAY"),phone("PHONE"),address("ADDRESS"),
    email("EMAIL"),remark("REMARK"),face("FACE"),faceId("FACEID")
{

}

Group::Group():table("group"),
    id("ID"),name("NAME"),userCount("USER_COUNT"),
    userId("UID")
{

}

ChatRoom::ChatRoom():
    table("chatroom"),id("ID"),name("NAME"),desc("DESC"),label("LABEL"),userId("UID")
{

}

User_Group::User_Group():
    id("ID"),groupId("GID"),userId("UID"),remarks("REMARKS"),visible("VISIBLE")
{

}

User_Chatroom::User_Chatroom():
    id("ID"),chatId("CID"),userId("UID")
{

}

RimConfig::RimConfig():table("RimConfig"),name("NAME"),value("VALUE"),accuoutId("ACCOUNT_ID")
{

}

RequestCache::RequestCache():table("requestcache"),id("ID"),account("ACCOUNT"),
    requestId("REQUESTID"),time("TIME")
{

}

}


