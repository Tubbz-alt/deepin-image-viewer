#ifndef BLUREFRAME_H
#define BLUREFRAME_H

#include <QFrame>

class BlureFrame : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ pos WRITE setPos)
public:
    explicit BlureFrame(QWidget *parent, QWidget *source);
    void setSourceWidget(QWidget *source);
    void setCoverBrush(const QBrush &brush);
    void setBlureRadius(int radius);
    void setPos(const QPoint &pos);
    void moveWithAnimation(int x, int y);

    int getBorderRadius() const;
    void setBorderRadius(int borderRadius);

    int getBorderWidth() const;
    void setBorderWidth(int borderWidth);

    QColor getBorderColor() const;
    void setBorderColor(const QColor &borderColor);

protected:
    void paintEvent(QPaintEvent *);
    QPixmap getResultPixmap();

private:
    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);

private:
    QWidget *m_sourceWidget;
    QBrush m_coverBrush = QBrush(QColor(0, 0, 0, 200));
    int m_blureRadius = 13;
    QPoint m_pos;
    int m_borderRadius = 0;
    int m_borderWidth = 0;
    QColor m_borderColor;
};

#endif // BLUREFRAME_H