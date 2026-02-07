#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 17 // Tamanho primo para melhor distribuição

// --- ESTRUTURAS DE DADOS ---

// 1. Estrutura para o Inventário (BST - Árvore Binária de Busca)
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esq, *dir;
} PistaNode;

// 2. Estrutura para o Mapa (Árvore Binária de Navegação)
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista estática presente na sala
    struct Sala *esquerda, *direita;
} Sala;

// 3. Estrutura para a Base de Conhecimento (Tabela Hash com Encadeamento)
typedef struct HashEntry {
    char pista[100];     // Chave
    char suspeito[50];   // Valor
    struct HashEntry *proximo; // Tratamento de colisão
} HashEntry;

// Variável Global para a Tabela Hash (Simulando banco de dados do jogo)
HashEntry* tabelaHash[TAM_HASH];

// --- FUNÇÕES DA TABELA HASH ---

int funcaoHash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash % TAM_HASH;
}

/* * Função: inserirNaHash
 * Objetivo: Mapeia uma pista a um suspeito na tabela Hash.
 * Lógica: Insere no início da lista encadeada caso haja colisão.
 */
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    HashEntry *novo = (HashEntry*)malloc(sizeof(HashEntry));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaHash[indice]; // Encadeamento
    tabelaHash[indice] = novo;
}

/* * Função: encontrarSuspeito
 * Objetivo: Consulta a tabela Hash para descobrir quem está ligado a uma pista.
 */
char* encontrarSuspeito(char *pista) {
    int indice = funcaoHash(pista);
    HashEntry *atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return "Desconhecido"; // Caso a pista não tenha dono no banco de dados
}

// --- FUNÇÕES DA BST (INVENTÁRIO) ---

/* * Função: inserirPista (adicionarPista)
 * Objetivo: Insere a pista coletada na árvore BST de forma ordenada.
 * Isso facilita a listagem alfabética posterior.
 */
