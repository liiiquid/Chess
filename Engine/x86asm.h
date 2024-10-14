#ifndef X86ASM_H
#define X86ASM_H

unsigned char _BitScanReverse64(unsigned long* Index, unsigned __int64 Mask);

int Bsr(unsigned long mask)
{
    unsigned long res = -1;
    _BitScanReverse64( &res, mask );
    return res;
}

#endif // X86ASM_H
