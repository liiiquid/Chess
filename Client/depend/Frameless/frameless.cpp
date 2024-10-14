#include "frameless.h"

#include <QCoreApplication>

Frameless::Frameless(QWindow *parent)
    : QQuickWindow(parent)
{
    init();
}

Frameless::~Frameless() {}

// void Frameless::mouseMoveEvent(QMouseEvent *event)
// {
//     if(event->buttons() & Qt::LeftButton)
//     {
//         QPoint pos = event->globalPos();
//         processPoState_onLeftButton(pos);
//     }else
//     {
//         checkPosState(event->pos());
//         setPosCursor();
//     }
// }

// void Frameless::mousePressEvent(QMouseEvent *event)
// {
//     _mousePressedPos_global = event->globalPos();
//     _originPos = position();
//     _originSize = size();
// }

// void Frameless::keyPressEvent(QKeyEvent *ev)
// {

// }

bool Frameless::eventFilter(QObject *object, QEvent *ev)
{
    if(object == this)
    {
        QMouseEvent* event = nullptr;
        if(ev->type()==QEvent::MouseMove)
        {
            event = dynamic_cast<QMouseEvent*>(ev);
            if(inDisableArea(event->pos()) == false)
            {
                if(event->buttons() & Qt::LeftButton)
                {
                    QPoint pos = event->globalPos();
                    processPoState_onLeftButton(pos);
                }else
                {
                    checkPosState(event->pos());
                    setPosCursor();
                }
            }


        }else if(ev->type() == QEvent::MouseButtonPress)
        {
            event = dynamic_cast<QMouseEvent*>(ev);
            if(inDisableArea(event->pos()) == false)
            {
                _mousePressedPos_global = event->globalPos();
                _originPos = position();
                _originSize = size();
            }
        }else if(ev->type() == QEvent::KeyPress)
        {
            QKeyEvent* e = dynamic_cast<QKeyEvent*>(ev);
            switch(e->key())
            {
            case Qt::Key_Escape:
                QCoreApplication::exit(0);
                break;
            case Qt::Key_1:
                for(auto& area:_disableArea)
                {
                    qDebug() << area;
                }
                qDebug() << "area print ok";
                break;

            }
        }
    }

    return __super::eventFilter(object, ev);
}

void Frameless::checkPosState(const QPoint& pos)
{
    if(pos.y() <= _padding )
    {
        if(pos.x() <= _padding)
        {
            _poState = UpLeft;
        }else if(pos.x() >= width() - _padding)
        {
            _poState = UpRight;
        }else
        {
            _poState = Up;
        }
    }
    else if(pos.y() >= height() - _padding)
    {
        if(pos.x() <= _padding)
        {
            _poState = DownLeft;
        }else if(pos.x() >= width() - _padding)
        {
            _poState = DownRight;
        }else
        {
            _poState = Down;
        }
    }

    else if(pos.x() <= _padding )
    {
        _poState = Left;
    }else if(pos.x() >= width() - _padding)
    {
        _poState = Right;
    }
    else
    {
        _poState = Normal;
    }
}

void Frameless::init()
{
    setFlags(flags() | Qt::FramelessWindowHint);
    _padding = 5;
    installEventFilter(this);
}

bool Frameless::inDisableArea(const QPoint &pos)
{
    for(auto& area:_disableArea)
    {
        if( area.contains(pos) )
        {
            return true;
        }
    }
    return false;
}

void Frameless::setPosCursor()
{
    switch(_poState)
    {
    case Normal:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    case Up:
        setCursor(QCursor(Qt::SizeVerCursor));
        break;
    case UpLeft:
        setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case UpRight:
        setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case Left:
        setCursor(QCursor(Qt::SizeHorCursor));
        break;
    case Right:
        setCursor(QCursor(Qt::SizeHorCursor));
        break;
    case Down:
        setCursor(QCursor(Qt::SizeVerCursor));
        break;
    case DownLeft:
        setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case DownRight:
        setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    }
}

/**
 * @brief Frameless::processPoState
 * @param pos, which equals mousePressPoint - curPoint.
 */
void Frameless::processPoState_onLeftButton(const QPoint &pos)
{
    int dw = _mousePressedPos_global.x() - pos.x();
    int dh = _mousePressedPos_global.y() - pos.y();
    switch(_poState)
    {
    case Up:
        changeWindowGeometry(_originPos + QPoint(0, -dh), _originSize + QSize(0, dh));
    break;
    case UpLeft:
        changeWindowGeometry(_originPos + QPoint(-dw, -dh), _originSize + QSize(dw, dh));
        break;
    case UpRight:
        changeWindowGeometry(_originPos + QPoint(0, -dh), _originSize + QSize(-dw, dh));
        break;
    case Left:
        changeWindowGeometry(_originPos + QPoint(-dw, 0), _originSize + QSize(dw, 0));
        break;
    case Right:
        changeWindowGeometry(_originPos + QPoint(0, 0), _originSize + QSize(-dw, 0));
        break;
    case Down:
        changeWindowGeometry(_originPos + QPoint(0, 0), _originSize + QSize(0, -dh));
        break;
    case DownLeft:
        changeWindowGeometry(_originPos + QPoint(-dw, 0), _originSize + QSize(dw, -dh));
        break;
    case DownRight:
        changeWindowGeometry(_originPos + QPoint(0, 0), _originSize + QSize(-dw, -dh));
        break;
    case Normal:
        setGeometry(QRect(_originPos + QPoint(-dw, -dh), _originSize));
        break;
    }
}

void Frameless::changeWindowGeometry( const QPoint &pos, const QSize &size)
{
    QRect r(pos, size);
    if(size.height() >= maximumHeight())
    {
        if(pos.y() != _originPos.y())
        {
            r.setTop(_originPos.y() + maximumHeight() - _originSize.height());
        }
        r.setHeight(maximumHeight());

    }else if(size.height() <= minimumHeight())
    {
        if(pos.y() != _originPos.y())
            r.setTop(_originPos.y() + _originSize.height() - minimumHeight());

        r.setHeight(minimumHeight());
    }

    if(size.width() >= maximumWidth())
    {
        if(pos.x() != _originPos.x())
            r.setLeft(_originPos.x() - maximumWidth() - _originSize.width());

        r.setWidth(maximumWidth());
    }else if(size.width() <= minimumWidth())
    {
        if(pos.x() != _originPos.x())
            r.setLeft(_originPos.x() + _originSize.width() - minimumWidth());
        r.setWidth(minimumWidth());
    }

    setGeometry(r);
}
