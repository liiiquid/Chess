#include "marka.h"

#include <QPainter>

MarkA::MarkA(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
    _r = 7;
    _dropColor = QColor(255, 0, 0);
    _beginMoveColor = QColor(0, 255, 0);
    _endMoveColor = QColor(0, 255, 0);
    _markBit = 0;

}

void MarkA::updateMark(int markBit)
{
    _markBit ^= markBit;
    update();
}

void MarkA::paint(QPainter *p)
{

    if(_markBit == MARK_MOVE_BEGIN )
    {
        drawMoveBegin(p);
    }
    else if(_markBit == MARK_MOVE_END)
    {
        drawMoveEnd(p);
    }
    else if(_markBit == MARK_DROP)
    {
        drawDropPoint(p);

    }else if( _markBit == ( MARK_DROP | MARK_MOVE_BEGIN ))
    {
        drawMoveBegin(p);
        drawDropPoint(p);
    }else if(_markBit == (MARK_DROP | MARK_MOVE_END) )
    {
        drawMoveEnd(p);
        drawDropPoint(p);
    }
}

void MarkA::drawDropPoint(QPainter *p)
{
    QPen pen;
    pen.setWidth(3);
    pen.setColor(_dropColor);
    p->setPen( pen);
    p->drawEllipse( width() / 2 - _r, height() / 2 - _r, _r << 1, _r << 1);
}

void MarkA::drawMoveBegin(QPainter *p)
{
    QPen pen;
    pen.setWidth(5);
    pen.setColor(_beginMoveColor);
    p->setPen( pen);
    p->drawRect( 1, 1, width() - 1, height() - 1);
}

void MarkA::drawMoveEnd(QPainter *p)
{
    QPen pen;
    pen.setWidth(5);
    pen.setColor(_beginMoveColor);
    p->setPen( pen);
    p->drawRect( 1, 1, width() - 1, height() - 1);
}

