#ifndef WORLDSIM_H
#define WORLDSIM_H

#include <QMouseEvent>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QGraphicsPixmapItem>
#include "bodydata.h"
#include <queue>

namespace b2Qt
{
    const float SCALE = 32.f;
}

class WorldSim : public QObject
{
    Q_OBJECT

public:
    WorldSim(int width, int height);
    void reset();
    void contactUpdate(int bucket,int ball, bool isTouching);

public slots:

    void startSim();
    void updateSimulation();

    b2Body* createCircle(bodydata* data, QPointF pos);
    b2Body* createSquare(bodydata* data, QPointF pos);
    b2Body* createCup(bodydata* data, QPointF pos);
    b2Body* createBigBucket(bodydata* data, QPointF pos, bool small);


    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);


signals:

    void updatePosition(QGraphicsPixmapItem* sprite, QPointF pos, double angle);
    void contactUpdateToJGraphicsView(int bucket, int ball, bool isTouching);

private:

    const float timeStep = (1.0f / 60.0f);
    const int velocityIterations = 6;
    const int positionIterations = 2;
    b2BodyDef boundingBoxDef;
    b2MouseJoint* mouseJoint;
    b2Body* mouseBody;

    b2Body* ballBody;

    b2World* world;
    QTimer* myTimer;
    int width;
    int height;
    void setBoundingBox(int width, int height);

};

#endif // WORLDSIM_H
