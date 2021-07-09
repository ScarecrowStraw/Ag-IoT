#include "MushroomView.h"
#include "ui_MushroomView.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>

MushroomView::MushroomView(QWidget *parent, double scl) :
    QWidget(parent),
    ui(new Ui::MushroomView)
{
    ui->setupUi(this);
    loadColormap();
//    qDebug() <<scl;
    this->setMinimumWidth(this->minimumWidth()*scl);
    this->setMinimumHeight(this->minimumHeight()*scl);

    int num_gauges=4;
    mRoundGaugeViews.resize(num_gauges);
    mScenes.resize(num_gauges);
    mRoundGauges.resize(num_gauges);
    // Assign Views
    mRoundGaugeViews[0] = ui->graphicsView_0;
    mRoundGaugeViews[1] = ui->graphicsView_1;
    mRoundGaugeViews[2] = ui->graphicsView_2;
    mRoundGaugeViews[3] = ui->graphicsView_3;

    QRect baseRect = ui->graphicsView_0->rect();
    mColor_OuterRing = QColor::fromRgb(160,160,160);

//Prepare scenes
    for (int i = 0; i < mRoundGaugeViews.size(); ++i) {
        QGraphicsView * _gaugeViews = mRoundGaugeViews[i];
        _gaugeViews->setDragMode(QGraphicsView::NoDrag);
        _gaugeViews->setRenderHint(QPainter::Antialiasing);

        mScenes[i] = new QGraphicsScene(0,0,baseRect.width()*scl,baseRect.height()*scl);
        mRoundGauges[i] = new RoundGaugeGraphicsObject(QRectF(27*scl, 37*scl, 180*scl, 180*scl));
    }

    setupGauge(0,GAUGE_TEMP);
    setupGauge(1,GAUGE_MOISTURE);
    setupGauge(2,GAUGE_TEMP_WATER);

    mLedToggle = new ToggleButton(15*scl, 10*scl);
    mPumpToggle = new ToggleButton(15*scl, 10*scl);
    mFanToggle = new ToggleButton(15*scl, 10*scl);
    mTempToggle = new ToggleButton(15*scl, 10*scl);

    ui->hL_2->insertWidget(1,mLedToggle);
    ui->hL_2->insertWidget(4,mPumpToggle);
    ui->hL_3->insertWidget(4,mFanToggle);
    ui->hL_3->insertWidget(1,mTempToggle);

    connect(ui->updateBtn, SIGNAL(pressed()), this, SLOT(forceUpdate()));

}

MushroomView::~MushroomView()
{
    delete ui;
}

void MushroomView::setupGauge(int viewID, GAUGE_TYPE type) {
    QGraphicsScene * tempScene = mScenes[viewID];
    RoundGaugeGraphicsObject * tempGauge = mRoundGauges[viewID];
    mRoundGaugeViews[viewID]->setScene(tempScene);
    tempScene->addItem(tempGauge);

    tempGauge->setOuterRingColor(mColor_OuterRing);
    tempGauge->setGlowRingEnabled(true);


    QColor default_color;
    switch (type) {
        case GAUGE_MOISTURE:
            default_color = QColor::fromRgb(100,160,255);
            tempScene->addText("Humidity (%)");
            break;
        case GAUGE_TEMP_WATER:
            default_color = QColor::fromRgb(255,255,128);
            tempScene->addText("Water Temperature (°C)");
            break;
        default:
            default_color = QColor::fromRgb(255,128,128);
            tempScene->addText("Temperature (°C)");
    }

    tempGauge->setStateColor(default_color);

    tempGauge->setRange(0, 100);
    tempGauge->setValue(50);
}

void MushroomView::loadColormap() {
    QImage pixmapJet, pixmapWinter;
    pixmapJet.load(":/Colormap/Resources/colormap_jet.png");
    pixmapWinter.load(":/Colormap/Resources/colormap_winter.png");

    double res = 100;
    double _step = (pixmapJet.width()-1)/res;
    for (int i = 0; i < res; ++i) {
        int id = i*_step;
        QColor c_Jet = pixmapJet.pixelColor(id,0).toRgb();
        QColor c_Winter = pixmapWinter.pixelColor(id,0).toRgb();
        mMapJet.push_back(c_Jet);
        mMapWinter.push_back(c_Winter);
    }
}

QColor MushroomView::getColorForValue(double value, double _min, double _max, bool useJet) {
    if(value < _min) value = _min;
    if(value > _max) value = _max;

    double band = _max - _min;
    double value_norm = (value - _min)/band;
    int scale = mMapJet.size()-1;

    QColor rs;
    if(useJet){
        rs = mMapJet[int(scale*value_norm)];
    } else{
        rs = mMapWinter[int(scale*value_norm)];
    }
    return rs;
}

void MushroomView::receiveMessage(const QByteArray &message, const QMqttTopicName &topic) {
//    qDebug() << "receive" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message);
    double t = doc["t"].toDouble();
    double h = doc["h"].toDouble();
    double tW = doc["tW"].toDouble();
//    qDebug() << t << h << tW;
    mRoundGauges[0]->setValue(t);
    mRoundGauges[1]->setValue(h);
    mRoundGauges[2]->setValue(tW);

    QColor temp_state_color = getColorForValue(t, 15,40);
    QColor humid_state_color = getColorForValue(h,50,95, false);
    QColor wTemp_state_color = getColorForValue(tW, 15, 40);
    mRoundGauges[0]->setStateColor(temp_state_color);
    mRoundGauges[1]->setStateColor(humid_state_color);
    mRoundGauges[2]->setStateColor(wTemp_state_color);
}

void MushroomView::forceUpdate() {
//    qDebug() << mLedToggle->isChecked() << mPumpToggle->isChecked() << ui->tempSlider->value();
    QJsonObject o;
    o["l"] = int8_t(mLedToggle->isChecked());
    o["p"] = int8_t(mPumpToggle->isChecked());
    o["t"] = int8_t(mFanToggle->isChecked());
    o["tW"] = int8_t(mTempToggle->isChecked());
    QJsonDocument doc(o);
    stateString = doc.toJson(QJsonDocument::Compact);
//    qDebug() << stateString;
    emit pushUpdate(stateString);
}
