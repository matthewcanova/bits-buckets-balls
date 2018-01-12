#include "worldsim.h"
#include "jgraphicsview.h"
#include "Box2D/Box2D.h"
#include <QGraphicsPixmapItem>
#include <iostream>

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
            m_fixture = fixture;

            // We are done, terminate the query.
            return false;
        }
        // Continue the query.
        return true;
    }

    b2Vec2 m_point;
    b2Fixture* m_fixture;
};

class contactListener : public b2ContactListener
{


public:
    void SetSim(WorldSim* world1)
    {
        sim = world1;
    }

private:
    WorldSim* sim;

    void BeginContact(b2Contact* contact)
    {
        bodydata* dataA = (bodydata *)contact->GetFixtureA()->GetBody()->GetUserData();
        bodydata* dataB = (bodydata *)contact->GetFixtureB()->GetBody()->GetUserData();

        if (dataA != nullptr && dataB != nullptr)
        {
            int ballA = (dataA->name).find("ball");
            int ballB = (dataB->name).find("ball");
            int cupA = (dataA->name).find("cup");
            int cupB = (dataB->name).find("cup");

            // If we have a ball and bucket collision, add names to list
            if ((ballA > -1 && cupB > -1))
            {
                std::string nameA = dataA->name;
                std::string nameB = dataB->name;
                std::string subCup = nameB.substr(3);
                std::string subBall = nameA.substr(4);
                try
                {
                    int cupBitNum = std::stoi(subCup);
                    int ballBitNum = std::stoi(subBall);
                    sim->contactUpdate(cupBitNum, ballBitNum, true);

                    dataB->ballCount++;
                }
                catch (std::invalid_argument)
                {
                    // Could not convert int in name
                }
            }
            else if((ballB > -1 && cupA > -1))
            {
                std::string nameA = dataA->name;
                std::string nameB = dataB->name;
                std::string subCup = nameA.substr(3);
                std::string subBall = nameB.substr(4);
                try
                {
                    int cupBitNum = std::stoi(subCup);
                    int ballBitNum = std::stoi(subBall);

                    sim->contactUpdate(cupBitNum, ballBitNum, true);

                    dataA->ballCount++;
                }
                catch (std::invalid_argument)
                {
                    // Could not convert int in name
                }
            }
        }
    }

    void EndContact(b2Contact* contact)
    {
        bodydata* dataA = (bodydata *)contact->GetFixtureA()->GetBody()->GetUserData();
        bodydata* dataB = (bodydata *)contact->GetFixtureB()->GetBody()->GetUserData();

        if (dataA && dataB)
        {
            int ballA = (dataA->name).find("ball");
            int ballB = (dataB->name).find("ball");
            int cupA = (dataA->name).find("cup");
            int cupB = (dataB->name).find("cup");

            // If we have a ball and bucket collision, add names to list
            if ((ballA > -1 && cupB > -1))
            {
                std::string nameA = dataA->name;
                std::string nameB = dataB->name;
                std::string subCup = nameB.substr(3);
                std::string subBall = nameA.substr(4);
                try
                {
                    int cupBitNum = std::stoi(subCup);
                    int ballBitNum = std::stoi(subBall);
                    dataB->ballCount--;
                    if(dataB->ballCount == 0)
                        sim->contactUpdate(cupBitNum, ballBitNum, false);

                }
                catch (std::invalid_argument)
                {
                    // Could not convert int in name
                }
            }
            else if((ballB > -1 && cupA > -1))
            {
                std::string nameA = dataA->name;
                std::string nameB = dataB->name;
                std::string subCup = nameA.substr(3);
                std::string subBall = nameB.substr(4);
                try
                {
                    int cupBitNum = std::stoi(subCup);
                    int ballBitNum = std::stoi(subBall);
                    dataA->ballCount--;
                    if(dataA->ballCount == 0)
                        sim->contactUpdate(cupBitNum, ballBitNum, false);
                }
                catch (std::invalid_argument)
                {
                }
            }
        }
    }
};

contactListener listener;

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
    myTimer->setInterval(20);
    connect(myTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));

    this->height = height;
    this->width = width;

    world = new b2World(b2Vec2(0.f, 10.f));
    world->SetAllowSleeping(false);
    setBoundingBox(width, height);

    mouseJoint = NULL;
    b2BodyDef bodyDef;
    mouseBody = world->CreateBody(&bodyDef);
    listener.SetSim(this);
    world->SetContactListener(&listener);
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

void WorldSim::contactUpdate(int bucket, int ball, bool isTouching)
{
    emit contactUpdateToJGraphicsView(bucket, ball, isTouching);
}

void WorldSim::updateSimulation()
{
    world->Step(timeStep, velocityIterations, positionIterations);
    for (b2Body* BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
    {
        if (BodyIterator->GetType() == b2_dynamicBody)
        {
            QGraphicsPixmapItem* temp = ((QGraphicsPixmapItem*)(((bodydata *)(BodyIterator->GetUserData()))->sprite));
            QPointF pos = B2VecToQPointF(BodyIterator->GetPosition());
            double angle = BodyIterator->GetAngle() * 180/b2_pi;
            emit updatePosition(temp, pos, angle);
        }
    }
}

void WorldSim::startSim()
{
    myTimer->start();
}

void WorldSim::mousePress(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton && mouseJoint == nullptr)
    {
        QPointF position(event->pos().x(), event->pos().y());
        b2Vec2 mousePos = QPointFToB2Vec(position);

        // Make a small box.
        b2AABB aabb;
        b2Vec2 d;
        d.Set(0.001f, 0.001f);
        aabb.lowerBound = mousePos + d;
        aabb.upperBound = mousePos - d;

        // Query the world for overlapping shapes.
        QueryCallback callback(mousePos);
        world->QueryAABB(&callback, aabb);


        if(callback.m_fixture)
        {
            b2Body* body = callback.m_fixture->GetBody();
            b2MouseJointDef md;
            md.bodyA = mouseBody;
            md.bodyB = body;
            md.target = mousePos;
            md.maxForce = 1000.0f * body->GetMass();
            mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
            body->SetAwake(true);
        }
    }
}

