# Stamon on Cyberpi

此项目将Stamon移植到了童芯派（Cyberpi）上。
由于童芯派采用的是esp32框架，因此只要稍加改动，就能将Stamon移植到其他esp32开发板上。

### 涉及仓库

[Stamon](https://github.com/CLimber-Rong/stamon)

[CyberPi-Library-for-Arduino](https://github.com/Makeblock-official/CyberPi-Library-for-Arduino)

### 开发环境

构建此项目，你需要：
* **Python3**：需拥有``re``和``os``两个模块
* **Arduino或VSCode+PlatformIO**：我的测试环境是VSCode+PlatformIO，因此我建议开发者和我使用同样的环境
* **Stamon**：建议使用Stamon 2.4.40以后的版本
* **一双勤劳的手+善于思考的大脑**

### 项目架构

* ``program/main.st``存放Stamon代码，``program/cyberpi.st``提供了一些基础的功能，开发者也可以添加自己喜欢的功能
* ``stamon``目录存放修改后的Stamon运行端源码
* ``src``目录存放构建系统生成的源码
* ``lib``目录存放着cyberpi的源码库

### 构建此项目

基于C++的童芯派开发环境配置的教程请见``lib/README.md``。

* 现在``program/main.st``中编写你的Stamon代码，``program/cyberpi.st``提供了一些基础的功能，开发者也可以添加自己喜欢的功能
* ``make.py``适用于Windows环境下，稍加修改也能应用于其他平台
* 请根据设备的实际情况修改``platformio.ini``
* 准备工作完成后，直接执行``make.py``即可开始构建

### 基本工作流程

``make.py``会编译``program/main.st``，生成一个``program/program.stvc``。
然后将该二进制文件写入到``stamon/include/BinarySTVC.cpp``中，以数组的形式保存。
接着将Stamon的源码全部整理并拼接到``src/main.ino``中，从而生成一个数千行的源文件。
最后调用platformio来编译上传源码到童芯派。

仓库的stamon目录和program目录提供了一个样例：在代码中存储一系列英语单词，每次会先随机显示一个英文单词，按下A按钮或B按钮后输出其对应的词性和中文，从而实现背单词的效果。开发者可以修改源码来实现不同的效果。

### 许可证

由于童芯派采用的是GPL3.0开源协议，Stamon采用的是Apache2.0开源协议，两者协议不一样，因此我为lib目录和stamon目录单独指定了开源协议。所以lib目录采用了GPL3.0协议，而stamon目录采用了Apache2.0协议。

感谢童心派开发团队！