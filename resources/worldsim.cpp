#include "worldsim.h"

#include "jgraphicsview.h"
#include "Box2D/Box2D.h"
#include <QGraphicsPixmapItem>
#include <iostream>

std::vector<b2Fixture*> fixtures;

class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point)
    {
        m_point = point;
        m_fixture = NULL;
    }

    bool ReportFixture(b2Fixture* fixture)
    {
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody)
        {
            fixtures.push_back(fixture);

            // We are done, terminate the query.
            return true;
        }
        // Continue the query.
        return true;
    }

    b2Vec2 m_point;
    b2Fixture* m_fixture;
};

class contactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {

    }

    void EndContact(b2Contact* contact)
    {

    }
};

QPointF B2VecToQPointF(const b2Vec2 &vector, bool scaleToPixels = true)
{
    qreal x = vector.x * (scaleToPixels ? b2Qt::SCALE : 1.f);
    qreal y = vector.y * (scaleToPixels ? b2Qt::SCALE : 1.f);
    return QPointF(x, y);
}

b2Vec2 QPointFToB2Vec(QPointF vector)
{
    float32 x = vector.x() / b2Qt::SCALE;
    float32 y = vector.y() / b2Qt::SCALE;
    return b2Vec2(x, y);
}

WorldSim::WorldSim(int width, int height)
{
    myTimer = new QTimer(this);
    myTimer->setInterval(120);
    connect(myTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));

    this->height = height;
    this->width = width;

    world = new b2World(b2Vec2(0.f, 10.f));
    world->SetAllowSleeping(false);
    contactListener listener;
    world->SetContactListener(&listener);

    setBoundingBox(width, height);
}

void WorldSim::setBoundingBox(int width, int height)
{
    boundingBoxDef.type = b2_staticBody;
    float xPos = (width / 2.f) / b2Qt::SCALE;
    float yPos = 0.5f;
    boundingBoxDef.position.Set(xPos, yPos);

    b2Body* boundingBoxBody = world->CreateBody(&boundingBoxDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / b2Qt::SCALE, 0.5f, b2Vec2(0.f, 0.f), 0.f);
    boundingBoxBody->CreateFixture(&boxShape, 1.0); //Top

    yPos = (height) / b2Qt::SCALE - 1.f;
    boxShape.SetAsBox(width / b2Qt::SCALE, 0.5f, b2Vec2(0.f, yPos), 0.f);
    boundingBoxBody->CreateFixture(&boxShape, 1.f); //Bottom

    xPos -= 0.5f;
    boxShape.SetAsBox(0.5f, (height) / b2Qt::SCALE, b2Vec2(-xPos, 0.f), 0.f);
    boundingBoxBody->CreateFixture(&boxShape, 1.f);//Left

    boxShape.SetAsBox(0.5f, (height) / b2Qt::SCALE, b2Vec2(xPos, 0.f), 0.f);
    boundingBoxBody->CreateFixture(&boxShape, 1.f);//Right
}

void WorldSim::updateSimulation()
{
    for (b2Body* BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
    {
        if (BodyIterator->GetType() == b2_dynamicBody)
        {
            QGraphicsPixmapItem* temp = (QGraphicsPixmapItem *)BodyIterator->GetUserData();
            b2Vec2 pos = BodyIterator->GetPosition();
            temp->setPos(B2VecToQPointF(pos));  //BROKEN
            std::cout<<"updating postion worked" << std::endl;
            float32 angle = BodyIterator->GetAngle() * 180/b2_pi;
            std::cout<<"get angle worked" << std::endl;
            temp->setRotation((qreal)angle);
            std::cout<<"set angle worked" << std::endl;
        }
    }
}

void WorldSim::startSim()
{
    myTimer->start();
}

void WorldSim::mouseEvent(QMouseEvent *event)
{

}

//Creates a box body and returns a reference to it
b2Body* WorldSim::createSquare(QGraphicsPixmapItem* sprite, QPointF pos)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);


    b2PolygonShape boxShape;
    boxShape.SetAsBox((192/2)/b2Qt::SCALE, (115/2)/b2Qt::SCALE, b2Vec2(0.f, 0.f), 0);
    body->CreateFixture(&boxShape, 1.f);

    return body;
}

//Creates a box body and returns a reference to it
b2Body* WorldSim::createCup(QGraphicsPixmapItem* sprite, QPointF pos)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);


    b2PolygonShape boxShape;
    boxShape.SetAsBox((192/2)/b2Qt::SCALE, (35/4)/b2Qt::SCALE, b2Vec2(0.f, ((80/2)/b2Qt::SCALE)), 0);
    body->CreateFixture(&boxShape, 1.f); // Bottom

    boxShape.SetAsBox((192/16)/b2Qt::SCALE, (115/4)/b2Qt::SCALE, b2Vec2(-((192*3/16)/b2Qt::SCALE), 0.f), 0);
    body->CreateFixture(&boxShape, 1.f); // Left

    boxShape.SetAsBox((192/16)/b2Qt::SCALE, (115/4)/b2Qt::SCALE, b2Vec2(((192*3/16)/b2Qt::SCALE), 0.f), 0);
    body->CreateFixture(&boxShape, 1.f); // Right

    return body;
}


//Creates a circle body and returns a reference to it
b2Body* WorldSim::createCircle(QGraphicsPixmapItem* sprite, QPointF pos)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.49f;
    circleShape.m_p.SetZero();
    body->CreateFixture(&circleShape, 1.f);

    return body;
}
