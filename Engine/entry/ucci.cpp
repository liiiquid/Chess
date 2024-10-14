#include "ucci.h"
#include "position.h"
#include "qdebug.h"

Ucci::Ucci() {

}

UcciType Ucci::processCommand(const QString &command, UcciComm& comm)
{
    QStringList arg = command.split(' ');
    int i = 0;
    if(arg[0] == "ucci")
    {
        return ucci;
    }
    else if(arg[0] == "isready")
    {
        return isready;
    }
    else if(arg[0] == "position")
    {

        this->coordList.resize(0);

        if(arg[1] == "startpos")
        {
            const char* startpos = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";
            this->charBuffer = startpos;
            this->coordList.resize(0);
        }
        else
        {
            i = 1;
            this->charBuffer = arg[1];
        }

        while( i < arg.length() && arg[i++] != "moves" );
        i+=1;
        while(i < arg.length() )
        {
            coordList.push_back(ConvertToArrayCoord(arg[i]));
            i++;
        }
        qDebug() << arg[0] << arg[1] << arg[2];
        comm.side = arg[2] == "b" ? 1 : 0;
        comm.fen = &this->charBuffer;
        comm.coordList = &this->coordList;
        return position;
    }else if(arg[0] == "go")
    {
        if(arg[1] == "depth")
        {
            comm.gomode = Depth;
            comm.depth = arg[2].toInt();
        }else if(arg[1] == "time")
        {
            comm.gomode = Time;
            comm.limitTime = arg[2].toInt();
        }else if(arg[1] == "node")
        {
            comm.gomode = Node;
            comm.maxNode = arg[2].toInt();
        }
        return go;
    }else if(arg[0] == "stop")
    {

    }else if(arg[0] == "quit")
    {
        return quit;
    }
    else if(arg[0] == "getpos")
    {
        return getpos;
    }else if(arg[0] == "getmv")
    {
        coordList.resize(0);
        coordList.push_back( COORD_PC( 9 - arg[1][1].toLower().unicode() + '0', arg[1][0].toLower().unicode() - 'a') );
        comm.coordList = &this->coordList;
        return getmv;
    }else if(arg[0] == "makemv")
    {
        coordList.resize(0);
        coordList.push_back( ConvertToArrayCoord(arg[1]) );

        comm.coordList = &this->coordList;
        return makemv;
    }else  if(arg[0] == "selpiece")
    {
        int pc = arg[1].toInt();
        comm.pc = pc;
        return selpiece;
    }else if(arg[0] == "score")
    {
        return score;
    }else if(arg[0] == "protect")
    {
        comm.checkProtectTurn = arg[1].toInt();
        charBuffer = arg[2];
        comm.checkProtectPos = &charBuffer;
        return protect;
    }else if(arg[0] == "undomakemv")
    {
        return undomakemv;
    }
    return POS_ERROR;
}


