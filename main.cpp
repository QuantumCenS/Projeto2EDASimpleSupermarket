#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include "supermercado.h"
#include "FilaArmazem.h"
#include "menu.h"

using namespace std;

int main(int argc, char* argv[]) {
    srand(time(NULL));

    int nAreas, nNomes, nFornec;

    // Carregar os ficheiros
    NoString* areas = carregarStrings("areas.txt", nAreas);
    NoString* nomes = carregarStrings("produtos.txt", nNomes);
    NoString* fornecedores = carregarStrings("fornecedores.txt", nFornec);

    if (!areas || !nomes || !fornecedores) {
        cout << "[ERRO] Erro ao abrir ficheiros de dados.\n";
        libertarStrings(areas);
        libertarStrings(nomes);
        libertarStrings(fornecedores);
        return 1;
    }

    SuperMercado super;
    Nova(super.armazem);
    super.inicioSectores = nullptr;
    super.campanhas = nullptr;

    bool carregado = false;

    //verifica se foi passado um save
    if (argc >= 2) {
        if (carregarSupermercado(super, argv[1])) {
            cout << "Estado carregado com sucesso.\n";
            carregado = true;
        } else {
            cout << "Erro ao carregar, iniciando novo supermercado.\n";
        }
    }

    // Se não carregou nenhum save, gera o supermercado do zero
    if (!carregado) {
        inicializarSupermercado(super, areas, nAreas, nomes, nNomes, fornecedores, nFornec);
    }

    char opcao;

    // ==========================================
    // LOOP PRINCIPAL DO SIMULADOR
    // ==========================================
    do {
        // Imprime o estado atual do supermercado
        imprimirProdutos(super);

        // Menu Gestao
        cout << "(s)eguinte  **********  (g)estao  **********  (q)sair\n";
        cout << "Seleccione a sua opcao: ";

        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcao == 's' || opcao == 'S') {
            simularCiclo(super,areas,nAreas, nomes, nNomes, fornecedores, nFornec);
        }
        else if (opcao == 'g' || opcao == 'G') {
            menuGestao(super, areas, nAreas);
        }

    } while (opcao != 'q' && opcao != 'Q');


    libertarStrings(areas);
    libertarStrings(nomes);
    libertarStrings(fornecedores);

    return 0;
}