#include "supermercado.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FilaArmazem.h"
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;


/**
 * gera um numero inteiro aleatorio dentro de um intervalo especifico
 * @param min valor minimo do intervalo (inclusivo)
 * @param max valor maximo do intervalo (inclusivo)
 * @return numero inteiro aleatorio gerado entre min e max
 */
int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}


/**
 * carrega os dados de um ficheiro de texto e guarda-os numa lista ligada
 * @param filename referencia constante para o nome do ficheiro a ler (ex: "areas.txt")
 * @param total referencia para a variavel que vai guardar o numero total de elementos lidos
 * @return ponteiro para o primeiro no (cabeca) da lista ligada gerada
 */
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


/**
 * obtem o texto de um no especifico da lista ligada com base no seu indice
 * @param cabeca ponteiro para o primeiro no da lista ligada
 * @param indice posicao (comecando em 0) do elemento que pretendemos obter
 * @return texto guardado nessa posicao, ou uma string vazia se o indice nao existir na lista
 */
string obterElementoLista(NoString* cabeca, int indice) {
    int cont = 0;
    while (cabeca != nullptr) {
        if (cont == indice) return cabeca->texto;
        cont++;
        cabeca = cabeca->prox;
    }
    return "";
}


/**
 * liberta a memoria alocada para todos os nos de uma lista ligada de strings
 * @param cabeca ponteiro para o primeiro no da lista ligada a ser eliminada
 */
void libertarStrings(NoString* cabeca) {
    while (cabeca != nullptr) {
        NoString* temp = cabeca;
        cabeca = cabeca->prox;
        delete temp;
    }
}


/**
 * verifica se uma determinada string ja existe na lista ligada
 * @param cabeca ponteiro para o primeiro no da lista ligada onde vamos procurar
 * @param str referencia constante para a string (texto) que queremos encontrar
 * @return true se a string for encontrada, false caso contrario
 */
bool existeString(NoString* cabeca, const string& str) {
    while (cabeca != nullptr) {
        if (cabeca->texto == str) return true;
        cabeca = cabeca->prox;
    }
    return false;
}


/**
 * adiciona uma nova string no final da lista ligada e atualiza o contador
 * @param cabeca referencia para o ponteiro do primeiro no da lista (pode ser modificada se a lista estiver vazia)
 * @param str referencia constante para a string (texto) a ser adicionada
 * @param contador referencia para a variavel do contador de elementos, que sera incrementada
 */
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



/**
 * inicializa a lista ligada de produtos, colocando-a vazia (aponta para nulo)
 * @param inicio referencia para o ponteiro do primeiro no da lista de produtos
 */
void inicializarListaProdutos(NoProduto*& inicio) {
    inicio = nullptr;
}


/**
 * adiciona um novo produto no final da lista ligada de produtos
 * @param inicio referencia para o ponteiro do primeiro no da lista (pode ser modificada se a lista estiver vazia)
 * @param p referencia constante para os dados do produto a adicionar
 */
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


/**
 * inicializa a arvore binaria de vendas, colocando-a vazia (aponta para nulo)
 * @param raiz referencia para o ponteiro do no raiz da arvore
 */
void inicializarArvore(NoVenda*& raiz) {
    raiz = nullptr;
}


/**
 * insere um novo registo de venda na arvore binaria de pesquisa de forma recursiva
 * @param raiz referencia para o ponteiro do no raiz (ou no atual) da arvore de vendas
 * @param preco valor do preco da venda (usado como chave de ordenacao na arvore)
 * @param nome referencia constante para o nome do produto vendido
 */
void inserirVendaRec(NoVenda*& raiz, int preco, const string& nome) {
    if (raiz == nullptr) raiz = new NoVenda{preco, nome, nullptr, nullptr};
    else if (preco < raiz->preco) inserirVendaRec(raiz->esq, preco, nome);
    else inserirVendaRec(raiz->dir, preco, nome);
}




/**
 * gera um numero especifico de produtos com dados aleatorios e coloca-os no armazem
 * @param sm referencia para o supermercado, onde o produto sera colocado na fila do armazem
 * @param quantidade numero de produtos a gerar
 * @param areas ponteiro para a lista ligada com os nomes das areas disponiveis
 * @param nAreas quantidade total de areas registadas na lista
 * @param nomes ponteiro para a lista ligada com os nomes possiveis dos produtos
 * @param nNomes quantidade total de nomes de produtos registados
 * @param fornecedores ponteiro para a lista ligada com os nomes dos fornecedores
 * @param nFornec quantidade total de fornecedores registados
 */
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


