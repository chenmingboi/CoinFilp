#include "mystartbutton.h"
#include<QDebug>
#include<QPropertyAnimation>


// MyStartButton::MyStartButton(QWidget *parent)
//     : QPushButton{parent}
// {}
MyStartButton::MyStartButton(QString normalImg, QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if(!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width(), pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(), pix.height()));
}

//向下跳和向上跳也可以封装为一个函数
//向下跳
void MyStartButton::zoomDown()
{
    //创建动画对象
    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
    //设置动画的时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //结束位置
    animation->setEndValue((QRect(this->x(), this->y()+10, this->width(), this->height())));

    //设置弹跳效果（即缓和曲线）
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}

//向上跳
void MyStartButton::zoomUp()
{
    //创建动画对象
    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
    //设置动画的时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(), this->y()+10, this->width(), this->height()));
    //结束位置
    animation->setEndValue((QRect(this->x(), this->y(), this->width(), this->height())));

    //设置弹跳效果（即缓和曲线）
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}


void MyStartButton::mousePressEvent(QMouseEvent* e)
{
    if(this->pressImgPath != "")//传入的按下图片不为空表明要切换图片
    {
        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }

        //设置图片固定大小
        this->setFixedSize(pix.width(), pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width(), pix.height()));
    }

    //让父类执行其他内容
    return QPushButton::mousePressEvent(e);

}


void MyStartButton::mouseReleaseEvent(QMouseEvent* e)
{
    if(this->pressImgPath != "")//传入的按下图片不为空表明要切换图片, 切换成初始图片
    {
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }

        //设置图片固定大小
        this->setFixedSize(pix.width(), pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width(), pix.height()));
    }

    //让父类执行其他内容
    return QPushButton::mouseReleaseEvent(e);

}

