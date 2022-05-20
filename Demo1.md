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
// glWidget.h
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>

class glWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core {
    Q_OBJECT
public:
    explicit glWidget(QWidget *parent = nullptr);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
};

#endif // GLWIDGET_H

```



```C++
// glWidget.cpp
#include "glwidget.h"

// 缓冲对象声明
GLuint VAO, VBO, EBO;
// 存放顶点信息
float vertices[] = { 0.5f,   0.5f, 0.0f,
                     -0.5f,  0.5f, 0.0f,
                     -0.5f, -0.5f, 0.0f,
                     0.5f,  -0.5f, 0.0f };
// 存放索引信息
int index[] = { 0, 1, 3,
                1, 2, 3 };

glWidget::glWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

// 初始化窗口
void glWidget::initializeGL() {
    initializeOpenGLFunctions();

    /* 创建对象 */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /* 绑定对象 */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /* 在缓冲区生成一个区域用于后续传入顶点数据 */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    /* 指定显存缓冲区数据读取方式 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    /* 解绑 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 切换绘制模式 */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void glWidget::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void glWidget::paintGL() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 使用索引缓冲对象的方式输出数据
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // 当没有绑定EVO的时候使用
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, index);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
}

```

## 2 ：窗口控件：

一个OpenGL窗口控件需要重载的函数：

```c++
virtual void initializeGL(); 
virtual void resizeGL(int w, int h);
virtual void paintGL();
```

