#ifndef SUPERMERCADO_H
#define SUPERMERCADO_H

#include <string>
#include "FilaArmazem.h" // Inclui a struct Produto e a implementação da Fila
using namespace std;

#include <string>


struct NoString {
    string texto;
    NoString* prox;
};




// CORREÇÃO: ÁRVORE DE VENDAS (Sem duplicação)
// Apenas precisamos do nó. A struct "ArvoreVendas" desaparece.
struct NoVenda {
    int preco;
    string nome;
    NoVenda* esq;
    NoVenda* dir;
};

// NÓ PARA A LISTA DE PRODUTOS NAS PRATELEIRAS
struct NoProduto {
    Produto info;
    NoProduto* prox;
};


// CORREÇÃO: O SETOR (Sem duplicação)
// O Setor guarda os seus dados e aponta para o próximo Setor.
// Desaparecem as structs "NoSetor" e "ListaSetores".
struct Sector {
    char id;
    string responsavel;
    string area;
    int capacidade;
    int ocupacao;

    NoProduto* inicioProdutos; // Ponteiro direto para os produtos na prateleira
    NoVenda* raizVendas;       // Ponteiro direto para as vendas (sem ArvoreVendas)

    Sector* prox;
};


struct Campanha {
    string area;
    int percentagem;
    int duracao;
    Campanha* prox;
};

struct SuperMercado {
    Sector* inicioSectores;     // Aponta diretamente para o primeiro Setor
    FilaArmazem armazem;
    Campanha* campanhas;
};






// FUNÇÕES UTILITÁRIAS E STRINGS
int gerarAleatorio(int min, int max);
NoString* carregarStrings(const std::string& filename, int& total);
string obterElementoLista(NoString* cabeca, int indice);
void libertarStrings(NoString* cabeca);
bool existeString(NoString* cabeca, const string& str);
void adicionarStringFim(NoString*& cabeca, const string& str, int& contador);

// Funções para gerir os Produtos nos Setores (Atualizado sem a struct ListaProdutos)
void inicializarListaProdutos(NoProduto*& inicio);
void adicionarProdutoFim(NoProduto*& inicio, const Produto& p);

// PONTO 2 - INICIALIZAÇÃO E FUNCIONAMENTO

// Funções para gerir a Árvore de Vendas (Atualizado sem a struct ArvoreVendas)
void inicializarArvore(NoVenda*& raiz);
void inserirVendaRec(NoVenda*& raiz, int preco, const string& nome);

void inicializarSupermercado(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);


void gerarProdutosParaArmazem(SuperMercado& sm, int quantidade, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);

// Ciclo de simulação acionado pela tecla (s)
void simularCiclo(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);
void venderProdutos(SuperMercado& sm);
void transferirArmazemParaSetores(SuperMercado& sm, int maxTransferir);

// Funções de Gestão (em falta a de gravar e carregar)
bool removerProdutoGlobal(SuperMercado& sm, const string& nome);
bool atualizarPrecoArmazem(SuperMercado& sm, const string& nome, int novoPreco);
bool adicionarCampanha(SuperMercado& sm, const string& area, int percentagem, int duracao);
bool gravarSupermercado( SuperMercado& sm, const string& filename);
bool carregarSupermercado(SuperMercado& sm, const string& filename);


void imprimirProdutos(const SuperMercado& sm);
bool criarNovaArea(NoString*& areas, int& nAreas, const string& novaArea);
bool mostrarRegistoVendas(const SuperMercado& sm, const string& responsavel);
void atualizarCampanhas(SuperMercado& sm);

void limparSupermercado(SuperMercado& sm);
void limparArvore(NoVenda* raiz);
void gravarVendasRec(NoVenda* raiz, char sectorID, ofstream& out);
Sector* encontrarSector(SuperMercado& sm, char id);
#endif