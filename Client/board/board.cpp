#include "board.h"

#include <iomanip>
#include <qpainter.h>

const int RowTopPos = 37;
const int ColLeftPos = 37;
const int IntervalPos = 56;

QPixmap* Board::imgBoard = nullptr;


Board::Board(QQuickItem *parent)
    : QQuickPaintedItem{parent}
{
    init();
}

void Board::addPiece(int pos, int pc)
{
    Piece* p = new Piece(pc, this);
    pieces[pc] = p;
    board[pos] = pc;
    p->setPos(pos, rBottom);
    p->setParentItem(this);
    p->setVisible(true);
    connect(p, &Piece::clicked, this, &Board::onPieceClicked);
}

void Board::delPiece(int pos)
{
    int pc = board[ pos ];
    pieces[pc]->setVisible(false);
    pieces[pc]->deleteLater();
    pieces[pc] = nullptr;
    board[pos] = 0;

}

void Board::movePiece( Move* mv )
{
    int pcCaptured;
    int pcMoved;
    int src, dst;
    Piece* captured;
    Piece* moved;

    src = mv->o1;
    dst = mv->oe;

    pcMoved = board[src];

    moved = pieces[pcMoved];

    pcCaptured = board[dst];
    if(pcCaptured != 0 )
    {
        mv->capture = pcCaptured;
        captured = pieces[pcCaptured];
        captured->setVisible(false);
    }else{
        mv->capture = 0;
    }

    moved->setPos( dst, rBottom);

    board[src] = 0;
    board[dst] = moved->_id;


}

void Board::undoMovePiece(const Move& mv)
{

    int src = mv.o1;
    int dst = mv.oe;
    int pcCap = mv.capture;
    int pcMoved = board[dst];

    Piece* captured = pieces[pcCap];
    Piece* moved = pieces[pcMoved];
    if(captured != nullptr )
    {
        captured->setPos(moved->pos(), rBottom);
        captured->setVisible(true);
    }

    moved->setPos(src, rBottom);

    board[dst] = pcCap;
    board[src] = pcMoved;

}

void Board::makeMove(int mv)
{

    socket->sendPacket( "makemv " + ConvertToUcciCoord(mv));
}

void Board::undoMakeMove()
{
    // if(trace.size() == 0) return;
    // const Trace& t = trace.last();
    // undoMovePiece(t.mv);
    // trace.pop_back();

    socket->sendPacket( "undomakemv ");

}

void Board::mirror_k1()
{
    int i;

    setDropPoint(false);
    rBottom = !rBottom;
    //fromFen(toFen());
    for(i = SIDE_TAG(0) + KING_FROM; i < SIDE_TAG(0) + 32; i++)
    {
        if(pieces[i] != nullptr && pieces[i]->isVisible())
        {
            pieces[i]->setPos(pieces[i]->pos(), rBottom);
        }
    }

    setDropPoint(false);

    mirrorMarkMove();



}
void Board::mirrorMarkMove()
{
    if(trace.size() == 0 ) return;
    Trace lastStep;
    Coord src, dst;
    int xr, xc;
    lastStep = trace[trace.size() - 1];
    src.coord = lastStep.mv.o1;
    dst.coord = lastStep.mv.oe;
    if(rBottom == false)
    {
        xr = GET_REL_ROW(src.coord);
        xc = GET_REL_COL(src.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_BEGIN);
        xr = GET_REL_ROW(dst.coord);
        xc = GET_REL_COL(dst.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_END);
        src.mirror_k1();
        dst.mirror_k1();
        xr = GET_REL_ROW(src.coord);
        xc = GET_REL_COL(src.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_BEGIN);
        xr = GET_REL_ROW(dst.coord);
        xc = GET_REL_COL(dst.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_END);
    }else
    {
        src.mirror_k1();
        dst.mirror_k1();
        xr = GET_REL_ROW(src.coord);
        xc = GET_REL_COL(src.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_BEGIN);
        xr = GET_REL_ROW(dst.coord);
        xc = GET_REL_COL(dst.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_END);
        src.mirror_k1();
        dst.mirror_k1();
        xr = GET_REL_ROW(src.coord);
        xc = GET_REL_COL(src.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_BEGIN);
        xr = GET_REL_ROW(dst.coord);
        xc = GET_REL_COL(dst.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_END);
    }
}

