#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da Sala (Nó da Árvore)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* * Função: criarSala
 * Objetivo: Aloca dinamicamente uma nova sala no mapa.
 */
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

/* * Função: explorarSalas
 * Objetivo: Gerencia a interação do usuário, permitindo navegar pela árvore.
 */
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha (fim da linha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce chegou ao fim do caminho. Investigacao concluida nesta ala!\n");
            break;
        }

        printf("Caminhos disponiveis: ");
        if (atual->esquerda) printf("[e] Esquerda: %s  ", atual->esquerda->nome);
        if (atual->direita) printf("[d] Direita: %s", atual->direita->nome);
        
        printf("\nEscolha seu caminho (ou 's' para sair): ");
        scanf(" %c", &escolha);

        if (escolha == 's') {
            printf("Saindo da investigacao...\n");
            break;
        } else if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Caminho invalido! Tente novamente.\n");
        }
    }
}

/* * Função: main
 * Objetivo: Monta a estrutura da mansão e inicia o jogo.
 */
int main() {
    // Montagem manual da árvore conforme requisitos
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *sotao = criarSala("Sotao");

    // Definindo as conexões (hierarquia)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = sotao;

    // Início da exploração
    printf("--- BEM-VINDO AO DETECTIVE QUEST ---\n");
    explorarSalas(hall);

    // Nota: Em um sistema real, seria necessária uma função para liberar a memória (free)
    return 0;
}