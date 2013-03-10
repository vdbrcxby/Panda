
// @Name   : bitmap.c 
//
// @Author : Yukang Chen (moorekang@gmail.com)
// @Date   : 2012-01-09 07:54:31
//
// @Brief  :

#include <system.h>
#include <bitmap.h>
#include <asm.h>

u32* frames;   //in page.c
u32  nr_frames;

inline static void INIT(u32 addr, u32* frame,
                        u32* index, u32* offset) {
    *frame  = addr/KB;
    *index  = INDEX(*frame);
    *offset = OFFSET(*frame);
}

void set_frame(u32 addr) {
    u32 frame, index, offset;
    INIT(addr, &frame, &index, &offset);
    frames[index] |= (0x1<<offset);
}

void clear_frame(u32 addr) {
    u32 frame, index, offset;
    INIT(addr, &frame, &index, &offset);
    frames[index] &= ~(0x1<<offset);
}

int test_frame(u32 addr) {
    u32 frame, index, offset;
    INIT(addr, &frame, &index, &offset);
    return frames[index] & (0x1<<offset);
}

u32 first_frame() {
    u32 index;
    for(index=0; index<INDEX(nr_frames); index++){
        if(frames[index] == 0xFFFFFFFF) //full
            continue;
        u32 v = frames[index];
        return 32*index + first_zerobit(v) - 1;
    }
    return -1;
}