wxWidgets库

1. FastTrader中引用此目录即可
2. wxWidgets各模块编译时，Runtime Library使用/MT或/MTD，静态链接运行时文件
3. 首次下载后需本地rebuild build/msw/wx.sln项目(Debug模式)，否则，没有debug的lib，引用wxWidgets的项目在debug下就无法编译。
   注意：不要上传rebuild的lib文件
   原因：debug版本的lib需要和pdb配对使用，并且记录了编译时的本地路径，配对不上则出现LNK4099的警告。因此无法提高一套通用的lib和pdb库，只能大家各自在本地编译一遍再用
4. Release版本不需要重编译
