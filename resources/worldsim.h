#ifndef WORLDSIM_H
#define WORLDSIM_H

#include <QMouseEvent>
#include <QGraphicsView>
#include <Box2D/Box2D.h>

namespace b2Qt
{
    const float SCALE = 32.f;
}

class WorldSim : public QObject
{
    Q_OBJECT

public:
    WorldSim(int width, int height);

public slots:

    void startSim();
    void updateSimulation();
    void mouseEvent(QMouseEvent* event);
    b2Body* createCircle(QGraphicsPixmapItem* sprite, QPointF pos);
    b2Body* createSquare(QGraphicsPixmapItem* sprite, QPointF pos);
    b2Body* createCup(QGraphicsPixmapItem* sprite, QPointF pos);


signals:

    void updatePosition(QGraphicsPixmapItem* sprite, QPointF pos, double angle);


private:

    const float timeStep = (1.0f / 60.0f);
    const int velocityIterations = 6;
    const int positionIterations = 2;
    b2BodyDef boundingBoxDef;
    b2MouseJoint* mouseJoint = NULL;
    b2Body* ground;
    b2Body* ballBody;

    b2World* world;
    QTimer* myTimer;
    int width;
    int height;
    void setBoundingBox(int width, int height);

};

#endif // WORLDSIM_H
