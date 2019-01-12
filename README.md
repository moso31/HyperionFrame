# HyperionFrame
HyperionFrame 是一个基于 DirectX 12 的离线渲染框架。目前最新的版本是 0.3.2。

## 安装须知
使用 HyperionFrame 需要安装 fbx sdk 以支持对 \*.fbx 模型文件。目前对 fbx 模型的支持尚不完善（仅限坐标/法线/UV）。

## 操作说明

在 Debug 版本下，点击鼠标左键可以获取该位置的光线追踪具体信息。

使用 G 键可以渲染图片，对渲染数据的存储采用了大量的 STL 迭代器。

	注意：务必在 Release 下进行渲染。如果使用 Debug 渲染图片，速度会很慢很慢。


## 功能支持

### 积分器：
目前只提供对 Whiited 积分器的支持。

### 材质：
支持哑光（matte）材质，镜面（mirror）材质和玻璃（glass）材质。

### 灯光：
目前只支持点光源。

### 形状：
目前支持 Box 和 Sphere，以及对 fbx 文件进行导入。

可以在 HScene.cpp 中自行添加代码以实现对场景的配置。
