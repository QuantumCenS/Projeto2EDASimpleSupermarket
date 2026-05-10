#include "supermercado.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FilaArmazem.h"
#include <iomanip>
#include <sstream>
#include <string>

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




void inicializarListaProdutos(NoProduto*& inicio) {
    inicio = nullptr;
}

void adicionarProdutoFim(NoProduto*& inicio, const Produto& p) {
    NoProduto* novo = new NoProduto{p, nullptr};
    if (inicio == nullptr) {
        inicio = novo;
    } else {
        NoProduto* atual = inicio;
        while (atual->prox != nullptr) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
}

void inicializarArvore(NoVenda*& raiz) {
    raiz = nullptr;
}

void inserirVendaRec(NoVenda*& raiz, int preco, const string& nome) {
    if (raiz == nullptr) raiz = new NoVenda{preco, nome, nullptr, nullptr};
    else if (preco < raiz->preco) inserirVendaRec(raiz->esq, preco, nome);
    else inserirVendaRec(raiz->dir, preco, nome);
}





void gerarProdutosParaArmazem(SuperMercado& sm, int quantidade, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    // Proteção: se a lista de áreas estiver vazia, não gera produtos
    if (nAreas <= 0 || areas == nullptr) return;

    for (int i = 0; i < quantidade; i++) {
        Produto p;
        // Seleciona Nome e Fornecedor aleatórios
        p.nome = obterElementoLista(nomes, gerarAleatorio(0, nNomes - 1));
        p.fornecedor = obterElementoLista(fornecedores, gerarAleatorio(0, nFornec - 1));

        // Como a 'interfaceCriarArea' adiciona as novas áreas a esta lista,
        // elas passarão a ser selecionadas aqui.
        p.area = obterElementoLista(areas, gerarAleatorio(0, nAreas - 1));

        p.preco = (gerarAleatorio(1, 40)) * 2;
        p.precoOriginal = p.preco;

        // Verifica se existe uma campanha ativa para esta área (mesmo sendo nova)
        for (Campanha* c = sm.campanhas; c != nullptr; c = c->prox) {
            if (c->area == p.area) {
                p.preco = p.precoOriginal * (100 - c->percentagem) / 100;
                break;
            }
        }

        Entra(sm.armazem, p);
    }
}

void inicializarSupermercado(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    sm.inicioSectores = nullptr;
    Nova(sm.armazem);

    int nSetores = gerarAleatorio(8, 12);
    NoString* areasAtivas = nullptr;
    int nAreasAtivas = 0;
    char id = 'A';

    for (int i = 0; i < nSetores; i++) {
        Sector* novoSetor = new Sector; // Substitui o NoSetor do Sérgio
        novoSetor->id = id++;
        novoSetor->capacidade = gerarAleatorio(5, 10);
        novoSetor->ocupacao = 0;
        novoSetor->area = obterElementoLista(areas, gerarAleatorio(0, nAreas - 1));

        inicializarListaProdutos(novoSetor->inicioProdutos);
        inicializarArvore(novoSetor->raizVendas);

        // Ciclo insiste ate que o utilizador escreva um nome valido
        do {
            cout << "Nome do responsavel pelo sector " << novoSetor->id << " (" << novoSetor->area << "): ";
            getline(cin, novoSetor->responsavel); // Removido o '>> ws'

            if (novoSetor->responsavel.empty()) {
                cout << "[ERRO] O nome do responsavel e obrigatorio!\n";
            }
        } while (novoSetor->responsavel.empty());

        novoSetor->prox = sm.inicioSectores;
        sm.inicioSectores = novoSetor;

        if (!existeString(areasAtivas, novoSetor->area)) {
            adicionarStringFim(areasAtivas, novoSetor->area, nAreasAtivas);
        }
    }

    gerarProdutosParaArmazem(sm, 50, areasAtivas, nAreasAtivas, nomes, nNomes, fornecedores, nFornec);
    libertarStrings(areasAtivas);
}

void venderProdutos(SuperMercado& sm) {
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        NoProduto* atual = s->inicioProdutos;
        NoProduto* anterior = nullptr;

        while (atual != nullptr) {
            bool vendido = (gerarAleatorio(1, 100) <= 25);
            NoProduto* proximo = atual->prox;

            if (vendido) {
                inserirVendaRec(s->raizVendas, atual->info.preco, atual->info.nome);

                if (anterior == nullptr) {
                    s->inicioProdutos = proximo;
                } else {
                    anterior->prox = proximo;
                }

                delete atual;
                s->ocupacao--;
            } else {
                anterior = atual;
            }
            atual = proximo;
        }
    }
}