void Board::print()
{
    printBoard();
}

void Board::getpos()
{
    socket->sendPacket("getpos");
}

void Board::setpos()
{
    position( buildPositionInstruct(toFen(), QStringList(), true) );
    getpos();
}

void Board::getmv(int pos)
{
    QString text = ConvertToUcciPoint(pos);
    socket->sendPacket("getmv " + text);
}

void Board::ucci()
{
    socket->sendPacket("ucci");
}

void Board::isready()
{
    socket->sendPacket("isready");
}

void Board::startpos()
{
    position(buildPositionInstruct("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR", QStringList(), true));
    getpos();
}

void Board::position(const QString &position)
{
    socket->sendPacket(position);
}

QString Board::buildPositionInstruct(const QString &fen, const QStringList &moves, bool rTurn)
{
    QString message = "position ";
    message += fen + " ";

    message += QString(rTurn == true ? "r" : "b") + " ";

    if(moves.size() > 0)
    {
        message += "moves ";
        for(auto& move: moves)
        {
            message += move + " ";
        }
    }
    message.remove(message.size() - 1, 1);
    return message;

}

void Board::selectPiece(int pc)
{
    _boardState = CheckSelectPiece;
    socket->sendPacket("selpiece " + QString::number(pc));
}

void Board::go(const QString& instruct)
{
    socket->sendPacket( instruct);
}

void Board::protect(const QString &cr)
{
    socket->sendPacket("protect " + cr);
}

void Board::resetBoard()
{
    memset(board, 0, sizeof(board));
    for(int i = SIDE_TAG(0); i < SIDE_TAG(0) + 32; i++)
    {
        if(pieces[i] != nullptr)
            pieces[i]->setVisible(false);
    }

    if(_selectedPiece != nullptr )
    {
        _selectedPiece->setSelected(false);
        _selectedPiece = nullptr;
    }
    trace.resize(0);
    _boardState = Selectable;

    setDropPoint(true);

    for(int i = 0; i < 90; i++)
    {
        markVector[i]->resetMarkBit();
    }

}

void Board::fromFen(const QString &fen)
{

    resetBoard();

    int i = 0, j = 0;
    int k = 0;
    int x;
    int pcWhite[7], pcBlack[7];
    pcWhite[0] = KING_FROM + SIDE_TAG(0);
    pcWhite[1] = ADVISOR_FROM+ SIDE_TAG(0);
    pcWhite[2] = BISHOP_FROM+ SIDE_TAG(0);
    pcWhite[3] = KNIGHT_FROM+ SIDE_TAG(0);
    pcWhite[4] = ROOK_FROM+ SIDE_TAG(0);
    pcWhite[5] = CANNON_FROM+ SIDE_TAG(0);
    pcWhite[6] = PAWN_FROM+ SIDE_TAG(0);

    for(k = 0; k < 7; k++)
    {
        pcBlack[k] = pcWhite[k] + 16;
    }
    for(k = 0; k < fen.length(); k++)
    {
        if(fen[k] >= 'A' && fen[k] <= 'Z')
        {
            x = FenPiece(fen[k]);
            if( pieces[pcWhite[x]] != nullptr )
            {
                pieces[pcWhite[x]]->setVisible(true);
                board[COORD_PC(i, j)] = pcWhite[x];
                pieces[pcWhite[x]]->setPos(COORD_PC(i, j), rBottom);

            }else{
                addPiece( COORD_PC(i, j), pcWhite[x] );
            }

            pcWhite[x] += 1;
            j += 1;
        }
        else if(fen[k] >= 'a' && fen[k] <= 'z')
        {
            x = FenPiece(fen[k]);
            if( pieces[pcBlack[x]] != nullptr )
            {
                pieces[pcBlack[x]]->setVisible(true);
                board[COORD_PC(i, j)] = pcBlack[x];
                pieces[pcBlack[x]]->setPos(COORD_PC(i, j), rBottom);

            }else
            {
                addPiece( COORD_PC(i, j), pcBlack[x] );
            }
            pcBlack[x] += 1;
            j += 1;
        }
        else if( fen[k] >= '0' && fen[k] <= '9')
        {
            j += fen[k].digitValue();
        }else if(fen[k] == '/')
        {
            i++;
            j = 0;
        }
    }
}

