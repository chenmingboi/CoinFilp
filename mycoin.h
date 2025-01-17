#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include<QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyCoin(QWidget *parent = nullptr);
    //参数表示传入的是金币路径还是银币路径

    MyCoin(QString btnImg);

    //金币的属性
    int posX;
    int posY;
    bool flag;//正反标识



    //改变标志的方法
    void changeFlag();
    QTimer* timer1;//正面翻反面的定时器
    QTimer* timer2;//反面翻正面的定时器
    int min = 1, max = 8;

    //执行动画 标志
    bool isAnimation = false;

    //重写按下
    void mousePressEvent(QMouseEvent* e);

    //胜利标志
    bool isWin = false;

signals:
};

#endif // MYCOIN_H
