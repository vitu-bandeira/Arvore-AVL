#include <stdio.h>
#include <stdlib.h>
#define RED 1
#define BLACK 0

typedef struct No{
    int valor; int cor;
    struct No *esq;
    struct No *dir;
}No;

int cor(No* node){
    if(node==NULL)
    return BLACK;
    return node->cor;
}

void trocar_cor(No* node){
    node->cor = !node->cor; // 0 -> 1, 1 -> 0 
    if(node->esq != NULL){
        node->esq->cor = !node->esq->cor; // 0 -> 1, 1 -> 0
    }
    if (node->dir != NULL)    
    {
        node->dir->cor = !node->dir->cor; // 0 -> 1, 1 -> 0
    }
}

No *rotacionar_esquerdo(No *nodeA){
    No *nodeB = nodeA->dir;
    nodeA->dir=nodeB->esq;
    nodeB->esq = nodeA;
    nodeB->cor = nodeA->cor;
    nodeA->cor=RED;
    return nodeB;
}

No *rotacionar_direito(No *nodeA){
    No *nodeB = nodeA->esq;
    nodeA->esq = nodeB->dir;
    nodeB->dir = nodeA;
    nodeB->cor = nodeA->cor;
    nodeA->cor = RED;
    return nodeB;
}

No *move2esq(No *node){
    trocar_cor(node);
    if (cor(node->dir->esq)==RED)
    {
        node->dir = rotacionar_direito(node->dir);
        node=rotacionar_esquerdo(node);
        trocar_cor(node);
    }
    return node;
}

No *move2dir(No *node){
    trocar_cor(node);
    if (cor(node->esq->esq) == RED)
    {
        node=rotacionar_direito(node);
        trocar_cor(node);
    }
    return node;
}

No *balancear(No *node) {
    if (cor(node->dir) == RED) {
        node = rotacionar_esquerdo(node);
    }
    if (node->esq != NULL && cor(node->esq) == RED && cor(node->esq->esq) == RED) {
        node = rotacionar_direito(node);
    }
    if (cor(node->esq) == RED && cor(node->dir) == RED) {
        trocar_cor(node);
    }
    return node;
}

// Procurar o menor nó
No *procurarMenor(No *atual) {
    No *no1 = atual;
    No *no2 = atual->esq;

    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

// Remover o menor nó
No *removerMenor(No *no) {
    if (no->esq == NULL) {
        free(no);
        return NULL;
    }
    if (cor(no->esq) == BLACK && cor(no->esq->esq) == BLACK) {
        no = move2esq(no);
    }
    no->esq = removerMenor(no->esq);
    return balancear(no);
}

No *criar_no(int valor){
    No *novo = (No*)malloc(sizeof(No));
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->cor = RED;
    return novo;    
}

No* inserir(No *no, int valor){
    if (no == NULL) {
        return criar_no(valor); // Se o nó for NULL, cria um novo nó
    }

    if (valor == no->valor) {
        printf("Número já existe\n"); // Se o valor já existe, exibe uma mensagem
        return no;
    }

    if (valor < no->valor) {
        no->esq = inserir(no->esq, valor); // Inserção à esquerda
    } else {
        no->dir = inserir(no->dir, valor); // Inserção à direita
    }

    //Balanceamento após a inserção
    if (cor(no->dir) == RED && cor(no->esq) == BLACK) {
        no = rotacionar_esquerdo(no);
    }
    if (cor(no->esq) == RED && cor(no->esq->esq) == RED) {
        no = rotacionar_direito(no);
    }
    if (cor(no->esq) == RED && cor(no->dir) == RED) {
        trocar_cor(no);
    }

    return no;
}



void imprimir(No *no, int tab) {
    if (no == NULL) {
        return;
    }
    imprimir(no->dir, tab + 1);
    for (int i = 0; i < tab; i++) {
        printf("\t");
    }
    printf("%d (%s)\n", no->valor, no->cor == RED ? "V" : "P");
    imprimir(no->esq, tab + 1);
}

No *buscar_no(No *node, int num) {
    if (node == NULL) {
        return NULL;
    }

    if (node->valor == num) {
        return node;
    }

    if (node->valor < num) {
        return buscar_no(node->dir, num);
    } else {
        return buscar_no(node->esq, num);
    }
}


void buscar_raiz(No *raiz) {
  int num = 0;
  printf("Digite o numero para buscar: ");
  scanf("%d", &num);
  No *res = buscar_no(raiz, num);
  if (res == NULL) {
    printf("Numero nao encontrado\n");
  } else {
    imprimir(res,0);
  }
}

No *remove_no(No *no, int valor) {
    if (no == NULL) return NULL; // Verificação para evitar erros de segmentação

    if (valor < no->valor) {
        if (cor(no->esq) == BLACK && cor(no->esq->esq) == BLACK) {
            no = move2esq(no);
        }
        no->esq = remove_no(no->esq, valor);
    } else {
        if (cor(no->esq) == RED) {
            no = rotacionar_direito(no);
        }
        if (valor == no->valor && no->dir == NULL) {
            free(no);
            return NULL;
        }
        if (cor(no->dir) == BLACK && cor(no->dir->esq) == BLACK) {
            no = move2dir(no);
        }
        if (valor == no->valor) {
            No *x = procurarMenor(no->dir);
            no->valor = x->valor;
            no->dir = removerMenor(no->dir);
        } else {
            no->dir = remove_no(no->dir, valor);
        }
    }
    return balancear(no);
}


void remover(No **raiz, int num) {
    if (*raiz != NULL) { // Verificação se a raiz não é nula
        *raiz = remove_no(*raiz, num);
        if (*raiz != NULL) {
            (*raiz)->cor = BLACK; // Garante que a raiz seja sempre preta
        }
    }
}

void inserir_No(No **raiz, int valor){
    *raiz = inserir(*raiz, valor);
    (*raiz)->cor = BLACK;
}

int main(int argc, char const *argv[]) {
    No *raiz = NULL;
    
    inserir_No(&raiz, 10);
    inserir_No(&raiz, 5);
    inserir_No(&raiz, 15);
    inserir_No(&raiz, 20);

    imprimir(raiz, 0);
    printf("\n");
    buscar_raiz(raiz);
    printf("\n");

    int num;
    printf("Digite o numero para remover: ");
    scanf("%d", &num);

    remover(&raiz, num); 

    printf("Arvore apos remocao:\n");
    imprimir(raiz, 0); 

    return 0;
}


