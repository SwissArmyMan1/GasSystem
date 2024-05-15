#include "rotatedlabel.h"

#include <QPainter>

RotatedLabel::RotatedLabel(QWidget *parent)
    : QLabel(parent)
{
    const int ms = 1000;
    m_animation.setTargetObject(this);
    m_animation.setPropertyName("angle");
    m_animation.setDuration(ms);
    m_animation.setStartValue(angle());

    int sign = m_clockwise ? 1 : -1;
    m_animation.setEndValue(angle() + sign * m_dps * ms / 1000);

    // When animation is finished then run animation again
    connect(&m_animation, &QPropertyAnimation::finished, this, [this]() { runAnimationAgain(); });

    QMetaObject::invokeMethod(this, [this]() { afterConstructor(); }, Qt::QueuedConnection);
//    QMetaObject::invokeMethod(this, [this]() { Fan(); }, Qt::QueuedConnection);
}

qreal RotatedLabel::angle() const
{
    return m_angle;
}

void RotatedLabel::setAngle(qreal deg)
{
    m_angle = deg;
    updateRotation();
}

bool RotatedLabel::isRotating() const
{
    return m_isRotating;
}

void RotatedLabel::startRotate()
{
    m_animation.start();
    m_isRotating = true;
}

void RotatedLabel::stopRotate()
{
    m_isRotating = false;
    m_animation.stop();
}

void RotatedLabel::runAnimationAgain()
{
    if (isRotating())
    {
        startRotate();
    }
}

void RotatedLabel::afterConstructor()
{
    updateOriginalPixmap();
}
//void Fan();

void RotatedLabel::updateRotation()
{
    QPixmap px = QPixmap(m_originalPixmap.size());
    px.fill(Qt::transparent);

    QPainter painter(&px);

    QPointF center = QPointF(px.size().width() / 2., px.size().height() / 2.);

    QTransform transform;
    transform.translate(center.x(), center.y());
    transform.rotate(angle());

    QRectF imageRect = m_originalPixmap.rect();
    imageRect.moveTo(-center);
    painter.setTransform(transform);
    painter.drawPixmap(imageRect, m_originalPixmap, m_originalPixmap.rect());

    setPixmap(px);
}

void RotatedLabel::updateOriginalPixmap()
{
    if (pixmap())
    {
        m_originalPixmap = *pixmap();
    }
}

void RotatedLabel::setDegreePerSecond(qreal dps)
{
    m_dps = dps;
}

qreal RotatedLabel::degreePerSecond() const
{
    return m_dps;
}