void transferirArmazemParaSetores(SuperMercado& sm, int maxTransferir) {
    int colocados = 0;
    int tamanhoArmazem = Comprimento(sm.armazem);

    for (int i = 0; i < tamanhoArmazem && colocados < maxTransferir; i++) {
        if (Vazia(sm.armazem)) break;

        Produto prod = Primeiro(sm.armazem);
        Sai(sm.armazem);

        Sector* setorAlvo = nullptr;

        for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
            if (s->area == prod.area && s->ocupacao < s->capacidade) {
                setorAlvo = s;
                break;
            }
        }

        if (setorAlvo != nullptr) {
            adicionarProdutoFim(setorAlvo->inicioProdutos, prod);
            setorAlvo->ocupacao++;
            colocados++;
        } else {
            Entra(sm.armazem, prod);
        }
    }
}

void atualizarCampanhas(SuperMercado& sm) {
    Campanha* atual = sm.campanhas;
    Campanha* anterior = nullptr;

    while (atual != nullptr) {
        atual->duracao--; // Passou um ciclo! Decrementa a duracao.

        // Se a duracao chegou ao fim, a campanha expira
        if (atual->duracao <= 0) {
            string areaExpirada = atual->area;
            cout << "\n[CAMPANHA] A campanha na area '" << areaExpirada << "' terminou! Precos originais restaurados.\n";

            // Restaurar precos nos Sectores
            for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
                if (s->area == areaExpirada) {
                    for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
                        p->info.preco = p->info.precoOriginal;
                    }
                }
            }

            // Restaurar precos no Armazem
            int total = Comprimento(sm.armazem);
            for (int i = 0; i < total; i++) {
                Produto p = Primeiro(sm.armazem);
                Sai(sm.armazem);

                if (p.area == areaExpirada) {
                    p.preco = p.precoOriginal;
                }

                Entra(sm.armazem, p);
            }

            // Remover a campanha da lista ligada com seguranca
            Campanha* apagar = atual;
            if (anterior == nullptr) {
                sm.campanhas = atual->prox;
                atual = sm.campanhas;
            } else {
                anterior->prox = atual->prox;
                atual = atual->prox;
            }
            delete apagar;

        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

// Alterei a assinatura para receber o 'areas' e 'nAreas' globais
void simularCiclo(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {

    atualizarCampanhas(sm);

    // Tentar vender os produtos que estão nas prateleiras
    venderProdutos(sm);

    // Gerar 10 produtos usando todas as áreas do sistema (incluindo as novas que possam ter sido criadas)
    gerarProdutosParaArmazem(sm, 10, areas, nAreas, nomes, nNomes, fornecedores, nFornec);

    // Tentar passar produtos do armazém para os setores
    transferirArmazemParaSetores(sm, 10);
}




bool removerProdutoGlobal(SuperMercado& sm, const string& nome) {
    bool encontrou=false;
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        NoProduto* atual = s->inicioProdutos;
        NoProduto* anterior = nullptr;
        int cont = 0;

        while (atual != nullptr) {
            if (atual->info.nome == nome) {
                encontrou=true;
                NoProduto* apagar = atual;
                if (anterior) anterior->prox = atual->prox;
                else s->inicioProdutos = atual->prox;

                atual = atual->prox;
                delete apagar;
            } else {
                anterior = atual;
                atual = atual->prox;
                cont++;
            }
        }
        s->ocupacao = cont;
    }

    int total = Comprimento(sm.armazem);
    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);

        if (p.nome != nome) {
            Entra(sm.armazem, p);
        }
        else {
            encontrou=true;
        }
    }
    return encontrou;
}

bool atualizarPrecoArmazem(SuperMercado& sm, const string& nome, int novoPreco) {
    bool atualizou=false;
    int total = Comprimento(sm.armazem);

    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);

        if (p.nome == nome) {
            p.preco = novoPreco;
            p.precoOriginal = novoPreco;
            atualizou=true;
        }
        Entra(sm.armazem, p);
    }

    return atualizou;
}

bool adicionarCampanha(SuperMercado& sm, const string& area, int percentagem, int duracao) {
    bool aplicouDesconto = false;

    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        if (s->area == area) {
            for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
                p->info.preco = p->info.precoOriginal * (100 - percentagem) / 100;
                aplicouDesconto = true;
            }
        }
    }

    int total = Comprimento(sm.armazem);
    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);

        if (p.area == area) {
            p.preco = p.precoOriginal * (100 - percentagem) / 100;
            aplicouDesconto = true;
        }

        Entra(sm.armazem, p);
    }
    if (aplicouDesconto) {
        Campanha* nova = new Campanha{area, percentagem, duracao, sm.campanhas};
        sm.campanhas = nova;
        return true;
    }
    return false;
}



