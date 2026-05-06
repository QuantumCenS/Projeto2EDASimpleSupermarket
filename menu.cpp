//
// Created by carlos on 06/05/2026.
//

#include "menu.h"
#include <iostream>

using namespace std;

// ==========================================
// FUNÇÕES DE INTERFACE DO MENU
// ==========================================

void interfaceRemoverProduto(SuperMercado& sm) {
    string nomeProd;
    cout << "-> Introduza o nome do produto a remover: ";
    getline(cin >> ws, nomeProd);

    removerProdutoGlobal(sm, nomeProd);
    cout << "[AVISO] Produto removido (caso existisse no sistema).\n";
}

void interfaceAtualizarPreco(SuperMercado& sm) {
    string nomeProd;
    int novoPreco;
    cout << "-> Introduza o nome do produto: ";
    getline(cin >> ws, nomeProd);
    cout << "-> Introduza o novo preco: ";
    cin >> novoPreco;

    atualizarPrecoArmazem(sm, nomeProd, novoPreco);
    cout << "[AVISO] Preco atualizado no armazem!\n";
}

void interfaceIniciarCampanha(SuperMercado& sm) {
    string area;
    int perc, duracao;
    cout << "-> Introduza a area da campanha: ";
    getline(cin >> ws, area);
    cout << "-> Percentagem de desconto (ex: 20): ";
    cin >> perc;
    cout << "-> Duracao da campanha (em ciclos): ";
    cin >> duracao;

    adicionarCampanha(sm, area, perc, duracao);
    cout << "[AVISO] Campanha iniciada com sucesso!\n";
}

void interfaceCriarArea(NoString*& areasAtivas, int& nAreasAtivas) {
    string novaArea;
    cout << "-> Introduza o nome da nova area: ";
    getline(cin >> ws, novaArea);

    criarNovaArea(areasAtivas, nAreasAtivas, novaArea);
    cout << "[AVISO] Area '" << novaArea << "' adicionada aos registos.\n";
}

void interfaceRegistoVendas(SuperMercado& sm) {
    string nomeResp;
    cout << "-> Introduza o nome do responsavel a pesquisar: ";
    getline(cin >> ws, nomeResp);

    mostrarRegistoVendas(sm, nomeResp);
}


void menuGestao(SuperMercado& sm, NoString*& areasAtivas, int& nAreasAtivas) {
    int opcaoMenu;

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

        cin >> opcaoMenu;

        if(cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n'); opcaoMenu = -1;
        }

        switch(opcaoMenu) {
            case 1: interfaceRemoverProduto(sm); break;
            case 2: interfaceAtualizarPreco(sm); break;
            case 3: interfaceIniciarCampanha(sm); break;
            case 4:
                cout << "[AVISO] A gravar...\n";
                break;
            case 5:
                cout << "[AVISO] A carregar...\n";
                break;
            case 6: imprimirProdutos(sm); break;
            case 7: interfaceCriarArea(areasAtivas, nAreasAtivas); break;
            case 8: interfaceRegistoVendas(sm); break;
            case 0: break;
            default: cout << "[ERRO] Opcao invalida.\n";
        }
    } while (opcaoMenu != 0);
}