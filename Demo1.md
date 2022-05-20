# OpenGL

## 0：对象：

​	一个对象指的是一些选项的集合。

```c++
/* 关于上下文，改状态机，用状态机 */
// 创建一个对象
GLuint objectid = 0;
glGenObject(1,&objectid);
// 绑定上下文，意味用上面的对象记录相应功能的上下文
glBindObject(GL_WINDOW_TARGET,objectid);
// 设置相关功能的一些选项
glSetObjectOption(GL_WINDOW_TARGET,GL_OPTION_WINDOW_WIDTH,800);
glSetObjectOption(GL_WINDOW_TARGET,GL_OPTION_WINDOW_HEIGHT,600);
// 上下文对象恢复默认（解绑）
glBindObject(GL_WINDOW_TARGET,0);

```

一个OpenGL窗口控件需要重载的函数：

```c++
virtual void initializeGL(); 
virtual void resizeGL(int w, int h);
virtual void paintGL();
```

### 顶点缓冲对象 (Vertex Buffer Object：VBO)：

**作用：**通过定点缓冲对象存储顶点数据。

**缓冲类型：**GL_ARRAY_BUFFER



### 顶点数组对象（Vertex Array Object：VAO）：

**作用：**存放顶点结构的定义。

类型唯一。



### 索引缓冲对象（Element Buffer Object：EBO）：

**作用：**存放缓冲索引。

类型：GL_ELEMENT_ARRAY_BUFFER

### 代码

```C++
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


void glWidget::initializeGL() {
    //TODO 初始化窗口
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



***