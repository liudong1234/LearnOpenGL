﻿---
typora-root-url: ./img
---



# 								openGL

## 一、窗口基本知识

### 1.glViewport(0, 0, 300, 400)

```tex
(1)glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
(2)OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
```

### 2.窗口维持比例

```cpp
//对窗口注册一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
{
    glViewport(0, 0, width, height);
}
//两个整数表示窗口新维度
//注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```

### 3.窗口保持不退出

```cpp
while(!glfwWindowShouldClose(window))
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}
//glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
//glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
```

### 4.颜色渲染

```cpp
glClearColor(0.2f, 0.3f, 0.4f, 1.0f); //设置清空屏幕所用的颜色
glClear(GL_COLOR_BUFFER_BIT);//调用glClear函数，清除颜色缓冲
//glClearColor函数是一个状态设置函数，而glClear函数则是一个状态使用的函数，它使用了当前的状态来获取应该清除为的颜色。
```

# 二、图形绘制与渲染

### 1.三个词

```tex
顶点数组对象：Vertex Array Object，VAO
顶点缓冲对象：Vertex Buffer Object，VBO   //在gpu中存储大量顶点
元素缓冲对象：Element Buffer Object，EBO 或 索引缓冲对象 Index Buffer Object，IBO
```

![image-20230908174000749](/image-20230908174000749.png)



### 2.着色器

opengl中的一个片段是opengl渲染一个像素所需的所有数据

```tex
1.顶点着色器：把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标（后面会解释），同时顶点着色器允许我们对顶点属性进行一些基本处理。
2.集合着色器：把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。
3.片段着色器：计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。
```

### 3.着色器语言

#### (1)顶点着色器

```cpp
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

```tex
OpenGL 3.3以及和更高版本中，GLSL版本号和OpenGL的版本是匹配的（比如说GLSL 420版本对应于OpenGL 4.2）。我们同样明确表示我们会使用核心模式。
下一步，使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性。GLSL有一个向量数据类型，它包含1到4个float分量，包含的数量可以从它的后缀数字看出来。由于每个顶点都有一个3D坐标，我们就创建一个vec3输入变量aPos。
```

##### 	编译着色器

```cpp
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
//储存这个顶点着色器为unsigned int，然后用glCreateShader创建这个着色器：
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
//着色器源码附加到着色器对象上，然后编译它
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

```

```cpp
//编译是否成功的检测
int  success;
char infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//首先我们定义一个整型变量来表示是否成功编译，还定义了一个储存错误消息（如果有的话）的容器。然后我们用glGetShaderiv检查是否编译成功。如果编译失败，我们会用glGetShaderInfoLog获取错误消息，然后打印它。
if(!success)
{
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```

#### (2)片段着色器

```cpp
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```

##### 	编译着色器

```cpp
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);
```

### 4.着色器程序

```cpp
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
//链接
glAttachShader(shaerProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
//激活
glUserProgram(shaderProgram);
//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

### 5.链接顶点属性

![](屏幕截图 2023-09-22 165708.png\)

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

```tex
解释：
1.第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。

2.第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。

3.第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。

4.下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。

5.第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。

6.最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
```

**每当我们绘制一个物体的时候都必须重复这一过程。**

```cpp
// 0. 复制顶点数组到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 1. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 2. 当我们渲染一个物体时要使用着色器程序
glUseProgram(shaderProgram);
// 3. 绘制物体
someOpenGLFunctionThatDrawsOurTriangle();
```

### 6.顶点数组对象

```tex
顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。刚刚设置的所有状态都将存储在VAO中
```

```cpp
//创建
unsigned int VAO;
glGenVertexArrays(1, &VAO);
//要想使用VAO，要做的只是使用glBindVertexArray绑定VAO。
```

```cpp
// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
// 1. 绑定VAO
glBindVertexArray(VAO);
// 2. 把顶点数组复制到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

[...]

// ..:: 绘制代码（渲染循环中） :: ..
// 4. 绘制物体
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
someOpenGLFunctionThatDrawsOurTriangle();
```

## 三、着色器详解

### 1.GLSL

```c
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

### 2.数据类型

```tex
int uint float double bool
向量，矩阵
```

#### (1)向量

```tex
vecn		包含n个float分量的默认向量
bvecn		包含n个bool分量的向量
ivecn		包含n个int分量的向量
uvecn 		包含n个unsigned int分量的向量
dvecn		包含n个double分量的向量

向量的分量采用
vec.x vec.y vec.z vec.w来获取

向量这一数据类型也允许一些有趣而灵活的分量选择方式，叫做重组(Swizzling)。重组允许这样的语法：
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);

```

#### (2)矩阵

....

### 3.输入与输出

#### (1)顶点着色器与片段着色器通信

```tex
如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）
```

```cpp
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);"
"}\0";


const char* fragment2 = "#version 330 core\n"
"in vec4 vertexColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\n\0";
```

#### (2)应用程序与片段着色器通信(uniform)

```cpp
用glGetUniformLocation查询uniform ourColor的位置值。我们为查询函数提供着色器程序和uniform的名字（这是我们希望获得的位置值的来源）。如果glGetUniformLocation返回-1就代表没有找到这个位置值。最后，我们可以通过glUniform4f函数设置uniform值。注意，查询uniform地址不要求你之前使用过着色器程序，但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
```

```cpp
float timeValue = static_cast<float>(glfwGetTime());
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
```

#### (3)更多属性（顶点数据更多）

```cpp
float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
```

用`layout`标识符来把aColor属性的位置值设置为1：

```cpp
#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}

//硬编码
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(aColor, 1.0);\n"
"}\0";
```

使用`ourColor`输出变量，我们必须再修改一下片段着色器：

```cpp
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}

//硬编码
const char* fragment2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\n\0";

