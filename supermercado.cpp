#include "supermercado.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FilaArmazem.h"

using namespace std;


int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

NoString* carregarStrings(const string& filename, int& total) {
    ifstream file(filename);
    if (!file.is_open()) return nullptr;

    NoString* cabeca = nullptr;
    NoString* cauda = nullptr;
    total = 0;
    string linha;

    while (getline(file, linha)) {
        if (!linha.empty()) {
            NoString* novo = new NoString{linha, nullptr};
            if (!cabeca) { cabeca = cauda = novo; }
            else { cauda->prox = novo; cauda = novo; }
            total++;
        }
    }
    file.close();
    return cabeca;
}

string obterElementoLista(NoString* cabeca, int indice) {
    int cont = 0;
    while (cabeca != nullptr) {
        if (cont == indice) return cabeca->texto;
        cont++;
        cabeca = cabeca->prox;
    }
    return "";
}

void libertarStrings(NoString* cabeca) {
    while (cabeca != nullptr) {
        NoString* temp = cabeca;
        cabeca = cabeca->prox;
        delete temp;
    }
}

bool existeString(NoString* cabeca, const string& str) {
    while (cabeca != nullptr) {
        if (cabeca->texto == str) return true;
        cabeca = cabeca->prox;
    }
    return false;
}

void adicionarStringFim(NoString*& cabeca, const string& str, int& contador) {
    NoString* novo = new NoString{str, nullptr};
    if (!cabeca) { cabeca = novo; }
    else {
        NoString* atual = cabeca;
        while (atual->prox != nullptr) atual = atual->prox;
        atual->prox = novo;
    }
    contador++;
}


void inicializarListaProdutos(ListaProdutos& lista) {
    lista.inicio = lista.fim = nullptr;
}

void adicionarProdutoFim(ListaProdutos& lista, const Produto& p) {
    NoProduto* novo = new NoProduto{p, nullptr};
    if (lista.fim == nullptr) {
        lista.inicio = lista.fim = novo;
    } else {
        lista.fim->prox = novo;
        lista.fim = novo;
    }
}

void inicializarArvore(ArvoreVendas& arv) { arv.raiz = nullptr; }

void inserirVendaRec(NoVenda*& raiz, int preco, const string& nome) {
    if (raiz == nullptr) raiz = new NoVenda{preco, nome, nullptr, nullptr};
    else if (preco < raiz->preco) inserirVendaRec(raiz->esq, preco, nome);
    else inserirVendaRec(raiz->dir, preco, nome);
}

void inserirVenda(ArvoreVendas& arv, int preco, const string& nome) {
    inserirVendaRec(arv.raiz, preco, nome);
}

void gerarProdutosParaArmazem(ListaProdutos& armazem, int quantidade, NoString* areasSetores, int nAreasSet, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    for (int i = 0; i < quantidade; i++) {
        Produto p;
        p.nome = obterElementoLista(nomes, gerarAleatorio(0, nNomes - 1));
        p.fornecedor = obterElementoLista(fornecedores, gerarAleatorio(0, nFornec - 1));
        p.area = obterElementoLista(areasSetores, gerarAleatorio(0, nAreasSet - 1));
        p.preco = (gerarAleatorio(1, 40)) * 2; // Preço múltiplo de 2 (2 a 80)
        p.precoOriginal = p.preco;
        adicionarProdutoFim(armazem, p);
    }
}

void inicializarSupermercado(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    sm.setores.inicio = nullptr;
    Nova(sm.armazem); // Inicializa a Fila em vez da ListaProdutos

    int nSetores = gerarAleatorio(8, 12);
    NoString* areasAtivas = nullptr;
    int nAreasAtivas = 0;
    char id = 'A';

    for (int i = 0; i < nSetores; i++) {
        NoSetor* novoSetor = new NoSetor;
        novoSetor->dados.id = id++;
        novoSetor->dados.capacidade = gerarAleatorio(5, 10);
        novoSetor->dados.ocupacao = 0;
        novoSetor->dados.area = obterElementoLista(areas, gerarAleatorio(0, nAreas - 1));

        inicializarListaProdutos(novoSetor->dados.produtos);
        inicializarArvore(novoSetor->dados.vendas);

        cout << "Nome do responsavel pelo sector " << novoSetor->dados.id << " (" << novoSetor->dados.area << "): ";
        getline(cin, novoSetor->dados.responsavel);

        novoSetor->prox = sm.setores.inicio;
        sm.setores.inicio = novoSetor;

        if (!existeString(areasAtivas, novoSetor->dados.area)) {
            adicionarStringFim(areasAtivas, novoSetor->dados.area, nAreasAtivas);
        }
    }

    gerarProdutosParaArmazem(sm.armazem, 50, areasAtivas, nAreasAtivas, nomes, nNomes, fornecedores, nFornec);
    libertarStrings(areasAtivas);
}

