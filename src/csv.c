#include "csv.h"

void init_csv()
{

    FILE *file1 = fopen("output/result_cont.csv", "r");
    if (file1 == NULL)
    {
        file1 = fopen("output/result_cont.csv", "w");
        fprintf(file1, "Arquivo 1;Arquivo 2;Similaridade\n");
        fclose(file1);
    }

    FILE *file2 = fopen("output/result_name.csv", "r");
    if (file2 == NULL)
    {
        file2 = fopen("output/result_name.csv", "w");
        fprintf(file2, "Arquivo 1;Arquivo 2;Porcentagem\n");
        fclose(file2);
    }
}

void write_csv_cont(char *file1, char *file2, double jaccard)
{
    FILE *csv = fopen("output/result_cont.csv", "a");
    if (csv == NULL)
    {
        printf("Error opening CSV file\n");
        return;
    }
    fprintf(csv, "%s;%s;%.2f\n", file1, file2, jaccard);
    fclose(csv);
}

void write_csv_name(char *file1, char *file2)
{
    FILE *csv = fopen("output/result_name.csv", "a");
    if (csv == NULL)
    {
        printf("Error opening CSV file\n");
        return;
    }
    fprintf(csv, "%s;%s;%.2f\n", file1, file2, 100.0);
    fclose(csv);
}
