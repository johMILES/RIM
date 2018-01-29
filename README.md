# RIM

### 20180129
- 添加快捷键设置界面，实现截屏快捷键设置、发送信息快捷键设置;

### 20180124
- 窗口抖动;
- 按回车键发送聊天信息;
- 添加登陆、注册结果信息验证，实现完整流程；
- 添加对个人信息查看、更新操作；
- 添加屏蔽网络连接宏__NO_SERVER__
- 添加SQL语句拼接，简化SQL使用;

### 20180123
- 增加客户端和服务器端分包发送、接收、组包功能；
- 添加聊天记录数据库；
- 添加截图插件功能；

### 20180119
- 修改登陆界面，取消UI界面；
- 实现异步网络连接，网络错误处理。

### 20180118
- 调整JSON传输key值访问；

### 20180117
- 添加注册页面；
- 添加网络设置页面，并支持修改和本地加载；
- 初步实现用户注册，验证流程；

### 20180116
- 添加基础通信协议格式；
- 实现客户端和服务器端双向基础信息通信；

### 20180114
- 添加数据库表以及连接管理；
- 添加服务器网络接收，实现与客户端基本通信；

### 20180109
- 封装windows底层的socket，并支持linux
- 调整工程结果文件保存路径(bin、lib)

### 20180108
- 调整程序结构(改成子项目模式)

### 20180105
- 修复自定义弹出框关闭奔溃；
- 建立网路哦通信协议。
- 添加网络接收模块。
- 添加线程处理模块。

### 20180102
- 完善系统设置界面(区分基础设置和安全设置)
- 添加自定义提示框

### 20171229
- 添加系统设置页面

### 20171228
- 添加系统图像选择页面;
- 解决VS和MingW下编译警告问题;
- 添加程序图标和程序描述信息

### 20171227
- 添加个人信息编辑页面；
- 修改工具栏，支持定制显示；

### 20171226
- 添加个人信息显示页面，支持延迟显示；
- 添加对快捷键的保存和加载；

### 20171225
- 添加联系人搜索和结果显示页面；
- 修改编码为utf-8(BOM)以支持MSVC和MING编译器;

### 20171222
- 完成聊天界面的设计；
- 添加样式表

### 20171221
- 修改图标显示为圆角;
- 调整工具栏创建。


### 20171220
- 添加联系人群组、历史聊天信息界面;
- 添加聊天界面抽象窗口;

### 20171219
- 添加自定义工具页面，参照于QQ；
- 添加动作处理ActionManager类，管理创建的Menu、Action以及ToolButton;
- 完成联系人界面基本的功能。

### 20171215
- 完成主面板的布局;
- 创建顶层窗口，实现对无边框窗口的拖拽、阴影的绘制.

### 20171214
- 添加主面板页面;
- 完善退出、配置文件记录等功能;
- 建立客户端登录页面;
- 添加日志记录;
- 添加系统配置文件操作.
