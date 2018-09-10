#ifndef BDUSER_H
#define BDUSER_H

#define BD_USER_SEND_TYPE_START 80000           // 北斗2号协议功能起始号
#define BD_USER_RECV_TYPE_START 90000           // 北斗2号协议功能起始号

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

enum BD2_USER_TYPE_SEND
{
    TYPE_SEND_TXA = BD_USER_SEND_TYPE_START,   // 通信申请,返回:TYPE_RECV_TXR
    TYPE_SEND_BSS,                              // 波束设置
    TYPE_SEND_JMS,                              // 静默设置
    TYPE_SEND_ICA,                              // 用户卡检测,返回:TYPE_RECV_ICI,TYPE_RECV_ICZ
    TYPE_SEND_ERR_CKS,                          // 数据发送校验和错误
    TYPE_SEND_ERR_COM,                          // 数据发送串口错误
};

enum BD2_USER_TYPE_RECV
{
    TYPE_RECV_TXR = BD_USER_RECV_TYPE_START,   // 通信信息(不包括混合报文)
    TYPE_RECV_ICI,                              // 本级用户卡信息
    TYPE_RECV_ICZ,                              // 下属用户卡信息
    TYPE_RECV_ERR_CKS,                          // 数据接收校验和错误
    TYPE_RECV_ERR_COM,                          // 数据接收串口错误
};

//----------------------------------------------
//外设-->用户机
//发送信息
//----------------------------------------------

#define USER_TXSQ_BWTX  0x02   //报文通信
#define USER_TXSQ_CXTX  0x03   //查询通信
// 通信申请
typedef struct tagUSET_TXSQ
{
    UINT8   Type;               //通信类别:USER_TXSQ_BWTX、USER_TXSQ_CXTX
    UINT8	Flag;				// 传输方式：0=汉字，1=代码，2=混合(扩展通信支持)
    UINT8	Fast;				// ****是否特快0=特快，1=普通
    UINT8	Key;				// ****口令识别0=通信，1=口令识别
    UINT32  DestID;				// 接收端ID,如果使用的是指挥卡,且该地址为通播地址,则为广播,下辖用户均可收到
    UINT16  Len;				// 电文长度(Byte)
    char	Msg[421];			// 电文内容
}USET_TXSQ,USET_TXA;

// 用户卡检测(获取用户信息)
typedef struct tagUSET_ICJC
{
    UINT8   FrameNo;		// 帧号(0=本机,0~127=卡内下辖用户,128~255=外加非密用户)
    UINT8	uchChipNo;		// 模块号(从0开始计)
} USET_ICJC,USET_ICA;

#define  USET_MIF USET_GXM

// 波束设置
typedef struct tagUSET_BSSZ
{
    UINT8 beamResponse;          //响应波束
    UINT8 beamTimelag;           //时差波束
} USET_BSSZ,USET_BSS;

// 静默设置
typedef struct tagUSET_JMSZ
{
    UINT8   Silience;			// 0=解除静默,1=静默1(禁止回执和入站),2=静默2(禁止回执，但可入站（通信/定位/定时）)
} USET_JMSZ,USET_JMS;




//----------------------------------------------
//用户机-->外设
//接收信息
//----------------------------------------------

// 通信信息（本机接收，查询通信，监收下属通信）
typedef struct tagUGET_TXXX
{
    UINT32	SendID;			// 发方ID
    UINT32	DestID;			// 收方ID
    UINT8	Kind;			// 信息类别:1=普通电文，2=特快，3=通播，4=按最新存入查询电文，5=按发方查询电文
    UINT8	Flag;			// 信息类别，0=汉字，1=BCD码，2=混合(扩展库支持)
    UINT8	H;				// 发送时间：小时,对通信查询的结果有效
    UINT8	M;				// 发送时间：分,对通信查询的结果有效
    UINT16	Len;			// 长度（字节)
    char	Msg[421];		// 内容(注意,没有在串后加'\0')
    UINT8	CRC;
}UGET_TXXX,UGET_TXR;

// 通信回执
// 回执查询申请的返回结果
typedef struct tagUGET_TXHZ
{
    UINT32 DestID;			// 目标ID
    UINT8  Count;			// 回执个数,最多5个
    struct
    {
        UINT8 SendHour;		// 发送时
        UINT8 SendMin;		// 发送分
        UINT8 AnswerHour;	// 应答时
        UINT8 AnswerMin;	// 应答分
    } Times[5];
}UGET_TXHZ,UGET_HZR;

// 发射反馈信息
typedef struct tagUGET_FKXX
{
    char	Cmd[4];			// 命令码(三个字符)
    UINT8	Result;			// 结果(0=成功，1=失败）
    UINT8	Freq;			// 频度：0=正确，1=超频
    UINT8	Launch;			// 发射指示:0=发射抑制解除，1=发射抑制，2=电力不足，3=无线静默
    UINT16	WaitTime;		// 等待时间(s)
} UGET_FKXX,UGET_FKI;

#endif // BD2USER_H
