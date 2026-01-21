#include "content.h"
#include "name.h"
#include "csv.h"

unsigned long hash_djb2(char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);
    return hash;
}

void get_files(DIR *dir, char *path, ArqList *list)
{
    rewinddir(dir);
    inicializar_Arqlist(list);

    struct dirent *entry;
    struct stat st;
    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        get_full_name(list, path, entry->d_name);

        // if (entry->d_type != DT_REG)  //nao tem no windows :c mas funciona no linux
        //     continue;

        if (stat(list->lista[list->counter].path, &st) != 0)
            continue;

        if (!S_ISREG(st.st_mode))
            continue; // nao e arquivo regular

        verificar_tamanho_ArquList(list);

        list->counter++;
    }
}

Shingle *initialize_shingle()
{
    Shingle *temp;
    temp = malloc(sizeof(Shingle));
    temp->hash = 0;
    temp->next = NULL;
    return temp;
}

ShingleSet *initialize_set(int size)
{
    ShingleSet *set = malloc(sizeof(ShingleSet));
    set->size = size;
    set->shingles = calloc(size, sizeof(Shingle *)); // inicia tudo com NULL
    set->count = 0;
    return set;
}
void free_shingle_set(ShingleSet *set)
{
    for (int i = 0; i < set->size; i++)
    {
        Shingle *cur = set->shingles[i];
        while (cur)
        {
            Shingle *next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(set->shingles);
    free(set);
}

// debugs e teste do shingle / hashtable

// void printShingleSet(ShingleSet *set)
//{
//     ShingleSet *temp_set = set;
//     for (int i = 0; i < set->size; i++)
//     {
//
//         if (set->shingles[i] == NULL)
//         {
//             continue;
//         }
//         Shingle *temp_shingle;
//         temp_shingle = temp_set->shingles[i];
//         printf("\n[Bucket %d]", i + 1);
//         while (temp_shingle != NULL)
//         {
//             printf("----->");
//             printf("[%lu]", temp_shingle->hash);
//             temp_shingle = temp_shingle->next;
//         }
//         printf("\n");
//     }
// }
//
// void printBucketSizes(ShingleSet *set)
//{
//     for (int i = 0; i < set->size; i++)
//     {
//         int count = 0;
//         Shingle *cur = set->shingles[i];
//
//         while (cur)
//         {
//             count++;
//             cur = cur->next;
//         }
//
//         if (count > 0)
//         {
//             printf("Bucket %d: %d\n", i, count);
//         }
//     }
// }

bool insert_in_head(ShingleSet *set, Shingle *shingle)
{
    if (!shingle)
        return false;

    int index = shingle->hash % set->size;

    Shingle *cur = set->shingles[index];
    while (cur)
    {
        if (cur->hash == shingle->hash)
            return false; // duplicado, em um set duplicado nao tem duplicado
        cur = cur->next;
    }

    shingle->next = set->shingles[index];
    set->shingles[index] = shingle;
    set->count++;

    return true;
}

void generate_shingle(FILE *file, int k, ShingleSet *set)
{
    char window[1024];
    int wlen = 0;
    int c;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            //      wlen = 0;                isso aqui nao geraria shingles quando tivesse /n, talvez eu mude de ideia mas acho melhor gerar tudo junto com a linha nova
            continue;
        }

        window[wlen++] = (char)c;

        if (wlen == k)
        {
            window[k] = '\0';

            Shingle *s = initialize_shingle();
            s->hash = hash_djb2(window);

            if (!insert_in_head(set, s))
            {
                free(s);
            }

            memmove(window, window + 1, k - 1);
            wlen--;
        }
    }
}

long int intersection(ShingleSet *set1, ShingleSet *set2)
{
    long int intersec = 0;

    for (int i = 0; i < set2->size; i++)
    {
        if (set2->shingles[i] == NULL)
        {
            continue;
        }
        Shingle *temp = set2->shingles[i];
        while (temp)
        {
            Shingle *temp2 = set1->shingles[(temp->hash) % set1->size];
            while (temp2)
            {
                if (temp2->hash == temp->hash)
                {
                    intersec++;
                    break;
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }
    return intersec;
}

double jaccard(ShingleSet *set1, ShingleSet *set2, long int intersec)
{
    long int uniao = set1->count + set2->count - intersec;

    if (uniao == 0)
    {
        return 0.0;
    }

    return ((double)intersec / (double)uniao) * 100;
}

double fallback_compare(FILE *file1, FILE *file2)
{ // funcao pra caso o texto nao gere shingles pq ele e muito pequeno
    bool char1[256] = {0};
    bool char2[256] = {0};
    rewind(file1);
    rewind(file2);
    int uniao = 0;
    int intersec = 0;
    int c;

    while ((c = fgetc(file1)) != EOF)
    {
        char1[(unsigned char)c] = true; // unsigned char vai me da sempre entre 0 e 255
    }

    while ((c = fgetc(file2)) != EOF)
    {
        char2[(unsigned char)c] = true;
    }

    for (int i = 0; i < 256; i++)
    {
        if (char1[i] || char2[i])
        {
            uniao++;
        }
        if (char1[i] && char2[i])
        {
            intersec++;
        }
    }

    if (uniao == 0)
        return 0.0;

    return ((double)intersec / uniao) * 100.0;
}

void compare_files(ArqList *list)
{
    for (int i = 0; i < list->counter; i++)
    {
        for (int j = i + 1; j < list->counter; j++)
        {

            FILE *file1 = fopen(list->lista[i].path, "r");
            FILE *file2 = fopen(list->lista[j].path, "r");

            if (!file1 || !file2)
            {
                printf("Erro ao abrir arquivos\n");
                if (file1)
                    fclose(file1);
                if (file2)
                    fclose(file2);
                continue;
            }

            ShingleSet *set1 = initialize_set(10007);
            ShingleSet *set2 = initialize_set(10007);
            generate_shingle(file1, 5, set1);
            generate_shingle(file2, 5, set2);
            //          printShingleSet(set1);
            //          printBucketSizes(set1);
            double similarity;
            if (set1->count == 0 || set2->count == 0)
            {
                similarity = fallback_compare(file1, file2);
            }
            else
            {
                long int intersec = intersection(set1, set2);
                similarity = jaccard(set1, set2, intersec);
            }
            write_csv_cont(
                list->lista[i].name,
                list->lista[j].name,
                similarity);

            printf(
                "Jaccard (%s || %s): %.2f%%\n",
                list->lista[i].name,
                list->lista[j].name,
                similarity);

            fclose(file1);
            fclose(file2);
            free_shingle_set(set1);
            free_shingle_set(set2);
        }
    }
}
