#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

//! [0]
class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(QObject *parent = nullptr);
    ~RenderThread();

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize, double devicePixelRatio);

signals:
    void renderedImage(const QImage &image, double scaleFactor);

protected:
    void run() override;

private:
    static uint rgbFromWaveLength(double wave);

    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    double devicePixelRatio;
    QSize resultSize;
    bool restart = false;
    bool abort = false;

    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];
};

#endif // RENDERTHREAD_H