```

**void glVertexAttribPointer(	GLuint index,**
**GLint size,**
**GLenum type,**
**GLboolean normalized,**
**GLsizei stride,**
**const GLvoid * pointer);**顶点属性含义示意图

![](屏幕截图 2023-09-26 173815.png)

## 四、纹理

把纹理映射(Map)到三角形上，我们需要指定三角形的每个顶点各自对应纹理的哪个部分。

![image-20230927132350655](屏幕截图 2023-09-27 132343.png)

### 1.纹理环绕方式

| 环绕方式           | 描述                                                         |
| :----------------- | :----------------------------------------------------------- |
| GL_REPEAT          | 对纹理的默认行为。重复纹理图像。                             |
| GL_MIRRORED_REPEAT | 和GL_REPEAT一样，但每次重复图片是镜像放置的。                |
| GL_CLAMP_TO_EDGE   | 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。 |
| GL_CLAMP_TO_BORDER | 超出的坐标为用户指定的边缘颜色。                             |

**每个选项都可以使用glTexParameter*函数对单独的一个坐标轴设置（`s`、`t`（如果是使用3D纹理那么还有一个`r`）它们和`x`、`y`、`z`是等价的）**

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//第一个参数指定了纹理目标；我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D。第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。最后一个参数需要我们传递一个环绕方式(Wrapping)，在这个例子中OpenGL会给当前激活的纹理设定纹理环绕方式为GL_MIRRORED_REPEAT。
```

```cpp
//如果我们选择GL_CLAMP_TO_BORDER选项，我们还需要指定一个边缘的颜色。这需要使用glTexParameter函数的fv后缀形式，用GL_TEXTURE_BORDER_COLOR作为它的选项，并且传递一个float数组作为边缘的颜色值：
float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
```

### 2.纹理过滤	

#### (1)邻近过滤（GL_NEAREST）

```tex
GL_NEAREST产生了颗粒状的图案，我们能够清晰看到组成纹理的像素，
```

![](屏幕截图 2023-09-27 133138.png)



#### (2)线性过滤(GL_LINEAR)

```tex
GL_LINEAR能够产生更平滑的图案，很难看出单个的纹理像素,GL_LINEAR可以产生更真实的输出
```

![](屏幕截图 2023-09-27 133346.png)

比如你可以在纹理被缩小的时候使用邻近过滤，被放大时使用线性过滤。

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

#### (3)多级渐远纹理

为了指定不同多级渐远纹理级别之间的过滤方式，你可以使用下面四个选项中的一个代替原有的过滤方式：

| 过滤方式                  | 描述                                                         |
| :------------------------ | :----------------------------------------------------------- |
| GL_NEAREST_MIPMAP_NEAREST | 使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样 |
| GL_LINEAR_MIPMAP_NEAREST  | 使用最邻近的多级渐远纹理级别，并使用线性插值进行采样         |
| GL_NEAREST_MIPMAP_LINEAR  | 在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样 |
| GL_LINEAR_MIPMAP_LINEAR   | 在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样 |

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

## 五、加载与创建纹理

### 1.加载图片工具库

[stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)链接

```cpp
//它以stb_image.h的名字加入你的工程，并另创建一个新的C++文件，输入以下代码：
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
```





## 六、坐标系统

### 1.概述

#### （1）5个不同的坐标系统：

- 局部空间(Local Space，或者称为物体空间(Object Space))
- 世界空间(World Space)
- 观察空间(View Space，或者称为视觉空间(Eye Space))
- 裁剪空间(Clip Space)
- 屏幕空间(Screen Space)

#### （2）解释

顶点坐标起始于局部空间(Local Space)，在这里它称为局部坐标(Local Coordinate)，它在之后会变为世界坐标(World Coordinate)，观察坐标(View Coordinate)，裁剪坐标(Clip Coordinate)，并最后以屏幕坐标(Screen Coordinate)的形式结束。

![](屏幕截图 2023-10-12 154644.png)

```tex
1.局部坐标是对象相对于局部原点的坐标，也是物体起始的坐标。 local
2.下一步是将局部坐标变换为世界空间坐标，世界空间坐标是处于一个更大的空间范围的。这些坐标相对于世界的全局原点，它们会和其它物体一起相对于世界的原点进行摆放。		model
3.接下来我们将世界坐标变换为观察空间坐标，使得每个坐标都是从摄像机或者说观察者的角度进行观察的。
坐标到达观察空间之后，我们需要将其投影到裁剪坐标。裁剪坐标会被处理至-1.0到1.0的范围内，并判断哪些顶点将会出现在屏幕上。									 view   projection
4.最后，我们将裁剪坐标变换为屏幕坐标，我们将使用一个叫做视口变换(Viewport Transform)的过程。视口变换将位于-1.0到1.0范围的坐标变换到由glViewport函数所定义的坐标范围内。最后变换出来的坐标将会送到光栅器，将其转化为片段。							 clip
```

### 4.组合起来

#### （1）创建变换矩阵：

模型矩阵、观察矩阵和投影矩阵。一个顶点坐标将会根据以下过程被变换到裁剪坐标：
$$
Vclip=Mprojection⋅Mview⋅Mmodel⋅Vlocal
$$
**注意矩阵运算的顺序是相反的（记住我们需要从右往左阅读矩阵的乘法）。最后的顶点应该被赋值到顶点着色器中的gl_Position，OpenGL将会自动进行透视除法和裁剪。**



### 2.正射投影

```tex
正射投影矩阵定义了一个类似立方体的平截头箱，它定义了一个裁剪空间，在这空间之外的顶点都会被裁剪掉。创建一个正射投影矩阵需要指定可见平截头体的宽、高和长度。在使用正射投影矩阵变换至裁剪空间之后处于这个平截头体内的所有坐标将不会被裁剪掉。它的平截头体看起来像一个容器：
```

![](屏幕截图 2023-10-12 160210.png)

```tex
glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);//GLM的内置函数
//前两个参数指定了平截头体的左右坐标，第三和第四参数指定了平截头体的底部和顶部。通过这四个参数我们定义了近平面和远平面的大小，然后第五和第六个参数则定义了近平面和远平面的距离。这个投影矩阵会将处于这些x，y，z值范围内的坐标变换为标准化设备坐标。
```

```tex
注：将摄像机向后移动，和将整个场景向前移动是一样的。
这正是观察矩阵所做的，我们以相反于摄像机移动的方向移动整个场景。因为我们想要往后移动，并且OpenGL是一个右手坐标系(Right-handed System)，所以我们需要沿着z轴的正方向移动。我们会通过将场景沿着z轴负方向平移来实现。它会给我们一种我们在往后移动的感觉。
```



### 3.透视投影

在GLM中可以这样创建一个透视投影矩阵：

