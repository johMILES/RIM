﻿#include "global.h"

namespace RGlobal
{

ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */

QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */

#ifdef __LOCAL_CONTACT__
ParameterSettings::ParaSettings* G_ParaSettings;     /*!< 本地参数配置 */
ParameterSettings::RouteSettings* G_RouteSettings;   /*!< 路由配置信息 */
#endif
}

