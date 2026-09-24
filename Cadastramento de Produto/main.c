/* ============================================================
   Projeto Final - Sistema de Estoque
   Tarefa 1: Menu e Cadastro de Produto

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUTOS 100
#define TAM_NOME 50

/* ---------- Estrutura do Produto ---------- */
typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char prateleira;      /* A, B, C ou D */
    float precoUnitario;
    int quantidade;
    int disponivel;       /* calculado: 1 se quantidade > 0, senão 0 */
} Produto;

/* ---------- "Banco de dados" em memória ---------- */
Produto estoque[MAX_PRODUTOS];
int totalProdutos = 0;

/* ---------- Protótipos ---------- */
void exibirMenu(void);
void cadastrarProduto(void);
void limparBufferEntrada(void);
void lerLinha(char *destino, int tamanho);

int lerInteiroPositivo(const char *mensagem);
int lerInteiroNaoNegativo(const char *mensagem);
float lerFloatPositivo(const char *mensagem);
char lerPrateleiraValida(const char *mensagem);

/* ============================================================
   main: laço principal do menu, repete até o usuário sair
   ============================================================ */
int main(void) {
    int opcao;

    do {
        exibirMenu();
        opcao = lerInteiroNaoNegativo("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                printf("\nSaindo do sistema... ate mais!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

        printf("\n");

    } while (opcao != 2);

    return 0;
}

/* ============================================================
   Exibe o menu principal.
   Novas tarefas devem adicionar novas linhas aqui (e um novo
   "case" correspondente no switch do main).
   ============================================================ */
void exibirMenu(void) {
    printf("=====================================\n");
    printf("      SISTEMA DE ESTOQUE - MENU\n");
    printf("=====================================\n");
    printf("1 - Cadastrar Produto\n");
    printf("2 - Sair\n");
    printf("=====================================\n");
}

/* ============================================================
   Cadastra um novo produto, validando cada campo.
   ============================================================ */
void cadastrarProduto(void) {

    if (totalProdutos >= MAX_PRODUTOS) {
        printf("\nLimite maximo de produtos atingido!\n");
        return;
    }

    Produto novo;

    printf("\n----- Cadastro de Produto -----\n");

    /* Codigo: numero positivo */
    novo.codigo = lerInteiroPositivo("Codigo do produto: ");

    /* Nome: string, nao pode ficar em branco */
    do {
        printf("Nome do produto: ");
        lerLinha(novo.nome, TAM_NOME);
        if (strlen(novo.nome) == 0) {
            printf("O nome nao pode ficar em branco! Tente novamente.\n");
        }
    } while (strlen(novo.nome) == 0);

    /* Prateleira: apenas A, B, C ou D */
    novo.prateleira = lerPrateleiraValida("Prateleira (A, B, C ou D): ");

    /* Preco unitario: deve ser maior que zero */
    novo.precoUnitario = lerFloatPositivo("Preco unitario: ");

    /* Quantidade em estoque: deve ser >= 0 */
    novo.quantidade = lerInteiroNaoNegativo("Quantidade em estoque: ");

    /* Disponivel para venda: calculado pelo sistema */
    novo.disponivel = (novo.quantidade > 0) ? 1 : 0;

    /* Armazena no "banco" em memoria */
    estoque[totalProdutos] = novo;
    totalProdutos++;

    /* Resumo do cadastro */
    printf("\n----- Resumo do Cadastro -----\n");
    printf("Codigo............: %d\n", novo.codigo);
    printf("Nome..............: %s\n", novo.nome);
    printf("Prateleira........: %c\n", novo.prateleira);
    printf("Preco unitario....: %.2f\n", novo.precoUnitario);
    printf("Quantidade........: %d\n", novo.quantidade);
    printf("Disponivel venda..: %s (%d)\n",
           novo.disponivel ? "SIM" : "NAO", novo.disponivel);
    printf("-------------------------------\n");
}

/* ============================================================
   Funcoes auxiliares de entrada com validacao
   ============================================================ */

/* Limpa o restante do buffer de entrada (ate o \n ou EOF) */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}

/* Le uma linha de texto (usada para o nome) e remove o \n final */
void lerLinha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            /* linha maior que o buffer: descarta o resto */
            limparBufferEntrada();
        }
    } else {
        destino[0] = '\0';
    }
}

/* Le um inteiro que deve ser estritamente positivo (> 0) */
int lerInteiroPositivo(const char *mensagem) {
    int valor;
    int ok;

    do {
        printf("%s", mensagem);
        ok = scanf("%d", &valor);
        limparBufferEntrada();

        if (ok != 1) {
            printf("Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }
        if (valor <= 0) {
            printf("O valor deve ser um numero positivo! Tente novamente.\n");
        }
    } while (ok != 1 || valor <= 0);

    return valor;
}

/* Le um inteiro que deve ser maior ou igual a zero (usado tambem no menu) */
int lerInteiroNaoNegativo(const char *mensagem) {
    int valor;
    int ok;

    do {
        printf("%s", mensagem);
        ok = scanf("%d", &valor);
        limparBufferEntrada();

        if (ok != 1) {
            printf("Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }
        if (valor < 0) {
            printf("Quantidade negativa nao existe! Tente novamente.\n");
        }
    } while (ok != 1 || valor < 0);

    return valor;
}

/* Le um float que deve ser estritamente positivo (> 0) */
float lerFloatPositivo(const char *mensagem) {
    float valor;
    int ok;

    do {
        printf("%s", mensagem);
        ok = scanf("%f", &valor);
        limparBufferEntrada();

        if (ok != 1) {
            printf("Entrada invalida! Digite um numero (ex: 10.50).\n");
            continue;
        }
        if (valor <= 0) {
            printf("O preco deve ser maior que zero! Tente novamente.\n");
        }
    } while (ok != 1 || valor <= 0);

    return valor;
}

/* Le a prateleira e valida se e uma das letras permitidas (A, B, C, D) */
char lerPrateleiraValida(const char *mensagem) {
    char buffer[10];
    char letra;
    int valida;

    do {
        printf("%s", mensagem);
        lerLinha(buffer, sizeof(buffer));

        /* remove espacos e pega o primeiro caractere valido */
        letra = '\0';
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isspace((unsigned char)buffer[i])) {
                letra = (char)toupper((unsigned char)buffer[i]);
                break;
            }
        }

        valida = (letra == 'A' || letra == 'B' || letra == 'C' || letra == 'D');

        if (!valida) {
            printf("Prateleira invalida! Use apenas A, B, C ou D.\n");
        }
    } while (!valida);

    return letra;
}