```cpp
glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
//它的第一个参数定义了fov的值，它表示的是视野(Field of View)，并且设置了观察空间的大小。如果想要一个真实的观察效果，它的值通常设置为45.0f，但想要一个末日风格的结果你可以将其设置一个更大的值。第二个参数设置了宽高比，由视口的宽除以高所得。第三和第四个参数设置了平截头体的近和远平面。我们通常设置近距离为0.1f，而远距离设为100.0f。所有在近平面和远平面内且处于平截头体内的顶点都会被渲染。
```

![透视平截头体的图片](屏幕截图 2023-10-12 160842.png)





# 七、摄像机

## 1.摄像机/观察空间

![image-20231030094337842](屏幕截图 2023-10-30 094353.png)

### (1)摄像机位置

摄像机位置简单来说就是世界空间中一个指向摄像机位置的向量。我们把摄像机位置设置为上一节中的那个相同的位置：

```cpp
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
```

**注：不要忘记正z轴是从屏幕指向你的，如果我们希望摄像机向后移动，我们就沿着z轴的正方向移动。**

### (2)摄像机方向

指的是摄像机指向的方向。

摄像机指向场景原点：(0, 0, 0)。

```cpp
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
```

用场景原点向量减去摄像机位置向量的结果就是摄像机的指向向量。

```cpp
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
```

**方向**向量(Direction Vector)并不是最好的名字，因为它实际上指向从它到目标向量的相反方向（译注：注意看前面的那个图，蓝色的方向向量大概指向z轴的正方向，与摄像机实际指向的方向是正好相反的）。

### (3)右轴(x轴)

**右向量**(Right Vector)，它代表摄像机空间的x轴的正方向。

**注：**叉乘方向的确定->向量叉乘的方向可以用**右手螺旋定则**判定，即将两向量移动到同一起点，右手四指从第一个向量转到第二个向量，拇指所指方向即为结果向量的方向

为获取右向量我们需要先使用一个小技巧：先定义一个**上向量**(Up Vector)。接下来把上向量和第二步得到的方向向量进行叉乘。两个向量叉乘的结果会同时垂直于两向量，因此我们会得到指向x轴正方向的那个向量（如果我们交换两个向量叉乘的顺序就会得到相反的指向x轴负方向的向量）：

```cpp
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
```

### (4)上轴(y轴)

右向量和方向向量进行叉乘：

```cpp
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
```

## 2.LookAt

![](屏幕截图 2023-10-30 102517.png)

其中R是右向量，U是上向量，D是方向向量, P是摄像机位置向量。

把这个LookAt矩阵作为观察矩阵可以很高效地把所有世界坐标变换到刚刚定义的观察空间。

GLM已经提供了这些支持。

定义一个摄像机位置，一个目标位置和一个表示世界空间中的上向量的向量（我们计算右向量使用的那个上向量）

```cpp
glm::mat4 view;
view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3(0.0f, 1.0f, 0.0f));
```

## 3.自由移动

### (1)定义一些摄像机变量

```cpp
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
```

将摄像机位置设置为之前定义的cameraPos。方向是当前的位置加上我们刚刚定义的方向向量。这样能保证无论我们怎么移动，摄像机都会注视着目标方向。

```cpp
view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
```

### (2)按键响应

```cpp
void processInput(GLFWwindow *window)
{
    ...
    float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
```

## 4.移动速度

(1)图形程序和游戏通常会跟踪一个时间差(Deltatime)变量，它储存了渲染上一帧所用的时间。

(2)我们把所有速度都去乘以deltaTime值。结果就是，如果我们的deltaTime很大，就意味着上一帧的渲染花费了更多时间，所以这一帧的速度需要变得更高来平衡渲染所花去的时间。使用这种方法时，无论你的电脑快还是慢，摄像机的速度都会相应平衡，这样每个用户的体验就都一样了。

```cpp
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
```

(3)在每一帧中我们计算出新的deltaTime以备后用。

```cpp
float currentFrame = glfwGetTime();
deltaTime = currentFrame - lastFrame;
lastFrame = currentFrame;
```

(4)有了deltaTime，在计算速度的时候可以将其考虑进去了：

```cpp
void processInput(GLFWwindow *window)
{
  float cameraSpeed = 2.5f * deltaTime;
  ...
}
```

## 5.欧拉角

见网址 [欧拉角](https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_2)

## 6.鼠标输入



# 八、光照

## 1、基础光照

```tex
1、环境光照(Ambient Lighting)：即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色。
2、漫反射光照(Diffuse Lighting)：模拟光源对物体的方向性影响(Directional Impact)。它是冯氏光照模型中视觉上最显著的分量。物体的某一部分越是正对着光源，它就会越亮。
3、镜面光照(Specular Lighting)：模拟有光泽物体上面出现的亮点。镜面光照的颜色相比于物体的颜色会更倾向于光的颜色。
```

### (1)环境光照

method:我们用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，然后将最终结果作为片段的颜色：

```glsl
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}
```

### (2)漫反射关照

method：计算漫反射光照需要什么？

- 法向量：一个垂直于顶点表面的向量。
- 定向的光线：作为光源的位置与片段的位置之间向量差的方向向量。为了计算这个光线，我们需要光的位置向量和片段的位置向量。

```tex
注：
法向量是一个垂直于顶点表面的（单位）向量。由于顶点本身并没有表面（它只是空间中一个独立的点），我们利用它周围的顶点来计算出这个顶点的表面。我们能够使用一个小技巧，使用叉乘对立方体所有的顶点计算法向量，但是由于3D立方体不是一个复杂的形状，所以我们可以简单地把法线数据手工添加到顶点数据中。更新后的顶点数据数组可以在这里找到。试着去想象一下，这些法向量真的是垂直于立方体各个平面的表面的（一个立方体由6个平面组成）。
```

```glsl
//在世界空间中进行所有的光照计算，因此我们需要一个在世界空间中的顶点位置。我们可以通过把顶点位置属性乘以模型矩阵（不是观察和投影矩阵）来把它变换到世界空间坐标。这个在顶点着色器中很容易完成，所以我们声明一个输出变量，并计算它的世界空间坐标：
#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
...
out vec3 FragPos;  
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
```

最后，在片段着色器中添加相应的输入变量。

