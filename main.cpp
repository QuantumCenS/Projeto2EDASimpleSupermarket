#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include "supermercado.h"
#include "FilaArmazem.h"

using namespace std;

// Assinatura atualizada para usar NoString*
void menuGestao(SuperMercado& sm, NoString*& areas, int& nAreas);

int main(int argc, char* argv[]) {
    srand(time(NULL));

    int nAreas, nNomes, nFornec;
    // Trocado string* por NoString*
    NoString* areas = carregarStrings("areas.txt", nAreas);
    NoString* nomes = carregarStrings("produtos.txt", nNomes);
    NoString* fornecedores = carregarStrings("fornecedores.txt", nFornec);

    if (!areas || !nomes || !fornecedores) {
        cout << "Erro ao abrir ficheiros de dados.\n";
        libertarStrings(areas);
        libertarStrings(nomes);
        libertarStrings(fornecedores);
        return 1;
    }

    SuperMercado super;
    Nova(super.armazem); // <-- CORREÇÃO: Agora usamos Nova() porque é uma Fila
    super.setores.inicio = nullptr;
    super.campanhas = nullptr;

    bool carregado = false;
    if (argc >= 2) {
        if (carregarSupermercado(super, argv[1])) {
            cout << "Estado carregado com sucesso.\n";
            carregado = true;
        } else {
            cout << "Erro ao carregar, iniciando novo supermercado.\n";
        }
    }

    if (!carregado) {
        inicializarSupermercado(super, areas, nAreas, nomes, nNomes, fornecedores, nFornec);
    }

    char opcao;
    do {
        imprimirProdutos(super);
        cout << "\n(s)eguinte ********** (g)estao\nSeleccione a sua opcao: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcao == 's' || opcao == 'S') {
            simularCiclo(super, nomes, nNomes, fornecedores, nFornec);
        }
        else if (opcao == 'g' || opcao == 'G') {
            menuGestao(super, areas, nAreas);
        }
    } while (opcao != 'q' && opcao != 'Q');

    // Assumindo que terás uma função libertarSupermercado adaptada para a fila
    // libertarSupermercado(super);
    libertarStrings(areas);
    libertarStrings(nomes);
    libertarStrings(fornecedores);

    return 0;
}

void menuGestao(SuperMercado& sm, NoString*& areas, int& nAreas) {
    int opcao;
    do {
        cout << "\n***** Bem Vindo Gestor *****\n";
        cout << "(1).Remover produto\n";
        cout << "(2).Atualizar preco\n";
        cout << "(3).Iniciar Campanha\n";
        cout << "(4).Gravar Supermercado\n";
        cout << "(5).Carregar Supermercado\n";
        cout << "(6).Imprimir Produtos\n";
        cout << "(7).Criar nova area\n";
        cout << "(8).Mostrar registo de vendas\n";
        cout << "(0).Voltar\n";
        cout << "Seleccione a sua opcao: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao) {
            case 1: {
                cout << "Nome do produto a remover: ";
                string nome; getline(cin, nome);
                removerProdutoGlobal(sm, nome);
                cout << "Produto(s) removido(s).\n";
                break;
            }
            case 2: {
                cout << "Nome do produto (no armazem): ";
                string nome; getline(cin, nome);
                cout << "Novo preco (multiplo de 2): ";
                int preco; cin >> preco;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                atualizarPrecoArmazem(sm, nome, preco);
                cout << "Preco atualizado.\n";
                break;
            }
            case 3: {
                cout << "Area da campanha: ";
                string area; getline(cin, area);
                cout << "Percentagem de desconto: ";
                int perc; cin >> perc;
                cout << "Duracao (em ciclos): ";
                int dur; cin >> dur;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                adicionarCampanha(sm, area, perc, dur);
                cout << "Campanha iniciada.\n";
                break;
            }
            case 4: {
                cout << "Nome do ficheiro para gravar: ";
                string ficheiro; getline(cin, ficheiro);
                if (gravarSupermercado(sm, ficheiro))
                    cout << "Estado gravado com sucesso.\n";
                else
                    cout << "Erro ao gravar.\n";
                break;
            }
            case 5: {
                cout << "Nome do ficheiro para carregar: ";
                string ficheiro; getline(cin, ficheiro);
                if (carregarSupermercado(sm, ficheiro))
                    cout << "Estado carregado com sucesso.\n";
                else
                    cout << "Erro ao carregar.\n";
                break;
            }
            case 6:
                imprimirProdutos(sm);
                break;
            case 7: {
                cout << "Nome da nova area: ";
                string nova; getline(cin, nova);
                criarNovaArea(areas, nAreas, nova);
                cout << "Area adicionada.\n";
                break;
            }
            case 8: {
                cout << "Nome do responsavel: ";
                string resp; getline(cin, resp);
                mostrarRegistoVendas(sm, resp);
                break;
            }
            case 0: break;
            default: cout << "Opcao invalida.\n";
        }
    } while (opcao != 0);
}