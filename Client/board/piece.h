#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <qpixmap.h>
#include <QDebug>

#include "global.h"

typedef union Coord{
    quint32 dword;
    quint8  coord;
    struct{
        unsigned char c : 4;
        unsigned char r : 4;
    };

    Coord() {}

    Coord(quint8 cr)
    {
        coord = cr;
    }
    Coord(int r, int c)
    {
        this->c = c;
        this->r = r;
    }

    Coord& mirror_k1()
    {
        this->r = 0xf - r;
        this->c = 0xe - c;
        return *this;
    }
}Coord;


class Piece : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Piece(int id, QQuickPaintedItem *parent = nullptr);
public:
    void paint(QPainter* p) override;
public:
    void setSelected(bool select);
    void setHovered(bool hover);
    void setPos(int pos, bool rBottom);
    bool selected() {return _selected;}

    int pos(){return this->_pos;}
protected:
    //bool eventFilter(QObject* watched, QEvent* ev) override;
    void hoverEnterEvent(QHoverEvent* ev) override;
    void hoverLeaveEvent(QHoverEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;
private:
    void init();
    void loadImage();

public:
    QString _name;
    int _id;
private:
    int _pos;

private:
    bool _selected;
    bool _hovered;
private:
    static QVector<QPixmap*>* imgPieces;

private:
    bool _marked;

signals:
    void clicked();
};

#endif // PIECE_H
