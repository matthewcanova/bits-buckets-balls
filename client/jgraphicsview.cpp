#include "jgraphicsview.h"
#include "Box2D/Box2D.h"

#include <QGraphicsPixmapItem>
#include <iostream>

JGraphicsView::JGraphicsView(QWidget* parentPtr) :
    QGraphicsView(parentPtr)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(QRect(0, 0, 1200, 300));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setStyleSheet("background: transparent");

    this->setScene(this->scene);

    sim = new WorldSim(1200, 300);
    // set queue up

    sim->moveToThread(&worldThread);

    // Allows signals/slots to finish processing once thread is closed.
    connect(&worldThread, &QThread::finished, sim, &QObject::deleteLater);
    connect(sim, &WorldSim::updatePosition, this, &JGraphicsView::updateSprite);
    connect(sim, &WorldSim::contactUpdateToJGraphicsView, this, &JGraphicsView::perpetuateContactUpdate);
    connect(this, &JGraphicsView::startSim, sim, &WorldSim::startSim);
    connect(this, &JGraphicsView::createCup, sim, &WorldSim::createCup);
    connect(this, &JGraphicsView::createSquare, sim, &WorldSim::createSquare);
    connect(this, &JGraphicsView::createCircle, sim, &WorldSim::createCircle);
    connect(this, &JGraphicsView::createBigBucket, sim, &WorldSim::createBigBucket);
    connect(this, &JGraphicsView::mousePress, sim, &WorldSim::mousePress);
    connect(this, &JGraphicsView::mouseRelease, sim, &WorldSim::mouseRelease);
    connect(this, &JGraphicsView::mouseMove, sim, &WorldSim::mouseMove);

    worldThread.start();
}

JGraphicsView::~JGraphicsView()
{
    delete scene;
    worldThread.quit();
    worldThread.wait();
}

void JGraphicsView::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit startSim();
}

void JGraphicsView::mousePressEvent(QMouseEvent *eventPtr)
{
    emit mousePress(eventPtr);
}


void JGraphicsView::mouseReleaseEvent(QMouseEvent *eventPtr)
{
    emit mouseRelease(eventPtr);
}

void JGraphicsView::mouseMoveEvent(QMouseEvent *eventPtr)
{
    emit mouseMove(eventPtr);
}

void JGraphicsView::perpetuateContactUpdate(int bucket, int ball, bool isActive)
{
    emit updateContactToLevelView(bucket, ball, isActive);
}

void JGraphicsView::updateSprite(QGraphicsPixmapItem *sprite, QPointF pos, double angle)
{
    sprite->setPos(pos);
    sprite->setRotation(angle);
}

