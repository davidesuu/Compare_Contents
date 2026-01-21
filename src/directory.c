#include "directory.h"

DIR *open_dir(char *path_f)
{
    DIR *dir;
    const char *path = path_f;
    dir = opendir(path);
    if (dir == NULL)
    {
        printf("Erro ao abrir diretorio, insira novamente:\n");
        return NULL;
    }
    return dir;
}

void print_dir(DIR *dir)
{
    printf("\n");
    printf("Arquivos no diretorio: \n");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            continue;
        }
        printf("%s\n", entry->d_name);
    }
    printf("\n");
}
