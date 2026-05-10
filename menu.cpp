//
// Created by carlos on 06/05/2026.
//

#include "menu.h"
#include <iostream>
#include <limits>

using namespace std;

// ==========================================
// FUNÇÕES DE INTERFACE DO MENU
// ==========================================

void interfaceRemoverProduto(SuperMercado& sm) {
    string nomeProd;
    cout << "-> Introduza o nome do produto a remover: ";
    getline(cin >> ws, nomeProd);

    bool sucesso = removerProdutoGlobal(sm, nomeProd);

    if (sucesso) {
        cout << "O produto '" << nomeProd << "' foi removido de todo o supermercado!\n";
    } else {
        cout << "O produto '" << nomeProd << "' nao existe em nenhum sector nem no armazem.\n";
    }
}

void interfaceAtualizarPreco(SuperMercado& sm) {
    string nomeProd;
    int novoPreco;

    cout << "-> Introduza o nome do produto: ";
    getline(cin >> ws, nomeProd);

    cout << "-> Introduza o novo preco: ";
    cin >> novoPreco;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Preco invalido! Por favor, introduza apenas numeros inteiros.\n";
        return;
    }

    bool sucesso= atualizarPrecoArmazem(sm, nomeProd, novoPreco);

    if (sucesso) {
        cout << "Preco do produto '" << nomeProd << "' atualizado no armazem!\n";
    } else {
        cout << "O produto '" << nomeProd << "' nao existe no armazem.\n";
    }
}

void interfaceIniciarCampanha(SuperMercado& sm) {
    string area;
    int perc, duracao;

    cout << "-> Introduza a area da campanha: ";
    getline(cin >> ws, area);

    cout << "-> Percentagem de desconto (ex: 20): ";
    cin >> perc;

    if (cin.fail() || perc <= 0 || perc >= 100) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Percentagem invalida! Introduza um numero entre 1 e 99.\n";
        return;
    }

    cout << "-> Duracao da campanha (em ciclos): ";
    cin >> duracao;

    if (cin.fail() || duracao <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Duracao invalida! Introduza um numero inteiro positivo.\n";
        return;
    }

    bool sucesso=adicionarCampanha(sm, area, perc, duracao);

    if (sucesso) {
        cout << "Campanha de " << perc << "% na area '" << area << "' iniciada por " << duracao << " ciclos!\n";
    } else {
        cout << "Nao foi possivel iniciar a campanha. A area '" << area << "' nao foi encontrada ou esta vazia.\n";
    }
}

void interfaceCriarArea(SuperMercado& sm, NoString*& areasAtivas, int& nAreasAtivas) {
    string novaArea;
    bool entradaValida = false;

    // Limpa o "Enter"
    if (cin.peek() == '\n') {
        cin.ignore();
    }

    // MOSTRAR AS ÁREAS EXISTENTES
    cout << "\n==================================================\n";
    cout << "           CRIAR NOVA AREA DE PRODUTOS            \n";
    cout << "==================================================\n";
    cout << "Areas atualmente registadas no sistema:\n";

    if (areasAtivas == nullptr) {
        cout << "  [ Nenhuma area registada ainda ]\n";
    } else {
        // Percorre a lista e imprime cada área
        int contador = 1;
        for (NoString* atual = areasAtivas; atual != nullptr; atual = atual->prox) {
            cout << "  " << contador << ". " << atual->texto << "\n";
            contador++;
        }
    }
    cout << "==================================================\n";


    while (!entradaValida) {
        cout << "\n-> Introduza o nome da nova area (ou '0' para cancelar): ";
        getline(cin, novaArea);

        // Botão de Cancelar
        if (novaArea == "0") {
            cout << "Operacao cancelada. A voltar ao menu...\n";
            return;
        }

        // Proteção contra espaços vazios
        if (novaArea.empty() || novaArea.find_first_not_of(" ") == string::npos) {
            cout << "[ERRO] Nome invalido! Nao pode deixar em branco nem usar apenas espacos.\n";
        }
        else {
            bool sucesso = criarNovaArea(areasAtivas, nAreasAtivas, novaArea);

            if (sucesso) {
                cout << "[SUCESSO] Area '" << novaArea << "' criada com exito!\n";
                entradaValida = true;


                char opcao;
                cout << "\nDeseja abrir ja um sector fisico na loja para esta area? (s/n): ";
                cin >> opcao;

                if (opcao == 's' || opcao == 'S') {
                    string resp;

                    cout << "Nome do responsavel pelo novo sector: ";
                    getline(cin >> ws, resp);

                    // Gera um número aleatório entre 5 e 10
                    int cap = rand() % 6 + 5;

                    // Chama a função que cria o sector na lista ligada
                    adicionarNovoSector(sm, novaArea, resp, cap);

                    cout << "\n[OK] Sector de '" << novaArea << "' aberto com sucesso e atribuido a " << resp << "!\n";
                    cout << "     -> O setor tera capacidade para " << cap << " produtos.\n";
                } else {
                    cout << "\n[OK] Area registada apenas no sistema. Nao foi aberto nenhum sector fisico.\n";
                }



            } else {
                cout << "[ERRO] A area '" << novaArea << "' ja existe na lista acima. Tente outro nome.\n";
            }
        }
    }
}
void interfaceRegistoVendas(SuperMercado& sm) {
    string nomeResp;
    cout << "-> Introduza o nome do responsavel a pesquisar: ";
    getline(cin >> ws, nomeResp);

    bool sucesso = mostrarRegistoVendas(sm, nomeResp);

    if (!sucesso) {
        cout << "Nenhum sector encontrado com o responsavel: '" << nomeResp << "'\n";
    }
}

void interfaceGravar(SuperMercado& sm) {
    string ficheiro;
    cout << "-> Introduza o nome do ficheiro para guardar (ex: save1.txt): ";
    getline(cin >> ws, ficheiro);

    if (gravarSupermercado(sm, ficheiro)) {
        cout << "[SUCESSO] Supermercado gravado no ficheiro '" << ficheiro << "'!\n";
    } else {
        cout << "[ERRO] Nao foi possivel criar o ficheiro.\n";
    }
}

void interfaceCarregar(SuperMercado& sm) {
    string ficheiro;
    cout << "-> Introduza o nome do ficheiro a carregar (ex: save1.txt): ";
    getline(cin >> ws, ficheiro);

    if (carregarSupermercado(sm, ficheiro)) {
        cout << "[SUCESSO] Supermercado carregado com sucesso!\n";
    } else {
        cout << "[ERRO] Ficheiro nao encontrado ou corrompido.\n";
    }
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
            case 4: interfaceGravar(sm); break;
            case 5: interfaceCarregar(sm); break;
            case 6: imprimirProdutos(sm); break;
            case 7: interfaceCriarArea(sm,areasAtivas, nAreasAtivas); break;
            case 8: interfaceRegistoVendas(sm); break;
            case 0: break;
            default: cout << "[ERRO] Opcao invalida.\n";
        }
    } while (opcaoMenu != 0);
}