```glsl
#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
uniform vec3 lightPos;

//添加光照计算
//计算光源和片段位置之间的方向向量。
//的方向向量是光源位置向量与片段位置向量之间的向量差。你可能记得在变换教程中，我们能够简单地通过让两个向量相减的方式计算向量差。我们同样希望确保所有相关向量最后都转换为单位向量，所以我们把法线和最终的方向向量都进行标准化：

void main()
{
    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
    //我们对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫反射影响。
    //两个向量之间的角度越大，漫反射分量就会越小：
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
}
```

### (3)镜面光照

**注：我们选择在世界空间进行光照计算，但是大多数人趋向于更偏向在观察空间进行光照计算。在观察空间计算的优势是，观察者的位置总是在(0, 0, 0)，所以你已经零成本地拿到了观察者的位置。然而，若以学习为目的，我认为在世界空间中计算光照更符合直觉。如果你仍然希望在观察空间计算光照的话，你需要将所有相关的向量也用观察矩阵进行变换（不要忘记也修改法线矩阵）。**

接上面的代码块

```glsl
//要得到观察者的世界空间坐标，我们直接使用摄像机的位置向量即可（它当然就是那个观察者）。那么让我们把另一个uniform添加到片段着色器中，并把摄像机的位置向量传给着色器：
uniform vec3 viewPos;

void main()
{
    float specularStrength = 0.5;
    //我们对lightDir向量进行了取反。reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。第二个参数要求是一个法向量，所以我们提供的是已标准化的norm向量。
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
    //计算镜面分量
    //这个32是高光的反光度(Shininess)。一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
```

## 2、材质

### (1)定义结构体设置分量

三个光照分量定义一个材质颜色(Material Color)：环境光照(Ambient Lighting)、漫反射光照(Diffuse Lighting)和镜面光照(Specular Lighting)。通过为每个分量指定一个颜色，我们就能够对表面的颜色输出有细粒度的控制了。现在，我们再添加一个反光度(Shininess)分量，结合上述的三个颜色，我们就有了全部所需的材质属性了：

```glsl
#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;
```

### (2)设置材质

```glsl
void main()
{    
    // 环境光
    vec3 ambient = lightColor * material.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
```

### (3)光的属性

（1）光源对环境光、漫反射和镜面光分量也分别具有不同的强度。

（2）为每个光照分量分别指定一个强度向量；

```glsl
vec3 ambient  = vec3(1.0) * material.ambient;
vec3 diffuse  = vec3(1.0) * (diff * material.diffuse);
vec3 specular = vec3(1.0) * (spec * material.specular);
```

（3）可以用同样的方式影响光源的漫反射和镜面光强度

（4）为光照属性创建类似材质结构体的东西：

```glsl
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
```

## 3、光照贴图

### (1)漫反射贴图

简单来讲，就是一个纹理，直接贴在模型上

（1）将漫反射颜色向量替换为漫反射贴图

```glsl
struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
}; 
...
in vec2 TexCoords;
```

（2）注意我们将在片段着色器中再次需要纹理坐标，所以我们声明一个额外的输入变量。接下来我们只需要从纹理中采样片段的漫反射颜色值即可：

```glsl
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
```

（3）不要忘记将环境光的材质颜色设置为漫反射材质颜色同样的值。

```glsl
vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
```

### (2)镜面光贴图

（1）你可能会注意到，镜面高光看起来有些奇怪，因为我们的物体大部分都是木头，我们知道木头不应该有这么强的镜面高光的。我们可以将物体的镜面光材质设置为`vec3(0.0)`来解决这个问题，但这也意味着箱子钢制的边框将不再能够显示镜面高光了，我们知道钢铁**应该**是有一些镜面高光的。所以，我们想要让物体的某些部分以不同的强度显示镜面高光。这个问题看起来和漫反射贴图非常相似。是巧合吗？我想不是。

（2）可以使用一个专门用于镜面高光的纹理贴图。这也就意味着我们需要生成一个黑白的（如果你想得话也可以是彩色的）纹理，来定义物体每部分的镜面光强度；

### (3)采用镜面光贴图

