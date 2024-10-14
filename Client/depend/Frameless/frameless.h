#ifndef FRAMELESS_H
#define FRAMELESS_H

#include <QQuickWindow>

class Frameless : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(QVector<QRect> disableArea MEMBER _disableArea)
enum PosState{
    Normal = 0,
    Up,
    UpLeft,
    UpRight,
    Left,
    Right,
    Down,
    DownLeft,
    DownRight
};

public:
    Frameless(QWindow *parent = nullptr);
    ~Frameless();
public:
    Q_INVOKABLE void appendArea(const QRect& rect)
    {
        qDebug() << rect;
        this->_disableArea.push_back(rect);
    }
protected:
    // void mouseMoveEvent(QMouseEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;
    //void keyPressEvent(QKeyEvent *ev) override;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void checkPosState(const QPoint& pos);
    void setPosCursor();

private:
    void processPoState_onLeftButton(const QPoint& pos);
    void changeWindowGeometry(const QPoint& pos, const QSize& size);
private:
    void init();
private:
    bool inDisableArea(const QPoint& pos);

private:
    int _padding;

private:
    PosState _poState;

private:
    QPoint _mousePressedPos_global;
    QPoint _originPos;
    QSize _originSize;

private:
    QVector<QRect> _disableArea;
signals:
    void disableAreaChanged(QVector<QRect>& areas);


};
#endif // FRAMELESS_H