void imprimirProdutos(const SuperMercado& sm) {
    // 1. Percorrer Sectores e Produtos
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        // Formatação exata do cabeçalho do sector (sem espaço depois de Sector:)
        cout << "Sector:" << s->id
             << " | Responsavel: " << s->responsavel
             << " | Capacidade: " << s->capacidade
             << " | Produtos: " << s->ocupacao
             << " | Area: " << s->area << "\n";

        for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
            // Formatação exata do produto (com espaço antes dos dois pontos no Preço)
            cout << "Produto: " << p->info.nome << " | Preco : " << p->info.preco << " Euros\n";
        }
        // Exatamente 50 traços de separador
        cout << "--------------------------------------------------\n";
    }

    // 2. Percorrer Armazém
    cout << "Armazem:\n";

    // Cabeçalho da Tabela
    cout << left << setw(30) << " PRODUTO"
         << " | " << setw(25) << "AREA"
         << " | " << "PRECO" << "\n";
    cout << "-------------------------------|---------------------------|------------------\n";

    SuperMercado& sm_mut = const_cast<SuperMercado&>(sm);
    int total = Comprimento(sm_mut.armazem);

    if (total == 0) {
        cout << "                         ( Armazem sem stock )                         \n";
    } else {
        for (int i = 0; i < total; i++) {
            Produto p = Primeiro(sm_mut.armazem);
            Sai(sm_mut.armazem);

            // Imprime apenas Nome, Área e Preço[cite: 1]
            cout << " " << left << setw(29) << p.nome
                 << " | " << setw(25) << p.area
                 << " | " << right << setw(3) << p.preco << " EUR\n";

            Entra(sm_mut.armazem, p);
        }
    }
    cout << "===============================================================================\n\n";
}
bool criarNovaArea(NoString*& areas, int& nAreas, const string& novaArea) {
    for (NoString* atual = areas; atual != nullptr; atual = atual->prox) {
        if (atual->texto == novaArea) {
            return false; // A área já existe! Aborta.
        }
    }

    adicionarStringFim(areas, novaArea, nAreas);
    return true;
}

void percorrerInOrder(NoVenda* no) {
    if (no == nullptr) return;
    percorrerInOrder(no->esq);
    cout << "Produto: " << no->nome << " | Preco: " << no->preco << " Euros\n";
    percorrerInOrder(no->dir);
}

bool mostrarRegistoVendas(const SuperMercado& sm, const string& responsavel) {
    bool encontrou = false;

    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        if (s->responsavel == responsavel) {
            cout << "\nSector " << s->id << " (" << s->area << "):\n";

            if (s->raizVendas == nullptr) {
                cout << "   -> Ainda nao foram registadas vendas neste sector.\n";
            } else {
                percorrerInOrder(s->raizVendas);
            }
            encontrou = true;
        }
    }

    return encontrou;
}



bool gravarSupermercado(SuperMercado& sm, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) return false;

    // A. Gravar Sectores e os seus Produtos
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        out << "SECTOR|" << s->id << "|" << s->capacidade << "|" << s->area << "|" << s->responsavel << "\n";

        for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
            out << "P_SEC|" << s->id << "|" << p->info.nome << "|" << p->info.area << "|"
                << p->info.fornecedor << "|" << p->info.preco << "|" << p->info.precoOriginal << "\n";
        }

        // B. Gravar Vendas (Árvore) - Usamos uma função auxiliar
        gravarVendasRec(s->raizVendas, s->id, out);
    }

    // C. Gravar Armazém (Dando a volta à "Roda Gigante")
    int total = Comprimento(sm.armazem);
    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);
        out << "P_ARM|" << p.nome << "|" << p.area << "|" << p.fornecedor << "|" << p.preco << "|" << p.precoOriginal << "\n";
        Entra(sm.armazem, p);
    }

    // D. Gravar Campanhas
    for (Campanha* c = sm.campanhas; c != nullptr; c = c->prox) {
        out << "CAMP|" << c->area << "|" << c->percentagem << "|" << c->duracao << "\n";
    }

    out.close();
    return true;
}

