#ifndef MUSHROOMVIEW_H
#define MUSHROOMVIEW_H

#include <QWidget>
#include <QMqttClient>
#include <QGraphicsView>
#include "RoundGaugeGraphicsObject.h"
#include "ToggleButton.h"

enum GAUGE_TYPE{
    GAUGE_TEMP=0,
    GAUGE_MOISTURE=1,
    GAUGE_TEMP_WATER=2
};

namespace Ui {
class MushroomView;
}

class MushroomView : public QWidget
{
    Q_OBJECT

public:
    explicit MushroomView(QWidget *parent = nullptr, double scl=1);
    ~MushroomView();

    ToggleButton * mLedToggle;
    ToggleButton * mPumpToggle;
    ToggleButton * mFanToggle;
    ToggleButton * mTempToggle;

    QString stateString;

signals:
    void pushUpdate(QString message);

public slots:
    void receiveMessage(const QByteArray &message, const QMqttTopicName &topic);
    void forceUpdate();
private:
    Ui::MushroomView *ui;

    // Screens
    QVector<QGraphicsView*> mRoundGaugeViews;
    QVector<RoundGaugeGraphicsObject*> mRoundGauges;
    QVector<QGraphicsScene*> mScenes;

    QColor mColor_OuterRing;

    QVector<QColor> mMapJet;
    QVector<QColor> mMapWinter;


    void loadColormap();

    QColor getColorForValue(double value, double _min=0, double _max=100, bool useJet=true);
    void setupGauge(int viewID, GAUGE_TYPE type=GAUGE_TEMP);
};

#endif // MUSHROOMVIEW_H
