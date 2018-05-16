#include <stdio.h>
#include <stdlib.h>
#include "Cache.h"

int obtener_dir(FILE* archivo, char delim);

int obtener_dato(FILE* archivo);

int main(int argc, char* argv[]) {
    if (argc != 2){
        printf("Error: cantidad incorrecta de argumentos.\n");
        return 1;
    }
    mem_t mem;
    cache_t cache;
    init(&cache);
    init_mem(&mem);
    FILE* archivo = fopen(argv[1], "r");
    if (archivo == NULL){
        printf("Error: El archivo no existe.\n");
        return 1;
    }
    while (!feof(archivo)){
        char c = fgetc(archivo);
        if (c == 'W'){
            int dir = obtener_dir(archivo, ',');
            unsigned int bloque = dir/32;
            if (dir < 0 || bloque > 125){
                printf("Error: la direccion: %d es invalida.\n",dir);
                fclose(archivo);
                return 0;
            }
            int dato = obtener_dato(archivo);
            unsigned int i = write_byte(dir,dato,&cache,&mem);
            printf("%d\n", i);
        }
        if (c == 'R'){
            int dir = obtener_dir(archivo, '\n');
            unsigned int bloque = dir/32;
            if (dir < 0 || bloque > 125){
                printf("Error: la direccion: %d es invalida.\n",dir);
                fclose(archivo);
                return 0;
            }
            unsigned int i = read_byte(dir,&cache,&mem);
            printf("%d\n", i);
        }
        if (c == 'M'){
            fgetc(archivo);
            fgetc(archivo);
            unsigned  int i = get_miss_rate(&cache);
            printf("miss rate: %d\n", i);
        }
    }
    fclose(archivo);
    return 0;
}

int obtener_dir(FILE* archivo, char delim){
    fgetc(archivo);
    char c = fgetc(archivo);
    int dir = 0;
    char direcion[10];
    int j = 0;
    while (c != delim){
        direcion[j++] = c;
        c = fgetc(archivo);
    }
    direcion[j] = '\0';
    dir = atoi(direcion);
    return dir;
}


int obtener_dato(FILE* archivo){
    fgetc(archivo);
    char c = fgetc(archivo);
    char data[10];
    int k = 0;
    while (c != '\n'){
        data[k++] = c;
        c = fgetc(archivo);
    }
    data[k] = '\0';
    return atoi(data);
}
