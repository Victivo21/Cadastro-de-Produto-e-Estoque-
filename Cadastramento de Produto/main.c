#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_NOME 50

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char prateleira;
    float precoUnitario;
    int quantidadeEstoque;
    int disponivelVenda;
} Produto;

/* Le uma linha inteira do teclado, remove o '\n' e limpa o buffer se sobrar algo */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

/* Codigo deve ser um numero inteiro positivo */
int lerCodigo() {
    char entrada[100];
    int codigo;
    int valido = 0;

    do {
        printf("Codigo do produto: ");
        lerLinha(entrada, sizeof(entrada));

        if (sscanf(entrada, "%d", &codigo) == 1 && codigo > 0) {
            valido = 1;
        } else {
            printf("Erro: o codigo deve ser um numero inteiro positivo.\n");
        }
    } while (!valido);

    return codigo;
}

/* Nome nao pode ficar em branco */
void lerNome(char *nome) {
    int valido = 0;
    int i;

    do {
        printf("Nome do produto: ");
        lerLinha(nome, TAM_NOME);

        i = 0;
        while (nome[i] == ' ') i++;

        if (strlen(nome) == 0 || nome[i] == '\0') {
            printf("Erro: o nome nao pode ficar em branco.\n");
        } else {
            valido = 1;
        }
    } while (!valido);
}

/* Prateleira deve ser uma letra entre A e D */
char lerPrateleira() {
    char entrada[100];
    char prateleira = '\0';
    int valido = 0;

    do {
        printf("Prateleira (A, B, C ou D): ");
        lerLinha(entrada, sizeof(entrada));

        valido = 0;
        if (strlen(entrada) == 1) {
            prateleira = (char) toupper((unsigned char) entrada[0]);
            if (prateleira >= 'A' && prateleira <= 'D') {
                valido = 1;
            }
        }

        if (!valido) {
            printf("Erro: prateleira invalida. Use apenas A, B, C ou D.\n");
        }
    } while (!valido);

    return prateleira;
}

/* Preco unitario deve ser maior que zero */
float lerPreco() {
    char entrada[100];
    float preco;
    int valido = 0;

    do {
        printf("Preco unitario: ");
        lerLinha(entrada, sizeof(entrada));

        if (sscanf(entrada, "%f", &preco) == 1 && preco > 0) {
            valido = 1;
        } else {
            printf("Erro: o preco deve ser maior que zero.\n");
        }
    } while (!valido);

    return preco;
}

/* Quantidade em estoque deve ser maior ou igual a zero */
int lerQuantidade() {
    char entrada[100];
    int quantidade;
    int valido = 0;

    do {
        printf("Quantidade em estoque: ");
        lerLinha(entrada, sizeof(entrada));

        if (sscanf(entrada, "%d", &quantidade) == 1 && quantidade >= 0) {
            valido = 1;
        } else {
            printf("Erro: a quantidade nao pode ser negativa.\n");
        }
    } while (!valido);

    return quantidade;
}

void cadastrarProduto() {
    Produto p;

    printf("\n=== Cadastro de Produto ===\n");

    p.codigo = lerCodigo();
    lerNome(p.nome);
    p.prateleira = lerPrateleira();
    p.precoUnitario = lerPreco();
    p.quantidadeEstoque = lerQuantidade();

    /* Disponivel para venda eh calculado pelo sistema, nunca informado pelo usuario */
    p.disponivelVenda = (p.quantidadeEstoque > 0) ? 1 : 0;

    printf("\n--- Resumo do Produto Cadastrado ---\n");
    printf("Codigo..................: %d\n", p.codigo);
    printf("Nome....................: %s\n", p.nome);
    printf("Prateleira..............: %c\n", p.prateleira);
    printf("Preco unitario..........: %.2f\n", p.precoUnitario);
    printf("Quantidade em estoque...: %d\n", p.quantidadeEstoque);
    printf("Disponivel para venda...: %s (%d)\n",
           p.disponivelVenda ? "Sim" : "Nao", p.disponivelVenda);
    printf("-------------------------------------\n\n");
}

void exibirMenu() {
    printf("===== MENU PRINCIPAL =====\n");
    printf("1 - Cadastrar Produto\n");
    printf("2 - Sair\n");
    /* Novas opcoes de futuras tarefas entram aqui,
       seguindo o mesmo padrao "numero - Descricao" */
    printf("===========================\n");
    printf("Escolha uma opcao: ");
}

int main(void) {
    char entrada[100];
    int opcao;
    int continuar = 1;

    while (continuar) {
        exibirMenu();
        lerLinha(entrada, sizeof(entrada));

        if (sscanf(entrada, "%d", &opcao) != 1) {
            printf("Opcao invalida. Tente novamente.\n\n");
            continue;
        }

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                printf("Encerrando o sistema. Ate logo!\n");
                continuar = 0;
                break;
            /* Novos "case" entram aqui conforme novas opcoes
               forem adicionadas ao menu */
            default:
                printf("Opcao invalida. Tente novamente.\n\n");
                break;
        }
    }

    return 0;
}