QString Board::toFen()
{
    int nSpace;
    QString ret;
    int pos;
    int rSideTag = SIDE_TAG(0);
    ret = "";
    for(int i = ROW_TOP; i < ROW_TOP + 10; i++)
    {
        nSpace = 0;
        for(int j = COL_LEFT; j < COL_LEFT + 9; j++ )
        {
            pos = COORD_XY(i, j);
            if(board[ pos ] == 0)
            {
                nSpace += 1;
            }else
            {
                if(nSpace > 0)
                {
                    ret += QString::number(nSpace);
                    nSpace = 0;
                }

                if( board[ pos ] >= rSideTag && board[pos] < rSideTag + 16 )
                {
                    ret += ToFenPiece(board[pos]);
                }else
                {
                    ret += ToFenPiece(board[pos]).toLower();
                }
            }
        }
        if(nSpace > 0)
        {
            ret += QString::number(nSpace);
        }
        ret += "/";
    }
    ret.remove(ret.size() - 1, 1);
    return ret;
}
#include <iostream>
using namespace std;
void Board::printBoard()
{
    for(int i = 0; i < 16; i++ )
    {
        for(int j = 0; j < 16; j++)
        {
            cout << setfill('0') << setw(2) << (int)board[i * 16 + j] << " ";
        }
        cout << endl;
    }
}

void Board::printPieces()
{
    for(int i = SIDE_TAG(0); i < 50; i++)
    {
        if(pieces[i] != nullptr)
        {
            qDebug() << GET_ROW(pieces[i]->pos()) << GET_COL(pieces[i]->pos()) << pieces[i]->position();
        }
    }
}

void Board::paint(QPainter *p)
{
    paintBackground(p);
    //paintCoord(p);

}

void Board::paintBackground(QPainter *p)
{
    p->drawPixmap(1, 1, imgBoard->width(), imgBoard->height(), *imgBoard);
    p->setPen(QColor(255, 0, 0));
    int row_top = 37;
    int col_left = 37;
    int interval = 56;
    int i;
    int w = imgBoard->width();
    int h = imgBoard->height();
    int x, y;
    // for( i = 0; i < 9; i++)
    // {
    //     x = col_left + i * interval;
    //     y = row_top;
    //     p->drawLine(QLine( QPoint(x, y), QPoint(x, y + h)));
    // }

    // for(i = 0; i < 10; i++)
    // {
    //     x = col_left;
    //     y = row_top + i * interval;
    //     p->drawLine(QLine( QPoint(x, y), QPoint(x + w, y)));
    // }

    const QString col[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i"};
    for(i = 0; i < 9; i++)
    {
        x = col_left + i * interval - 5;
        y = row_top - 20;
        p->drawText(QPoint(x, y), col[i]);
        y = h + 30;
        p->drawText(QPoint(x, y), col[i]);
    }

    const QString row[] = {"9", "8", "7", "6", "5", "4", "3", "2", "1", "0"};
    for(i = 0; i < 10; i++)
    {
        x = col_left - 20;
        y = row_top + i * interval + 5;
        p->drawText(QPoint(x, y), row[i]);
        x = w +10;
        p->drawText(QPoint(x, y), row[i]);
    }
}

bool Board::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent* me;
    //qDebug() << ((quint32*)watched == (quint32*)this);
    if(watched == this)
    {
        if(event->type() ==  QEvent::MouseButtonPress)
        {
            me = dynamic_cast<QMouseEvent*>(event);
            if(_selectedPiece != nullptr )
            {
                Move mv;
                Coord rc;
                mv.o1 = _selectedPiece->pos();
                rc = ConvertToRC(me->pos());
                if(rBottom == false)
                {
                    rc.mirror_k1();
                }
                mv.oe = rc.coord;
                if(checkValidMove(mv.mv) == false)
                {
                    emit invalidMove();
                }else
                {
                    makeMove(mv.t);
                }
            }
        }
    }
    return __super::eventFilter(watched, event);
}

