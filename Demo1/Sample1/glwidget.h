#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>

class glwidget : public QOpenGLWidget, QOpenGLFunctions_4_4_Core {
    Q_OBJECT
public:
    enum Shape { None, Rect, Circle, Triangle };

    explicit glwidget(QWidget *parent = nullptr);
    ~glwidget();

    void drawShape(Shape shape);
    void setPolygon(bool isPolygon);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    Shape m_shape;
    QOpenGLShaderProgram shaderProgram;
};

#endif // GLWIDGET_H
