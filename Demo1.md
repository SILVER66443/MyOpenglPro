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

```C++
// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader/Shader.h>

int main() {
    GLFWwindow *window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(1024, 768, "chap1", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load glad!\n";
        return -1;
    }

    Shader shader(
        "./src/chap_1/shaderFile/vertexShader.vert",
        "./src/chap_1/shaderFile/fragShader.frag"
        );

    float vertices[] = {
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f,   0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, (GLFW_KEY_ESCAPE == GLFW_PRESS))) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.actShader();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

```
<<<<<<< HEAD

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
    Shader(const char *vertexPath, const char *fragPath) {
        /* read file */
        string   vertexCode, fragCode;
        ifstream vShaderFile, fShaderFile;
        stringstream vShaderStream, fShaderStream;

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragPath);

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragCode = fShaderStream.str();

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragCode.c_str();

        //* compile
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        //* shader program
        this->ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // check error

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    //* activate shader
    void actShader() {
        glUseProgram(ID);
    }

private:
    void checkCompileErrors(unsigned int shader, string type) {
        int  suc;
        char infoLog[1024];

        if ("PROGRAM" != type) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &suc);
            if (!suc) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: " 
                    << type << "\n" << infoLog 
                    << "\n -- --------------------------------------------------- -- " 
                    << std::endl;
            }
        }else  {
            glGetProgramiv(shader, GL_LINK_STATUS, &suc);
            if (!suc) {
                glGetProgramInfoLog(shader,1024,NULL,infoLog);
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



=======
***
>>>>>>> 2b305a1ccf86e62634107090c4373c4b094a894b
## 2 ：窗口控件：

一个OpenGL窗口控件需要重载的函数：

```c++
virtual void initializeGL(); 
virtual void resizeGL(int w, int h);
virtual void paintGL();
```
***
## 3：Shader相关知识

