#pragma once

//启动uniServer
int Start_uniServer(int argc,char* argv[]);

//用RunAsyn模式启动uniServer
void Start_uniServer_InRunAsynMode(void);

//停止uniServer。只有在RunAsyn模式下才需要这样停止。在服务模式和Cmd窗口模式下不用
void Stop_uniServer_InRunAsynMode(void);
