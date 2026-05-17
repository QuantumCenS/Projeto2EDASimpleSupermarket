#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include "supermercado.h"
#include "FilaArmazem.h"
#include "menu.h"

using namespace std;


/**
 * ponto de entrada principal do simulador do supermercado.
 * cordena todo o fluxo da aplicacao: inicializa o gerador de numeros aleatorios,
 * carrega os catalogos base (areas, produtos e fornecedores), inicializa as estruturas
 * criticas do supermercado e gere a persistencia de dados (via argumentos da linha de comandos).
 * controla tambem o ciclo principal do simulador (loop) que alterna entre avanco de ciclos,
 * menu de gestao administrativa e a exibicao do estado da loja, garantindo a libertacao
 * total de memoria de todas as listas, filas, pilhas e arvores antes do encerramento.
 * @param argc contador de argumentos passados na linha de comandos
 * @param argv vetor de strings com os argumentos (argv[1] pode conter o caminho de um ficheiro de salvaguarda)
 * @return 0 se o programa terminar com sucesso, 1 se falhar a leitura dos catalogos base
 */
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