#include "csv.h"

void inicializar_Arqlist(ArqList *list)
{
    list->counter = 0;
    list->capacidade = 30;
    list->lista = malloc(list->capacidade * sizeof(Arq));
}

void verificar_tamanho_ArquList(ArqList *list)
{
    if (list->counter >= list->capacidade)
    {
        list->capacidade += 1; // nao sei se isso � eficiente ou se eu multiplico pra aumentar de vez...
        list->lista = realloc(list->lista, list->capacidade * sizeof(Arq));
    }
}
void inserir_ArqList(ArqList *list, const char *filename)
{
    verificar_tamanho_ArquList(list);

    char *dot = strrchr(filename, '.');

    if (dot)
    {
        size_t len = dot - filename;
        strncpy(list->lista[list->counter].name, filename, len);
        list->lista[list->counter].name[len] = '\0';
        strcpy(list->lista[list->counter].ext, dot);
    }
    else
    {
        strcpy(list->lista[list->counter].name, filename);
        list->lista[list->counter].ext[0] = '\0';
    }

    list->counter++;
}

void get_full_name(ArqList *list, char *path, char *nome)
{
    strcpy(list->lista[list->counter].name, nome);
    snprintf(
        list->lista[list->counter].path,
        sizeof(list->lista[list->counter].path),
        "%s/%s", path, nome);
}
void compare_names(ArqList *list)
{
    printf("Arquivos com o mesmo nome: \n");
    for (int i = 0; i < list->counter; i++)
    {
        Arq *file1 = &list->lista[i];

        for (int j = i + 1; j < list->counter; j++)
        {
            Arq *file2 = &list->lista[j];

            if (strcmp(file1->name, file2->name) == 0)
            {
                write_csv_name(
                    list->lista[i].name,
                    list->lista[j].name);
                printf("%s%s  ||  %s%s\n", file1->name,
                       file1->ext, file2->name, file2->ext);
            }
        }
    }
}

void read_names(DIR *dir, char *path)
{
    rewinddir(dir);
    struct dirent *entry;
    ArqList *list = malloc(sizeof(ArqList));
    inicializar_Arqlist(list);

    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        get_full_name(list, path, entry->d_name);

        inserir_ArqList(list, entry->d_name);
    }

    compare_names(list);
}
