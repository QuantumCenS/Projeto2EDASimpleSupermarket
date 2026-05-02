//
// Created by Sergio on 5/2/2026.
//

#ifndef PROJETO2EDA_FILAARMAZEM_H
#define PROJETO2EDA_FILAARMAZEM_H

#ifndef FILAARMAZEM_H
#define FILAARMAZEM_H

#include <string>

// Definição da estrutura Produto
struct Produto {
    std::string nome;
    std::string fornecedor;
    int preco;
    int precoOriginal;
    std::string area;
};

// Estrutura da Fila adaptada
struct FilaArmazem {
    struct Item {
        Produto valor;      // O valor guardado passa a ser o Produto em vez de um int
        Item* seguinte;     // Apontador para o próximo nó
    };
    Item* primeiro;         // Apontador para o início da fila
};

// Declarações das funções padrão de manipulação da Fila
void Nova(FilaArmazem& f);
void Entra(FilaArmazem& f, Produto elemento);
void Sai(FilaArmazem& f);
bool Vazia(FilaArmazem& f);
Produto Primeiro(FilaArmazem& f);
int Comprimento(FilaArmazem& f);
void Escreve(FilaArmazem& f);

#endif // FILAARMAZEM_H


#endif //PROJETO2EDA_FILAARMAZEM_H