void JGraphicsView::setupLevel(int level, Type type, bool existing)
{
    //Create all the buckets and balls if this is the first round of the game
    if  (!existing)
    {
        if (level != 3)
            createBucketsAndBalls(true);
        else
            createBucketsAndBalls(false);
    }

    //game is existing
    else if (existing)
    {
        //clear existing items
        sim->reset();
        if (level == 3)
            emit createBigBucket(&bigBucketData, bigBucketStart, false);
        else
            emit createBigBucket(&bigBucketData, bigBucketStart, true);
    }

    if (level != 3)
    {

        if (type == easy)
        {
            if (existing)
            {
                //reset positions for buckets
                cupStart1.setX(890);
                cupSprite1->setPos(cupStart1);
                cupStart2.setX(700);
                cupSprite2->setPos(cupStart2);
                cupStart3.setX(510);
                cupSprite3->setPos(cupStart3);
                cupStart4.setX(320);
                cupSprite4->setPos(cupStart4);
            }

            //hide other buckets and balls
            cupSprite3->show();
            cupSprite4->show();
            ballSprite3->show();
            ballSprite4->show();
            cupSprite5->hide();
            cupSprite6->hide();
            cupSprite7->hide();
            cupSprite8->hide();
            ballSprite5->hide();
            ballSprite6->hide();
            ballSprite7->hide();
            ballSprite8->hide();
        }

        if (type == medium)
        {
            //reset coordinates
            cupStart1.setX(925);
            cupSprite1->setPos(cupStart1);
            cupStart2.setX(800);
            cupSprite2->setPos(cupStart2);
            cupStart3.setX(675);
            cupSprite3->setPos(cupStart3);
            cupStart4.setX(550);
            cupSprite4->setPos(cupStart4);
            cupStart5.setX(425);
            cupSprite5->setPos(cupStart5);
            cupStart6.setX(300);
            cupSprite6->setPos(cupStart6);

            cupSprite5->show();
            cupSprite6->show();
            cupSprite7->hide();
            cupSprite8->hide();
            ballSprite5->show();
            ballSprite6->show();
            ballSprite7->hide();
            ballSprite8->hide();
        }

        if (type == hard)
        {
            //reset the coordinates
            cupStart1.setX(945);
            cupSprite1->setPos(cupStart1);
            cupStart2.setX(850);
            cupSprite2->setPos(cupStart2);
            cupStart3.setX(755);
            cupSprite3->setPos(cupStart3);
            cupStart4.setX(660);
            cupSprite4->setPos(cupStart4);
            cupStart5.setX(565);
            cupSprite5->setPos(cupStart5);
            cupStart6.setX(470);
            cupSprite6->setPos(cupStart6);
            cupStart7.setX(375);
            cupSprite7->setPos(cupStart7);
            cupStart8.setX(280);
            cupSprite8->setPos(cupStart8);

            //show all buckets and balls
            cupSprite5->show();
            cupSprite6->show();
            ballSprite5->show();
            ballSprite6->show();
            cupSprite7->show();
            cupSprite8->show();
            ballSprite7->show();
            ballSprite8->show();
        }

        //Create the objects in the world
        emit createCup(&cup1Data, cupStart1);
        emit createCup(&cup2Data, cupStart2);
        emit createCup(&cup3Data, cupStart3);
        emit createCup(&cup4Data, cupStart4);
        emit createCircle(&ball1Data, ballStart1);
        emit createCircle(&ball2Data, ballStart2);
        emit createCircle(&ball3Data, ballStart3);
        emit createCircle(&ball4Data, ballStart4);

        if (type != easy)
        {
            emit createCup(&cup5Data, cupStart5);
            emit createCup(&cup6Data, cupStart6);
            emit createCircle(&ball5Data, ballStart5);
            emit createCircle(&ball6Data, ballStart6);
        }

        if (type == hard)
        {
            emit createCup(&cup7Data, cupStart7);
            emit createCup(&cup8Data, cupStart8);
            emit createCircle(&ball7Data, ballStart7);
            emit createCircle(&ball8Data, ballStart8);
        }

        bigBucket->show();
        bigBucket2->hide();
        hexBall0->hide();
        hexBall1->hide();
        hexBall2->hide();
        hexBall3->hide();
        hexBall4->hide();
        hexBall5->hide();
        hexBall6->hide();
        hexBall7->hide();
        hexBall8->hide();
        hexBall9->hide();
        hexBallA->hide();
        hexBallB->hide();
        hexBallC->hide();
        hexBallD->hide();
        hexBallE->hide();
        hexBallF->hide();
        hexBall0Dup->hide();
        hexBall1Dup->hide();
        hexBall2Dup->hide();
        hexBall3Dup->hide();
        hexBall4Dup->hide();
        hexBall5Dup->hide();
        hexBall6Dup->hide();
        hexBall7Dup->hide();
        hexBall8Dup->hide();
        hexBall9Dup->hide();
        hexBallADup->hide();
        hexBallBDup->hide();
        hexBallCDup->hide();
        hexBallDDup->hide();
        hexBallEDup->hide();
        hexBallFDup->hide();
    }

    //level is 3
    else if (level == 3)
    {
        cupSprite2->hide();
        cupSprite3->hide();
        cupSprite4->hide();
        cupSprite5->hide();
        cupSprite6->hide();
        cupSprite7->hide();
        cupSprite8->hide();
        ballSprite1->hide();
        ballSprite2->hide();
        ballSprite3->hide();
        ballSprite4->hide();
        ballSprite5->hide();
        ballSprite6->hide();
        ballSprite7->hide();
        ballSprite8->hide();
        bigBucket->hide();
        bigBucket2->show();

        if (type == easy)
        {
            cupStart1.setX(615);
            cupSprite1->setPos(cupStart1);
            emit createCup(&cup1Data, cupStart1);

            hexBall0Dup->hide();
            hexBall1Dup->hide();
            hexBall2Dup->hide();
            hexBall3Dup->hide();
            hexBall4Dup->hide();
            hexBall5Dup->hide();
            hexBall6Dup->hide();
            hexBall7Dup->hide();
            hexBall8Dup->hide();
            hexBall9Dup->hide();
            hexBallADup->hide();
            hexBallBDup->hide();
            hexBallCDup->hide();
            hexBallDDup->hide();
            hexBallEDup->hide();
            hexBallFDup->hide();
        }
        if (type == hard)
        {
            cupStart2.setX(425);
            cupSprite2->setPos(cupStart2);
            cupSprite2->show();
            cupSprite1->show();
            cupStart1.setX(800);
            cupSprite1->setPos(cupStart1);

            hexBall0Dup->show();
            hexBall1Dup->show();
            hexBall2Dup->show();
            hexBall3Dup->show();
            hexBall4Dup->show();
            hexBall5Dup->show();
            hexBall6Dup->show();
            hexBall7Dup->show();
            hexBall8Dup->show();
            hexBall9Dup->show();
            hexBallADup->show();
            hexBallBDup->show();
            hexBallCDup->show();
            hexBallDDup->show();
            hexBallEDup->show();
            hexBallFDup->show();

            emit createCup(&cup1Data, cupStart1);
            emit createCup(&cup2Data, cupStart2);
            emit createCircle(&hex0DataDup, hexStart0);
            emit createCircle(&hex1DataDup, hexStart1);
            emit createCircle(&hex2DataDup, hexStart2);
            emit createCircle(&hex3DataDup, hexStart3);
            emit createCircle(&hex4DataDup, hexStart4);
            emit createCircle(&hex5DataDup, hexStart5);
            emit createCircle(&hex6DataDup, hexStart6);
            emit createCircle(&hex7DataDup, hexStart7);
            emit createCircle(&hex8DataDup, hexStart8);
            emit createCircle(&hex9DataDup, hexStart9);
            emit createCircle(&hexADataDup, hexStartA);
            emit createCircle(&hexBDataDup, hexStartB);
            emit createCircle(&hexCDataDup, hexStartC);
            emit createCircle(&hexDDataDup, hexStartD);
            emit createCircle(&hexEDataDup, hexStartE);
            emit createCircle(&hexFDataDup, hexStartF);
        }

       hexBall0->show();
       hexBall1->show();
       hexBall2->show();
       hexBall3->show();
       hexBall4->show();
       hexBall5->show();
       hexBall6->show();
       hexBall7->show();
       hexBall8->show();
       hexBall9->show();
       hexBallA->show();
       hexBallB->show();
       hexBallC->show();
       hexBallD->show();
       hexBallE->show();
       hexBallF->show();

       emit createCircle(&hex0Data, hexStart0);
       emit createCircle(&hex1Data, hexStart1);
       emit createCircle(&hex2Data, hexStart2);
       emit createCircle(&hex3Data, hexStart3);
       emit createCircle(&hex4Data, hexStart4);
       emit createCircle(&hex5Data, hexStart5);
       emit createCircle(&hex6Data, hexStart6);
       emit createCircle(&hex7Data, hexStart7);
       emit createCircle(&hex8Data, hexStart8);
       emit createCircle(&hex9Data, hexStart9);
       emit createCircle(&hexAData, hexStartA);
       emit createCircle(&hexBData, hexStartB);
       emit createCircle(&hexCData, hexStartC);
       emit createCircle(&hexDData, hexStartD);
       emit createCircle(&hexEData, hexStartE);
       emit createCircle(&hexFData, hexStartF);

    }
}



