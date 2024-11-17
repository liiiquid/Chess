#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QQuickPaintedItem.h>
#include <qimage.h>
#include <QPixmap>
#include <QPainter>
#include "tcpsocket.h"
#include "piece.h"
#include "marka.h"

typedef struct Trace{
    Move mv;
    Trace() {}
    Trace(const Move& v){ mv = v; }
}Trace;

typedef enum{
    Selectable,
    CheckSelectPiece,
}BoardState;

class VerticalRuler : public QQuickPaintedItem{
    Q_OBJECT
public:

    explicit VerticalRuler(QQuickItem *parent = nullptr)
        : QQuickPaintedItem{parent}
    {
    }

    void paint(QPainter* p) override
    {
        p->setPen(QColor(255, 0, 0));
        p->drawRect(QRect(0, 0, width() - 1, height() - 1));
        int row_top = 42;
        int interval = 56;
        int i;
        int x, y;
        const QString row[] = {"9", "8", "7", "6", "5", "4", "3", "2", "1", "0"};
        for(i = 0; i < 10; i++)
        {
            x = width() / 2 - 2;
            y = row_top + i * interval;
            p->drawText( QPoint(x, y), row[i]);
        }
    }
};

class HorizonRuler : public QQuickPaintedItem{
    Q_OBJECT
public:

    explicit HorizonRuler(QQuickItem *parent = nullptr)
        : QQuickPaintedItem{parent}
    {
    }

    void paint(QPainter* p) override
    {
        p->setPen(QColor(255, 0, 0));
        p->drawRect(QRect(0, 0, width() - 1, height() - 1));
        int col_left = 32;
        int interval = 56;
        int i;
        int x, y;
        const QString col[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i"};
        for(i = 0; i < 9; i++)
        {
            x = col_left + i * interval;
            y = height() / 2 + 2;
            p->drawText(QPoint(x, y), col[i]);
        }
    }
};

class Board : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit Board(QQuickItem *parent = nullptr);    

public:

    void makeMove(int mv);
    Q_INVOKABLE void undoMakeMove();
    Q_INVOKABLE void mirror_k1();
    Q_INVOKABLE void print();
    Q_INVOKABLE void getpos();
    Q_INVOKABLE void setpos();
    Q_INVOKABLE void getmv(int pos);
    Q_INVOKABLE void ucci();
    Q_INVOKABLE void isready();
    Q_INVOKABLE void startpos();
    Q_INVOKABLE void position(const QString& position);
    Q_INVOKABLE void selectPiece(int pc);
    Q_INVOKABLE void go(const QString& instruct);
    Q_INVOKABLE void protect(const QString& cr);
public:
    void fromFen(const QString& fen);
    QString toFen();
    QString buildPositionInstruct(const QString &fen, const QStringList &moves, bool rTurn);
private:
    void addPiece(int pos, int pc);
    void delPiece(int pos);
    void movePiece(Move* mv);
    void undoMovePiece(const Move& mv);
private:
    void printBoard();
    void printPieces();

public:
    void paint(QPainter* p) override;
    void paintBackground(QPainter *p);
    void setDropPoint(bool hide);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void init();
    void loadImage();
    void resetBoard();
public:
    // 令屏幕坐标转为数组棋盘坐标
    static Coord ConvertToRC(const QPoint& pos);
    // 令数组棋盘坐标转为屏幕坐标.
    static QPoint ConvertToBoardPos(const Coord& cr);

public slots:
    void onPieceClicked();

private:
    Piece* _selectedPiece;
    qint8 board[256];
    Piece* pieces[50];
    QVector<Trace> trace;
    QVector<QString> markmv;
    bool rBottom;
    static QPixmap* imgBoard;
    QVector<MarkA*> markVector;

    BoardState _boardState;

private:
    TcpSocket* socket;
private:
    bool doMakeMove(int mv);
    bool doUndoMakeMove(Move mv);
    bool checkValidMove(int mv);
    void markMove();
    void mirrorMarkMove();
private slots:
    void onReceivePacket(const QString& instruct);
signals:
    void messageReceived(const QString& rec);

private slots:
    void onConnected();
    void onDisconnected();
    void onReconnecting();
signals:
    void netStateChanged(const QString& str);
    void invalidMove();
    void selectPieceError();
    void undomakeError();
    void gamewin(const QString& winner);
    void goRecovered();

};

#endif // BOARD_H