PistaNode* inserirPista(PistaNode *raiz, char *pista) {
    // Se a árvore estiver vazia, cria o nó
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->conteudo, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    
    // Navegação para inserção ordenada
    int comparacao = strcmp(pista, raiz->conteudo);
    
    if (comparacao < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (comparacao > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    // Se for igual (pista duplicada), não faz nada (não insere novamente)
        
    return raiz;
}

void exibirInventario(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirInventario(raiz->esq);
        printf("- %s\n", raiz->conteudo);
        exibirInventario(raiz->dir);
    }
}

// --- FUNÇÕES DE LÓGICA DO JOGO E MAPA ---

/* * Função: criarSala
 * Objetivo: Cria dinamicamente um cômodo do mapa.
 */
Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função auxiliar recursiva para contar provas contra um suspeito
int contarProvasRecursivo(PistaNode *raiz, char *acusado) {
    if (raiz == NULL) return 0;
    
    int contagem = 0;
    char *suspeitoDaPista = encontrarSuspeito(raiz->conteudo);
    
    // Comparação insensível a maiúsculas/minúsculas seria ideal, mas usaremos strcmp simples
    if (strcmp(suspeitoDaPista, acusado) == 0) {
        contagem = 1;
    }
    
    return contagem + 
           contarProvasRecursivo(raiz->esq, acusado) + 
           contarProvasRecursivo(raiz->dir, acusado);
}

/* * Função: verificarSuspeitoFinal
 * Objetivo: Conduz o julgamento final.
 * Lógica: Percorre a BST do jogador, consulta a Hash para cada pista
 * e verifica se o acusado tem 2 ou mais evidências contra si.
 */
void verificarSuspeitoFinal(PistaNode *inventario) {
    char acusado[50];
    
    printf("\n====================================\n");
    printf("       TRIBUNAL DA MANSÃO\n");
    printf("====================================\n");
    printf("Pistas que voce coletou:\n");
    exibirInventario(inventario);
    
    printf("\nQuem e o culpado? (Mordomo, Jardineiro ou Cozinheira): ");
    scanf(" %[^\n]", acusado); // Lê string com espaços
    
    int provas = contarProvasRecursivo(inventario, acusado);
    
    printf("\nAnalisando evidencias...\n");
    if (provas >= 2) {
        printf("\n>>> RESULTADO: CULPADO!\n");
        printf("Voce encontrou %d pistas solidas contra %s.\n", provas, acusado);
        printf("A policia esta a caminho. Bom trabalho, detetive!\n");
    } else {
        printf("\n>>> RESULTADO: INOCENTE (ou provas insuficientes).\n");
        printf("Voce so tem %d pista(s) ligando %s ao crime.\n", provas, acusado);
        printf("O culpado escapou. Game Over.\n");
    }
}

/* * Função: explorarSalas
 * Objetivo: Navega pela árvore (Mapa) e preenche a BST (Inventário).
 */
void explorarSalas(Sala *salaAtual, PistaNode **inventario) {
    char opcao;
    
    while (salaAtual != NULL) {
        printf("\n------------------------------------\n");
        printf("VOCE ESTA EM: %s\n", salaAtual->nome);
        
        // Coleta automática de pista
        if (strlen(salaAtual->pista) > 0) {
            printf("(!) Voce encontrou uma pista: [%s]\n", salaAtual->pista);
            *inventario = inserirPista(*inventario, salaAtual->pista);
        } else {
            printf("( ) Nenhuma pista neste local.\n");
        }
        
        printf("Caminhos: ");
        if (salaAtual->esquerda) printf("[e] Esquerda (%s) ", salaAtual->esquerda->nome);
        if (salaAtual->direita) printf("[d] Direita (%s) ", salaAtual->direita->nome);
        printf("\n[s] Sair e Acusar\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &opcao);
        
        if (opcao == 's') {
            break;
        } else if (opcao == 'e' && salaAtual->esquerda) {
            salaAtual = salaAtual->esquerda;
        } else if (opcao == 'd' && salaAtual->direita) {
            salaAtual = salaAtual->direita;
        } else {
            printf("Caminho bloqueado ou invalido!\n");
        }
    }
}

// --- FUNÇÃO MAIN ---

int main() {
    // 1. Inicializar Tabela Hash (Limpar memória)
    for(int i=0; i<TAM_HASH; i++) tabelaHash[i] = NULL;
    
    // 2. Carregar "Conhecimento do Mundo" (Associações Pista -> Suspeito)
    // Mordomo é o culpado neste cenário
    inserirNaHash("Castical torto", "Mordomo");
    inserirNaHash("Relogio de bolso quebrado", "Mordomo");
    inserirNaHash("Luvas brancas manchadas", "Mordomo");
    
    // Outros suspeitos (Red Herrings)
    inserirNaHash("Faca de peixe", "Cozinheira");
    inserirNaHash("Botas com lama", "Jardineiro");

    // 3. Construir o Mapa (Árvore Binária)
    // Mapa: Hall -> (Esq: Biblioteca -> Jardim) / (Dir: Cozinha -> Porão)
    Sala *hall = criarSala("Hall", "Relogio de bolso quebrado");
    Sala *biblioteca = criarSala("Biblioteca", "Castical torto"); // Pista incriminadora
    Sala *jardim = criarSala("Jardim", "Botas com lama"); // Pista do Jardineiro
    Sala *cozinha = criarSala("Cozinha", "Faca de peixe"); // Pista da Cozinheira
    Sala *porao = criarSala("Porao", "Luvas brancas manchadas"); // Pista incriminadora

    // Conexões
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = jardim;
    cozinha->direita = porao;
    
    // Variável para armazenar o inventário do jogador (Raiz da BST)
    PistaNode *meuInventario = NULL;

    printf("BEM-VINDO AO DETECTIVE QUEST: EDICAO MESTRE\n");
    printf("Explore a mansao, colete pistas e acuse o culpado.\n");
    
    // 4. Iniciar Exploração
    explorarSalas(hall, &meuInventario);
    
    // 5. Fase Final
    verificarSuspeitoFinal(meuInventario);

    return 0;
}