void JGraphicsView::createBucketsAndBalls(bool small)
{
    //Setup initial coordinates
    cupStart1.setX(890);
    cupStart1.setY(220);
    cupStart2.setX(700);
    cupStart2.setY(220);
    cupStart3.setX(510);
    cupStart3.setY(220);
    cupStart4.setX(320);
    cupStart4.setY(220);
    cupStart5.setX(420);
    cupStart5.setY(220);
    cupStart6.setX(300);
    cupStart6.setY(220);
    cupStart7.setX(375);
    cupStart7.setY(220);
    cupStart8.setX(280);
    cupStart8.setY(220);

    ballStart1.setX(100);
    ballStart1.setY(150);
    ballStart2.setX(105);
    ballStart2.setY(150);
    ballStart3.setX(110);
    ballStart3.setY(150);
    ballStart4.setX(115);
    ballStart4.setY(150);
    ballStart5.setX(103);
    ballStart5.setY(130);
    ballStart6.setX(108);
    ballStart6.setY(130);
    ballStart7.setX(112);
    ballStart7.setY(130);
    ballStart8.setX(114);
    ballStart8.setY(130);

    hexStart0.setX(105);
    hexStart0.setY(140);
    hexStart1.setX(100);
    hexStart1.setY(150);
    hexStart2.setX(105);
    hexStart2.setY(150);
    hexStart3.setX(110);
    hexStart3.setY(150);
    hexStart4.setX(115);
    hexStart4.setY(150);
    hexStart5.setX(103);
    hexStart5.setY(130);
    hexStart6.setX(108);
    hexStart6.setY(130);
    hexStart7.setX(112);
    hexStart7.setY(130);
    hexStart8.setX(114);
    hexStart8.setY(130);
    hexStart9.setX(100);
    hexStart9.setY(120);
    hexStartA.setX(105);
    hexStartA.setY(120);
    hexStartB.setX(110);
    hexStartB.setY(120);
    hexStartC.setX(115);
    hexStartC.setY(120);
    hexStartD.setX(103);
    hexStartD.setY(110);
    hexStartE.setX(108);
    hexStartE.setY(110);
    hexStartF.setX(112);
    hexStartF.setY(110);

    //setup for all levels and rounds
    QImage cupImage1("../resources/Bucket.png");
    QImage ballImage1("../resources/Ball.png");

    bigBucketStart.setX(100);
    bigBucketStart.setY(200);
    QImage bigBucketImage("../resources/BigBucket.png");
    bigBucket = new QGraphicsPixmapItem(QPixmap::fromImage(bigBucketImage));
    bigBucket->setOffset(-256, -256);
    bigBucket->setScale(0.3);
    scene->addItem(bigBucket);
    bigBucket->setPos(bigBucketStart);
    bigBucketData.sprite = bigBucket;
    bigBucketData.name = "bigBucket";

    bigBucketStart.setX(100);
    bigBucketStart.setY(200);
    bigBucket2 = new QGraphicsPixmapItem(QPixmap::fromImage(bigBucketImage));
    bigBucket2->setOffset(-256, -256);
    bigBucket2->setScale(0.4);
    scene->addItem(bigBucket2);
    bigBucket2->setPos(bigBucketStart);
    bigBucket2Data.sprite = bigBucket2;
    bigBucket2Data.name = "bigBucket2";

    if(small)
        emit createBigBucket(&bigBucketData, bigBucketStart, true);
    else
        emit createBigBucket(&bigBucket2Data, bigBucketStart, false);



    //Bucket1
    cupSprite1 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite1->setOffset(-64, -64);
    cupSprite1->setScale(0.6);
    cup1Data.sprite = cupSprite1;
    cup1Data.name = "cup1";
    scene->addItem(cupSprite1);
    cupSprite1->setPos(cupStart1);

    //Bucket2
    cupSprite2 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite2->setOffset(-64, -64);
    cupSprite2->setScale(0.6);
    cup2Data.sprite = cupSprite2;
    cup2Data.name = "cup2";
    scene->addItem(cupSprite2);
    cupSprite2->setPos(cupStart2);

    //Bucket3
    cupSprite3 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite3->setOffset(-64, -64);
    cupSprite3->setScale(0.6);
    cup3Data.sprite = cupSprite3;
    cup3Data.name = "cup3";
    scene->addItem(cupSprite3);
    cupSprite3->setPos(cupStart3);

    //Bucket4
    cupSprite4 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite4->setOffset(-64, -64);
    cupSprite4->setScale(0.6);
    cup4Data.sprite = cupSprite4;
    cup4Data.name = "cup4";
    scene->addItem(cupSprite4);
    cupSprite4->setPos(cupStart4);

    //Bucket5
    cupSprite5 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite5->setOffset(-64, -64);
    cupSprite5->setScale(0.6);
    cup5Data.sprite = cupSprite5;
    cup5Data.name = "cup5";
    scene->addItem(cupSprite5);
    cupSprite5->setPos(cupStart5);

    //Bucket6
    cupSprite6 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite6->setOffset(-64, -64);
    cupSprite6->setScale(0.6);
    cup6Data.sprite = cupSprite6;
    cup6Data.name = "cup6";
    scene->addItem(cupSprite6);
    cupSprite6->setPos(cupStart6);

    //Bucket7
    cupSprite7 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite7->setOffset(-64, -64);
    cupSprite7->setScale(0.6);
    cup7Data.sprite = cupSprite7;
    cup7Data.name = "cup7";
    scene->addItem(cupSprite7);
    cupSprite7->setPos(cupStart7);

    //Bucket8
    cupSprite8 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    cupSprite8->setOffset(-64, -64);
    cupSprite8->setScale(0.6);
    cup8Data.sprite = cupSprite8;
    cup8Data.name = "cup8";
    scene->addItem(cupSprite8);
    cupSprite8->setPos(cupStart8);

    //Ball1
    ballSprite1 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite1->setOffset(-64, -64);
    ballSprite1->setScale(0.2);
    ball1Data.sprite = ballSprite1;
    ball1Data.name = "ball1";
    scene->addItem(ballSprite1);
    ballSprite1->setPos(ballStart1);

    //Ball2
    ballSprite2 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite2->setOffset(-64, -64);
    ballSprite2->setScale(0.2);
    ball2Data.sprite = ballSprite2;
    ball2Data.name = "ball2";
    scene->addItem(ballSprite2);
    ballSprite2->setPos(ballStart2);

    //Ball3
    ballSprite3 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite3->setOffset(-64, -64);
    ballSprite3->setScale(0.2);
    ball3Data.sprite = ballSprite3;
    ball3Data.name = "ball3";
    scene->addItem(ballSprite3);
    ballSprite3->setPos(ballStart3);

    //Ball4
    ballSprite4 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite4->setOffset(-64, -64);
    ballSprite4->setScale(0.2);
    ball4Data.sprite = ballSprite4;
    ball4Data.name = "ball4";
    scene->addItem(ballSprite4);
    ballSprite4->setPos(ballStart4);

    //Ball5
    ballSprite5 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite5->setOffset(-64, -64);
    ballSprite5->setScale(0.2);
    ball5Data.sprite = ballSprite5;
    ball5Data.name = "ball5";
    scene->addItem(ballSprite5);
    ballSprite5->setPos(ballStart5);

    //Ball6
    ballSprite6 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite6->setOffset(-64, -64);
    ballSprite6->setScale(0.2);
    ball6Data.sprite = ballSprite6;
    ball6Data.name = "ball6";
    scene->addItem(ballSprite6);
    ballSprite6->setPos(ballStart6);

    //Ball7
    ballSprite7 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite7->setOffset(-64, -64);
    ballSprite7->setScale(0.2);
    ball7Data.sprite = ballSprite7;
    ball7Data.name = "ball7";
    scene->addItem(ballSprite7);
    ballSprite7->setPos(ballStart7);

    //Ball8
    ballSprite8 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    ballSprite8->setOffset(-64, -64);
    ballSprite8->setScale(0.2);
    ball8Data.sprite = ballSprite8;
    ball8Data.name = "ball8";
    scene->addItem(ballSprite8);
    ballSprite8->setPos(ballStart8);

    //HexBall0
    QImage hexImage0("../resources/hex0.png");
    hexBall0 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage0));
    hexBall0->setOffset(-64, -64);
    hexBall0->setScale(0.2);
    hex0Data.sprite = hexBall0;
    hex0Data.name = "ball0";
    scene->addItem(hexBall0);
    hexBall0->setPos(hexStart0);

    //HexBall1
    QImage hexImage1("../resources/hex1.png");
    hexBall1 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage1));
    hexBall1->setOffset(-64, -64);
    hexBall1->setScale(0.2);
    hex1Data.sprite = hexBall1;
    hex1Data.name = "ball1";
    scene->addItem(hexBall1);
    hexBall1->setPos(hexStart1);

    //HexBall2
    QImage hexImage2("../resources/hex2.png");
    hexBall2 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage2));
    hexBall2->setOffset(-64, -64);
    hexBall2->setScale(0.2);
    hex2Data.sprite = hexBall2;
    hex2Data.name = "ball2";
    scene->addItem(hexBall2);
    hexBall2->setPos(hexStart2);

    //HexBall3
    QImage hexImage3("../resources/hex3.png");
    hexBall3 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage3));
    hexBall3->setOffset(-64, -64);
    hexBall3->setScale(0.2);
    hex3Data.sprite = hexBall3;
    hex3Data.name = "ball3";
    scene->addItem(hexBall3);
    hexBall3->setPos(hexStart3);

    //HexBall4
    QImage hexImage4("../resources/hex4.png");
    hexBall4 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage4));
    hexBall4->setOffset(-64, -64);
    hexBall4->setScale(0.2);
    hex4Data.sprite = hexBall4;
    hex4Data.name = "ball4";
    scene->addItem(hexBall4);
    hexBall4->setPos(hexStart4);

    //HexBall5
    QImage hexImage5("../resources/hex5.png");
    hexBall5 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage5));
    hexBall5->setOffset(-64, -64);
    hexBall5->setScale(0.2);
    hex5Data.sprite = hexBall5;
    hex5Data.name = "ball5";
    scene->addItem(hexBall5);
    hexBall5->setPos(hexStart5);

    //HexBall6
    QImage hexImage6("../resources/hex6.png");
    hexBall6 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage6));
    hexBall6->setOffset(-64, -64);
    hexBall6->setScale(0.2);
    hex6Data.sprite = hexBall6;
    hex6Data.name = "ball6";
    scene->addItem(hexBall6);
    hexBall6->setPos(hexStart6);

    //HexBall7
    QImage hexImage7("../resources/hex7.png");
    hexBall7 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage7));
    hexBall7->setOffset(-64, -64);
    hexBall7->setScale(0.2);
    hex7Data.sprite = hexBall7;
    hex7Data.name = "ball7";
    scene->addItem(hexBall7);
    hexBall7->setPos(hexStart7);

    //HexBall8
    QImage hexImage8("../resources/hex8.png");
    hexBall8 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage8));
    hexBall8->setOffset(-64, -64);
    hexBall8->setScale(0.2);
    hex8Data.sprite = hexBall8;
    hex8Data.name = "ball8";
    scene->addItem(hexBall8);
    hexBall8->setPos(hexStart8);

    //HexBall9
    QImage hexImage9("../resources/hex9.png");
    hexBall9 = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage9));
    hexBall9->setOffset(-64, -64);
    hexBall9->setScale(0.2);
    hex9Data.sprite = hexBall9;
    hex9Data.name = "ball9";
    scene->addItem(hexBall9);
    hexBall9->setPos(hexStart9);

    //HexBallA
    QImage hexImageA("../resources/hexA.png");
    hexBallA = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageA));
    hexBallA->setOffset(-64, -64);
    hexBallA->setScale(0.2);
    hexAData.sprite = hexBallA;
    hexAData.name = "ball10";
    scene->addItem(hexBallA);
    hexBallA->setPos(hexStartA);

    //HexBallB
    QImage hexImageB("../resources/hexB.png");
    hexBallB = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageB));
    hexBallB->setOffset(-64, -64);
    hexBallB->setScale(0.2);
    hexBData.sprite = hexBallB;
    hexBData.name = "ball11";
    scene->addItem(hexBallB);
    hexBallB->setPos(hexStartB);

    //HexBallC
    QImage hexImageC("../resources/hexC.png");
    hexBallC = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageC));
    hexBallC->setOffset(-64, -64);
    hexBallC->setScale(0.2);
    hexCData.sprite = hexBallC;
    hexCData.name = "ball12";
    scene->addItem(hexBallC);
    hexBallC->setPos(hexStartC);

    //HexBallD
    QImage hexImageD("../resources/hexD.png");
    hexBallD = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageD));
    hexBallD->setOffset(-64, -64);
    hexBallD->setScale(0.2);
    hexDData.sprite = hexBallD;
    hexDData.name = "ball13";
    scene->addItem(hexBallD);
    hexBallD->setPos(hexStartD);

    //HexBallE
    QImage hexImageE("../resources/hexE.png");
    hexBallE = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageE));
    hexBallE->setOffset(-64, -64);
    hexBallE->setScale(0.2);
    hexEData.sprite = hexBallE;
    hexEData.name = "ball14";
    scene->addItem(hexBallE);
    hexBallE->setPos(hexStartE);

    //HexBallF
    QImage hexImageF("../resources/hexF.png");
    hexBallF = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageF));
    hexBallF->setOffset(-64, -64);
    hexBallF->setScale(0.2);
    hexFData.sprite = hexBallF;
    hexFData.name = "ball15";
    scene->addItem(hexBallF);
    hexBallF->setPos(hexStartF);

    //HexBall0Dup
    hexBall0Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage0));
    hexBall0Dup->setOffset(-64, -64);
    hexBall0Dup->setScale(0.2);
    hex0DataDup.sprite = hexBall0Dup;
    hex0DataDup.name = "ball0";
    scene->addItem(hexBall0Dup);
    hexBall0Dup->setPos(hexStart0);

    //HexBall1dup
    hexBall1Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage1));
    hexBall1Dup->setOffset(-64, -64);
    hexBall1Dup->setScale(0.2);
    hex1DataDup.sprite = hexBall1Dup;
    hex1DataDup.name = "ball1";
    scene->addItem(hexBall1Dup);
    hexBall1Dup->setPos(hexStart1);

    //HexBall2dup
    hexBall2Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage2));
    hexBall2Dup->setOffset(-64, -64);
    hexBall2Dup->setScale(0.2);
    hex2DataDup.sprite = hexBall2Dup;
    hex2DataDup.name = "ball2";
    scene->addItem(hexBall2Dup);
    hexBall2Dup->setPos(hexStart2);

    //HexBall3dup
    hexBall3Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage3));
    hexBall3Dup->setOffset(-64, -64);
    hexBall3Dup->setScale(0.2);
    hex3DataDup.sprite = hexBall3Dup;
    hex3DataDup.name = "ball3";
    scene->addItem(hexBall3Dup);
    hexBall3Dup->setPos(hexStart3);

    //HexBall4dup
    hexBall4Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage4));
    hexBall4Dup->setOffset(-64, -64);
    hexBall4Dup->setScale(0.2);
    hex4DataDup.sprite = hexBall4Dup;
    hex4DataDup.name = "ball4";
    scene->addItem(hexBall4Dup);
    hexBall4Dup->setPos(hexStart4);

    //HexBall5dup
    hexBall5Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage5));
    hexBall5Dup->setOffset(-64, -64);
    hexBall5Dup->setScale(0.2);
    hex5DataDup.sprite = hexBall5Dup;
    hex5DataDup.name = "ball5";
    scene->addItem(hexBall5Dup);
    hexBall5Dup->setPos(hexStart5);

    //HexBall6dup
    hexBall6Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage6));
    hexBall6Dup->setOffset(-64, -64);
    hexBall6Dup->setScale(0.2);
    hex6DataDup.sprite = hexBall6Dup;
    hex6DataDup.name = "ball6";
    scene->addItem(hexBall6Dup);
    hexBall6Dup->setPos(hexStart6);

    //HexBall7dup
    hexBall7Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage7));
    hexBall7Dup->setOffset(-64, -64);
    hexBall7Dup->setScale(0.2);
    hex7DataDup.sprite = hexBall7Dup;
    hex7DataDup.name = "ball7";
    scene->addItem(hexBall7Dup);
    hexBall7Dup->setPos(hexStart7);

    //HexBall8dup
    hexBall8Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage8));
    hexBall8Dup->setOffset(-64, -64);
    hexBall8Dup->setScale(0.2);
    hex8DataDup.sprite = hexBall8Dup;
    hex8DataDup.name = "ball8";
    scene->addItem(hexBall8Dup);
    hexBall8Dup->setPos(hexStart8);

    //HexBall9dup
    hexBall9Dup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImage9));
    hexBall9Dup->setOffset(-64, -64);
    hexBall9Dup->setScale(0.2);
    hex9DataDup.sprite = hexBall9Dup;
    hex9DataDup.name = "ball9";
    scene->addItem(hexBall9Dup);
    hexBall9Dup->setPos(hexStart9);

    //HexBallAdup
    hexBallADup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageA));
    hexBallADup->setOffset(-64, -64);
    hexBallADup->setScale(0.2);
    hexADataDup.sprite = hexBallADup;
    hexADataDup.name = "ball10";
    scene->addItem(hexBallADup);
    hexBallADup->setPos(hexStartA);

    //HexBallBdup
    hexBallBDup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageB));
    hexBallBDup->setOffset(-64, -64);
    hexBallBDup->setScale(0.2);
    hexBDataDup.sprite = hexBallBDup;
    hexBDataDup.name = "ball11";
    scene->addItem(hexBallBDup);
    hexBallBDup->setPos(hexStartB);

    //HexBallCdup
    hexBallCDup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageC));
    hexBallCDup->setOffset(-64, -64);
    hexBallCDup->setScale(0.2);
    hexCDataDup.sprite = hexBallCDup;
    hexCDataDup.name = "ball12";
    scene->addItem(hexBallCDup);
    hexBallCDup->setPos(hexStartC);

    //HexBallDdup
    hexBallDDup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageD));
    hexBallDDup->setOffset(-64, -64);
    hexBallDDup->setScale(0.2);
    hexDDataDup.sprite = hexBallDDup;
    hexDDataDup.name = "ball13";
    scene->addItem(hexBallDDup);
    hexBallDDup->setPos(hexStartD);

    //HexBallEdup
    hexBallEDup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageE));
    hexBallEDup->setOffset(-64, -64);
    hexBallEDup->setScale(0.2);
    hexEDataDup.sprite = hexBallEDup;
    hexEDataDup.name = "ball14";
    scene->addItem(hexBallEDup);
    hexBallEDup->setPos(hexStartE);

    //HexBallFdup
    hexBallFDup = new QGraphicsPixmapItem(QPixmap::fromImage(hexImageF));
    hexBallFDup->setOffset(-64, -64);
    hexBallFDup->setScale(0.2);
    hexFDataDup.sprite = hexBallFDup;
    hexFDataDup.name = "ball15";
    scene->addItem(hexBallFDup);
    hexBallFDup->setPos(hexStartF);
}

