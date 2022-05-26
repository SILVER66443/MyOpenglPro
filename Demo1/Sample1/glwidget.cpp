#include "glwidget.h"

unsigned int VAO, VBO, EBO;

int index[] = { 0, 1, 3,
                1, 2, 3 };

float vertices[] = { 0.5f,   0.5f, 0.0f,
                     -0.5f,  0.5f, 0.0f,
                     -0.5f, -0.5f, 0.0f,
                     0.5f,  -0.5f, 0.0f };

glwidget::glwidget(QWidget *parent) : QOpenGLWidget(parent) {
}


glwidget::~glwidget() {
    makeCurrent();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    doneCurrent();
}


void glwidget::drawShape(Shape shape) {
    this->m_shape = shape;
    update();
}


void glwidget::setPolygon(bool isPolygon) {
    makeCurrent();
    if (isPolygon) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    update();
    doneCurrent();
}


void glwidget::initializeGL() {
    // 初始化OpenGL窗口
    initializeOpenGLFunctions();

    // 生成 VAO VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 将对象绑定到相关功能
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 为当前绑定到目标的缓冲区对象创建一个新的数据存储区域
    // 如果数据不为NULL将使用参数中的指针初始化存储区域
    // 绑定target、大小、数据指针、绘制方式
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    // 显卡解析显存缓冲的格式
    // 起始下标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    // 开启VAO管理的属性值
    glEnableVertexAttribArray(0);

    // 暂时解绑缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void glwidget::resizeGL(int w, int h) {
}


void glwidget::paintGL() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    switch (this->m_shape) {
    case Rect:
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        break;

    default:
        break;
    }
    // glDrawArrays(GL_TRIANGLES, 0, 6);
}
