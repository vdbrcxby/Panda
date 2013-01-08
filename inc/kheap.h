
// @Name   : SORTLINK_H 
//
// @Author : Yukang Chen (moorekang@gmail.com)
// @Date   : 2012-03-20 05:34:50
//
// @Brief  : this is used for kheap, make sure this link list is sorted 

#if !defined(SORTLINK_H)
#define SORTLINK_H

#include <system.h>

typedef void* type_t;
typedef long Align ;

union header {
    struct {
        union header* next;
        u32  size;
    }s;
    Align x; //this is never used!
};

typedef union header Header;

#define KHEAP_START_ADDR    0xF0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define KHEAP_END_ADDR      0xF0100000

void kheap_init(void* start_addr, void* end_addr);
void kfree(void* ap);
void* kmalloc(u32 nbytes);
void* kmalloc_align(u32 nbytes, u32 align);

#endif
