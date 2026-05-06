#ifndef SUPERMERCADO_H
#define SUPERMERCADO_H

#include <string>
#include "FilaArmazem.h" // Inclui a struct Produto e a implementação da Fila


#include <string>


struct NoString {
    std::string texto;
    NoString* prox;
};




// CORREÇÃO: ÁRVORE DE VENDAS (Sem duplicação)
// Apenas precisamos do nó. A struct "ArvoreVendas" desaparece.
struct NoVenda {
    int preco;
    std::string nome;
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
    std::string responsavel;
    std::string area;
    int capacidade;
    int ocupacao;

    NoProduto* inicioProdutos; // Ponteiro direto para os produtos na prateleira
    NoVenda* raizVendas;       // Ponteiro direto para as vendas (sem ArvoreVendas)

    Sector* prox;
};


struct Campanha {
    std::string area;
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
std::string obterElementoLista(NoString* cabeca, int indice);
void libertarStrings(NoString* cabeca);
bool existeString(NoString* cabeca, const std::string& str);
void adicionarStringFim(NoString*& cabeca, const std::string& str, int& contador);

// Funções para gerir os Produtos nos Setores (Atualizado sem a struct ListaProdutos)
void inicializarListaProdutos(NoProduto*& inicio);
void adicionarProdutoFim(NoProduto*& inicio, const Produto& p);

// PONTO 2 - INICIALIZAÇÃO E FUNCIONAMENTO

// Funções para gerir a Árvore de Vendas (Atualizado sem a struct ArvoreVendas)
void inicializarArvore(NoVenda*& raiz);
void inserirVenda(NoVenda*& raiz, int preco, const std::string& nome);

void inicializarSupermercado(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);


void gerarProdutosParaArmazem(SuperMercado& sm, int quantidade, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);

// Ciclo de simulação acionado pela tecla (s)
void simularCiclo(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec);
void venderProdutos(SuperMercado& sm);
void transferirArmazemParaSetores(SuperMercado& sm, int maxTransferir);

// Funções de Gestão (em falta a de gravar e carregar)
void removerProdutoGlobal(SuperMercado& sm, const std::string& nome);
void atualizarPrecoArmazem(SuperMercado& sm, const std::string& nome, int novoPreco);
void adicionarCampanha(SuperMercado& sm, const std::string& area, int percentagem, int duracao);
bool gravarSupermercado(const SuperMercado& sm, const std::string& filename);
bool carregarSupermercado(SuperMercado& sm, const std::string& filename);


void imprimirProdutos(const SuperMercado& sm);
void criarNovaArea(NoString*& areas, int& nAreas, const std::string& novaArea);
void mostrarRegistoVendas(const SuperMercado& sm, const std::string& responsavel);
#endif