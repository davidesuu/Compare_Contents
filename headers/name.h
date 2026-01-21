#ifndef NAME_H
#define NAME_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include "csv.h"
#include "directory.h"


typedef struct Arq{
    char name[300];
    char ext[300];
    char path[1000];
} Arq;

typedef struct ArqList{
    Arq *lista;
    int counter;
    int capacidade;
} ArqList;

DIR* open_dir(char *path);

void verificar_tamanho_ArquList(ArqList *list);

void inicializar_Arqlist(ArqList *list);

void get_full_name(ArqList *list, char *path, char *nome);

void inserir_ArqList(ArqList *list, const char *filename);

void compare_names(ArqList *list);

void read_names(DIR *dir, char *path);

#endif
