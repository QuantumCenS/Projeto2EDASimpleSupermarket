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

void inicializarSupermercado(SuperMercado& sm, NoString* areas, int nAreas, NoString*& areasAtivas, int& nAreasAtivas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {
    sm.inicioSectores = nullptr;
    Nova(sm.armazem);

    int nSetores = gerarAleatorio(8, 12);
    char id = 'A';

    for (int i = 0; i < nSetores; i++) {
        Sector* novoSetor = new Sector;
        novoSetor->id = id++;
        novoSetor->capacidade = gerarAleatorio(5, 10);
        novoSetor->ocupacao = 0;
        novoSetor->area = obterElementoLista(areas, gerarAleatorio(0, nAreas - 1));

        inicializarListaProdutos(novoSetor->inicioProdutos);
        inicializarArvore(novoSetor->raizVendas);

        // Ciclo insiste ate que o utilizador escreva um nome valido
        do {
            cout << "Nome do responsavel pelo sector " << novoSetor->id << " (" << novoSetor->area << "): ";
            getline(cin, novoSetor->responsavel);

            if (novoSetor->responsavel.empty()) {
                cout << "[ERRO] O nome do responsavel e obrigatorio!\n";
            }
        } while (novoSetor->responsavel.empty());

        // Inserir no final da lista ligada de sectores para manter a ordem ID (A, B, C...)
        novoSetor->prox = nullptr;
        if (sm.inicioSectores == nullptr) {
            sm.inicioSectores = novoSetor;
        } else {
            Sector* aux = sm.inicioSectores;
            while (aux->prox != nullptr) {
                aux = aux->prox;
            }
            aux->prox = novoSetor;
        }

        // Regista esta área como "Ativa" no sistema
        if (!existeString(areasAtivas, novoSetor->area)) {
            adicionarStringFim(areasAtivas, novoSetor->area, nAreasAtivas);
        }
    }

    // Gera os 50 produtos iniciais usando APENAS as áreas ativas!
    gerarProdutosParaArmazem(sm, 50, areasAtivas, nAreasAtivas, nomes, nNomes, fornecedores, nFornec);

    // NOTA: O libertarStrings(areasAtivas) foi removido daqui porque o Menu vai precisar da lista!
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
        int maxEspacoLivre = 0; // Vai garantir que distribuímos bem o stock

        for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
            if (s->area == prod.area) {
                int espacoLivre = s->capacidade - s->ocupacao;

                // Se este sector tem mais espaço livre que o anterior, passa a ser o alvo
                if (espacoLivre > maxEspacoLivre) {
                    maxEspacoLivre = espacoLivre;
                    setorAlvo = s;
                }
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
        atual->duracao--;
        if (atual->duracao <= 0) {
            cout << "\n[CAMPANHA] A campanha na area '" << atual->area << "' terminou!\n";
            // Restaura prateleiras
            for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
                if (s->area == atual->area) {
                    for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
                        p->info.preco = p->info.precoOriginal;
                    }
                }
            }
            // Restaura armazem
            int total = Comprimento(sm.armazem);
            for (int i = 0; i < total; i++) {
                Produto p = Primeiro(sm.armazem);
                Sai(sm.armazem);
                if (p.area == atual->area) p.preco = p.precoOriginal;
                Entra(sm.armazem, p);
            }
            // Remove nó da lista
            Campanha* apagar = atual;
            if (anterior == nullptr) sm.campanhas = atual->prox;
            else anterior->prox = atual->prox;

            atual = atual->prox;
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




int removerProdutoGlobal(SuperMercado& sm, const string& nome) {
    // 1. DESCOBRIR A ÁREA DO PRODUTO (antes de apagar qualquer coisa)
    string areaDoProduto = "";

    // Procurar nos sectores
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
            if (p->info.nome == nome) {
                areaDoProduto = p->info.area;
                break;
            }
        }
        if (!areaDoProduto.empty()) break;
    }

    // Se não encontrou nas prateleiras, procurar no armazém
    if (areaDoProduto.empty()) {
        int total = Comprimento(sm.armazem);
        for (int i = 0; i < total; i++) {
            Produto p = Primeiro(sm.armazem);
            Sai(sm.armazem);
            if (p.nome == nome && areaDoProduto.empty()) {
                areaDoProduto = p.area;
            }
            Entra(sm.armazem, p);
        }
    }

    // Se a área continua vazia, é porque o produto não existe na loja
    if (areaDoProduto.empty()) return 0; // 0 = Não encontrado

    // 2. VERIFICAR SE A ÁREA TEM UMA CAMPANHA ATIVA
    for (Campanha* c = sm.campanhas; c != nullptr; c = c->prox) {
        if (c->area == areaDoProduto) {
            return -1; // -1 = Bloqueado (Em Campanha!)
        }
    }

    // =========================================================
    // 3. REMOÇÃO EFETIVA (Se passou a validação da campanha)
    // =========================================================

    // Remover nas prateleiras
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        NoProduto* atual = s->inicioProdutos;
        NoProduto* anterior = nullptr;
        int cont = 0;

        while (atual != nullptr) {
            if (atual->info.nome == nome) {
                NoProduto* apagar = atual;
                if (anterior) anterior->prox = atual->prox;
                else s->inicioProdutos = atual->prox;

                atual = atual->prox;

                // Guarda no histórico para o UNDO
                pushHistorico(sm, apagar->info);
                delete apagar;
            } else {
                anterior = atual;
                atual = atual->prox;
                cont++;
            }
        }
        s->ocupacao = cont;
    }

    // Remover no armazém
    int total = Comprimento(sm.armazem);
    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(sm.armazem);
        Sai(sm.armazem);

        if (p.nome != nome) {
            Entra(sm.armazem, p);
        } else {
            // Guarda no histórico para o UNDO
            pushHistorico(sm, p);
        }
    }

    return 1; // 1 = Removido com Sucesso
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

