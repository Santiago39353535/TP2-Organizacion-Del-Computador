#include <stdio.h>
#include "Cache.h"

#define off 0x0000001F

void poner_en_cero(bloque_t* bloque);

bool esta_en_a(cache_t* cache, int indice, unsigned int tag);

bool esta_en_b(cache_t *pCache, int indice, unsigned int tag);

void cargar_bolque(bloque_t* bloque_cache, bloque_t* bloque_mem, int tag);

void init_mem(mem_t* mem){
    for (int i = 0; i < 125; ++i) {
        for (int j = 0; j < 32; ++j) {
            mem->datos[i].data[j] = 0;
        }
    }
}

void init(cache_t* cache){
    cache->misrate = 0;
    cache->operaciones = 0;
    for( int i = 0; i < 16; i++){
        poner_en_cero(&cache->lado_a[i]);
        poner_en_cero(&cache->lado_b[i]);
    }
}


unsigned int read_byte(int addres, cache_t* cache, mem_t* mem){
    unsigned int bloque = addres/32;
    int indice = bloque % 16;
    unsigned int tag = bloque/16;
    bool esta_en_A = esta_en_a(cache, indice, tag);
    if (esta_en_A){
        cache->lado_a[indice].usos++;
        return addres;
    }

    bool esta_en_B = esta_en_b(cache, indice, tag);
    if (esta_en_B){
        cache->lado_b[indice].usos++;
        return addres;
    }
    if (cache->lado_a[indice].usos < cache->lado_b[indice].usos){
        cargar_bolque(&cache->lado_a[indice], &mem->datos[bloque], tag);
        cache->lado_a[indice].usos = 1;
    } else {
        cargar_bolque(&cache->lado_b[indice], &mem->datos[bloque], tag);
        cache->lado_b[indice].usos = 1;
    }
    cache->misrate++;
    return -1;
}


int write_byte(int addres, unsigned char value, cache_t* cache, mem_t* mem){
    unsigned int bloque = addres/32;
    int indice = bloque % 16;
    unsigned int tag = bloque/16;
    unsigned int offset = addres & off;
    bool esta_en_A = esta_en_a(cache, indice, tag);
    if (esta_en_A){
        cache->lado_a[indice].data[offset] = value;
        mem->datos[bloque].data[offset] = value;
        cache->lado_a[indice].usos++;
        return 0;
    }

    bool esta_en_B = esta_en_b(cache, indice, tag);
    if (esta_en_B){
        cache->lado_b[indice].data[offset] = value;
        mem->datos[bloque].data[offset] = value;
        cache->lado_b[indice].usos++;
        return 0;
    }
    mem->datos[bloque].data[offset] = value;
    cache->misrate++;
    return -1;
}

unsigned int get_miss_rate(cache_t* cache){
    return cache->misrate;
}


bool esta_en_b(cache_t *cache, int indice, unsigned int tag) {
    if (cache->lado_b[indice].v){
        return cache->lado_b[indice].tag == tag;
    }
    return false;
}

bool esta_en_a(cache_t* cache, int indice, unsigned int tag) {
    if (cache->lado_a[indice].v){
        return cache->lado_a[indice].tag == tag;
    }
    return false;
}


void poner_en_cero(bloque_t *bloque) {
    bloque->v = false;
    bloque->usos = 0;
}

void cargar_bolque(bloque_t* bloque_cache, bloque_t* bloque_mem, int tag) {
    bloque_cache->v = true;
    bloque_cache->tag = tag;
    for (int i = 0; i < 32; ++i) {
       bloque_cache->data[i] = bloque_mem->data[i];
    }
}
