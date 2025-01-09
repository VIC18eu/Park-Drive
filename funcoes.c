#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include "funcoes.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#define LUGARES 10
#define LINHA "%99[^\n]"

// Cores e efeitos
#define RESET "\x1b[0m"
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define BOLD "\x1b[1m"
#define UNDERLINE "\x1b[4m"
#define BG_BLACK "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GREEN "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN "\x1b[46m"
#define BG_WHITE "\x1b[47m"

// Struct do Utilizador
typedef struct
{
    char email[250];
    char password[250];
    int admin;
    int dinheiro;
} User;

typedef struct
{
    int dia;
    int mes;
    int ano;
    char nome[250];
    int lugar;
    char comentario[1000];
} Comentario;

void titulo()
{
    printf("*======================================================================================================*\n");
    printf("    ____                     __            ___              ____     ____     ____ _    __    ______\n");
    printf("   / __ \\  ____ _   _____   / /__         ( _ )            / __ \\   / __ \\   /  _/| |  / /   / ____/\n");
    printf("  / /_/ / / __ `/  / ___/  / //_/        / __ \\/|         / / / /  / /_/ /   / /  | | / /   / __/   \n");
    printf(" / ____/ / /_/ /  / /     / ,<          / /_/  <         / /_/ /  / _, _/  _/ /   | |/ /   / /___   \n");
    printf("/_/      \\__,_/  /_/     /_/|_|         \\____/\\/        /_____/  /_/ |_|  /___/   |___/   /_____/   \n");
    printf("                                                                                                    \n");
    printf("\n\n\n");
}

void printClientes()
{
    char nome[250];
    FILE *fptr = fopen("dados.txt", "a+");

    printf("+----+----------------------+\n");
    printf("| ID |        Nome          |\n");
    printf("+----+----------------------+\n");

    int count = 0;
    while (fscanf(fptr, "%s", nome) != EOF)
    {
        count++;
        printf("| %-2d | %-20s |\n", count, nome);
        fscanf(fptr, "%*s %*s %*s");
    }

    printf("+----+----------------------+\n");

    fclose(fptr);
}

int diaMaxMes()
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int diasMeses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int *mes = diasMeses;
    mes += tm.tm_mon;

    return *mes;
}

