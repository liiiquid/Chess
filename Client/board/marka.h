#ifndef MARKA_H
#define MARKA_H

#include <QObject>
#include <QQuickPaintedItem>

const int MARK_MOVE_BEGIN = (1 << 0);
const int MARK_MOVE_END = (1 << 1);
const int MARK_DROP = (1 << 2);

class MarkA : public QQuickPaintedItem
{
    Q_OBJECT
public:
    MarkA(QQuickItem* parent = nullptr);

public:
    void setR(int r) {this->_r = r; update();}
    int r(){return _r;}
    void setDropColor(const QColor& color) {this->_dropColor = color; update();}
    void setBeginMoveColor(const QColor& color) {this->_beginMoveColor = color; update();}
    void setEndMoveColor(const QColor& color) {this->_endMoveColor = color; update();}

    void updateMark(int markBit);
    void resetMarkBit() { _markBit = 0; update(); }

public:
    void paint(QPainter* p) override;

private:
    void drawDropPoint(QPainter* p);
    void drawMoveBegin(QPainter* p);
    void drawMoveEnd(QPainter* p);

private:
    quint32 _markBit;

private:
    int _r;
    QColor _dropColor;
    QColor _beginMoveColor;
    QColor _endMoveColor;
};

#endif // MARKA_H
