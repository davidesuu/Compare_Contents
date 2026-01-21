#include "name.h"
#include "content.h"
#include "csv.h"
#include "directory.h"

int main()
{
    int option;
    DIR *dir;
    ArqList *list;
    char path[600];
    do
    {
        printf("\nInsira o caminho da pasta que voce deseja analisar ou digite \"sair\" para sair: \n");
        printf("Exemplo: C:/Users/Seu_User/Sua_Pasta\n");
        fgets(path, 600, stdin);
        path[strcspn(path, "\n")] = '\0';
        while (!(dir = open_dir(path)))
        {
            fgets(path, 600, stdin);
            path[strcspn(path, "\n")] = '\0';
            if ((strcmp(path, "sair") == 0))
            {
                return 0;
            }
        }
        print_dir(dir);
        printf("1 - Comparacao por nome.\n2 - Comparacao por conteudo\n3 - Sair\n");
        scanf("%d", &option);
        getchar();
        init_csv();
        switch (option)
        {
        case 1:
            printf("Comparando nomes...\n");
            read_names(dir, path);
            break;
        case 2:
            printf("Comparando conteudo...\n");
            list = malloc(sizeof(ArqList));
            get_files(dir, path, list);
            compare_files(list);
            free(list);
            closedir(dir);
            break;
        case 3:
            printf("Saindo...\n");
            closedir(dir);
            break;
        default:
            printf("Opcao invalida. \n");
            break;
        }
    } while (option != 3);

    return 0;
}
