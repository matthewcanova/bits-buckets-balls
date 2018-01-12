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

    onInit();

    this->setScene(this->scene);

    std::cout << "Right before the thread is created" << std::endl;

    sim = new WorldSim(1200, 300);
    sim->moveToThread(&worldThread);
    connect(&worldThread, &QThread::finished, sim, &QObject::deleteLater); // Do we need this?
    connect(sim, &WorldSim::updatePosition, this, &JGraphicsView::updateSprite);

    worldThread.start();

}

JGraphicsView::~JGraphicsView()
{
    delete scene;
}

void JGraphicsView::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit startSim();
}

void JGraphicsView::mousePressEvent(QMouseEvent *eventPtr)
{

}


void JGraphicsView::mouseReleaseEvent(QMouseEvent *eventPtr)
{


}

void JGraphicsView::mouseMoveEvent(QMouseEvent *eventPtr)
{

}

void JGraphicsView::onInit()
{   
    QPointF cupStart1(600, 268);

    QImage cupImage1("../resources/Bucket.png");
    QGraphicsPixmapItem* cupSprite1 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage1));
    scene->addItem(cupSprite1);
    cupSprite1->setPos(cupStart1);
    emit createCup(cupSprite1, cupStart1);


    QPointF ballStart1(600, 250);

    QImage ballImage1("../resources/Ball.png");
    ballSprite1 = new QGraphicsPixmapItem(QPixmap::fromImage(ballImage1));
    scene->addItem(ballSprite1);
    ballSprite1->setPos(ballStart1);
    ballSprite1->setScale(0.1);
    emit createCircle(ballSprite1, ballStart1);


//    b2Vec2 cupStart2(scene->sceneRect().size().width()/2/b2Qt::SCALE, scene->sceneRect().size().height()/1.5/b2Qt::SCALE);
//    b2Body* cupBody2 = createCup(world, cupStart2);

//    QImage cupImage2("../resources/Bucket.png");
//    QGraphicsPixmapItem* cupSprite2 = new QGraphicsPixmapItem(QPixmap::fromImage(cupImage2));
//    scene->addItem(cupSprite2);
//    cupSprite2->setPos(B2VecToQPointF(cupStart2));

//    // Dynamic Bodies



//    // Dynamic Body Sprites

//    b2BodyDef bodyDef;
//    ground = world->CreateBody(&bodyDef); //This is not the body of the bounding box
//                                                 //This body exists to serve as an anchor point for the mouse joint

//    std::cout << "Got to end of init" << std::endl;

}

void JGraphicsView::updateSprite(QGraphicsPixmapItem *sprite, QPointF pos, double angle)
{
    sprite->setPos(pos);
    sprite->setRotation(angle);
}

