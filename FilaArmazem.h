//
// Created by Sergio on 5/2/2026.
//

#ifndef PROJETO2EDA_FILAARMAZEM_H
#define PROJETO2EDA_FILAARMAZEM_H

#ifndef FILAARMAZEM_H
#define FILAARMAZEM_H

#include <string>

using namespace std;

struct Produto {
    string nome;
    string fornecedor;
    int preco;
    int precoOriginal;
    string area;
};

// Estrutura da Fila adaptada
struct NoArmazem {
    Produto valor;
    NoArmazem* seguinte;
};

struct FilaArmazem {
    NoArmazem* primeiro;
};

// Declarações das funções padrão de manipulação da Fila
void Nova(FilaArmazem& f);
void Entra(FilaArmazem& f, Produto elemento);
void Sai(FilaArmazem& f);
bool Vazia(FilaArmazem& f);
Produto Primeiro(FilaArmazem& f);
int Comprimento(FilaArmazem& f);

#endif // FILAARMAZEM_H


#endif //PROJETO2EDA_FILAARMAZEM_H