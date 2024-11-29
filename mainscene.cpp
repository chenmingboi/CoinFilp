#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPainter>
#include<mystartbutton.h>
#include<QDebug>
#include<QTimer>
#include<QMediaPlayer>
#include<QSoundEffect>
#include<QUrl>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(320, 588);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币主场景");

    //退出按钮实现
    connect(ui->actionquit, &QAction::triggered, [=](){
        this->close();
    });




    //准备开始按钮的音效
    QSoundEffect* startSound = new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));

    //设置循环 如果是QSoundEffect::Infinite则表示无限循环
    // startSound->setLoopCount(QSoundEffect::Infinite);
    // startSound->play();



    //开始按钮
    MyStartButton* startButton = new MyStartButton(":/res/MenuSceneStartButton.png");
    startButton->setParent(this);
    startButton->move(this->width() * 0.5 - startButton->width()*0.5, this->height()*0.7);

    //实例化选择关卡场景
    chooseScene = new ChooseLevelScene;

    //监听选择关卡的返回按钮的信号
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, this, [=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();
        this->show();
    });

    connect(startButton, &MyStartButton::clicked,[=](){
        qDebug() << "点击开始按钮";

        //播放开始音效资源
        startSound->play();

        //弹跳
        startButton->zoomDown();
        startButton->zoomUp();


        //延时进入到选择关卡中
        QTimer::singleShot(200, this, [=](){

            //设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());


            //进入到选择关卡的场景中
            //自身隐藏
            this->hide();
            //显示选择关卡场景
            chooseScene->show();

        });
    });

}


void MainScene::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);//把图片拉伸成设置窗口的宽度和高度

    //画背景上的图标
    pix.load(":/res/Title.png");
    //对背景图标进行缩放
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);



}

MainScene::~MainScene()
{
    delete ui;
}
