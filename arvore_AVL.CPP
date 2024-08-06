#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definição da estrutura de um nó da árvore
typedef struct Node {
    int num; // Valor do nó
    struct Node *esq; // Ponteiro para o filho esquerdo
    struct Node *dir; // Ponteiro para o filho direito
    int altura; // Altura do nó
} No;

// Função para criar um novo nó com o valor fornecido
No *criar_no(int num) {
    No *node = (No*)(malloc(sizeof(No))); // Aloca memória para o novo nó
    node->num = num; // Define o valor do nó como o número fornecido
    node->esq = NULL; // Define o ponteiro para o filho esquerdo como NULL
    node->dir = NULL; // Define o ponteiro para o filho direito como NULL
    node->altura = 1; // Inicializa a altura como 1
    return node; // Retorna o novo nó criado
}

// Função para calcular a altura de um nó
int altura(No *node) {
    if (node == NULL) {
        return 0;
    }
    return node->altura;
}

// Função para calcular o fator de balanceamento de um nó
int fator_balanceamento(No *node) {
    if (node == NULL) {
        return 0;
    }
    return altura(node->esq) - altura(node->dir);
}

// Função para realizar uma rotação simples à direita
No* rotacao_simples_dir(No *rot) {
    No* novo = rot->esq;
    No* novo_dir = novo->dir;

    novo->dir = rot;
    rot->esq = novo_dir;

    rot->altura = fmax(altura(rot->esq), altura(rot->dir)) + 1;
    novo->altura = fmax(altura(novo->esq), altura(novo->dir)) + 1;

    return novo;
}

// Função para realizar uma rotação simples à esquerda
No* rotacao_simples_esquerda(No *rot) {
    No* novo = rot->dir;
    No* novo_esq = novo->esq;

    novo->esq = rot;
    rot->dir = novo_esq;

    rot->altura = fmax(altura(rot->esq), altura(rot->dir)) + 1;
    novo->altura = fmax(altura(novo->esq), altura(novo->dir)) + 1;

    return novo;
}

// Função para balancear um nó após inserção ou remoção
No* balancear(No* node) {
    if (node == NULL) {
        return NULL;
    }

    int fb = fator_balanceamento(node);

    // Caso LL
    if (fb > 1 && fator_balanceamento(node->esq) >= 0) {
        return rotacao_simples_dir(node);
    }

    // Caso LR
    if (fb > 1 && fator_balanceamento(node->esq) < 0) {
        node->esq = rotacao_simples_esquerda(node->esq);
        return rotacao_simples_dir(node);
    }

    // Caso RR
    if (fb < -1 && fator_balanceamento(node->dir) <= 0) {
        return rotacao_simples_esquerda(node);
    }

    // Caso RL
    if (fb < -1 && fator_balanceamento(node->dir) > 0) {
        node->dir = rotacao_simples_dir(node->dir);
        return rotacao_simples_esquerda(node);
    }

    return node;
}

// Função para inserir um novo nó na árvore AVL
No* inserir(No* node, No* novo) {
    if (node == NULL) {
        return novo;
    }

    if (novo->num < node->num) {
        node->esq = inserir(node->esq, novo);
    } else if (novo->num > node->num) {
        node->dir = inserir(node->dir, novo);
    } else {
        printf("Numero ja existe\n");
        return node;
    }

    node->altura = fmax(altura(node->esq), altura(node->dir)) + 1;

    return balancear(node);
}

// Função para remover um nó da árvore AVL
No* remover(No *node, int num) {
    if (node == NULL) {
        return NULL;
    }

    if (num < node->num) {
        node->esq = remover(node->esq, num);
    } else if (num > node->num) {
        node->dir = remover(node->dir, num);
    } else {
        if (node->esq == NULL || node->dir == NULL) {
            No *temp = node->esq ? node->esq : node->dir;

            if (temp == NULL) {
                temp = node;
                node = NULL;
            } else {
                *node = *temp;
            }

            free(temp);
        } else {
            No *temp = node->dir;
            while (temp->esq != NULL) {
                temp = temp->esq;
            }

            node->num = temp->num;
            node->dir = remover(node->dir, temp->num);
        }
    }

    if (node == NULL) {
        return node;
    }

    node->altura = fmax(altura(node->esq), altura(node->dir)) + 1;

    return balancear(node);
}

// Função para buscar um nó com o valor fornecido na árvore AVL
No *buscar_no(No *node, int num) {
    if (node == NULL || node->num == num) {
        return node;
    }
    if (num < node->num) {
        return buscar_no(node->esq, num);
    } else {
        return buscar_no(node->dir, num);
    }
}

// Função para imprimir a árvore AVL
void imprimir(No *node, int tab) {
    if (node == NULL) {
        return;
    }
    imprimir(node->dir, tab + 1);
    for (int i = 0; i < tab; i++) {
        printf("\t");
    }
    printf("%d\n", node->num);
    imprimir(node->esq, tab + 1);
}

int main(int argc, char const *argv[]) {
    No *raiz = NULL; // Inicializa a raiz como NULL
    int opcao, valor;
    No *buscado; // Declara a variável fora do switch

    do {
        printf("\nMenu:\n");
        printf("1. Inserir elemento\n");
        printf("2. Remover elemento\n");
        printf("3. Buscar elemento\n");
        printf("4. Imprimir arvore\n");
        printf("5. Mostrar fator de balanceamento da raiz\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, criar_no(valor));
                break;
            case 2:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
            case 3:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                buscado = buscar_no(raiz, valor); // Inicializa a variável aqui
                if (buscado != NULL) {
                    printf("Valor %d encontrado na arvore.\n", buscado->num);
                } else {
                    printf("Valor %d nao encontrado na arvore.\n", valor);
                }
                break;
            case 4:
                imprimir(raiz, 0);
                break;
            case 5:
                printf("Fator de balanceamento da raiz: %d\n", fator_balanceamento(raiz));
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while(opcao != 6);

    return 0;
}
