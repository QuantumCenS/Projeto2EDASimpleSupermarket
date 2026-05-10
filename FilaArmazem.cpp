//
// Created by Sergio on 5/2/2026.
//

#include "FilaArmazem.h"
#include <iostream>
#include <string>

using namespace std;

// Inicializa a fila definindo o primeiro elemento como nulo
void Nova(FilaArmazem &f) {
    f.primeiro = nullptr;
}

// Insere um novo produto no fim da fila
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

// Verifica se a fila está vazia
bool Vazia(FilaArmazem &f) {
    return f.primeiro == nullptr;
}

// Remove o primeiro elemento da fila (FIFO)
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

// Retorna o produto que está na frente da fila sem o remover
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

// Calcula e retorna o número total de elementos na fila
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
