#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include<mystartbutton.h>
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include<QSoundEffect>
#include<QUrl>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    //配置选择关卡场景
    this->setFixedSize(320, 588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar* bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu* startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction* quitAction = startMenu->addAction("退出");

    //点击退出实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });



    //选择关卡的音效
    QSoundEffect* chooseSound = new QSoundEffect(this);
    chooseSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));
    //返回按钮的音效
    QSoundEffect* backSound = new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));

    //返回按钮
    MyStartButton* backBtn = new MyStartButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    //点击返回
    connect(backBtn, &MyStartButton::clicked, [=](){
        qDebug()<< "点击了返回";


        //播放返回按钮的音效
        backSound->play();


        //告诉主场景 我已返回 主场景监听ChooseLevelScene的返回按钮
        //延时返回
        QTimer::singleShot(100, this, [=](){
            emit this->chooseSceneBack();
        });
    });




    //创建选择关卡的按钮
    for(int i = 0;i < 20;i++)
    {
        MyStartButton* menuBtn = new MyStartButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + i % 4 * 70, 130 +  i / 4 * 70);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyStartButton::clicked,[=](){
            QString str = QString("您选择的是第 %1 关").arg(i+1);
            qDebug() << str.toUtf8().data();

            //播放选择关卡的音效
            chooseSound->play();


            //进入到游戏场景
            this->hide();//将选关场景隐藏掉
            play = new PlayScene(i+1);//创建游戏场景

            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());

            play->show();//显示游戏场景

            connect(play, &PlayScene::chooseSceneBack, [=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play = NULL;
            });

        });

        QLabel* label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 + i % 4 * 70, 130 +  i / 4 * 70);

        //设置label上的对齐方式 设置水平居中和垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        //设置让鼠标进行穿透 51号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent*)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width() - pix.width()) * 0.5, 30, pix.width(),pix.height(), pix);


}