/**
 * inicializa o supermercado gerando sectores aleatorios, pedindo os responsaveis e enchendo o armazem
 * @param sm referencia para a estrutura principal do supermercado a ser inicializada
 * @param areas ponteiro para a lista de todas as areas possiveis carregadas do ficheiro
 * @param nAreas numero total de areas carregadas
 * @param areasAtivas referencia para o ponteiro da lista que vai guardar apenas as areas efetivamente em uso (geradas)
 * @param nAreasAtivas referencia para a variavel que conta quantas areas ativas existem
 * @param nomes ponteiro para a lista de nomes possiveis dos produtos
 * @param nNomes numero total de nomes de produtos
 * @param fornecedores ponteiro para a lista de fornecedores disponiveis
 * @param nFornec numero total de fornecedores
 */
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



/**
 * percorre todos os sectores e simula a venda de produtos (25% de probabilidade de venda por produto)
 * os produtos vendidos sao removidos das prateleiras (lista ligada) e registados na arvore de vendas do sector
 * @param sm referencia para a estrutura principal do supermercado
 */
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


/**
 * tenta transferir produtos da fila do armazem para as prateleiras dos sectores correspondentes
 * a colocacao prioriza sempre o sector da mesma area que tenha o maior espaco livre disponivel
 * se um produto nao couber em nenhum sector, volta para o final da fila do armazem
 * @param sm referencia para a estrutura principal do supermercado (armazem e sectores)
 * @param maxTransferir numero maximo de produtos que a funcao vai tentar tirar do armazem neste ciclo
 */
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


/**
 * atualiza a duracao das campanhas ativas e remove as que chegaram ao fim.
 * quando uma campanha termina, restaura o preco original de todos os produtos
 * afetados por essa area, tanto nas prateleiras dos sectores como na fila do armazem.
 * @param sm referencia para a estrutura principal do supermercado
 */
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

/**
 * executa um ciclo completo de simulacao do supermercado.
 * atualiza as campanhas, simula a venda de produtos nas prateleiras, gera novo stock
 * para o armazem e tenta repor as prateleiras dos sectores com os produtos guardados.
 * @param sm referencia para a estrutura principal do supermercado
 * @param areas ponteiro para a lista ligada de areas disponiveis no sistema
 * @param nAreas quantidade total de areas registadas
 * @param nomes ponteiro para a lista ligada com os nomes possiveis dos produtos
 * @param nNomes quantidade total de nomes de produtos
 * @param fornecedores ponteiro para a lista ligada com os nomes dos fornecedores
 * @param nFornec quantidade total de fornecedores registados
 */
void simularCiclo(SuperMercado& sm, NoString* areas, int nAreas, NoString* nomes, int nNomes, NoString* fornecedores, int nFornec) {

    atualizarCampanhas(sm);

    // Tentar vender os produtos que estão nas prateleiras
    venderProdutos(sm);

    // Gerar 10 produtos usando todas as áreas do sistema (incluindo as novas que possam ter sido criadas)
    gerarProdutosParaArmazem(sm, 10, areas, nAreas, nomes, nNomes, fornecedores, nFornec);

    // Tentar passar produtos do armazém para os setores
    transferirArmazemParaSetores(sm, 10);
}



/**
 * procura e remove todas as unidades de um determinado produto no supermercado inteiro (prateleiras e armazem).
 * antes de apagar, verifica se a area do produto esta sob uma campanha ativa (se estiver, a remocao e bloqueada).
 * todas as unidades apagadas sao guardadas no historico para permitir a reversao.
 * @param sm referencia para a estrutura principal do supermercado
 * @param nome referencia constante para o nome exato do produto a remover
 * @return 1 se o produto foi removido com sucesso, 0 se o produto nao foi encontrado, ou -1 se a remocao foi bloqueada devido a uma campanha ativa
 */
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


/**
 * atualiza o preco (atual e original) de todas as unidades de um determinado produto que estao na fila do armazem.
 * percorre toda a fila retirando e voltando a colocar os produtos, alterando apenas aqueles que correspondem ao nome.
 * @param sm referencia para a estrutura principal do supermercado
 * @param nome referencia constante para o nome exato do produto a atualizar
 * @param novoPreco valor do novo preco a aplicar ao produto
 * @return true se pelo menos uma unidade do produto foi encontrada e atualizada, false caso contrario
 */
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