bool carregarSupermercado(SuperMercado& sm, const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return false;

    // 1. LIMPAR A MEMÓRIA ATUAL
    // Muito importante para não misturar dados de sessões diferentes!
    limparSupermercado(sm);

    string linha;
    while (getline(in, linha)) {
        if (linha.empty()) continue; // Ignora linhas em branco

        stringstream ss(linha);
        string etiqueta;
        getline(ss, etiqueta, '|'); // Lemos a primeira palavra até ao '|'

        if (etiqueta == "SECTOR") {
            Sector* novo = new Sector;
            string idStr, capStr;

            getline(ss, idStr, '|');
            getline(ss, capStr, '|');
            getline(ss, novo->area, '|');
            getline(ss, novo->responsavel, '|');

            novo->id = idStr[0];
            novo->capacidade = stoi(capStr); // Converte string para int
            novo->ocupacao = 0;             // Será somado à medida que carregamos produtos
            novo->inicioProdutos = nullptr;
            novo->raizVendas = nullptr;

            // Adicionar à lista de sectores
            novo->prox = sm.inicioSectores;
            sm.inicioSectores = novo;

        } else if (etiqueta == "P_SEC") {
            string idSecStr, precoStr, precoOrigStr;
            getline(ss, idSecStr, '|');

            Sector* s = encontrarSector(sm, idSecStr[0]);
            if (s) {
                Produto p;
                getline(ss, p.nome, '|');
                getline(ss, p.area, '|');
                getline(ss, p.fornecedor, '|');
                getline(ss, precoStr, '|');
                getline(ss, precoOrigStr, '|');

                p.preco = stoi(precoStr);
                p.precoOriginal = stoi(precoOrigStr);

                // Usamos a tua função que já tinhas feito!
                adicionarProdutoFim(s->inicioProdutos, p);
                s->ocupacao++;
            }

        } else if (etiqueta == "P_ARM") {
            Produto p;
            string precoStr, precoOrigStr;
            getline(ss, p.nome, '|');
            getline(ss, p.area, '|');
            getline(ss, p.fornecedor, '|');
            getline(ss, precoStr, '|');
            getline(ss, precoOrigStr, '|');

            p.preco = stoi(precoStr);
            p.precoOriginal = stoi(precoOrigStr);

            Entra(sm.armazem, p);

        } else if (etiqueta == "VENDA") {
            string idSecStr, precoStr, nomeProd;
            getline(ss, idSecStr, '|');
            getline(ss, nomeProd, '|');
            getline(ss, precoStr, '|');

            Sector* s = encontrarSector(sm, idSecStr[0]);
            if (s) {
                // Usamos a tua função recursiva de inserção na árvore!
                inserirVendaRec(s->raizVendas, stoi(precoStr), nomeProd);
            }

        } else if (etiqueta == "CAMP") {
            string area, percStr, durStr;
            getline(ss, area, '|');
            getline(ss, percStr, '|');
            getline(ss, durStr, '|');

            Campanha* nova = new Campanha{area, stoi(percStr), stoi(durStr), sm.campanhas};
            sm.campanhas = nova;
        }
    }

    in.close();
    return true;
}

void limparSupermercado(SuperMercado& sm) {
    // 1. Limpar Sectores e os Produtos dentro deles
    Sector* sAtual = sm.inicioSectores;
    while (sAtual != nullptr) {
        // Limpar a lista de produtos deste sector
        NoProduto* pAtual = sAtual->inicioProdutos;
        while (pAtual != nullptr) {
            NoProduto* auxP = pAtual;
            pAtual = pAtual->prox;
            delete auxP;
        }

        // Limpar a Árvore de Vendas deste sector (precisas de uma função recursiva)
        limparArvore(sAtual->raizVendas);

        Sector* auxS = sAtual;
        sAtual = sAtual->prox;
        delete auxS;
    }
    sm.inicioSectores = nullptr;

    // 2. Limpar o Armazém (Fila)
    while (!Vazia(sm.armazem)) {
        Sai(sm.armazem); // O teu Sai já deve fazer o delete do nó
    }

    // 3. Limpar Campanhas
    Campanha* cAtual = sm.campanhas;
    while (cAtual != nullptr) {
        Campanha* auxC = cAtual;
        cAtual = cAtual->prox;
        delete auxC;
    }
    sm.campanhas = nullptr;
}

// Função auxiliar para a árvore
void limparArvore(NoVenda* raiz) {
    if (raiz == nullptr) return;
    limparArvore(raiz->esq);
    limparArvore(raiz->dir);
    delete raiz;
}

void gravarVendasRec(NoVenda* raiz, char sectorID, ofstream& out) {
    if (raiz == nullptr) return;
    out << "VENDA|" << sectorID << "|" << raiz->nome << "|" << raiz->preco << "\n";
    gravarVendasRec(raiz->esq, sectorID, out);
    gravarVendasRec(raiz->dir, sectorID, out);
}

Sector* encontrarSector(SuperMercado& sm, char id) {
    Sector* atual = sm.inicioSectores;
    while (atual != nullptr) {
        if (atual->id == id) return atual;
        atual = atual->prox;
    }
    return nullptr;
}




