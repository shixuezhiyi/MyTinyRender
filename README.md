# MyTinyRender


此项目为个人练手独立项目,是基于 c++ 的一个软光栅化渲染器,除了 c++标准库以外,只依赖了一个对于 TGA 格式的图片的读写操作文件(Tgaimge.cpp  Tgaimage.h)

本次项目有借鉴: https://github.com/ssloy/tinyrenderer/wiki

但包括数学库,渲染流程,OBJ 文件解析等核心功能都是独立完成.

实现了以下功能

1. OBJ 文件解析
2. 纹理贴图
3. 凹凸贴图
4. 高光贴图
5. Blinn-Phong 着色模型
6. 软阴影

输出结果:

<img src="https://tva1.sinaimg.cn/large/008i3skNgy1gxgtj87wkfj30qv0rw769.jpg" alt="image-20211217143430161" style="zoom:40%;float:left" />

### 