/**
 * cria uma nova campanha de descontos para uma area especifica e aplica o novo preco a todos os produtos dessa area.
 * a atualizacao dos precos e feita tanto nas prateleiras dos sectores como nos produtos guardados no armazem.
 * a campanha so e registada na lista de campanhas ativas se existir pelo menos um produto dessa area no supermercado.
 * @param sm referencia para a estrutura principal do supermercado
 * @param area referencia constante para o nome da area alvo da campanha
 * @param percentagem valor da percentagem de desconto a aplicar (ex: 20 para 20%)
 * @param duracao numero de ciclos (turnos) que a campanha vai durar antes de expirar
 * @return true se a campanha foi criada e o desconto aplicado com sucesso, false se nao havia produtos dessa area na loja
 */
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


/**
 * imprime o estado atual do supermercado, exibindo a informacao de todos os sectores e os respetivos produtos nas prateleiras.
 * no final, apresenta uma tabela formatada com todos os produtos que aguardam na fila do armazem.
 * @param sm referencia constante para a estrutura principal do supermercado a ser mostrada no ecra
 */
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


/**
 * tenta criar e registar uma nova area na lista de areas do sistema.
 * percorre a lista para garantir que nao ha duplicados; se a area ja existir, a operacao e cancelada.
 * @param areas referencia para o ponteiro do primeiro no da lista ligada de areas
 * @param nAreas referencia para a variavel com o total de areas, que sera incrementada se houver sucesso
 * @param novaArea referencia constante para o nome da nova area a adicionar
 * @return true se a area foi criada e adicionada com sucesso, false se a area ja existia
 */
bool criarNovaArea(NoString*& areas, int& nAreas, const string& novaArea) {
    for (NoString* atual = areas; atual != nullptr; atual = atual->prox) {
        if (atual->texto == novaArea) {
            return false; // A área já existe! Aborta.
        }
    }

    adicionarStringFim(areas, novaArea, nAreas);
    return true;
}


/**
 * percorre a arvore binaria de vendas em ordem (in-order) e imprime os registos no ecra.
 * devido a natureza da arvore binaria de pesquisa (ordenada pelo preco), esta travessia imprime as vendas por ordem crescente de valor.
 * @param no ponteiro para o no atual da arvore binaria de vendas (normalmente a chamada inicial recebe a raiz da arvore)
 */
void percorrerInOrder(NoVenda* no) {
    if (no == nullptr) return;
    percorrerInOrder(no->esq);
    cout << "Produto: " << no->nome << " | Preco: " << no->preco << " Euros\n";
    percorrerInOrder(no->dir);
}


/**
 * procura e exibe o registo de vendas de todos os sectores geridos por um determinado responsavel.
 * caso encontre o responsavel, imprime as vendas do seu sector de forma ordenada (recorrendo a travessia in-order da arvore).
 * @param sm referencia constante para a estrutura principal do supermercado
 * @param responsavel referencia constante com o nome do gestor/responsavel do sector a pesquisar
 * @return true se encontrou pelo menos um sector gerido por esse responsavel, false se o nome nao existir no sistema
 */
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


/**
 * grava o estado atual de todo o supermercado (sectores, produtos nas prateleiras, vendas, armazem e campanhas) num ficheiro de texto
 * percorre as estruturas de dados gerando linhas com prefixos especificos (SECTOR, P_SEC, P_ARM, CAMP) para facilitar a leitura posterior
 * @param sm referencia para a estrutura principal do supermercado cujos dados serao salvos
 * @param filename referencia constante para o nome do ficheiro onde os dados serao gravados (ex: "supermercado.txt")
 * @return true se o ficheiro foi aberto e gravado com sucesso, false caso contrario
 */
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


/**
 * carrega e reconstrói o estado completo do supermercado a partir de um ficheiro de texto guardado.
 * limpa os dados atualmente em memória, lê o ficheiro linha a linha e processa cada estrutura
 * (sectores, produtos nas prateleiras, armazém, vendas e campanhas) com base nas etiquetas identificadoras.
 * sincroniza também de forma automática as áreas lidas com a lista global de áreas do sistema.
 * @param sm referência para a estrutura principal do supermercado que será preenchida
 * @param filename referência constante para o nome do ficheiro a ler (ex: "supermercado.txt")
 * @param areas referência para o ponteiro da lista ligada de áreas, para sincronizar novas áreas encontradas
 * @param nAreas referência para o contador do total de áreas do sistema
 * @return true se o ficheiro foi aberto e processado com sucesso, false caso contrário
 */
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