void venderProdutos(SuperMercado& sm) {
    // Percorre todos os setores
    for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
        NoProduto* atual = ns->dados.produtos.inicio;
        NoProduto* anterior = nullptr;

        while (atual != nullptr) {
            // 25% de probabilidade de venda
            bool vendido = (gerarAleatorio(1, 100) <= 25);
            NoProduto* proximo = atual->prox;

            if (vendido) {
                inserirVenda(ns->dados.vendas, atual->info.preco, atual->info.nome);

                if (anterior == nullptr) {
                    ns->dados.produtos.inicio = proximo; // Remove do início
                } else {
                    anterior->prox = proximo; // Remove do meio/fim
                }

                if (atual == ns->dados.produtos.fim) {
                    ns->dados.produtos.fim = anterior;
                }

                delete atual;
                ns->dados.ocupacao--;
            } else {
                anterior = atual; // Não vendeu, avança o ponteiro anterior
            }
            atual = proximo; // Avança para o próximo produto
        }
    }
}

void transferirArmazemParaSetores(SuperMercado& sm, int maxTransferir) {
    int colocados = 0;
    int tamanhoArmazem = Comprimento(sm.armazem); // Vê quantos produtos existem

    // Só iteramos o número de produtos que estavam inicialmente no armazém
    for (int i = 0; i < tamanhoArmazem && colocados < maxTransferir; i++) {
        if (Vazia(sm.armazem)) break;

        Produto prod = Primeiro(sm.armazem); // 1. Espreita o primeiro produto
        Sai(sm.armazem);                     // 2. Tira o produto da fila

        NoSetor* setorAlvo = nullptr;

        // Procurar setor adequado
        for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
            if (ns->dados.area == prod.area && ns->dados.ocupacao < ns->dados.capacidade) {
                setorAlvo = ns;
                break;
            }
        }

        if (setorAlvo != nullptr) {
            // Coube no setor!
            adicionarProdutoFim(setorAlvo->dados.produtos, prod);
            setorAlvo->dados.ocupacao++;
            colocados++;
        } else {
            // CASO ESPECIAL: Não cabe / Setor cheio.
            // Volta a entrar no armazém (vai para o fim da fila)
            Entra(sm.armazem, prod);
        }
    }
}

void simularCiclo(SuperMercado& sm, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    venderProdutos(sm);

    // Identificar áreas ativas para garantir que os novos produtos pertencem a áreas válidas
    NoString* areasAtivas = nullptr;
    int nAreasAtivas = 0;
    for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
        if (!existeString(areasAtivas, ns->dados.area)) {
            adicionarStringFim(areasAtivas, ns->dados.area, nAreasAtivas);
        }
    }

    // Criação de 10 novos produtos aleatórios e adição ao armazém
    gerarProdutosParaArmazem(sm.armazem, 10, areasAtivas, nAreasAtivas, nomes, nNomes, fornecedores, nFornec);

    libertarStrings(areasAtivas); // Limpeza de memória

    // Transferência para os setores
    transferirArmazemParaSetores(sm, 10);
}


void gerarProdutosParaArmazem(FilaArmazem& armazem, int quantidade, NoString* areasSetores, int nAreasSet, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    for (int i = 0; i < quantidade; i++) {
        Produto p;
        p.nome = obterElementoLista(nomes, gerarAleatorio(0, nNomes - 1));
        p.fornecedor = obterElementoLista(fornecedores, gerarAleatorio(0, nFornec - 1));
        p.area = obterElementoLista(areasSetores, gerarAleatorio(0, nAreasSet - 1));
        p.preco = (gerarAleatorio(1, 40)) * 2;
        p.precoOriginal = p.preco;

        Entra(armazem, p); // Usar a função da Fila
    }
}

