#ifndef MYSTARTBUTTON_H
#define MYSTARTBUTTON_H

#include <QPushButton>

class MyStartButton : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyStartButton(QWidget *parent = nullptr);

    //构造函数，参数1正常显示图片路径，参数2按下后显示图片的路径
    MyStartButton(QString normalImg, QString pressImg = "");
    //成员属性 保存用户传入的默认显示路径，和按下后图片的路径
    QString normalImgPath;
    QString pressImgPath;


    //弹跳特效
    void zoomDown();//向下跳
    void zoomUp();//向上跳

    //重写按钮按下 和释放事件
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

signals:
};

#endif // MYSTARTBUTTON_H