/**
 * liberta toda a memoria dinamica alocada para o supermercado, limpando todas as estruturas.
 * elimina os produtos das prateleiras, destroi recursivamente as arvores de vendas de cada sector,
 * remove os sectores, esvazia a fila do armazem, apaga as campanhas ativas e limpa a pilha do historico.
 * garante que todos os ponteiros principais sao repostos a nulo (nullptr).
 * @param sm referencia para a estrutura principal do supermercado a ser limpa e reinicializada
 */
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
/**
 * liberta recursivamente toda a memoria alocada para os nos de uma arvore binaria de vendas.
 * utiliza uma travessia em pos-ordem (post-order), garantindo que os nos filhos (esquerda
 * e direita) sao eliminados antes do no pai, evitando a perda de ponteiros e memory leaks.
 * @param raiz ponteiro para o no raiz (ou no atual) da arvore de vendas a ser destruida
 */
void limparArvore(NoVenda* raiz) {
    if (raiz == nullptr) return;
    limparArvore(raiz->esq);
    limparArvore(raiz->dir);
    delete raiz;
}


/**
 * grava recursivamente todos os registos de venda de uma arvore binaria num ficheiro de texto.
 * utiliza uma travessia em pre-ordem (pre-order), escrevendo primeiro o no atual e depois os
 * seus filhos (esquerda e direita), o que facilita a reconstrucao exata da arvore ao ler o ficheiro.
 * @param raiz ponteiro para o no raiz (ou no atual) da arvore de vendas a gravar
 * @param sectorID caracter identificador do sector ao qual estas vendas pertencem (ex: 'A')
 * @param out referencia para o fluxo do ficheiro de saida (ofstream) onde os dados sao escritos
 */
void gravarVendasRec(NoVenda* raiz, char sectorID, ofstream& out) {
    if (raiz == nullptr) return;
    out << "VENDA|" << sectorID << "|" << raiz->nome << "|" << raiz->preco << "\n";
    gravarVendasRec(raiz->esq, sectorID, out);
    gravarVendasRec(raiz->dir, sectorID, out);
}


/**
 * procura um sector especifico na lista ligada de sectores do supermercado atraves do seu ID.
 * realiza uma pesquisa linear do inicio ao fim da lista ate encontrar o caracter correspondente.
 * @param sm referencia para a estrutura principal do supermercado
 * @param id caracter identificador do sector a procurar (ex: 'A', 'B')
 * @return ponteiro para o sector encontrado, ou nullptr caso o ID nao exista no sistema
 */
Sector* encontrarSector(SuperMercado& sm, char id) {
    Sector* atual = sm.inicioSectores;
    while (atual != nullptr) {
        if (atual->id == id) return atual;
        atual = atual->prox;
    }
    return nullptr;
}

//Funcao usada para efetuar testes
/*
int somarVendasRec(NoVenda* raiz) {
    if (raiz == nullptr) return 0;
    return raiz->preco + somarVendasRec(raiz->esq) + somarVendasRec(raiz->dir);
}
*/


//Funcao usada para efetuar testes
/*
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
*/


//Funcao usada para efetuar testes
/*
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
*/


//Funcao usada para efetuar testes
/*
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
*/


/**
 * insere (empilha) os dados de um produto removido na pilha de historico do supermercado.
 * cria um novo no dinamicamente e coloca-o no topo da pilha, deslocando o topo antigo.
 * @param sm referencia para a estrutura principal do supermercado (contem o topo da pilha)
 * @param p referencia constante para a estrutura do produto a ser guardada no historico
 */
void pushHistorico(SuperMercado& sm, const Produto& p) {
    // Alocação dinâmica do novo nó
    NoHistorico* novo = new NoHistorico;
    novo->info = p;
    novo->prox = sm.topoHistorico; // O próximo aponta para o antigo topo

    sm.topoHistorico = novo; // O topo passa a ser este novo nó
}


// Desempilha o último produto removido e volta a colocá-lo no armazém (Pop / Undo)
/**
 * desfaz a ultima remocao de produto realizada, retirando-o da pilha de historico (pop) e devolvendo-o ao armazem.
 * remove o no que se encontra no topo da pilha, liberta a sua memoria e reinsere o produto na fila do armazem.
 * @param sm referencia para a estrutura principal do supermercado
 * @return true se o processo de undo foi concluido com sucesso, false se a pilha de historico estava vazia
 */
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


/**
 * cria e adiciona um novo sector ao supermercado dinamicamente durante a execucao do programa.
 * calcula de forma automatica o proximo ID disponivel (com base no caracter ASCII mais alto existente)
 * e insere o sector devidamente inicializado no final da lista ligada de sectores.
 * @param sm referencia para a estrutura principal do supermercado
 * @param area referencia constante com o nome da area comercial atribuida ao novo sector
 * @param resp referencia constante com o nome do responsavel pelo sector
 * @param cap capacidade maxima de armazenamento de produtos na prateleira deste sector
 */
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
