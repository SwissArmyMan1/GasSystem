#ifndef ROTATEDLABEL_H
#define ROTATEDLABEL_H

#include <QLabel>
#include <QPropertyAnimation>



class RotatedLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)

public:
    RotatedLabel(QWidget *parent = nullptr);

    qreal angle() const;
    void setAngle(qreal deg);

    bool isRotating() const;

    qreal degreePerSecond() const;
    void setDegreePerSecond(qreal dps);

    qreal m_dps = 360;

public slots:
    void startRotate();
    void stopRotate();

private slots:
    void runAnimationAgain();

private:
    void afterConstructor();
//    void Fan();

    void updateRotation();
    void updateOriginalPixmap();

private:
//    qreal m_dps = 360;            ///< degree per second
    const bool m_clockwise = true;     ///< true - clockwise, false - counterclockwise

    qreal m_angle = 0;                  ///< in deg
    bool m_isRotating = false;
    QPropertyAnimation m_animation;
    QPixmap m_originalPixmap;
};
#endif // ROTATEDLABEL_H