void imprimirProdutos(const SuperMercado& sm) {
    cout << "\n===== SUPERMERCADO =====\n";
    for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
        cout << "--- Sector:" << ns->dados.id
             << " | Responsavel: " << ns->dados.responsavel
             << " | Capacidade: " << ns->dados.capacidade
             << " | Produtos: " << ns->dados.ocupacao
             << " | Area: " << ns->dados.area << " ---\n";
        for (NoProduto* p = ns->dados.produtos.inicio; p != nullptr; p = p->prox)
            cout << "Produto: " << p->info.nome << " | Preco: " << p->info.preco << " Euros\n";
    }

    cout << "\n--- Armazem ---\n";
    // TRUQUE DA FILA: Rotação para imprimir sem perder dados
    SuperMercado& sm_mut = const_cast<SuperMercado&>(sm); // Permite mexer na fila localmente
    int total = Comprimento(sm_mut.armazem);

    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm_mut.armazem);
        Sai(sm_mut.armazem);

        cout << "Produto: " << p.nome << " | Preco: " << p.preco << " Euros | Area: " << p.area << "\n";

        Entra(sm_mut.armazem, p); // Volta a guardar
    }
}

void removerProdutoGlobal(SuperMercado& sm, const string& nome) {
    // 1. Remover dos setores (Mantém-se a lógica de lista ligada)
    for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
        NoProduto* atual = ns->dados.produtos.inicio;
        NoProduto* anterior = nullptr;
        int cont = 0;

        while (atual != nullptr) {
            if (atual->info.nome == nome) {
                NoProduto* apagar = atual;
                if (anterior) anterior->prox = atual->prox;
                else ns->dados.produtos.inicio = atual->prox;

                if (apagar == ns->dados.produtos.fim) ns->dados.produtos.fim = anterior;

                atual = atual->prox;
                delete apagar;
            } else {
                anterior = atual;
                atual = atual->prox;
                cont++;
            }
        }
        ns->dados.ocupacao = cont;
    }

    // 2. Remover do Armazém usando as regras da FILA
    int total = Comprimento(sm.armazem);
    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem); // Tira da fila

        // Só volta a entrar se NÃO for para apagar
        if (p.nome != nome) {
            Entra(sm.armazem, p);
        }
    }
}

void atualizarPrecoArmazem(SuperMercado& sm, const string& nome, int novoPreco) {
    int total = Comprimento(sm.armazem);

    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);

        if (p.nome == nome) {
            p.preco = novoPreco;
            p.precoOriginal = novoPreco;
        }

        Entra(sm.armazem, p); // Volta para a fila (atualizado ou não)
    }
}

void adicionarCampanha(SuperMercado& sm, const string& area, int percentagem, int duracao) {
    Campanha* nova = new Campanha{area, percentagem, duracao, sm.campanhas};
    sm.campanhas = nova;

    // Aplicar aos setores
    for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
        if (ns->dados.area == area) {
            for (NoProduto* p = ns->dados.produtos.inicio; p != nullptr; p = p->prox) {
                p->info.preco = p->info.precoOriginal * (100 - percentagem) / 100;
            }
        }
    }

    // Aplicar ao armazém usando as regras da FILA
    int total = Comprimento(sm.armazem);
    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);

        if (p.area == area) {
            p.preco = p.precoOriginal * (100 - percentagem) / 100;
        }

        Entra(sm.armazem, p);
    }
}

void criarNovaArea(NoString*& areas, int& nAreas, const string& novaArea) {
    adicionarStringFim(areas, novaArea, nAreas);
}

// Funções auxiliares para mostrar registo de vendas
void percorrerInOrder(NoVenda* no) {
    if (no == nullptr) return;
    percorrerInOrder(no->esq);
    cout << "   " << no->nome << " - " << no->preco << " Euros\n";
    percorrerInOrder(no->dir);
}

void mostrarRegistoVendas(const SuperMercado& sm, const string& responsavel) {
    bool encontrou = false;
    for (NoSetor* ns = sm.setores.inicio; ns != nullptr; ns = ns->prox) {
        if (ns->dados.responsavel == responsavel) {
            cout << "\nSector " << ns->dados.id << " (" << ns->dados.area << "):\n";
            percorrerInOrder(ns->dados.vendas.raiz);
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum sector encontrado com esse responsavel.\n";
}

// Stubs (Implementações base) para o Gravar/Carregar não bloquearem o código
bool gravarSupermercado(const SuperMercado& sm, const string& filename) {
    cout << "A gravar estado para " << filename << "..." << endl;
    return true;
}

bool carregarSupermercado(SuperMercado& sm, const string& filename) {
    cout << "A carregar estado de " << filename << "..." << endl;
    return false; // Retorna false para simular um "novo jogo" por padrão
}