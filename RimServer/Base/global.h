#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include "protocol/datastruct.h"
#include "base_global.h"
using namespace Datastruct;
class GlobalConfigFile;

namespace ParameterSettings{
    struct ParaSettings;
}

namespace RGlobal
{

BASESHARED_EXPORT extern ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */
BASESHARED_EXPORT extern QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */
BASESHARED_EXPORT extern DBFeature G_DB_FEATURE;                 /*!< 当前数据库驱动支持的功能 */

#ifdef __LOCAL_CONTACT__
BASESHARED_EXPORT extern ParameterSettings::RouteSetting* G_RouteSettings;   /*!< 路由配置信息 */
#endif

BASESHARED_EXPORT extern GlobalConfigFile * G_GlobalConfigFile;

}

#endif // GLOBAL_H
