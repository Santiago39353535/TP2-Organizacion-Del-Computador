#ifndef TP2ORGA_CACHE_H
#define TP2ORGA_CACHE_H

#include <stdbool.h>


typedef struct bloque{
    bool v;
    char data [32];
    unsigned int usos;
    unsigned int tag;
} bloque_t;

typedef struct mem{
    bloque_t datos[125];
} mem_t;

typedef struct cache{
    //4 bits de indice
    //5 bits de offset
    //23 bits para el tag
    bloque_t lado_a [16];
    bloque_t lado_b [16];
    unsigned int misrate;
    unsigned int operaciones;
} cache_t;

void init_mem(mem_t* mem);

void init(cache_t* cache);

unsigned int read_byte(int addres, cache_t* cache, mem_t* mem);

int write_byte(int addres, unsigned char value, cache_t* cache, mem_t* mem);

unsigned int get_miss_rate(cache_t* cache);


#endif