void Board::init()
{
    static bool _initok = false;
    if(_initok == false)
    {
        imgBoard = new QPixmap;
        loadImage();
        _initok = true;
    }

    _boardState = Selectable;
    _selectedPiece = nullptr;
    memset(pieces, 0, sizeof(pieces));

    installEventFilter(this);

    setAcceptedMouseButtons(Qt::AllButtons);
    //setAcceptHoverEvents(true);
    // const char* startpos = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

    // fromFen(startpos);

    socket = new TcpSocket("127.0.0.1", "9999", this);
    connect(socket, &TcpSocket::receivePacket, this, &Board::onReceivePacket);
    connect(socket, &TcpSocket::connected, this, &Board::onConnected);
    connect(socket, &TcpSocket::disconnected, this, &Board::onDisconnected);
    connect(socket, &TcpSocket::reconnecting, this, &Board::onReconnecting);
    int i, j, k;
    MarkA* mark;
    QSize pieceSize = QPixmap(":/img/board1/ra.ico").size();
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 9; j++)
        {
            k = i * 9 + j;

            mark = new MarkA(this);
            mark->setParent(this);
            mark->setZ(1);
            mark->setPosition(ConvertToBoardPos(Coord(i + ROW_TOP, j + COL_LEFT)));
            mark->setSize(pieceSize);
            mark->setVisible(true);
            markVector.push_back(mark);
        }
    }

    resetBoard();

}

void Board::loadImage()
{
    const QString path = ":/img/board1/";
    if( imgBoard->load(path + "/BOARD.BMP") == false )
    {
        qDebug() << "imgBoard laod failed";
    }
}
/*
 coord, 存储数组棋盘坐标
 pos, 为屏幕坐标
 ColLeftPos, 第一列左侧的空白部分像素值偏移.
 RowTopPos,第一行上面的空白部分像素值偏移
 IntervalPos, 行和列之间的像素距离.
 ROW_TOP, 数组棋盘的行坐标起始偏移.
 COL_LEFT, 数组棋盘的列坐标起始偏移.
*/
Coord Board::ConvertToRC(const QPoint &pos)
{
    Coord coord;
    int x = pos.x() - ColLeftPos;
    int y = pos.y() - RowTopPos;
    coord.r = y / IntervalPos + ROW_TOP;
    coord.c = x / IntervalPos + COL_LEFT;
    if( (y % IntervalPos) > ( IntervalPos >> 1)) coord.r += 1;
    if( (x % IntervalPos) > ( IntervalPos >> 1)) coord.c += 1;
    return coord;
}

QPoint Board::ConvertToBoardPos(const Coord &cr)
{
    QPoint pos;
    pos.setX( (cr.c - COL_LEFT) * IntervalPos + ColLeftPos - (IntervalPos >> 1) );
    pos.setY( (cr.r - ROW_TOP) * IntervalPos + RowTopPos - (IntervalPos >> 1) );
    pos.isNull();
    return pos;
}

void Board::onPieceClicked()
{
    if(_boardState != Selectable ) return;
    Piece* p = dynamic_cast<Piece*>(sender());
    if( _selectedPiece != nullptr )
    {
        if(_selectedPiece == p)
        {
            _selectedPiece->setSelected(false);
            _selectedPiece = nullptr;
            setDropPoint(true);
            return;
        }
        else{
            Move mv = {0};
            mv.o1 = _selectedPiece->pos();
            mv.oe = p->pos();
            if(checkValidMove(mv.mv) == false)
            {
                emit invalidMove();
            }
            else
            {
                makeMove( mv.t );
            }

        }

    }else
    {
        selectPiece(p->_id);
    }
}

bool Board::doMakeMove(int mv)
{   
    Move* move = (Move*)&mv;
    Piece* moved = pieces[board[move->o1]];
    trace.push_back(Trace() );
    movePiece( move );
    if(moved->selected() )
    {
        Q_ASSERT(moved == _selectedPiece);
        moved->setSelected(false);
        _selectedPiece = nullptr;
    }
    trace.last().mv = *move;
    setDropPoint(true);
    markMove();


    return true;
}

bool Board::doUndoMakeMove(Move mv)
{
    undoMovePiece(mv);
    markMove();
    trace.removeLast();
    return true;
}

bool Board::checkValidMove(int mv)
{
    const QString& checkedMv = ConvertToUcciCoord(mv);
    bool valid = false;
    for(auto& ucciMv : markmv)
    {
        if(ucciMv == checkedMv)
        {
            valid = true;
            break;
        }
    }
    return valid;
}

