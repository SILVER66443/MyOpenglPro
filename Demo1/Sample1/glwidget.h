#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_4_Core>

class glwidget : public QOpenGLWidget, QOpenGLFunctions_4_4_Core {
    Q_OBJECT
public:
    explicit glwidget(QWidget *parent = nullptr);

    unsigned int VAO, VBO;

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
};

#endif // GLWIDGET_H