void verificarDatas()
{
    FILE *file, *tempFile;
    char line[256];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Reserva data;

    int diaAtual = tm.tm_mday;

    file = fopen("reservas.txt", "r");
    if (file == NULL)
    {
        perror(RED "Erro ao abrir o ficheiro" WHITE);
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        perror(RED "Erro ao abrir o ficheiro temporário" WHITE);
        fclose(file);
    }

    // Verificação das Datas que têm de ser maiores que o dia atual e têm de ser deste mês
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d %s %d %d %d", &data.lugar, data.nome, &data.diaInicio, &data.diaSaida, &data.mes);

        if (data.diaSaida >= diaAtual || data.mes >= tm.tm_mon)
        {
            fprintf(tempFile, "%d %s %d %d %d\n", data.lugar, data.nome, data.diaInicio, data.diaSaida, data.mes);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("reservas.txt");
    rename("temp.txt", "reservas.txt");
}

int verificarInt(char *frase)
{

    char teste[250];
    bool validar = true;
    bool continuar = true;

    while (continuar)
    {

        scanf(LINHA, teste);
        while (getchar() != '\n')
            ;

        validar = true;
        for (int i = 0; i < strlen(teste) && validar; i++)
        {
            if (isdigit(teste[i]) == 0)
            {
                validar = false;
                printf(RED "Erro! Tente de novo.\n" WHITE);
                printf("%s", frase);
            }
        }
        if (validar)
        {
            continuar = false;
        }
    }

    return atoi(teste);
}

bool validarDias(int diaInicio, int diaFim, char *nome, int validarDinheiro)
{

    int maxMes = diaMaxMes();
    FILE *fptr;
    User user;
    char line[250];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    bool continuar = true;

    fptr = fopen("dados.txt", "a+");
    if (fptr == NULL)
    {
        printf(RED "Erro a abrir ficheiro!" WHITE);
    }

    while (fgets(line, sizeof(line), fptr) && continuar)
    {
        sscanf(line, "%s %*s %*d %d", user.email, &user.dinheiro);
        if (strcmp(user.email, nome) == 0)
        {
            continuar = false;
        }
    }

    fclose(fptr);

    if ((user.dinheiro < (diaFim + 1 - diaInicio) * 10) && validarDinheiro)
    {
        printf(RED "Dinheiro insuficiente! (%d€)\n" WHITE, (diaFim + 1 - diaInicio) * 10);
        return false;
    }
    if ((diaInicio > 0 && diaInicio <= maxMes) && (diaFim > 0 && diaFim <= maxMes) && (diaInicio <= diaFim && diaInicio >= tm.tm_mday))
    {
        return true;
    }
    else
    {
        printf(RED "Ocorreu um erro! Tente de novo\n" WHITE);
        return false;
    }
}

int validarLugar(char *frase)
{
    bool continuar = true;
    int lugar;

    while (continuar)
    {
        if (scanf("%d", &lugar) == 1 && (lugar >= 0 && lugar <= LUGARES))
        {
            continuar = false;
        }
        else
        {
            printf(RED "Lugar inválido! Tente de novo.\n" WHITE);
            printf("%s", frase);
            fflush(stdin);
        }
    }

    return lugar;
}

//---------FUNÇÕES RESERVA----------

void printCalendar(int lugar)
{
    verificarDatas();

    system("cls");
    titulo();

    Reserva dias;

    int dMes[31] = {0};
    char line[250];
    int diasMes = diaMaxMes();

    FILE *fptr = fopen("reservas.txt", "a+");
    for (int i = 0; i < diasMes; i++)
    {
        while (fgets(line, sizeof(line), fptr))
        {
            sscanf(line, "%d %*s %d %d %*d", &dias.lugar, &dias.diaInicio, &dias.diaSaida);
            if (dias.lugar == lugar)
            {
                for (int i = dias.diaInicio - 1; i < dias.diaSaida && i <= diasMes; i++)
                {
                    dMes[i] = 1;
                }
            }
        }
    }

    // Imprime os dias do mês
    for (int dia = 1; dia < diasMes; dia++)
    {
        if (dMes[dia - 1] == 1)
        {
            printf(RED "%3d " WHITE, dia);
        }
        else
        {
            printf("%3d ", dia);
        }

        if ((dia + 1) % 7 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");

    fclose(fptr);

    printf("Pressione alguma tecla para continuar...");
    getch();
}

void mostrarLugares(int diaInicio, int diaFim)
{

    verificarDatas();

    system("cls");
    titulo();

    char line[256];

    FILE *fptr = fopen("reservas.txt", "a+");

    int lugares[LUGARES] = {0};
    int lugar;

    int lerInicio, lerFim;

    while (fgets(line, sizeof(line), fptr))
    {
        sscanf(line, "%d %*s %d %d %*d", &lugar, &lerInicio, &lerFim);
        bool lugarValido = true;
        for (int i = diaInicio; i <= diaFim && lugarValido; i++)
        {
            if (i >= lerInicio && i <= lerFim)
            {
                lugares[lugar - 1] = 1;
                lugarValido = false;
            }
        }
    }

    printf(RESET);
    printf("+---------------------------+\n");
    printf("| Lugares de Estacionamento |\n");
    printf("+------+--------------------+\n");

    for (int i = 0; i < LUGARES; i++)
        if (lugares[i] == 0)
        {
            printf("| %2d |" GREEN "  Disponível          |\n" WHITE, i + 1);
        }
        else
        {
            printf("| %2d |" RED "  Ocupado             |\n" WHITE, i + 1);
        }

    printf("+------+--------------------+\n");

    fclose(fptr);
}

void escolherPesquisa(char *nome)
{

    system("cls");

    verificarDatas();

    char escolhaPesquisa;
    bool continuar = true;
    Reserva reserva;
    bool validarData;

    while (continuar)
    {
        system("cls");
        titulo();
        printf("Por qual você quer procurar?\n\n1 - Lugares disponiveis\n2 - Dias disponiveis\n\nEsc - Sair\n");
        escolhaPesquisa = getch();
        fflush(stdin);
        switch (escolhaPesquisa)
        {
        case '1':
            validarData = false;
            while (!validarData)
            {
                system("cls");
                titulo();
                printf(WHITE BOLD "Preencha os dados aqui em baixo.\n" RESET);

                printf("\nDia de Inicio\n-> ");
                reserva.diaInicio = verificarInt("Dia de Inicio\n-> ");

                printf("\n");

                printf("Dia do Fim\n-> ");
                reserva.diaSaida = verificarInt("Dia do Fim\n-> ");

                system("cls");
                validarData = validarDias(reserva.diaInicio, reserva.diaSaida, nome, 0);
                Sleep(1500);
            }

            mostrarLugares(reserva.diaInicio, reserva.diaSaida);
            printf("Pressione alguma tecla para continuar...");
            getch();

            break;
        case '2':

            system("cls");
            titulo();
            printf("Escreva o número do lugar: ");
            reserva.lugar = validarLugar("Escreva o número do lugar: ");
            printCalendar(reserva.lugar);

            break;
        case 27:

            continuar = false;

            break;
        default:
            printf(YELLOW "Erro! Opção inválida." WHITE);
            Sleep(1500);
            break;
        }
    }
}

void dinheiro(char *nome, int valor, int modo)
{
    verificarDatas();

    char line[256];
    User user;

    FILE *fptr = fopen("dados.txt", "a+");
    FILE *tempFile = fopen("tempMoney.txt", "a+");

    if (fptr == NULL || tempFile == NULL)
    {
        perror(RED "Erro a abrir ficheiro!" WHITE);
        if (fptr != NULL)
        {
            fclose(fptr);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
    }

    while (fgets(line, sizeof(line), fptr))
    {
        sscanf(line, "%s %s %d %d", user.email, user.password, &user.admin, &user.dinheiro);
        if (strcmp(user.email, nome) == 0)
        {
            if (modo == 1)
            {
                user.dinheiro += valor;
                printf(GREEN "Valor depositado com sucesso!" WHITE);
                Sleep(1500);
            }
            else
            {
                user.dinheiro -= valor;
            }
        }
        fprintf(tempFile, "%s %s %d %d\n", user.email, user.password, user.admin, user.dinheiro);
    }

    fclose(fptr);
    fclose(tempFile);

    remove("dados.txt");
    rename("tempMoney.txt", "dados.txt");
}

bool verificarReserva(Reserva reserva)
{
    verificarDatas();

    char line[256];
    FILE *fptr = fopen("reservas.txt", "a+");

    printf(RED);
    bool dataJaReservada = false;
    bool erro = true;

    int lugarLido, diaInicio, diaFim, mes;
    char nomeLido[250];

    while (fgets(line, sizeof(line), fptr) && dataJaReservada == false)
    {
        sscanf(line, "%d %s %d %d %d", &lugarLido, nomeLido, &diaInicio, &diaFim, &mes);
        if (lugarLido == reserva.lugar)
        {
            for (int i = diaInicio; i <= diaFim; i++)
            {
                if (i >= reserva.diaInicio && i <= reserva.diaSaida)
                {
                    dataJaReservada = true;
                }
            }
        }
    }

    if (dataJaReservada)
    {
        printf(YELLOW "Data já reservada!\n" WHITE);
    }
    else
    {
        fprintf(fptr, "%d %s %d %d %d", reserva.lugar, reserva.nome, reserva.diaInicio, reserva.diaSaida, reserva.mes);
        erro = false;
    }

    fclose(fptr);

    printf(WHITE);
    return erro;
}

void reserva(char *nomeUser)
{

    system("cls");
    verificarDatas();

    Reserva reserva;
    bool continuar = true;
    bool validarData = false;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    reserva.mes = tm.tm_mon + 1;
    strcpy(reserva.nome, nomeUser);

    while (continuar || !validarData)
    {
        titulo();
        while (!validarData)
        {
            printf(WHITE BOLD "Preencha os dados aqui em baixo. (0 para sair)\n" RESET);

            printf("\nDia de Inicio\n-> ");
            reserva.diaInicio = verificarInt("Dia de Inicio\n-> ");
            if (reserva.diaInicio == 0)
            {
                return;
            }

            printf("\n");

            printf("Dia do Fim\n-> ");
            reserva.diaSaida = verificarInt("Dia do Fim\n-> ");
            if (reserva.diaSaida == 0)
            {
                return;
            }

            system("cls");
            titulo();
            validarData = validarDias(reserva.diaInicio, reserva.diaSaida, nomeUser, 1);
        }

        mostrarLugares(reserva.diaInicio, reserva.diaSaida);

        printf("\n");

        printf("Escolha um lugar válido (0 para sair): ");
        reserva.lugar = validarLugar("Escolha um lugar válido: ");
        if (reserva.lugar == 0)
        {
            return;
        }

        continuar = verificarReserva(reserva);
        Sleep(1000);
        system("cls");
    }

    dinheiro(nomeUser, (reserva.diaSaida + 1 - reserva.diaInicio) * 10, 0);

    printf(GREEN "Reserva efetuada com  sucesso!" WHITE);
    Sleep(2000);
}

void printarReservas(char *nome)
{
    char line[256];
    Reserva reserva;

    system("cls");
    titulo();

    FILE *fptr = fopen("reservas.txt", "r");
    if (fptr == NULL)
    {
        perror("Erro a abrir ficheiro!");
    }

    printf("Reservas para %s:\n", nome);
    printf("+--------+--------+-------------+------------+\n");
    printf("| ID     | Lugar  | Data Inicio | Data Fim   |\n");
    printf("+--------+--------+-------------+------------+\n");

    bool reservas = false;
    int count = 0;
    while (fgets(line, sizeof(line), fptr))
    {
        count++;
        sscanf(line, "%d %s %d %d %d", &reserva.lugar, reserva.nome, &reserva.diaInicio, &reserva.diaSaida, &reserva.mes);
        if (strcmp(reserva.nome, nome) == 0)
        {
            printf("| %6d | %6d | %02d/%02d       | %02d/%02d      |\n", count, reserva.lugar, reserva.diaInicio, reserva.mes, reserva.diaSaida, reserva.mes);
            reservas = true;
        }
    }

    if (!reservas)
    {
        printf("| Nenhuma reserva encontrada.                |\n");
    }

    printf("+--------+--------+-------------+------------+\n");

    fclose(fptr);
}

void cancelarReserva(char *nome)
{

    verificarDatas();

    system("cls");
    titulo();
    

    FILE *fptr = fopen("reservas.txt", "r");
    if (fptr == NULL)
    {   
        fclose(fptr);
        printf(RED "Erro ao abrir ficheiro!" WHITE);
        Sleep(1500);
        return;
    }
    FILE *tempFile = fopen("temp.txt", "a+");
    if (tempFile == NULL)
    {
        fclose(tempFile);
        printf(RED "Erro ao abrir ficheiro!" WHITE);
        Sleep(1500);
        return;
    }

    char line[1000];
    bool nomeLido = false;
    Reserva reserva;
    int ids[100] = {0};
    int count = 0;

    printarReservas(nome);
    while (fgets(line, sizeof(line), fptr))
    {
        sscanf(line, "%*d %s %*d %*d %*d", reserva.nome);
        if (strcmp(reserva.nome, nome) == 0)
        {
            nomeLido = true;
            ids[count] = 1;
        }
        count++;
    }

    if (!nomeLido)
    {
        printf("Pressione uma tecla para sair...");
        getch();

        fclose(fptr);
        fclose(tempFile);

        remove("temp.txt");

        return;
    }

    rewind(fptr);

    bool continuar = true;
    int numero;

    while (continuar){
        printf("Número da reserva que deseja cancelar: ");
        numero = verificarInt("Número da reserva que deseja cancelar: ");
        if (ids[numero - 1] == 1)
        {
            continuar = false;
        }
        else
        {
            printf(RED"Este ID não é válido! Tente de novo.\n"WHITE);
        }
    }
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int linhaAtual = 1;
    while(fgets(line, sizeof(line), fptr)){
        if(linhaAtual != numero){
            fputs(line, tempFile);
        }
        else{
            sscanf(line, "%*d %s %d %d %*d", reserva.nome, &reserva.diaInicio, &reserva.diaSaida);
        }
        linhaAtual ++;
    }

    fclose(fptr);
    fclose(tempFile);

    remove("reservas.txt");
    rename("temp.txt", "reservas.txt");
        
    sscanf(line, "%d %s %d %d %d", &reserva.lugar, reserva.nome, &reserva.diaInicio, &reserva.diaSaida, &reserva.mes);
    if (reserva.diaInicio < tm.tm_mday)
    {
        dinheiro(reserva.nome, ((reserva.diaSaida + 1) - tm.tm_mday) * 10, 1);
        system("cls");
        printf(GREEN"Reserva cancelada com sucesso!"WHITE);
        Sleep(2000);
    }
    else
    {
        dinheiro(reserva.nome, ((reserva.diaSaida + 1) - reserva.diaInicio) * 10, 1);
        system("cls");
        printf(GREEN"Reserva cancelada com sucesso!"WHITE);
        Sleep(2000);
    }


}

//---------FUNÇÕES COMENTÁRIOS--------

void escreverComentario(char nome[250])
{
    FILE *fptr;
    int lugar;
    char comentario[1000];

    time_t now = time(NULL);
    struct tm tm = *localtime(&now);

    system("cls");
    titulo();

    fptr = fopen("comentarios.txt", "a+");

    if (fptr == NULL)
    {
        printf(RED "Erro ao abrir o arquivo.\n" WHITE);
        return;
    }

    printf("Digite o número do lugar (entre 1 e 10): ");
    scanf("%d", &lugar);

    if (lugar <= 0 || lugar > 10)
    {
        printf(RED "Número de lugar inválido. Deve ser entre 0 e 10.\n" WHITE);
        fclose(fptr);
        Sleep(1500);
        return;
    }

    while (getchar() != '\n');
    printf("Digite o comentário: ");
    fgets(comentario, sizeof(comentario), stdin);

    fprintf(fptr, "%d-%d-%d %s %d %s", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, nome, lugar, comentario);

    fclose(fptr);

    printf(GREEN "\nComentário gravado com sucesso.\n" WHITE);
    Sleep(2000);
}

void verComentario()
{

    system("cls");
    titulo();

    FILE *fptr = fopen("comentarios.txt", "r");
    bool lido = false;

    Comentario comentario;

    while (fscanf(fptr, "%d-%d-%d %s %d ", &comentario.dia, &comentario.mes, &comentario.ano, comentario.nome, &comentario.lugar) != EOF)
    {
        fgets(comentario.comentario, sizeof(comentario.comentario), fptr);
        printf("Data: %d/%d/%d\nNome: %s(%d)\nComentário:%s\n", comentario.dia, comentario.mes, comentario.ano, comentario.nome,
               comentario.lugar, comentario.comentario);
        lido = true;
    }

    if (!lido)
    {
        printf("Ainda não exitem comentários!\n");
    }

    printf("\nPressione alguma tecla para continuar...");
    getch();

    fflush(stdin);
    fclose(fptr);
}

void lerUser(char nome[250])
{
    titulo();
    printClientes();

    bool continuar = true;
    FILE *fptr = fopen("dados.txt", "a+");
    char teste[250];

    while (continuar)
    {
        printf("\nEscreva o nome do cliente: ");
        scanf("%s", nome);

        bool emailExiste = false;

        while (fscanf(fptr, "%s", teste) != EOF && emailExiste == false)
        {
            if (strcmp(teste, nome) == 0)
            {
                emailExiste = true;
            }

            fscanf(fptr, "%*s %*s %*s");
        }

        if (emailExiste)
        {
            continuar = false;
        }
        else
        {
            printf(RED "Este cliente não existe" WHITE);
        }

        rewind(fptr);
    }

    fclose(fptr);
}

//---------FUNÇÕES GERENCIAR CONTA---------

void verDados(char nome[250])
{
    FILE *fptr = fopen("dados.txt", "r");
    if (fptr == NULL)
    {
        printf("Erro a abrir ficheiro!\n");
        return;
    }
    User user;
    char line[250];

    system("cls");
    titulo();

    bool userEncontrado = false;

    while (fgets(line, sizeof(line), fptr))
    {
        sscanf(line, "%s %s %d %d", user.email, user.password, &user.admin, &user.dinheiro);
        if (strcmp(nome, user.email) == 0)
        {
            printf(BOLD "NOME: " RESET "%s\n", user.email);
            printf(BOLD "PASSWORD: " RESET);
            for (int i = 0; i < strlen(user.password); i++)
            {
                printf("*");
            }
            printf("\n");

            if (user.admin)
            {
                printf(BOLD "MODO: " RESET "ADMIN\n");
            }
            else
            {
                printf(BOLD "MODO: " RESET "CLIENTE\n");
            }
            printf(BOLD "DINHEIRO: " RESET "%d€", user.dinheiro);
            userEncontrado = true;
        }
    }

    if (!userEncontrado)
    {
        printf(RED"Este cliente não existe!"WHITE);
        Sleep(1500);
        return;
    }


    printf("\n\nPressione uma tecla para continuar...");
    getch();

    fclose(fptr);
}

void tornarAdministrador(char nome[250])
{

    FILE *fptr = fopen("dados.txt", "a+");
    FILE *tempFile = fopen("temp.txt", "a+");
    User user;
    char line[250];

    system("cls");
    titulo();

    bool userEncontrado = false;

    while (fgets(line, sizeof(line), fptr))
    {
        sscanf(line, "%s %s %d %d", user.email, user.password, &user.admin, &user.dinheiro);
        if (strcmp(nome, user.email) == 0)
        {
            fprintf(tempFile, "%s %s 1 %d\n", user.email, user.password, user.dinheiro);
            userEncontrado = true;
        }
        else
        {
            fprintf(tempFile, "%s %s %d %d\n", user.email, user.password, user.admin, user.dinheiro);

        }
    }

    fclose(fptr);
    fclose(tempFile);

    remove("dados.txt");
    rename("temp.txt", "dados.txt");

    if (!userEncontrado)
    {
        printf(RED"Este cliente não existe!"WHITE);
        Sleep(1500);
        return;
    }

    printf(GREEN "Conta alterada com sucesso!\n" WHITE);
    Sleep(1500);
}

void apagarConta(char nome[250])
{
    FILE *fptr = fopen("dados.txt", "a+");
    FILE *tempFile = fopen("temp.txt", "a+");
    User user;
    char line[250];
    char conta[250];

    FILE *contaAtual = fopen("tempConta.txt", "r");
    fscanf(contaAtual, "%s", conta);
    fclose(contaAtual);

    if (strcmp(conta, nome) == 0)
    {
        printf(RED "Não pode apagar esta conta!" WHITE);
        Sleep(2000);
        return;
    }

    system("cls");
    titulo();

    while (fgets(line, sizeof(line), fptr))
    {
        sscanf(line, "%s %s %d %d", user.email, user.password, &user.admin, &user.dinheiro);
        if (strcmp(nome, user.email) != 0)
        {
            fprintf(tempFile, "%s %s %d %d\n", user.email, user.password, user.admin, user.dinheiro);
        }
    }
    fclose(fptr);
    fclose(tempFile);

    remove("dados.txt");
    rename("temp.txt", "dados.txt");
    printf(GREEN "Conta apagada com sucesso!\n" WHITE);
}

void gerenciarContas(char nome[250])
{
    int escolha;
    bool continuar = true;

    do
    {
        system("cls");
        titulo();
        printf("1 - Ver dados\n2 - Tornar Administrador\n3 - Apagar conta\n\n0 - Sair\n\nEscolha uma opção: ");
        escolha = verificarInt("Escolha uma opção: ");
        switch (escolha)
        {
        case 1:
            verDados(nome);
            break;
        case 2:
            tornarAdministrador(nome);
            break;
        case 3:
            apagarConta(nome);
            Sleep(1500);
            break;
        case 0:
            continuar = false;
            break;
        default:
            break;
        }
    } while (continuar);
}

// Menu Admin
void menuAdmin()
{

    int escolha;
    bool continuar = true;
    char nome[250];

    do
    {
        system("cls");
        titulo();
        fflush(stdin);
        printf("1 - Fazer Reserva\n2 - Ver Reservas\n3 - Ver Lugares\n4 - Gerenciar Contas\n5 - Ver comentários\n\n0 - Sair\n\nEscolha uma opção: ");
        escolha = verificarInt("Escolha uma opção: ");

        switch (escolha)
        {
        case 1:
            system("cls");
            lerUser(nome);
            fflush(stdin);
            reserva(nome);
            break;

        case 2:

            system("cls");
            lerUser(nome);
            fflush(stdin);
            printarReservas(nome);
            printf("\nPressione alguma tecla para continuar...");
            getch();

            break;
        case 3:

            system("cls");
            escolherPesquisa(nome);
            break;

        case 4:

            system("cls");
            lerUser(nome);
            fflush(stdin);
            gerenciarContas(nome);
            break;

        case 5:
            verComentario();
            break;
        case 0:
            continuar = false;
            break;

        default:
            printf(YELLOW "Opção inválida! Tente de novo.\n" WHITE);
            break;
        }
    } while (continuar);

    system("cls");

    printf("A sair do modo Admin...\n");
    Sleep(2500);

    system("cls");
}

// Menu Cliente
void menuCliente(char nome[250])
{
    int escolha;
    bool continuar = true;
    int deposito;
    int saldo;
    do
    {
        // Ver saldo
        FILE *fptr = fopen("dados.txt", "r+");
        char nomeLido[250];
        bool lerSaldo = true;
        while (lerSaldo && fscanf(fptr, "%s %*s %*s %d", nomeLido, &saldo) != EOF)
        {
            if (strcmp(nome, nomeLido) == 0)
            {
                lerSaldo = false;
            }
        }
        fclose(fptr);

        system("cls");
        titulo();
        fflush(stdin);
        printf("\t\t\t\t\t\t\t\t\t\tSaldo: %d€\n1 - Fazer Reserva (10€ p/ dia)\n2 - Cancelar Reserva\n3 - Minhas Reservas\n4 - Ver Lugares\n5 - Depositar dinheiro\n"
               "6 - Fazer Comentário\n\n0 - Sair\n\nEscolha uma opção: ",
               saldo);
        escolha = verificarInt("Escolha uma opção: ");

        switch (escolha)
        {
        case 1:
            reserva(nome);
            break;
        case 2:
            cancelarReserva(nome);
            break;
        case 3:
            printarReservas(nome);
            printf("\nPressione alguma tecla para continuar...");
            getch();
            break;
        case 4:
            escolherPesquisa(nome);
            break;

        case 5:
            system("cls");
            titulo();
            printf("Escreva o dinheiro que deseja depositar: ");
            deposito = verificarInt("Escreva o dinheiro que deseja depositar: ");
            dinheiro(nome, deposito, 1);

            break;
        case 6:
            escreverComentario(nome);
            break;
        case 0:
            continuar = false;
            break;

        default:
            printf(YELLOW "Opção inválida! Tente de novo.\n" WHITE);
            break;
        }
    } while (continuar);

    system("cls");

    printf("A sair da sua conta...\n");
    Sleep(2500);

    system("cls");
}

// Menu Inicial
void menuInicial()
{
    printf("1 - Criar uma Conta\n2 - Iniciar Sessão\n\n0 - Sair\n\nEscolha uma opção: ");
}

// Funcão de Criar Conta
void criarConta()
{
    system("cls");
    titulo();

    FILE *fptr;

    bool criarConta = true;
    User user;
    char teste[250];

    while (criarConta)
    {
        system("cls");
        titulo();
        fptr = fopen("dados.txt", "a+");
        if (fptr == NULL)
        {
            printf(RED "Erro ao abrir o arquivo!\n" WHITE);
            return;
        }

        printf(BOLD "Faça o seu registo aqui:\n\n" RESET);

        printf("--NOME USER--\n-> ");
        scanf("%s", user.email);

        printf("\n");

        int i = 0;
        char ch;

        fflush(stdin);

        printf("--PASSWORD--\n-> ");
        while ((ch = getch()) != 13)
        {
            if (ch != 8)
            {
                user.password[i] = ch;
                i++;
                printf("*");
            }
            else if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        user.password[i] = '\0';

        bool emailExiste = false;

        while (fscanf(fptr, "%s", teste) != EOF)
        {
            if (strcmp(teste, user.email) == 0)
            {
                emailExiste = true;
            }

            fscanf(fptr, "%*s %*s %*s");
        }

        if (emailExiste)
        {
            printf(RED "\nEste email já existe! Tente um novo.\n" WHITE);
        }
        else
        {

            fprintf(fptr, "%s %s 0 0\n", user.email, user.password);
            printf(GREEN "\nDados salvos com sucesso!\n" WHITE);
            criarConta = false;
        }

        Sleep(3000);
        system("cls");

        fclose(fptr);
    }
}

// Funcão de Inicar Sessão
void iniciarSessao()
{
    system("cls");
    titulo();

    FILE *fptr = fopen("dados.txt", "a+");
    if (fptr == NULL)
    {
        printf(RED "Erro ao abrir o arquivo de dados!\n" WHITE);
        return;
    }
    int admin;

    char testeEmail[250];
    char testePassword[250];
    char testeAdmin[2];
    bool sessaoIniciada = false;
    bool leitura = false;
    User user;

    printf(BOLD "Preencha os seus dados:\n\n" RESET);

    printf("--NOME USER--\n-> ");
    scanf("%s", user.email);

    printf("\n");
    int i = 0;
    char ch;

    fflush(stdin);

    printf("--PASSWORD--\n-> ");
    while ((ch = getch()) != 13)
    {
        if (ch != 8)
        {
            user.password[i] = ch;
            i++;
            printf("*");
        }
        else if (i > 0)
        {
            i--;
            printf("\b \b");
        }
    }
    user.password[i] = '\0';

    while (fscanf(fptr, "%s %s %s %*s", testeEmail, testePassword, testeAdmin) != EOF && leitura == false)
    {
        if (strcmp(testeEmail, user.email) == 0 && strcmp(testePassword, user.password) == 0)
        {
            sessaoIniciada = true;
            leitura = true;
            admin = atoi(testeAdmin);
        }
    }

    fclose(fptr);
    if (sessaoIniciada)
    {
        printf(GREEN "\nConta iniciada com sucesso!\n" WHITE);
        Sleep(2000);
        if (admin)
        {
            FILE *temp = fopen("tempConta.txt", "a+");
            fprintf(fptr, "%s", user.email);
            fclose(temp);
            menuAdmin(user.email);
            remove("tempConta.txt");
        }
        else
        {
            menuCliente(user.email);
        }
    }
    else
    {
        printf(YELLOW "\nEsta conta não existe ou a password está errada! Tente criar uma.\n" WHITE);
        Sleep(2000);
        system("cls");
    }
}