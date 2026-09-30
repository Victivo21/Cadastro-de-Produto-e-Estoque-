#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int codigo;
    char nome[50];
    char prateleira;
    float precoUnitario;
    int quantidadeEstoque;
    int disponivelVenda;
} Produto;

/* "Banco de dados" em memoria dos produtos cadastrados */
Produto estoque[100];
int totalProdutos = 0;

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
        lerLinha(nome, 50);

        i = 0;

        while (nome[i] == ' ')
            i++;

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
            prateleira = (char)toupper((unsigned char)entrada[0]);

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

/* Procura um produto pelo codigo.
   Retorna o indice no array ou -1 se nao achar */
int localizarPorCodigo(int codigo) {
    int i;

    for (i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == codigo) {
            return i;
        }
    }

    return -1;
}

/* Verifica se 'busca' esta contido em 'texto',
   ignorando maiusculas/minusculas */
int contemIgnorandoCase(const char *texto, const char *busca) {
    char t[50], b[50];
    int i;

    for (i = 0; texto[i] != '\0' && i < 50 - 1; i++) {
        t[i] = (char)tolower((unsigned char)texto[i]);
    }

    t[i] = '\0';

    for (i = 0; busca[i] != '\0' && i < 50 - 1; i++) {
        b[i] = (char)tolower((unsigned char)busca[i]);
    }

    b[i] = '\0';

    return strstr(t, b) != NULL;
}

/* Imprime os dados completos de um unico produto */
void imprimirProdutoDetalhado(Produto p) {
    printf("Codigo..................: %d\n", p.codigo);
    printf("Nome....................: %s\n", p.nome);
    printf("Prateleira..............: %c\n", p.prateleira);
    printf("Preco unitario..........: %.2f\n", p.precoUnitario);
    printf("Quantidade em estoque...: %d\n", p.quantidadeEstoque);

    printf("Disponivel para venda...: %s (%d)\n",
           p.disponivelVenda ? "Sim" : "Nao",
           p.disponivelVenda);

    printf("-------------------------------------\n");
}

/* =========================================================
   CADASTRO DE PRODUTO
   ========================================================= */

void cadastrarProduto() {
    Produto p;

    printf("\n=== Cadastro de Produto ===\n");

    if (totalProdutos >= 100) {
        printf("Erro: limite maximo de produtos atingido.\n\n");
        return;
    }

    p.codigo = lerCodigo();

    if (localizarPorCodigo(p.codigo) != -1) {
        printf("Erro: ja existe um produto cadastrado com esse codigo.\n\n");
        return;
    }

    lerNome(p.nome);

    p.prateleira = lerPrateleira();

    p.precoUnitario = lerPreco();

    p.quantidadeEstoque = lerQuantidade();

    /* Disponivel para venda eh calculado pelo sistema */
    p.disponivelVenda = (p.quantidadeEstoque > 0) ? 1 : 0;

    /* Guarda o produto no "banco de dados" em memoria */
    estoque[totalProdutos] = p;
    totalProdutos++;

    printf("\n--- Resumo do Produto Cadastrado ---\n");

    imprimirProdutoDetalhado(p);

    printf("\n");
}

/* =========================================================
   LISTAGEM DE PRODUTOS
   ========================================================= */

void listarProdutos() {
    int i;

    printf("\n=== Lista de Produtos ===\n");

    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n\n");
        return;
    }

    printf("%-6s %-20s %-6s %-10s %-6s %-8s\n",
           "Cod",
           "Nome",
           "Prat",
           "Preco",
           "Qtd",
           "Disp");

    printf("---------------------------------------------------------\n");

    for (i = 0; i < totalProdutos; i++) {
        Produto p = estoque[i];

        printf("%-6d %-20s %-6c %-10.2f %-6d %-8s\n",
               p.codigo,
               p.nome,
               p.prateleira,
               p.precoUnitario,
               p.quantidadeEstoque,
               p.disponivelVenda ? "Sim" : "Nao");
    }

    printf("\nTotal de produtos cadastrados: %d\n\n", totalProdutos);
}

/* =========================================================
   PESQUISA POR CODIGO
   ========================================================= */

void pesquisarProdutoPorCodigo() {
    int codigo;
    int indice;

    codigo = lerCodigo();

    indice = localizarPorCodigo(codigo);

    printf("\n");

    if (indice == -1) {
        printf("Nenhum produto encontrado com o codigo %d.\n\n",
               codigo);
    } else {
        printf("--- Produto Encontrado ---\n");

        imprimirProdutoDetalhado(estoque[indice]);

        printf("\n");
    }
}

/* =========================================================
   PESQUISA POR NOME
   ========================================================= */

void pesquisarProdutoPorNome() {
    char busca[50];
    int i;
    int encontrados = 0;

    printf("Nome (ou parte do nome) a pesquisar: ");

    lerLinha(busca, sizeof(busca));

    printf("\n");

    for (i = 0; i < totalProdutos; i++) {

        if (contemIgnorandoCase(estoque[i].nome, busca)) {

            printf("--- Produto Encontrado ---\n");

            imprimirProdutoDetalhado(estoque[i]);

            encontrados++;
        }
    }

    if (encontrados == 0) {

        printf("Nenhum produto encontrado com o nome \"%s\".\n",
               busca);

    } else {

        printf("Total encontrado: %d\n",
               encontrados);
    }

    printf("\n");
}

/* =========================================================
   MENU DE LISTAGEM
   ========================================================= */

void menuListagem() {
    char entrada[100];
    int opcao;
    int continuar = 1;

    while (continuar) {

        printf("\n");
        printf("===== MENU DE LISTAGEM =====\n");
        printf("1 - Listar todos os produtos\n");
        printf("2 - Voltar ao menu principal\n");
        printf("============================\n");
        printf("Escolha uma opcao: ");

        lerLinha(entrada, sizeof(entrada));

        if (sscanf(entrada, "%d", &opcao) != 1) {
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }

        switch (opcao) {

            case 1:
                listarProdutos();
                break;

            case 2:
                continuar = 0;
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }
}

/* =========================================================
   MENU DE PESQUISA
   ========================================================= */

void menuPesquisa() {
    char entrada[100];
    int opcao;
    int continuar = 1;

    while (continuar) {

        printf("\n");
        printf("===== MENU DE PESQUISA =====\n");
        printf("1 - Pesquisar por codigo\n");
        printf("2 - Pesquisar por nome\n");
        printf("3 - Voltar ao menu principal\n");
        printf("============================\n");
        printf("Escolha uma opcao: ");

        lerLinha(entrada, sizeof(entrada));

        if (sscanf(entrada, "%d", &opcao) != 1) {
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }

        switch (opcao) {

            case 1:
                pesquisarProdutoPorCodigo();
                break;

            case 2:
                pesquisarProdutoPorNome();
                break;

            case 3:
                continuar = 0;
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }
}

/* =========================================================
   MENU PRINCIPAL
   ========================================================= */

void exibirMenu() {
    printf("\n");
    printf("===== MENU PRINCIPAL =====\n");
    printf("1 - Cadastrar Produto\n");
    printf("2 - Listagem de Produtos\n");
    printf("3 - Pesquisa de Produto\n");
    printf("4 - Sair\n");
    printf("===========================\n");
    printf("Escolha uma opcao: ");
}

/* =========================================================
   FUNCAO PRINCIPAL
   ========================================================= */

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
                menuListagem();
                break;

            case 3:
                menuPesquisa();
                break;

            case 4:
                printf("Encerrando o sistema. Ate logo!\n");
                continuar = 0;
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n\n");
                break;
        }
    }

    return 0;
}
