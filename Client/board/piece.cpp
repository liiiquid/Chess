#include "piece.h"
#include "board.h"
#include <QPainter>
#include <QPropertyAnimation>
QVector<QPixmap*>* Piece::imgPieces = nullptr;
extern const int IntervalPos;
Piece::Piece(int id, QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
{
    static QString pieceName[] = {"K", "A", "B", "N", "R", "C", "P",
                                  "k", "a", "b", "n", "r", "c", "p"};
    _id = id;
    _name = pieceName[PIECE_TYPE_SIDE(_id)];
    init();
}

void Piece::paint(QPainter *p)
{
    const QPixmap& piece = *imgPieces->at(PIECE_TYPE_SIDE(_id));
    p->drawPixmap(0, 0, piece.width(), piece.height(), piece);

    if(_selected == true)
    {
        p->setBrush(QColor(255,0,0,127));
        p->drawRect(0, 0, this->width(), this->height());
    }

    else if(_hovered == true)
    {
        p->setBrush(QColor(100,0,0,127));
        p->drawRect(0, 0, this->width(), this->height());
    }
}

void Piece::setSelected(bool select)
{_selected = select; update();}

void Piece::setHovered(bool hover)
{_hovered = hover; update();}

void Piece::setPos(int pos, bool rBottom)
{
    this->_pos = pos;

    Coord cr;
    cr.coord = pos;
    if(rBottom == false )
    {
        cr.mirror_k1();
    }
    QPoint p = Board::ConvertToBoardPos(cr );

    setPosition(p);
}

void Piece::hoverEnterEvent(QHoverEvent *ev)
{
    setHovered(true);
}

void Piece::hoverLeaveEvent(QHoverEvent *ev)
{
    setHovered(false);
}

void Piece::mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
}

void Piece::init()
{
    static bool _initok = false;
    if(_initok == false)
    {
        imgPieces = new QVector<QPixmap*>;
        loadImage();
        _initok = true;
    }
    int imgW = imgPieces->at(PIECE_TYPE_SIDE(_id))->width();
    int imgH = imgPieces->at(PIECE_TYPE_SIDE(_id))->height();
    this->setSize(QSize(imgW, imgH));
    this->setVisible(true);

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    _selected = _hovered = _marked = false;

}

void Piece::loadImage()
{
    const QString path = ":/img/board1/";
    static const char* pcImgName[] = { "rk.ico", "ra.ico", "rb.ico", "rn.ico", "rr.ico", "rc.ico", "rp.ico",
                                      "bk.ico", "ba.ico", "bb.ico", "bn.ico", "br.ico", "bc.ico", "bp.ico"};

    for(int i = 0; i < 14; i++)
    {
        QPixmap* pixmap = new QPixmap(path + pcImgName[i]);
        if(pixmap->isNull())
        {
            qDebug() << "imgPiece: " << path + pcImgName[i] << " load failed";
        }
        imgPieces->push_back( pixmap );

    }
}
