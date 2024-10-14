#include "ucci.h"

const char* startpos = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

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
            this->charBuffer = startpos;
            this->coordList.resize(0);
        }
        else
        {
            i = 1;
            this->charBuffer = arg[1];
            while( i < arg.length() && arg[i++] != "moves" );
            i+=1;
            while(i < arg.length() )
            {
                coordList.push_back(ConvertToArrayCoord(arg[i]));
                i++;
            }
        }
        comm.side = arg[2] == "b" ? 1 : 0;
        comm.fen = &this->charBuffer;
        comm.coordList = &this->coordList;
        return position;
    }else if(arg[0] == "go")
    {

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
    }
    return POS_ERROR;
}