void Board::markMove()
{
    Trace lastlastStep;
    Trace lastStep;
    Coord src;
    Coord dst;
    int xr,xc;
    if(trace.size() > 1)
    {
        lastlastStep = trace[trace.size() - 2 ];
        src.coord = lastlastStep.mv.o1;
        dst.coord = lastlastStep.mv.oe;

        if(rBottom == false)
        {
            src.mirror_k1();
            dst.mirror_k1();
        }
        xr = GET_REL_ROW(src.coord);
        xc = GET_REL_COL(src.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_BEGIN);
        xr = GET_REL_ROW(dst.coord);
        xc = GET_REL_COL(dst.coord);
        markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_END);
    }

    lastStep = trace[trace.size() - 1];
    src.coord = lastStep.mv.o1;
    dst.coord = lastStep.mv.oe;

    if(rBottom == false)
    {
        src.mirror_k1();
        dst.mirror_k1();
    }

    xr = GET_REL_ROW(src.coord);
    xc = GET_REL_COL(src.coord);
    markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_BEGIN);
    xr = GET_REL_ROW(dst.coord);
    xc = GET_REL_COL(dst.coord);
    markVector[ xr * 9 + xc ]->updateMark(MARK_MOVE_END);
}


void Board::setDropPoint(bool hide)
{
    Move mv;
    int i;
    if(markmv.size() > 0)
    {
        for(auto& ucciMove:markmv)
        {
            mv.t = ConvertToArrayCoord(ucciMove);
            Coord cr( mv.oe );
            cr = rBottom ? cr : cr.mirror_k1();
            i = ( cr.r - ROW_TOP) * 9 + ( cr.c - COL_LEFT );
            markVector[i]->updateMark(MARK_DROP);
        }
    }
    if(hide)
    {
        markmv.resize(0);
    }

}
void Board::onReceivePacket(const QString &a)
{
    QString instruct = a;
    if(instruct.endsWith(' '))
    {
        instruct.remove(instruct.size() - 1, 1);
    }
    qDebug() << instruct;
    int i;
    const QStringList& arg = instruct.split(" ");
    if(arg[0] == "getpos")
    {
        fromFen(arg[1]);
    }else if(arg[0] == "getmv")
    {
        for( i = 1; i < arg.size(); i++)
        {
            markmv.push_back(arg[i]);
        }
        setDropPoint(false);
    }else if(arg[0] == "makemv")
    {
        if(arg[1] != "illegal")
        {
            int mv = ConvertToArrayCoord(arg[1]);
            if( doMakeMove(mv) == false)
            {
                emit invalidMove();
            }
            emit goRecovered();
        }else
        {
            emit invalidMove();
            if(_selectedPiece != nullptr)
            {
                _selectedPiece->setSelected(false);
                _selectedPiece = nullptr;
            }

            setDropPoint(true);
        }
    }else if(arg[0] == "selpiece")
    {
        if( arg[1] == "illegal" )
        {
            emit selectPieceError();
        }else
        {
            _selectedPiece = pieces[arg[1].toInt()];
            _selectedPiece->setSelected(true);

            for( i = 2; i < arg.size(); i++)
            {
                markmv.push_back(arg[i]);
            }
            setDropPoint(false);
        }
        _boardState = Selectable;
    }else if(arg[0] == "undomakemv")
    {
        if(arg[1] != "illegal")
        {
            int mv = ConvertToArrayCoord(arg[1]);
            int cap = arg[2].toInt();
            Move v;
            v.mv = mv;
            v.cap = cap;
            doUndoMakeMove(v);
        }else
        {
            emit undomakeError();
        }

    }else if( arg[0] == "go" )
    {
        if(arg[1] == "win" || arg[1] == "resign")
        {
            emit gamewin( arg[2] + " " + arg[1]);
            emit goRecovered();
        }else
        {
            makeMove( ConvertToArrayCoord(arg[1]) );
        }
    }

}

void Board::onConnected()
{
    emit netStateChanged("connected");
}

void Board::onDisconnected()
{
    emit netStateChanged("unconnected");
}

void Board::onReconnecting()
{
    emit netStateChanged("reconnecting");
}
