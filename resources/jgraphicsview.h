#ifndef JGRAPHICSVIEW_H
#define JGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <cmath>
#include <string>
#include "Box2D/Box2D.h"
#include <QTimer>
#include "Box2D/Common/b2Math.h"
#include <QApplication>
#include <QThread>
#include "worldsim.h"



class JGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    JGraphicsView(QWidget* parent);
    ~JGraphicsView();

public slots:
    void updateSprite(QGraphicsPixmapItem* sprite, QPointF pos, double angle);
    
signals:
    void processEvents();
    void startSim();
    void createCircle(QGraphicsPixmapItem* sprite, QPointF pos);
    void createCup(QGraphicsPixmapItem* sprite, QPointF pos);
    void createSquare(QGraphicsPixmapItem* sprite, QPointF pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:


    QGraphicsPixmapItem* ballSprite1;

    QGraphicsScene* scene;
    WorldSim* sim;
    void onInit();
    bool initialized;
    void showEvent(QShowEvent *event);
    QThread worldThread;
};

#endif // JGRAPHICSVIEW_H
