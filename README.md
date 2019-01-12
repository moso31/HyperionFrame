# HyperionFrame
HyperionFrame 是一个基于 DirectX 12 的离线渲染框架。目前最新的版本是 0.3.2。

## 环境配置

### 安装 fbx sdk 2019

1.	HyperionFrame 需要安装 fbx sdk 2019 以支持对 \*.fbx 模型文件。目前 HyperionFrame 对 fbx 模型的支持尚不完善（仅支持获取坐标/法线/UV的数据）。fbx sdk 官方下载地址：https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-0
	
2.	下载完成后，执行exe文件将sdk包的内容解压到任意位置，然后将对应目录下的Autodesk\FBX\FBX SDK\2019.0\lib 文件夹及所有子内容拷贝一份，放到HyperionFrame解决方案中FBXImporter项目下。（HyperionFrame\HyperionFrame\FBxImporter）
	
3.	照以上步骤配置完毕后，某些机型可能会出现“error C2903: “GetPropertyValue”: 符号既不是类 模板 也不是函数 模板”及相关内容错误。如果出现此问题，请在FbxImporter项目的配置属性中关闭符合模式（C/C++->语言->符合模式，设置为否）



## 操作说明

在 Debug 版本下，点击鼠标左键可以获取该位置的光线追踪具体信息。

使用 G 键可以渲染图片，对渲染数据的存储采用了大量的 STL 迭代器。

	注意：务必在 Release 下进行渲染。如果使用 Debug 渲染图片，速度会很慢很慢。


## 功能支持

### 积分器：
目前只提供对 Whitted 积分器的支持。

### 材质：
支持哑光（matte）材质，镜面（mirror）材质和玻璃（glass）材质。

### 灯光：
目前只支持点光源。

### 形状：
目前支持 Box 和 Sphere，以及对 fbx 文件进行导入。

可以在 HScene.cpp 中自行添加代码以实现对场景的配置。
