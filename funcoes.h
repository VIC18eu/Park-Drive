typedef struct
{
    int lugar;
    char nome[250];
    int diaInicio;
    int diaSaida;
    int mes;
} Reserva;

void titulo();
void printClientes();
int diaMaxMes();
void verificarDatas();
int verificarInt(char *frase);
bool validarDias(int diaInicio, int diaFim, char *nome, int validarDinheiro);
int validarLugar(char *frase);
//---------FUNÇÕES RESERVA----------
void printCalendar(int lugar);
void mostrarLugares(int diaInicio, int diaFim);
void escolherPesquisa(char *nome);
void dinheiro(char *nome, int valor, int modo);
bool verificarReserva(Reserva reserva);
void reserva(char *nomeUser);
void printarReservas(char *nome);
void cancelarReserva(char *nome);
//---------FUNÇÕES COMENTÁRIOS--------
void escreverComentario(char nome[250]);
void verComentario();
void lerUser(char nome[250]);
//---------FUNÇÕES GERENCIAR CONTA---------
void verDados(char nome[250]);
void tornarAdministrador(char nome[250]);
void apagarConta(char nome[250]);
void gerenciarContas(char nome[250]);
// Menu Admin
void menuAdmin();
// Menu Cliente
void menuCliente(char nome[250]);
// Menu Inicial
void menuInicial();
// Funcão de Criar Conta
void criarConta();
// Funcão de Inicar Sessão
void iniciarSessao();