#ifndef CONTENT_H
#define CONTENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>
#include "dirent.h"
#include "name.h"
#include "directory.h"

typedef struct Shingle
{
    unsigned long hash;
    struct Shingle *next;
} Shingle;

typedef struct ShingleSet // Hash table
{
    Shingle **shingles;
    size_t size; // buckets
    size_t count;
} ShingleSet;

unsigned long hash_djb2(char *str);

void get_files(DIR *dir, char *path, ArqList *list);

Shingle *initialize_shingle();

ShingleSet *initialize_set(int size);

void free_shingle_set(ShingleSet *set);

void printShingleSet(ShingleSet *set);

void printBucketSizes(ShingleSet *set);

bool insert_in_head(ShingleSet *set, Shingle *shingle);

void generate_shingle(FILE *file, int k, ShingleSet *set);

long int intersection(ShingleSet *set1, ShingleSet *set2);

double jaccard(ShingleSet *set1, ShingleSet *set2, long int intersec);

double fallback_compare(FILE *file1, FILE *file2);

void compare_files(ArqList *list);

#endif
