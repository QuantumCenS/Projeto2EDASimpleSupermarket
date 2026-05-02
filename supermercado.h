#ifndef SUPERMERCADO_H
#define SUPERMERCADO_H

#include <string>
#include "FilaArmazem.h" // Inclui a struct Produto e a implementação da Fila

// Estrutura genérica para Listas Ligadas de Strings (substitui string*)
struct NoString {
    std::string texto;
    NoString* prox;
};


// ESTRUTURAS DE DADOS BASE (Apenas Listas Ligadas e Árvores)


// Mantemos a ListaProdutos para os setores, pois a remoção por venda é aleatória
struct NoProduto {
    Produto info;
    NoProduto* prox;
};

struct ListaProdutos {
    NoProduto* inicio;
    NoProduto* fim;
};

// Árvore Binária de Pesquisa (Registo de Vendas)
struct NoVenda {
    int preco;
    std::string nome;
    NoVenda* esq;
    NoVenda* dir;
};

struct ArvoreVendas {
    NoVenda* raiz;
};

struct Setor {
    char id;
    std::string responsavel;
    std::string area;
    int capacidade;
    int ocupacao;
    ListaProdutos produtos; // Produtos expostos no setor (Lista Ligada)
    ArvoreVendas vendas;    // Registo do que foi vendido (Árvore)
};

struct NoSetor {
    Setor dados;
    NoSetor* prox;
};

struct ListaSetores {
    NoSetor* inicio;
};

struct Campanha {
    std::string area;
    int percentagem;
    int duracao;
    Campanha* prox;
};

// Estrutura principal do supermercado
struct SuperMercado {
    ListaSetores setores;
    FilaArmazem armazem;
    Campanha* campanhas;// <-- ATUALIZADO: O armazém passa a usar estritamente a Fila (FIFO)
};


// FUNÇÕES UTILITÁRIAS E LISTAS

int gerarAleatorio(int min, int max);
NoString* carregarStrings(const std::string& filename, int& total);
std::string obterElementoLista(NoString* cabeca, int indice);
void libertarStrings(NoString* cabeca);
bool existeString(NoString* cabeca, const std::string& str);
void adicionarStringFim(NoString*& cabeca, const std::string& str, int& contador);

// Funções para gerir a ListaProdutos (exclusivo para os setores agora)
void inicializarListaProdutos(ListaProdutos& lista);
void adicionarProdutoFim(ListaProdutos& lista, const Produto& p);


// PONTO 2 - INICIALIZAÇÃO E FUNCIONAMENTO

void inicializarArvore(ArvoreVendas& arv);
void inserirVenda(ArvoreVendas& arv, int preco, const std::string& nome);

void inicializarSupermercado(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);

void gerarProdutosParaArmazem(FilaArmazem& armazem, int quantidade, NoString* areasSetores, int nAreasSet, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);

// Ciclo de simulação acionado pela tecla (s)
void simularCiclo(SuperMercado& sm, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);
void venderProdutos(SuperMercado& sm);
void transferirArmazemParaSetores(SuperMercado& sm, int maxTransferir);


//Funcoes a definir no futuro
void removerProdutoGlobal(SuperMercado& sm, const std::string& nome);
void atualizarPrecoArmazem(SuperMercado& sm, const std::string& nome, int novoPreco);
void adicionarCampanha(SuperMercado& sm, const std::string& area, int percentagem, int duracao);
bool gravarSupermercado(const SuperMercado& sm, const std::string& filename);
bool carregarSupermercado(SuperMercado& sm, const std::string& filename);
void imprimirProdutos(const SuperMercado& sm);
void criarNovaArea(NoString*& areas, int& nAreas, const std::string& novaArea); // <-- Atualizado para NoString*
void mostrarRegistoVendas(const SuperMercado& sm, const std::string& responsavel);
#endif