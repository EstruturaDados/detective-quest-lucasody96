#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para a Árvore de Pistas (BST)
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esq, *dir;
} PistaNode;

// Estrutura para o Mapa da Mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* * Função: inserirPista
 * Objetivo: Insere uma pista na BST seguindo a ordem alfabética (strcmp).
 */
PistaNode* inserirPista(PistaNode *raiz, char *novaPista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->conteudo, novaPista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    // Lógica BST: Strings menores para a esquerda, maiores para a direita
    if (strcmp(novaPista, raiz->conteudo) < 0)
        raiz->esq = inserirPista(raiz->esq, novaPista);
    else if (strcmp(novaPista, raiz->conteudo) > 0)
        raiz->dir = inserirPista(raiz->dir, novaPista);
        
    return raiz;
}

/* * Função: criarSala
 * Objetivo: Cria um cômodo da mansão com uma pista associada.
 */
Sala* criarSala(char *nome, char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

/* * Função: exibirPistas
 * Objetivo: Percorre a BST em ordem (In-Order) para listar pistas alfabeticamente.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->conteudo);
        exibirPistas(raiz->dir);
    }
}

/* * Função: explorarSalasComPistas
 * Objetivo: Gerencia a navegação e coleta automática para a árvore de inventário.
 */
void explorarSalasComPistas(Sala *raizMansao) {
    Sala *atual = raizMansao;
    PistaNode *inventario = NULL;
    char escolha;

    while (atual != NULL) {
        printf("\nVoce entrou em: '%s'\n", atual->nome);
        
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: [%s]\n", atual->pista);
            inventario = inserirPista(inventario, atual->pista);
        }

        printf("Para onde deseja ir? ");
        if (atual->esquerda) printf("'e' para %s ", atual->esquerda->nome);
        if (atual->direita) printf("'d' para %s ", atual->direita->nome);
        printf("\nOu digite 's' para encerrar a investigacao e ver o relatorio: ");
        
        scanf(" %c", &escolha);

        if (escolha == 's') break;
        else if (escolha == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita) atual = atual->direita;
        else printf("Caminho sem saida ou opcao invalida!\n");
    }

    printf("\n--- RELATORIO FINAL DE PISTAS (ORDEM ALFABETICA) ---\n");
    if (inventario == NULL) printf("Nenhuma pista coletada.\n");
    else exibirPistas(inventario);
}

int main() {
    // Montagem da Mansão (Mapa Fixo)
    Sala *hall = criarSala("Hall de Entrada", "Chave enferrujada");
    Sala *biblioteca = criarSala("Biblioteca", "Bilhete com ameaca");
    Sala *cozinha = criarSala("Cozinha", "Aroma de amendoas (veneno?)");
    Sala *jardim = criarSala("Jardim", "Pegada de lama");
    Sala *sotao = criarSala("Sotao", "Diario antigo");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = jardim;
    cozinha->direita = sotao;

    printf("--- INICIANDO DETECTIVE QUEST: EXPANSÃO DE PISTAS ---\n");
    explorarSalasComPistas(hall);

    return 0;
}