bool carregarSupermercado(SuperMercado& sm, const string& filename, NoString*& areas, int& nAreas) {
    ifstream in(filename);
    if (!in.is_open()) return false;

    limparSupermercado(sm);

    string linha;
    while (getline(in, linha)) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        string etiqueta;
        getline(ss, etiqueta, '|');

        if (etiqueta == "SECTOR") {
            Sector* novo = new Sector;
            string idStr, capStr;

            getline(ss, idStr, '|');
            getline(ss, capStr, '|');
            getline(ss, novo->area, '|');
            getline(ss, novo->responsavel, '|');

            novo->id = idStr[0];
            novo->capacidade = stoi(capStr);
            novo->ocupacao = 0;
            novo->inicioProdutos = nullptr;
            novo->raizVendas = nullptr;
            novo->prox = nullptr; // Ficará no fim da lista

            // --- CORREÇÃO DA ORDEM (Adiciona ao FIM da lista) ---
            if (sm.inicioSectores == nullptr) {
                sm.inicioSectores = novo;
            } else {
                Sector* aux = sm.inicioSectores;
                while (aux->prox != nullptr) {
                    aux = aux->prox;
                }
                aux->prox = novo;
            }

            // --- SINCRONIZAÇÃO DA ÁREA ---
            if (!existeString(areas, novo->area)) {
                adicionarStringFim(areas, novo->area, nAreas);
            }

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

                adicionarProdutoFim(s->inicioProdutos, p);
                s->ocupacao++;

                if (!existeString(areas, p.area)) adicionarStringFim(areas, p.area, nAreas);
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

            if (!existeString(areas, p.area)) adicionarStringFim(areas, p.area, nAreas);

        } else if (etiqueta == "VENDA") {
            string idSecStr, precoStr, nomeProd;
            getline(ss, idSecStr, '|');
            getline(ss, nomeProd, '|');
            getline(ss, precoStr, '|');

            Sector* s = encontrarSector(sm, idSecStr[0]);
            if (s) inserirVendaRec(s->raizVendas, stoi(precoStr), nomeProd);

        } else if (etiqueta == "CAMP") {
            string area, percStr, durStr;
            getline(ss, area, '|');
            getline(ss, percStr, '|');
            getline(ss, durStr, '|');

            Campanha* nova = new Campanha{area, stoi(percStr), stoi(durStr), sm.campanhas};
            sm.campanhas = nova;

            if (!existeString(areas, area)) adicionarStringFim(areas, area, nAreas);
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

    // Limpar Pilha de Historico
    NoHistorico* hAtual = sm.topoHistorico;
    while (hAtual != nullptr) {
        NoHistorico* auxH = hAtual;
        hAtual = hAtual->prox;
        delete auxH;
    }
    sm.topoHistorico = nullptr;
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

int somarVendasRec(NoVenda* raiz) {
    if (raiz == nullptr) return 0;
    return raiz->preco + somarVendasRec(raiz->esq) + somarVendasRec(raiz->dir);
}

void relatorioFaturacaoGlobal(const SuperMercado& sm) {
    int totalGlobal = 0;
    cout << "\n========== FATURACAO GLOBAL ==========\n";
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        int totalSector = somarVendasRec(s->raizVendas);
        cout << " Sector " << s->id << " (" << s->area << "): " << totalSector << " EUR\n";
        totalGlobal += totalSector;
    }
    cout << "--------------------------------------\n";
    cout << " TOTAL DA LOJA: " << totalGlobal << " EUR\n";
    cout << "======================================\n";
}

bool fecharSector(SuperMercado& sm, char idSector) {
    Sector* atual = sm.inicioSectores;
    Sector* anterior = nullptr;

    while (atual != nullptr) {
        if (atual->id == idSector) {
            // 1. Mover produtos das prateleiras de volta para o armazem
            NoProduto* pAtual = atual->inicioProdutos;
            while (pAtual != nullptr) {
                Entra(sm.armazem, pAtual->info);
                NoProduto* apagar = pAtual;
                pAtual = pAtual->prox;
                delete apagar;
            }

            // 2. Apagar a árvore de vendas
            limparArvore(atual->raizVendas);

            // 3. Remover Sector da lista ligada
            if (anterior == nullptr) sm.inicioSectores = atual->prox;
            else anterior->prox = atual->prox;

            delete atual;
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return false;
}


void pesquisarProduto(const SuperMercado& sm, const string& nomeProd) {
    bool encontrou = false;
    cout << "\nResultados para '" << nomeProd << "':\n";

    // Procurar nas prateleiras (Sectores)
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        for (NoProduto* p = s->inicioProdutos; p != nullptr; p = p->prox) {
            if (p->info.nome == nomeProd) {
                cout << " -> Na Prateleira: Sector " << s->id << " (" << p->info.preco << " EUR)\n";
                encontrou = true;
            }
        }
    }

    // Procurar no armazém (Percorrer a Fila)
    int qtdArmazem = 0;
    SuperMercado& smMut = const_cast<SuperMercado&>(sm);
    int total = Comprimento(smMut.armazem);

    for (int i = 0; i < total; i++) {
        Produto p = Primeiro(smMut.armazem);
        Sai(smMut.armazem);
        if (p.nome == nomeProd) qtdArmazem++;
        Entra(smMut.armazem, p);
    }

    if (qtdArmazem > 0) {
        cout << " -> No Armazem: " << qtdArmazem << " unidade(s) a aguardar reposicao.\n";
        encontrou = true;
    }

    if (!encontrou) cout << " -> Produto nao encontrado em lado nenhum.\n";
}

void pushHistorico(SuperMercado& sm, const Produto& p) {
    // Alocação dinâmica do novo nó
    NoHistorico* novo = new NoHistorico;
    novo->info = p;
    novo->prox = sm.topoHistorico; // O próximo aponta para o antigo topo

    sm.topoHistorico = novo; // O topo passa a ser este novo nó
}


// Desempilha o último produto removido e volta a colocá-lo no armazém (Pop / Undo)
bool desfazerUltimaRemocao(SuperMercado& sm) {
    // Se o histórico estiver vazio, não há nada a desfazer
    if (sm.topoHistorico == nullptr) {
        return false;
    }

    // Guarda os dados do nó que está no topo
    NoHistorico* apagar = sm.topoHistorico;
    Produto pRecuperado = apagar->info;

    // O topo avança para o elemento seguinte na pilha
    sm.topoHistorico = apagar->prox;

    // Liberta a memória do nó do histórico
    delete apagar;

    // Devolve o produto recuperado à fila do armazém
    Entra(sm.armazem, pRecuperado);

    cout << "[UNDO] O produto '" << pRecuperado.nome << "' foi recuperado para o armazem!\n";
    return true;
}

void adicionarNovoSector(SuperMercado& sm, const string& area, const string& resp, int cap) {
    // Criar o novo nó
    Sector* novo = new Sector;

    // Calcular o próximo ID (Letras A, B, C...)
    char ultimoID = 64; // 64 é o caractere imediatamente antes do 'A' na tabela ASCII
    for (Sector* s = sm.inicioSectores; s != nullptr; s = s->prox) {
        if (s->id > ultimoID) {
            ultimoID = s->id;
        }
    }

    if (ultimoID == 64) {
        novo->id = 'A';
    } else {
        novo->id = ultimoID + 1;
    }

    // Preencher os dados
    novo->area = area;
    novo->responsavel = resp;
    novo->capacidade = cap;
    novo->ocupacao = 0;
    novo->inicioProdutos = nullptr;
    novo->raizVendas = nullptr;

    // Inserir à cabeça da lista ligada de sectores
    novo->prox = nullptr;
    if (sm.inicioSectores == nullptr) {
        sm.inicioSectores = novo;
    } else {
        Sector* aux = sm.inicioSectores;
        while (aux->prox != nullptr) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}
