#include "playscene.h"
#include<QDebug>
#include<QMenuBar>
#include<QPainter>
#include<mystartbutton.h>
#include<QTimer>
#include<QLabel>
#include<QFont>
#include<QString>
#include<mycoin.h>
#include<dataconfig.h>
#include<QPropertyAnimation>
#include<QSoundEffect>
#include<QUrl>

// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}

PlayScene::PlayScene(int levelNum)
{
    QString str = QString("进入了第 %1 关").arg(levelNum);
    qDebug() << str.toUtf8().data();
    this->levelIndex = levelNum;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320, 588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币场景");

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



    //返回按钮的音效
    QSoundEffect* backSound = new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));
    //翻金币音效
    QSoundEffect* flipSound = new QSoundEffect(this);
    flipSound->setSource(QUrl::fromLocalFile(":/res/ConFlipSound.wav"));
    //胜利音效
    QSoundEffect* winSound = new QSoundEffect(this);
    winSound->setSource(QUrl::fromLocalFile(":/res/LevelWinSound.wav"));
    winSound->setLoopCount(QSoundEffect::Infinite);

    \

    //返回按钮
    MyStartButton* backBtn = new MyStartButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    //点击返回
    connect(backBtn, &MyStartButton::clicked, [=](){
        qDebug()<< "点击了返回";


        //播放返回音效
        backSound->play();


        // 告诉主场景 我已返回 主场景监听ChooseLevelScene的返回按钮
        // 延时返回
        QTimer::singleShot(100, this, [=](){
            emit this->chooseSceneBack();
        });
    });


    //显示当前关卡数
    QLabel* label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString levelStr = QString("Level: %1").arg(this->levelIndex);
    //将字体设置到标签空间中
    label->setFont(font);
    label->setText(levelStr);
    label->setGeometry(30, this->height()-50, 120, 50);



    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 4;j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }



    //胜利图片的显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0, 0, tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width())*0.5, -tmpPix.height());//初始位置在窗口的上方，未胜利的时候看不到




    //显示金币的背景图案
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 4;j++)
        {
            //绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            QLabel* label = new QLabel;
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57+i*50, 200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }
            else if(this->gameArray[i][j] == 0)
            {
                str = ":/res/Coin0008.png";
            }
            MyCoin* coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50, 204+j*50);


            //给金币的属性复制
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];//0为反面， 1为正面



            //将金币放入到金币的二维数组中 以便后期的维护
            coinBtn[i][j] = coin;

            //点击金币进行翻转
            connect(coin, &MyCoin::clicked, [=](){
                //点击按钮 将所有的按钮先都禁用
                for(int i = 0;i < 4;i++)
                {
                    for(int j = 0;j < 4;j++)
                    {
                        this->coinBtn[i][j]->isWin = true;
                    }
                }

                //播放翻金币的音效
                flipSound->play();


                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;

                //反转周围硬币
                QTimer::singleShot(200, this, [=](){ //实现延时处理
                    if(coin->posX+1 <=3)//周围右侧翻转条件
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                    }
                    if(coin->posX-1 >= 0)//左侧
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                    }
                    if(coin->posY+1 <= 3)//下侧
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                    }
                    if(coin->posY-1 >= 0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                    }


                    //翻完周围金币后 将所有的硬币解除禁用
                    for(int i = 0;i < 4;i++)
                    {
                        for(int j = 0;j < 4;j++)
                        {
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }


                    //判断是否胜利
                    this->isWin = true;
                    for(int i = 0;i < 4;i++)
                    {
                        for(int j = 0;j < 4;j++)
                        {
                            if(coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                        if(this->isWin == false) break;
                    }
                    if(this->isWin == true)
                    {
                        //胜利了
                        qDebug() << "胜利";

                        //播放胜利音效
                        winSound->play();


                        //将所有按钮的胜利标志改为true 如果再次点击按钮，直接return 不做响应
                        for(int i = 0;i < 4;i++)
                        {
                            for(int j = 0;j < 4;j++)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                        }


                        //将胜利的图片移动下来
                        QPropertyAnimation* animation = new QPropertyAnimation(winLabel, "geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(), winLabel->y()+114, winLabel->width(), winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();

                    }

                });

            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent* )
{
    //创建标题
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);
}
