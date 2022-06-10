#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPainterPath>
#include <QPropertyAnimation>


class Animation : public QPropertyAnimation
{
public:
    enum PathType {
        LinearPath,
        CirclePath,
        NPathTypes,
    };

public:
    Animation(QObject* target, const QByteArray& prop) : QPropertyAnimation(target, prop)
    {
        setPathType (LinearPath);
    }

    void setPathType (PathType pathType)
    {
        if (pathType > NPathTypes) {
            return;
        }
        mPathType = pathType;
        mPath = QPainterPath();
    }

    void updateCurrentTime (int currentTime) override
    {
        if (mPathType == CirclePath) {
            if (mPath.isEmpty()) {
                QPointF to = endValue().toPointF();
                QPointF from = startValue().toPointF();
                mPath.moveTo(from);
                mPath.addEllipse(QRectF(from, to));
            }
            int dura = duration();
            const qreal progress = ((dura == 0) ? 1 : ((((currentTime - 1) % dura) + 1) / qreal(dura)));

            qreal easedProgress = easingCurve().valueForProgress(progress);
            if (easedProgress > 1.0) {
                easedProgress -= 1.0;
            } else if (easedProgress < 0) {
                easedProgress += 1.0;
            }
            QPointF pt = mPath.pointAtPercent(easedProgress);
            updateCurrentValue(pt);
            emit valueChanged(pt);
        } else {
            QPropertyAnimation::updateCurrentTime(currentTime);
        }
    }

public:
    QPainterPath mPath;
    PathType mPathType;
};

#endif // ANIMATION_H
