#ifndef JGRAPHICSVIEW_H
#define JGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QThread>
#include <QGraphicsPixmapItem>
#include "worldsim.h"

enum Type {easy, medium, hard};

class JGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    JGraphicsView(QWidget* parent);
    ~JGraphicsView();

    void setupLevel(int level, Type type, bool existing);

public slots:
    void updateSprite(QGraphicsPixmapItem* sprite, QPointF pos, double angle);
    void perpetuateContactUpdate(int bucket, int ball, bool isActive);
signals:
    void processEvents();
    void startSim();
    void createCircle(bodydata* data, QPointF pos);
    void createCup(bodydata* data, QPointF pos);
    void createSquare(bodydata* data, QPointF pos);
    void createBigBucket(bodydata* data, QPointF pos, bool small);
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void updateContactToLevelView(int bucket, int ball, bool isActive);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
    QGraphicsPixmapItem *ballSprite1, *ballSprite2, *ballSprite3, *ballSprite4,
                        *ballSprite5, *ballSprite6, *ballSprite7, *ballSprite8,
                        *cupSprite1, *cupSprite2, *cupSprite3, *cupSprite4,
                        *cupSprite5, *cupSprite6, *cupSprite7, *cupSprite8,
                        *hexBall0, *hexBall1, *hexBall2, *hexBall3, *hexBall4, *hexBall5,
                        *hexBall6, *hexBall7, *hexBall8, *hexBall9, *hexBallA,
                        *hexBallB, *hexBallC, *hexBallD, *hexBallE, *hexBallF,
                        *hexBall0Dup, *hexBall1Dup, *hexBall2Dup, *hexBall3Dup, *hexBall4Dup, *hexBall5Dup,
                        *hexBall6Dup, *hexBall7Dup, *hexBall8Dup, *hexBall9Dup, *hexBallADup,
                        *hexBallBDup, *hexBallCDup, *hexBallDDup, *hexBallEDup, *hexBallFDup,
                        *bigBucket, *bigBucket2;
    QPointF cupStart1, cupStart2, cupStart3, cupStart4, cupStart5, cupStart6, cupStart7,
            cupStart8, ballStart1, ballStart2, ballStart3, ballStart4, ballStart5, ballStart6,
            ballStart7, ballStart8, hexStart0, hexStart1, hexStart2, hexStart3, hexStart4, hexStart5,
            hexStart6, hexStart7, hexStart8, hexStart9, hexStartA, hexStartB, hexStartC,
            hexStartD, hexStartE, hexStartF, bigBucketStart;
    QGraphicsScene* scene;
    WorldSim* sim;
    void showEvent(QShowEvent *event);
    void createBucketsAndBalls(bool small);
    QThread worldThread;


    bodydata cup1Data, cup2Data, cup3Data, cup4Data, cup5Data, cup6Data, cup7Data, cup8Data,
             ball1Data, ball2Data, ball3Data, ball4Data, ball5Data, ball6Data, ball7Data,
             ball8Data, hex0Data, hex1Data, hex2Data, hex3Data, hex4Data, hex5Data, hex6Data, hex7Data,
             hex8Data, hex9Data, hexAData, hexBData, hexCData, hexDData, hexEData, hexFData,
             hex0DataDup, hex1DataDup, hex2DataDup, hex3DataDup, hex4DataDup, hex5DataDup, hex6DataDup, hex7DataDup,
             hex8DataDup, hex9DataDup, hexADataDup, hexBDataDup, hexCDataDup, hexDDataDup, hexEDataDup, hexFDataDup,
             bigBucketData, bigBucket2Data;
};

#endif // JGRAPHICSVIEW_H