void WorldSim::mouseRelease(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && mouseJoint != NULL)
    {
        world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
    }
}

void WorldSim::mouseMove(QMouseEvent *event)
{
    if(mouseJoint != NULL)
    {
        QPointF position(event->pos().x(), event->pos().y());
        b2Vec2 mousePos = QPointFToB2Vec(position);
        mouseJoint->SetTarget(mousePos);
    }
}

//Creates a box body and returns a reference to it
b2Body* WorldSim::createSquare(bodydata* data, QPointF pos)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(data);


    b2PolygonShape boxShape;
    boxShape.SetAsBox((192/2)/b2Qt::SCALE, (115/2)/b2Qt::SCALE, b2Vec2(0.f, 0.f), 0);
    body->CreateFixture(&boxShape, 1.f);

    return body;
}

//Creates a box body and returns a reference to it
b2Body* WorldSim::createCup(bodydata* data, QPointF pos)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((40/2)/b2Qt::SCALE, (40/4)/b2Qt::SCALE, b2Vec2(0.f, ((45)/b2Qt::SCALE)), 0);
    body->CreateFixture(&boxShape, 1.f); // Bottom

    boxShape.SetAsBox((0.1)/b2Qt::SCALE, (40/1.5)/b2Qt::SCALE, b2Vec2(-((40/2)/b2Qt::SCALE), 0.f), 0);
    body->CreateFixture(&boxShape, 1.f); // Left

    boxShape.SetAsBox((0.1)/b2Qt::SCALE, (40/1.5)/b2Qt::SCALE, b2Vec2(((40/2)/b2Qt::SCALE), 0.f), 0);
    body->CreateFixture(&boxShape, 1.f); // Right

    b2BodyDef bodyDef2;
    bodyDef2.type = b2_staticBody;
    bodyDef2.position = posNew;
    b2Body* body2 = world->CreateBody(&bodyDef2);

    boxShape.SetAsBox((40/2.1)/b2Qt::SCALE, (40/4)/b2Qt::SCALE, b2Vec2(0.f, ((44)/b2Qt::SCALE)), 0);
    body2->CreateFixture(&boxShape, 1.f); // Bottom

    body2->SetUserData(data);

    return body2;
}


//Creates a circle body and returns a reference to it
b2Body* WorldSim::createCircle(bodydata* data, QPointF pos)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(data);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.3f;
    circleShape.m_p.SetZero();
    body->CreateFixture(&circleShape, 1.f);

    return body;
}

void WorldSim::reset()
{
    myTimer->stop();
    for (b2Body* BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
    {
        world->DestroyBody(BodyIterator);
    }
    setBoundingBox(1200, 300);
}

//Creates a box body and returns a reference to it
b2Body* WorldSim::createBigBucket(bodydata* data, QPointF pos, bool small)
{
    b2Vec2 posNew = QPointFToB2Vec(pos);
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = posNew;
    b2Body* body = world->CreateBody(&bodyDef);
    body->SetUserData(data);


    if (small)
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox((154/2)/b2Qt::SCALE, (154/4)/b2Qt::SCALE, b2Vec2(0.f, ((154)/b2Qt::SCALE)), 0);
        body->CreateFixture(&boxShape, 1.f); // Bottom

        boxShape.SetAsBox((0.1)/b2Qt::SCALE, (154/2)/b2Qt::SCALE, b2Vec2(-((100/2)/b2Qt::SCALE), 0.f), 0);
        body->CreateFixture(&boxShape, 1.f); // Left

        boxShape.SetAsBox((0.1)/b2Qt::SCALE, (154/2)/b2Qt::SCALE, b2Vec2(((100/2)/b2Qt::SCALE), 0.f), 0);
        body->CreateFixture(&boxShape, 1.f); // Right

    }

    else
    {
        b2PolygonShape boxShape;
        boxShape.SetAsBox((154/2)/b2Qt::SCALE, (154/4)/b2Qt::SCALE, b2Vec2(0.f, ((175)/b2Qt::SCALE)), 0);
        body->CreateFixture(&boxShape, 1.f); // Bottom

        boxShape.SetAsBox((0.1)/b2Qt::SCALE, (154/2)/b2Qt::SCALE, b2Vec2(-((150/2)/b2Qt::SCALE), 0.f), 0);
        body->CreateFixture(&boxShape, 1.f); // Left

        boxShape.SetAsBox((0.1)/b2Qt::SCALE, (154/2)/b2Qt::SCALE, b2Vec2(((150/2)/b2Qt::SCALE), 0.f), 0);
        body->CreateFixture(&boxShape, 1.f); // Right

    }

    return body;
}
