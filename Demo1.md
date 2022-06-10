## 1：缓冲对象

### 顶点缓冲对象 (Vertex Buffer Object：VBO)：

> 作用：通过顶点缓冲对象存储顶点数据。
>
> 缓冲类型：GL_ARRAY_BUFFER



### 顶点数组对象（Vertex Array Object：VAO）：

> 作用：存放顶点结构的定义（不存放数据）。
>
> 缓冲类型：类型唯一（不指定类型）。



### 索引缓冲对象（Element Buffer Object：EBO）：

> 作用：存放缓冲索引（用于读取VBO）。
>
> 缓冲类型：GL_ELEMENT_ARRAY_BUFFER



### 代码

```c++
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader/Shader.h>

int main() {
	//* 创建一个glfw窗口对象
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	//* 初始化对象（分辨率，窗口名，null，null）
	window = glfwCreateWindow(1024, 768, "chap1", NULL, NULL);

	//* 获得上下文休息
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load glad!\n";
		return -1;
	}

	//* 创建shader对象
	Shader shader(
		"./src/shaderDir/vertexShader.vert",
		"./src/shaderDir/fragShader.frag"
	);

	//* 顶点数据和纹理数据
	float vertices[] = {
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f,   0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	//* 使用顶点数组对象和顶点缓冲对象进行绘制
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//* 数据传入缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//* 传入数据的属性定义
	//! 顶点属性参数（（0：顶点属性、1：纹理属性、2：法线），顶点数据的组件个数[1,4]，是否归一化（减少cpu传数据的带宽），元素间隔 ）
	//! 开启顶点属性（对应顶点属性、纹理、法线，也就是shader中的location，但shader的location一般在shader中指定）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//* 设置线框模式
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, (GLFW_KEY_ESCAPE == GLFW_PRESS))) {
			glfwSetWindowShouldClose(window, true);
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.actShader();

		//* 绑定顶点缓冲对象
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//* 删除缓冲对象
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	//获取着色器顶点个数
	//int count;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &count);
	//std::cout << count << std::endl;

	return 0;
}

```



```C++

//shader.h
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Shader {
public:
	unsigned int ID;

	//* constructor
	Shader(const char* vertexPath, const char* fragPath) {
		//* 创建对象用于读取数据
		string   vertexCode, fragCode;
		ifstream vShaderFile, fShaderFile;
		stringstream vShaderStream, fShaderStream;

		//* 打开文件建立流输入数据
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragPath);

		//* 将ifstream读入stringstream对象
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//* 关闭文件
		vShaderFile.close();
		fShaderFile.close();

		//* 将流转换为string类型对象
		vertexCode = vShaderStream.str();
		fragCode = fShaderStream.str();

		//* 将string类型对象转换为字符串常量（char*）
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragCode.c_str();

		//* 创建shader对象
		unsigned int vertex, fragment;

		//* 初始化shader、指向shader代码、编译shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//* 附着shader，link
		this->ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//* 删除shader对象

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//* activate shader
	void actShader() {
		glUseProgram(ID);
	}

private:
	void checkCompileErrors(unsigned int shader, string type) {
		//! 检测shader编译错位的部分
		int  suc;
		char infoLog[1024];

		if( "PROGRAM" != type ) {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &suc);
			if( !suc ) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
					<< type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &suc);
			if( !suc ) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
					<< type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		}
	}
};

#endif

```

## 2 ：窗口控件：

一个OpenGL窗口控件需要重载的函数：

```c++
virtual void initializeGL(); 
virtual void resizeGL(int w, int h);
virtual void paintGL();
```
***
## 3：Shader相关知识

**概念：**shader程序的运行是基于渲染管线。

不同shader之间的信息交互通过in变量和out变量来做。

程序格式：

```
#version 330 core                      # 版本号
layout (location = 0) in vec3 aPos;    # in变量
layout (location = 1) in vec3 aColor;

out vec3 ourColor;                     # out变量

void main()
{
	# 处理输出数据
    gl_Position = vec4(aPos, 1.0);
    # 处理输出数据
    ourColor = aColor;
}
```

版本号 -> 输入数据声明 -> 输出数据声明

### 3.1 **shader**在opengl中的使用方法：

>1：创建shader对象（*unsigned int*）
>
>> 大部分的缓冲对象都是无符号整型
>
>2：初始化shader（*glCreateShader()*）
>
>>glCreateShader(*GLenum shaderType*);
>>
>>参数：
>>
>>`GL_COMPUTE_SHADER`：
>>
>>`GL_VERTEX_SHADER`：顶点着色器
>>
>>`GL_TESS_CONTROL_SHADER`
>>
>> `GL_TESS_EVALUATION_SHADER`
>>
>>`GL_GEOMETRY_SHADER`
>>
>>`GL_FRAGMENT_SHADER`：片段着色器
>>
>>
>
>3：链接glsl代码（*glShaderSource()*）
>
>>glShaderSource(GLuint shader,GLsize count,const GLchar *string,const GLint *length)
>>
>>*`shader`*:shader对象
>>
>>*`count`*:shader个数
>>
>>*`string`*:shader代码
>>
>>*`length`*:字符串长度
>
>4：编译shader代码（*glCompileShader()*）
>
>>glCompileShader(GLuint shader)
>>
>>*`shader`*：要编译的shader。
>
>5：创建program（*unsigned int，glCreateProgram()*）
>
>>创建shader program。
>
>6：附着shader对象到shader program（*glAttachShader()*）
>
>>void glAttachShader(GLuint program,GLuint shader)
>>
>>*`program`*：附着的program。
>>
>>*`shader`*：需要附着的着色器。
>
>7：链接shader program(*glLinkProgram()*)
>
>>void glLinkProgram(GLuint program);
>>
>>*`program`*：需要链接的program。
>
>8：删除shader和使用shader
>
>>void glDeleteShader(GLuint shader)
>>
>>`GLuint shader`删除已经附着后的shader对象
>>
>>void glUseProgram(GLuint program)
>>
>>`GLuint shader`使用shader program

### 3.2 shader代码debug



### 3.3：shader类封装方式：

```c++
//shader.h
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Shader {
public:
	unsigned int ID;

	//* constructor
	Shader(const char* vertexPath, const char* fragPath) {
		//* 创建对象用于读取数据
		string   vertexCode, fragCode;
		ifstream vShaderFile, fShaderFile;
		stringstream vShaderStream, fShaderStream;

		//* 打开文件建立流输入数据
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragPath);

		//* 将ifstream读入stringstream对象
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//* 关闭文件
		vShaderFile.close();
		fShaderFile.close();

		//* 将流转换为string类型对象
		vertexCode = vShaderStream.str();
		fragCode = fShaderStream.str();

		//* 将string类型对象转换为字符串常量（char*）
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragCode.c_str();

		//* 创建shader对象
		unsigned int vertex, fragment;

		//* 初始化shader、指向shader代码、编译shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//* 附着shader，link
		this->ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//* 删除shader对象

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//* activate shader
	void actShader() {
		glUseProgram(ID);
	}

private:
	void checkCompileErrors(unsigned int shader, string type) {
		//! 检测shader编译错位的部分
		int  suc;
		char infoLog[1024];

		if( "PROGRAM" != type ) {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &suc);
			if( !suc ) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
					<< type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &suc);
			if( !suc ) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
					<< type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- "
					<< std::endl;
			}
		}
	}
};

#endif

```

