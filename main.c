#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include "funcoes.c"

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

int main()
{

    FILE *fptr = fopen("dados.txt", "a+");
    char line[1000];
    if (fgets(line, sizeof(line), fptr) == NULL)
    {
        fprintf(fptr, "admin admin123 1 100\n");
    }

    fclose(fptr);

    system("chcp 65001");
    system("cls");

    int escolha;
    bool login = true;

    do
    {
        titulo();
        fflush(stdin);
        menuInicial();
        escolha = verificarInt("Escolha uma opção: ");
        printf("\n");

        switch (escolha)
        {
        case 1:
            criarConta();
            break;
        case 2:
            iniciarSessao();
            break;
        case 0:
            login = false;
            break;
        default:
            printf(RED"Erro! Tente algo válido!"RESET);
            Sleep(2000);
            system("cls");
            break;
        }
    } while (login);

    return 0;
}