（1）镜面光贴图和其它的纹理非常类似，所以代码也和漫反射贴图的代码很类似。记得要保证正确地加载图像并生成一个纹理对象。由于我们正在同一个片段着色器中使用另一个纹理采样器，我们必须要对镜面光贴图使用一个不同的纹理单元（见[纹理](https://learnopengl-cn.github.io/01 Getting started/06 Textures/)），所以我们在渲染之前先把它绑定到合适的纹理单元上：

```glsl
lightingShader.setInt("material.specular", 1);
...
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularMap);
```

（2）新片段着色器的材质属性，让其接受一个`sampler2D`而不是`vec3`作为镜面光分量：

```glsl
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
```

（3）采样镜面光贴图，来获取片段所对应的镜面光强度：

```glsl
vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));  
vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
FragColor = vec4(ambient + diffuse + specular, 1.0);
```

## 4、投光物(Light Caster)

将光**投射**(Cast)到物体的光源叫做投光物(Light Caster)

学会模拟不同种类的光源是又一个能够进一步丰富场景的工具。

### (1)平行光

（1）所有的光线都是平行的，所以物体与光源的相对位置是不重要的，因为对场景中每一个物体光的方向都是一致的。由于光的位置向量保持一致，场景中每个物体的光照计算将会是类似的。
（2）定义一个光线方向向量而不是位置向量来模拟一个定向光，将直接使用光的direction向量而不是通过position来计算lightDir向量

```glsl
struct Light {
    // vec3 position; // 使用定向光就不再需要了
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
...
void main()
{
  vec3 lightDir = normalize(-light.direction);
  ...
}
```

注：我们首先对light.direction向量取反。我们目前使用的光照计算需求一个从片段**至**光源的光线方向，但人们更习惯定义定向光为一个**从**光源出发的全局方向。所以我们需要对全局光照方向向量取反来改变它的方向，它现在是一个指向光源的方向向量了。而且，记得对向量进行标准化，假设输入向量为一个单位向量是很不明智的。最终的lightDir向量将和以前一样用在漫反射和镜面光计算中。

### (2)点光源(Point Light)

（1）衰减：随着光线传播距离的增长逐渐削减光的强度通常叫做衰减(Attenuation)，

下面这个公式根据片段距光源的距离计算了衰减值，之后我们会将它乘以光的强度向量：
$$
Fatt=1.0(Kc+Kl∗d+Kq∗d2)
$$

```tex
常数项Kc、一次项Kl和二次项Kq

常数项通常保持为1.0，它的主要作用是保证分母永远不会比1小，否则的话在某些距离上它反而会增加强度，这肯定不是我们想要的效果。
一次项会与距离值相乘，以线性的方式减少强度。
二次项会与距离的平方相乘，让光源以二次递减的方式减少强度。二次项在距离比较小的时候影响会比一次项小很多，但当距离值比较大的时候它就会比一次项更大了。
```

（2）选择相应的k参数，[见网站](https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)

![image-20231031111604466](屏幕截图 2023-10-31 111609.png)

（3）实现衰减

为了实现衰减，将公式中的常数项、一次项和二次项。储存在之前定义的Light结构体中。

```glsl
struct Light {
    vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
```

希望光源能够覆盖50的距离，所以我们会使用网站中对应的常数项、一次项和二次项：

```glsl
lightingShader.setFloat("light.constant",  1.0f);
lightingShader.setFloat("light.linear",    0.09f);
lightingShader.setFloat("light.quadratic", 0.032f);
```

<u>(根据公式计算衰减值，之后再分别乘以环境光、漫反射和镜面光分量。)</u>

需要公式中距光源的距离，可以通过获取片段和光源之间的向量差，并获取结果向量的长度作为距离项。我们可以使用GLSL内建的length函数来完成这一点：

```glsl
float distance    = length(light.position - FragPos);
float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));
```

我们将包含这个衰减值到光照计算中，将它分别乘以环境光、漫反射和镜面光颜色。

```glsl
ambient  *= attenuation; 
diffuse  *= attenuation;
specular *= attenuation;
```

### (3)聚光(Spotlight)

聚光是位于环境中某个位置的光源，它只朝一个特定方向而不是所有方向照射光线。

（1）OpenGL中聚光是用一个世界空间位置、一个方向和一个切光角(Cutoff Angle)来表示的，切光角指定了聚光的半径（译注：是圆锥的半径不是距光源距离那个半径）。

![](屏幕截图 2023-10-30 111551.png)

```tex
LightDir：从片段指向光源的向量。
SpotDir：聚光所指向的方向。
Phiϕ：指定了聚光半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
Thetaθ：LightDir向量和SpotDir向量之间的夹角。在聚光内部的话θ值应该比ϕ值小。
```

（2）构建结构体

```glsl
struct SpotLight 
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
};
```

（3）传入适当的值

```glsl
	this->SetUniform3f("spotLight.position", spotLight.position);
	this->SetUniform3f("spotLight.direction", spotLight.direction);
	this->SetUniform3f("spotLight.ambient", spotLight.ambient);
	this->SetUniform3f("spotLight.diffuse", spotLight.diffuse);
	this->SetUniform3f("spotLight.specular", spotLight.specular);
	this->SetUniform1f("spotLight.cutOff", spotLight.cutOff);
```

（4）计算LightDir向量和SpotDir向量之间的点积, 并将它和切光角ϕ对比，来决定是否在聚光的内部：

```glsl
float theta = dot(lightDir, normalize(-spotLight.direction));

if(theta > spotLight.cutOff) 
{       
  // 执行光照计算
}
else  // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
  color = vec4(spotLight.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
```

（5）平滑/软化边缘

```tex
为了创建一种看起来边缘平滑的聚光，我们需要模拟聚光有一个内圆锥(Inner Cone)和一个外圆锥(Outer Cone)。我们可以将内圆锥设置为上一部分中的那个圆锥，但我们也需要一个外圆锥，来让光从内圆锥逐渐减暗，直到外圆锥的边界。

为了创建一个外圆锥，我们只需要再定义一个余弦值来代表聚光方向向量和外圆锥向量（等于它的半径）的夹角。然后，如果一个片段处于内外圆锥之间，将会给它计算出一个0.0到1.0之间的强度值。如果片段在内圆锥之内，它的强度就是1.0，如果在外圆锥之外强度值就是0.0。
```

$$
I=(θ−γ)/ϵ
$$

```tex
这里ϵ(Epsilon)是内（ϕ）和外圆锥（γ）之间的余弦值差（ϵ=ϕ−γ）。最终的I值就是在当前片段聚光的强度。
```

（6）着色器计算

```glsl
float theta     = dot(lightDir, normalize(-light.direction));
float epsilon   = light.cutOff - light.outerCutOff;
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
...
// 将不对环境光做出影响，让它总是能有一点光
diffuse  *= intensity;
specular *= intensity;
...
```

```tex
clamp函数，它把第一个参数约束(Clamp)在了0.0到1.0之间。这保证强度值不会在[0, 1]区间之外。
```

# 九、深度测试

## 1.前言

深度缓冲是在片段着色器运行之后（以及模板测试(Stencil Testing)运行之后）在屏幕空间中运行的。

屏幕空间坐标与通过OpenGL的glViewport所定义的视口密切相关，并且可以直接使用GLSL内建变量gl_FragCoord从片段着色器中直接访问。gl_FragCoord的x和y分量代表了片段的屏幕空间坐标（其中(0, 0)位于左下角）。

gl_FragCoord中也包含了一个z分量，它包含了片段真正的深度值。z值就是需要与深度缓冲内容所对比的那个值。

## 2.一些操作

(1)启用了深度缓冲，应该在每个渲染迭代之前使用GL_DEPTH_BUFFER_BIT来清除深度缓冲，否则你会仍在使用上一次渲染迭代中的写入的深度值：

```glsl
glEnable(GL_DEPTH_TEST);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

(2)在某些情况下你会需要对所有片段都执行深度测试并丢弃相应的片段，但**不**希望更新深度缓冲。基本上来说，你在使用一个只读的(Read-only)深度缓冲。OpenGL允许我们禁用深度缓冲的写入，只需要设置它的深度掩码(Depth Mask)设置为`GL_FALSE`就可以了：

```glsl
glDepthMask(GL_FALSE);
```

(3)OpenGL允许我们修改深度测试中使用的比较运算符。这允许我们来控制OpenGL什么时候该通过或丢弃一个片段，什么时候去更新深度缓冲。我们可以调用glDepthFunc函数来设置比较运算符（或者说深度函数(Depth Function)）：

```
glDepthFunc(GL_LESS);
```

有一下运算符

|    函数     |                     描述                     |
| :---------: | :------------------------------------------: |
|  GL_ALWAYS  |               永远通过深度测试               |
|  GL_NEVER   |              永远不通过深度测试              |
|   GL_LESS   |    在片段深度值小于缓冲的深度值时通过测试    |
|  GL_EQUAL   |   在片段深度值等于缓冲区的深度值时通过测试   |
|  GL_LEQUAL  | 在片段深度值小于等于缓冲区的深度值时通过测试 |
| GL_GREATER  |   在片段深度值大于缓冲区的深度值时通过测试   |
| GL_NOTEQUAL |  在片段深度值不等于缓冲区的深度值时通过测试  |
|  GL_GEQUAL  | 在片段深度值大于等于缓冲区的深度值时通过测试 |

## 3.深度值精度

(1)深度缓冲包含了一个介于0.0和1.0之间的深度值，它将会与观察者视角所看见的场景中所有物体的z值进行比较。观察空间的z值可能是投影平截头体的**近平面**(Near)和**远平面**(Far)之间的任何值。

下面的（线性）方程将z值变换到了0.0到1.0之间的深度值：
$$
Fdepth=(z-near)/(far−near)
$$
near值和far值是我们之前提供给投影矩阵设置可视平截头体的（见[坐标系统](https://learnopengl-cn.github.io/01 Getting started/08 Coordinate Systems/)）那个 *near* 和 *far* 值

(2)实际中并不使用线性的方程，而是采用非线性的，如下：
$$
Fdepth=(1/z−1/near)/(1/far−1/near)
$$
**这在z值很小的情况下提供了非常大的精度**

## 4.深度缓存的可视化

(1)内建gl_FragCoord向量的z值包含了那个特定片段的深度值。将这个深度值输出为颜色，我们可以显示场景中所有片段的深度值，可以根据片段的深度值返回一个颜色向量来完成这一工作：

```glsl
void main()
{
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
```

采用上述着色器程序，在物体离的越远时，颜色越白，反之，越接近原本色。

## 5.深度冲突

面与面距离太近导致的。。

# 十、模板测试

## 1.前言

(1)当片段着色器处理完一个片段之后，模板测试(Stencil Test)会开始执行，和深度测试一样，它也可能会丢弃片段。接下来，被保留的片段会进入深度测试，它可能会丢弃更多的片段。模板测试是根据又一个缓冲来进行的，它叫做模板缓冲(Stencil Buffer)，我们可以在渲染的时候更新它来获得一些很有意思的效果。

(2)一个模板缓冲中，（通常）每个模板值(Stencil Value)是8位的。所以每个像素/片段一共能有256种不同的模板值。我们可以将这些模板值设置为我们想要的值，然后当某一个片段有某一个模板值的时候，我们就可以选择丢弃或是保留这个片段了。

(3)模板缓冲操作允许我们在渲染片段时将模板缓冲设定为一个特定的值。通过在渲染时修改模板缓冲的内容，我们写入了模板缓冲。在同一个（或者接下来的）渲染迭代中，我们可以**读取**这些值，来决定丢弃还是保留某个片段

步骤：

- 启用模板缓冲的写入。
- 渲染物体，更新模板缓冲的内容。
- 禁用模板缓冲的写入。
- 渲染（其它）物体，这次根据模板缓冲的内容丢弃特定的片段。

启用**GL_STENCIL_TEST**来启用模板测试

```glsl
glEnable(GL_STENCIL_TEST);
```

和颜色和深度缓冲一样，你也需要在每次迭代之前清除模板缓冲。

```glsl
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
```

和深度测试的**glDepthMask**函数一样，模板缓冲也有一个类似的函数。glStencilMask允许我们设置一个位掩码(Bitmask)，它会与将要写入缓冲的模板值进行与(AND)运算。默认情况下设置的位掩码所有位都为1，不影响输出，但如果我们将它设置为`0x00`，写入缓冲的所有模板值最后都会变成0.这与深度测试中的**glDepthMask**(GL_FALSE)是等价的。

```glsl
glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
glStencilMask(0x00); // 每一位在写入模板缓冲时都会变成0（禁用写入）
```

## 2.模板函数

(1)和深度测试一样，我们对模板缓冲应该通过还是失败，以及它应该如何影响模板缓冲，也是有一定控制的。一共有两个函数能够用来配置模板测试：**glStencilFunc**和**glStencilOp**。

```c++
glStencilFunc(GLenum func, GLint ref, GLuint mask);
/*一共包含三个参数：
func：设置模板测试函数(Stencil Test Function)。这个测试函数将会应用到已储存的模板值上和glStencilFunc函数的ref值上。可用的选项有：GL_NEVER、GL_LESS、GL_LEQUAL、GL_GREATER、GL_GEQUAL、GL_EQUAL、GL_NOTEQUAL和GL_ALWAYS。它们的语义和深度缓冲的函数类似。
ref：设置了模板测试的参考值(Reference Value)。模板缓冲的内容将会与这个值进行比较。
mask：设置一个掩码，它将会与参考值和储存的模板值在测试比较它们之前进行与(AND)运算。初始情况下所有位都为1。*/
```

```c++
glStencilFunc(GL_EQUAL, 1, 0xFF);
/*
这会告诉OpenGL，只要一个片段的模板值等于(GL_EQUAL)参考值1，片段将会通过测试并被绘制，否则会被丢弃。

但是glStencilFunc仅仅描述了OpenGL应该对模板缓冲内容做什么，而不是我们应该如何更新缓冲。这就需要glStencilOp这个函数了。
*/
```

```c++
glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
/*一共包含三个选项，我们能够设定每个选项应该采取的行为：

sfail：模板测试失败时采取的行为。
dpfail：模板测试通过，但深度测试失败时采取的行为。
dppass：模板测试和深度测试都通过时采取的行为。*/
```

以上每个选项都可以选用以下的其中一种行为：

|     行为     |                        描述                        |
| :----------: | :------------------------------------------------: |
|   GL_KEEP    |                保持当前储存的模板值                |
|   GL_ZERO    |                  将模板值设置为0                   |
|  GL_REPLACE  |    将模板值设置为glStencilFunc函数设置的`ref`值    |
|   GL_INCR    |         如果模板值小于最大值则将模板值加1          |
| GL_INCR_WRAP |   与GL_INCR一样，但如果模板值超过了最大值则归零    |
|   GL_DECR    |         如果模板值大于最小值则将模板值减1          |
| GL_DECR_WRAP | 与GL_DECR一样，但如果模板值小于0则将其设置为最大值 |
|  GL_INVERT   |              按位翻转当前的模板缓冲值              |

默认情况下glStencilOp是设置为`(GL_KEEP, GL_KEEP, GL_KEEP)`的，所以不论任何测试的结果是如何，模板缓冲都会保留它的值。默认的行为不会更新模板缓冲，所以如果你想写入模板缓冲的话，你需要至少对其中一个选项设置不同的值。

所以，通过使用glStencilFunc和glStencilOp，我们可以精确地指定更新模板缓冲的时机与行为了，我们也可以指定什么时候该让模板缓冲通过，即什么时候片段需要被丢弃。

## 3.物体轮廓

(1)为物体创建轮廓的步骤如下：

- 在绘制（需要添加轮廓的）物体之前，将模板函数设置为GL_ALWAYS，每当物体的片段被渲染时，将模板缓冲更新为1。
- 渲染物体。
- 禁用模板写入以及深度测试。
- 将每个物体缩放一点点。
- 使用一个不同的片段着色器，输出一个单独的（边框）颜色。
- 再次绘制物体，但只在它们片段的模板值不等于1时才绘制。
- 再次启用模板写入和深度测试。

这个过程将每个物体的片段的模板缓冲设置为1，当我们想要绘制边框的时候，我们主要绘制放大版本的物体中模板测试通过的部分，也就是物体的边框的位置。我们主要使用模板缓冲丢弃了放大版本中属于原物体片段的部分。

(2)我们首先来创建一个很简单的片段着色器，它会输出一个边框颜色。我们简单地给它设置一个硬编码的颜色值，将这个着色器命名为shaderSingleColor：

```glsl
void main()
{
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
```

(3)我们只想给那两个箱子加上边框，所以我们让地板不参与这个过程。我们希望首先绘制地板，再绘制两个箱子（并写入模板缓冲），之后绘制放大的箱子（并丢弃覆盖了之前绘制的箱子片段的那些片段）。

我们首先启用模板测试，并设置测试通过或失败时的行为：

```c++
glEnable(GL_STENCIL_TEST);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
```

(4)如果其中的一个测试失败了，我们什么都不做，我们仅仅保留当前储存在模板缓冲中的值。如果模板测试和深度测试都通过了，那么我们希望将储存的模板值设置为参考值，参考值能够通过glStencilFunc来设置，我们之后会设置为1。

我们将模板缓冲清除为0，对箱子中所有绘制的片段，将模板值更新为1：

```c++
glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
glStencilMask(0xFF); // 启用模板缓冲写入
normalShader.use();
DrawTwoContainers();
```

(5)通过使用GL_ALWAYS模板测试函数，我们保证了箱子的每个片段都会将模板缓冲的模板值更新为1。因为片段永远会通过模板测试，在绘制片段的地方，模板缓冲会被更新为参考值。

现在模板缓冲在箱子被绘制的地方都更新为1了，我们将要绘制放大的箱子，但这次要禁用模板缓冲的写入：

```c++
glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilMask(0x00); // 禁止模板缓冲的写入
glDisable(GL_DEPTH_TEST);
shaderSingleColor.use(); 
DrawTwoScaledUpContainers();
```

(6)我们将模板函数设置为**GL_NOTEQUAL**，它会保证我们只绘制箱子上模板值不为1的部分，即只绘制箱子在之前绘制的箱子之外的部分。注意我们也禁用了深度测试，让放大的箱子，即边框，不会被地板所覆盖。

记得要在完成之后重新启用深度缓冲。

场景中物体轮廓的完整步骤会看起来像这样：

```c++
glEnable(GL_DEPTH_TEST);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

glStencilMask(0x00); // 记得保证我们在绘制地板的时候不会更新模板缓冲
normalShader.use();
DrawFloor()  

glStencilFunc(GL_ALWAYS, 1, 0xFF); 
glStencilMask(0xFF); 
DrawTwoContainers();

glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilMask(0x00); 
glDisable(GL_DEPTH_TEST);
shaderSingleColor.use(); 
DrawTwoScaledUpContainers();
glStencilMask(0xFF);
glEnable(GL_DEPTH_TEST);  
```



# 几、词汇表

```tex
OpenGL： 一个定义了函数布局和输出的图形API的正式规范。
GLAD： 一个扩展加载库，用来为我们加载并设定所有OpenGL函数指针，从而让我们能够使用所有（现代）OpenGL函数。
视口(Viewport)： 我们需要渲染的窗口。
图形管线(Graphics Pipeline)： 一个顶点在呈现为像素之前经过的全部过程。
着色器(Shader)： 一个运行在显卡上的小型程序。很多阶段的图形管道都可以使用自定义的着色器来代替原有的功能。
标准化设备坐标(Normalized Device Coordinates, NDC)： 顶点在通过在剪裁坐标系中剪裁与透视除法后最终呈现在的坐标系。所有位置在NDC下-1.0到1.0的顶点将不会被丢弃并且可见。
顶点缓冲对象(Vertex Buffer Object)： 一个调用显存并存储所有顶点数据供显卡使用的缓冲对象。
顶点数组对象(Vertex Array Object)： 存储缓冲区和顶点属性状态。
元素缓冲对象(Element Buffer Object，EBO)，也叫索引缓冲对象(Index Buffer Object，IBO)： 一个存储元素索引供索引化绘制使用的缓冲对象。
Uniform： 一个特殊类型的GLSL变量。它是全局的（在一个着色器程序中每一个着色器都能够访问uniform变量），并且只需要被设定一次。
纹理(Texture)： 一种包裹着物体的特殊类型图像，给物体精细的视觉效果。
纹理环绕(Texture Wrapping)： 定义了一种当纹理顶点超出范围(0, 1)时指定OpenGL如何采样纹理的模式。
纹理过滤(Texture Filtering)： 定义了一种当有多种纹素选择时指定OpenGL如何采样纹理的模式。这通常在纹理被放大情况下发生。
多级渐远纹理(Mipmaps)： 被存储的材质的一些缩小版本，根据距观察者的距离会使用材质的合适大小。
stb_image.h： 图像加载库。
纹理单元(Texture Units)： 通过绑定纹理到不同纹理单元从而允许多个纹理在同一对象上渲染。
向量(Vector)： 一个定义了在空间中方向和/或位置的数学实体。
矩阵(Matrix)： 一个矩形阵列的数学表达式。
GLM： 一个为OpenGL打造的数学库。
局部空间(Local Space)： 一个物体的初始空间。所有的坐标都是相对于物体的原点的。
世界空间(World Space)： 所有的坐标都相对于全局原点。
观察空间(View Space)： 所有的坐标都是从摄像机的视角观察的。
裁剪空间(Clip Space)： 所有的坐标都是从摄像机视角观察的，但是该空间应用了投影。这个空间应该是一个顶点坐标最终的空间，作为顶点着色器的输出。OpenGL负责处理剩下的事情（裁剪/透视除法）。
屏幕空间(Screen Space)： 所有的坐标都由屏幕视角来观察。坐标的范围是从0到屏幕的宽/高。
LookAt矩阵： 一种特殊类型的观察矩阵，它创建了一个坐标系，其中所有坐标都根据从一个位置正在观察目标的用户旋转或者平移。
欧拉角(Euler Angles)： 被定义为偏航角(Yaw)，俯仰角(Pitch)，和滚转角(Roll)从而允许我们通过这三个值构造任何3D方向。

颜色向量(Color Vector)：一个通过红绿蓝(RGB)分量的组合描绘大部分真实颜色的向量。一个物体的颜色实际上是该物体所不能吸收的反射颜色分量。
冯氏光照模型(Phong Lighting Model)：一个通过计算环境光，漫反射，和镜面光分量的值来估计真实光照的模型。
环境光照(Ambient Lighting)：通过给每个没有被光照的物体很小的亮度，使其不是完全黑暗的，从而对全局光照进行估计。
漫反射着色(Diffuse Shading)：一个顶点/片段与光线方向越接近，光照会越强。使用了法向量来计算角度。
法向量(Normal Vector)：一个垂直于平面的单位向量。
法线矩阵(Normal Matrix)：一个3x3矩阵，或者说是没有平移的模型（或者模型-观察）矩阵。它也被以某种方式修改（逆转置），从而在应用非统一缩放时，保持法向量朝向正确的方向。否则法向量会在使用非统一缩放时被扭曲。
镜面光照(Specular Lighting)：当观察者视线靠近光源在表面的反射线时会显示的镜面高光。镜面光照是由观察者的方向，光源的方向和设定高光分散量的反光度值三个量共同决定的。
冯氏着色(Phong Shading)：冯氏光照模型应用在片段着色器。
Gouraud着色(Gouraud shading)：冯氏光照模型应用在顶点着色器上。在使用很少数量的顶点时会产生明显的瑕疵。会得到效率提升但是损失了视觉质量。
GLSL结构体(GLSL struct)：一个类似于C的结构体，用作着色器变量的容器。大部分时间用来管理输入/输出/uniform。
材质(Material)：一个物体反射的环境光，漫反射，镜面光颜色。这些东西设定了物体所拥有的颜色。
光照属性(Light(properties))：一个光的环境光，漫反射，镜面光的强度。可以使用任何颜色值，对每一个冯氏分量(Phong Component)定义光源发出的颜色/强度。
漫反射贴图(Diffuse Map)：一个设定了每个片段中漫反射颜色的纹理图片。
镜面光贴图(Specular Map)：一个设定了每一个片段的镜面光强度/颜色的纹理贴图。仅在物体的特定区域显示镜面高光。
定向光(Directional Light)：只有一个方向的光源。它被建模为不管距离有多长所有光束都是平行而且其方向向量在整个场景中保持不变。
点光源(Point Light)：一个在场景中有位置的，光线逐渐衰减的光源。
衰减(Attenuation)：光随着距离减少强度减小的过程，通常使用在点光源和聚光下。
聚光(Spotlight)：一个被定义为在某一个方向上的锥形的光源。
手电筒(Flashlight)：一个摆放在观察者视角的聚光。
GLSL Uniform数组(GLSL Uniform Array)：一个uniform值数组。它的工作原理和C语言数组大致一样，只是不能动态分配内存。

```





## 附录、函数参数意义

### 1.glVertexAttribPointer()

```cpp
//链接顶点属性
void glVertexAttribPointer(	GLuint index,
GLint size,
GLenum type,
GLboolean normalized,
GLsizei stride,
const GLvoid * pointer);
//
index		指定要修改的通用顶点属性的索引
size		指定每个通用顶点属性的组件数量。必须是 1, 2, 3, 4。似乎是指颜色通道rgba
type		指定数组中每个组件的数据类型。符号常量GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, 和被和GL_UNSIGNED_INT接受。
normalized	对于，指定在访问glVertexAttribPointer定点数据值时是应将其标准化 ( GL_TRUE) 还是直接转换为定点值 ( )
stride		指定连续通用顶点属性之间的字节偏移量。如果stride为 0，则通用顶点属性被理解为紧密包装在数组中。初始值为0。
pointer		指定当前绑定到目标的缓冲区的数据存储中数组中第一个通用顶点属性的第一个组件的偏移量GL_ARRAY_BUFFER

```

### 2.glGenBuffers()

```cpp
unsigned int VBO;
glGenBuffers(1, &VBO)//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象：
```

### 3.glBindBuffer()

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);//glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
```

### 4.glBufferData()

```cpp
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中：
//lBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。第三个参数是我们希望发送的实际数据。

```

```tex
第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：

GL_STATIC_DRAW ：数据不会或几乎不会改变。
GL_DYNAMIC_DRAW：数据会被改变很多。
GL_STREAM_DRAW ：数据每次绘制时都会改变。
三角形的位置数据不会改变，每次渲染调用时都保持原样，所以它的使用类型最好是GL_STATIC_DRAW。如果，比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，这样就能确保显卡把数据放在能够高速写入的内存部分。

现在我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理。下面我们会创建一个顶点着色器和片段着色器来真正处理这些数据。现在我们开始着手创建它们吧
```

ㄈ