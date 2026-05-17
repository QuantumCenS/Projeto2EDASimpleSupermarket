//
// Created by Sergio on 5/2/2026.
//

#include "FilaArmazem.h"
#include <iostream>
#include <string>

using namespace std;


/**
 * inicializa uma nova fila de armazem, garantindo que comeca num estado vazio.
 * define o ponteiro do primeiro elemento como nulo (nullptr), preparando a estrutura
 * para receber as futuras operacoes de insercao de produtos.
 * @param f referencia para a estrutura da fila do armazem a ser inicializada
 */
void Nova(FilaArmazem &f) {
    f.primeiro = nullptr;
}


/**
 * insere (enfileira) um novo produto no fim da fila do armazem.
 * cria dinamicamente um novo no, preenche-o com os dados do produto e coloca-o na
 * ultima posicao da estrutura, mantendo a regra FIFO.
 * @param f referencia para a fila do armazem onde o produto sera inserido
 * @param elemento os dados do produto a serem adicionados a fila
 */
void Entra(FilaArmazem &f, Produto elemento) {
    // ATUALIZADO: Usar NoArmazem em vez de FilaArmazem::Item
    NoArmazem *novo = new NoArmazem;
    novo->valor = elemento;
    novo->seguinte = nullptr;

    // Se a fila não estiver vazia, percorre até ao fim
    if (!Vazia(f)) {
        NoArmazem *aux = f.primeiro;
        while (aux->seguinte != nullptr) {
            aux = aux->seguinte;
        }
        aux->seguinte = novo;
    }
    // Se estiver vazia, o novo elemento passa a ser o primeiro
    else {
        f.primeiro = novo;
    }
}


/**
 * verifica se a fila do armazem nao contem nenhum elemento.
 * @param f referencia para a fila do armazem a ser avaliada
 * @return true se a fila estiver vazia (ponteiro primeiro e nulo), false caso contrario
 */
bool Vazia(FilaArmazem &f) {
    return f.primeiro == nullptr;
}

/**
 * remove o produto que esta na frente da fila do armazem.
 * retira o primeiro elemento respeitando a regra FIFO, atualiza o ponteiro inicial
 * da estrutura para o no seguinte e liberta a memoria alocada para o no removido.
 * @param f referencia para a fila do armazem de onde sera removido o primeiro produto
 */
void Sai(FilaArmazem &f) {
    if (!Vazia(f)) {
        // ATUALIZADO: Usar NoArmazem
        NoArmazem *sai = f.primeiro;
        f.primeiro = sai->seguinte;
        sai->seguinte = nullptr;
        delete sai;
    }
    else {
        cout << "[ERRO] A fila (armazem) esta vazia. Nao e possivel remover." << endl;
    }
}

/**
 * consulta o produto que esta na frente da fila do armazem sem o remover.
 * permite espreitar os dados do proximo elemento a ser processado. se a fila estiver
 * vazia, devolve uma estrutura de produto preenchida com marcas de erro.
 * @param f referencia para a fila do armazem a ser consultada
 * @return copia do produto que se encontra na primeira posicao, ou um produto de erro caso esteja vazia
 */
Produto Primeiro(FilaArmazem &f) {
    if (!Vazia(f)) {
        return f.primeiro->valor;
    }
    else {
        // Retorna um produto de erro caso a fila esteja vazia
        Produto erro = {"ERRO", "ERRO", 0, 0, "ERRO"};
        return erro;
    }
}

/**
 * calcula e contabiliza o numero total de produtos atualmente armazenados na fila.
 * realiza uma travessia linear (pesquisa sequencial) desde o primeiro elemento ate
 * ao fim da fila, incrementando um contador por cada no encontrado.
 * @param f referencia para a fila do armazem a ser contabilizada
 * @return o numero inteiro total de elementos presentes na fila
 */
int Comprimento(FilaArmazem &f) {
    int comp = 0;
    if (!Vazia(f)) {
        // ATUALIZADO: Usar NoArmazem
        NoArmazem *aux = f.primeiro;
        while (aux != nullptr) {
            comp++;
            aux = aux->seguinte;
        }
    }
    return comp;
}
