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

    // Carregar os ficheiros globais
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
    super.topoHistorico = nullptr; // Inicia a pilha do histórico a vazio

    bool carregado = false;

    // --- VARIÁVEIS PARA AS ÁREAS ATIVAS (A CORREÇÃO ESTÁ AQUI) ---
    NoString* areasAtivas = nullptr;
    int nAreasAtivas = 0;

    // Verifica se foi passado um save por argumento
    if (argc >= 2) {
        // Agora já passa as areasAtivas para reconstruir o menu a partir do save
        if (carregarSupermercado(super, argv[1], areasAtivas, nAreasAtivas)) {
            cout << "Estado carregado com sucesso.\n";
            carregado = true;
        } else {
            cout << "Erro ao carregar, iniciando novo supermercado.\n";
        }
    }

    // Se não carregou nenhum save, gera o supermercado do zero
    if (!carregado) {
        // Agora já passa as areasAtivas para garantir que gera produtos certos
        inicializarSupermercado(super, areas, nAreas, areasAtivas, nAreasAtivas, nomes, nNomes, fornecedores, nFornec);
    }

    char opcao;

    // ==========================================
    // LOOP PRINCIPAL DO SIMULADOR
    // ==========================================
    do {
        // Imprime o estado atual do supermercado
        imprimirProdutos(super);

        // Menu Gestao
        cout << "(s)eguinte  ********** (g)estao  ********** (q)sair\n";
        cout << "Seleccione a sua opcao: ";

        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcao == 's' || opcao == 'S') {
            // Agora o ciclo também respeita as áreas ativas!
            simularCiclo(super, areasAtivas, nAreasAtivas, nomes, nNomes, fornecedores, nFornec);
        }
        else if (opcao == 'g' || opcao == 'G') {
            menuGestao(super, areasAtivas, nAreasAtivas);
        }

    } while (opcao != 'q' && opcao != 'Q');


    // Limpeza de memória antes de fechar o programa
    libertarStrings(areas);
    libertarStrings(nomes);
    libertarStrings(fornecedores);
    libertarStrings(areasAtivas); // Liberta a lista das áreas ativas
    limparSupermercado(super);    // Limpa armazém, sectores, árvores e pilha

    return 0;
}