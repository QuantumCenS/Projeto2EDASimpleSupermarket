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


/**
 * gere a interface de utilizador para a remocao de um produto do supermercado.
 * solicita o nome do produto através do teclado, invoca a funcao de remocao global
 * e apresenta uma mensagem apropriada com base no resultado (sucesso, bloqueio por campanha ou nao encontrado).
 * @param sm referencia para a estrutura principal do supermercado
 */
void interfaceRemoverProduto(SuperMercado& sm) {
    string nomeProd;
    cout << "-> Introduza o nome do produto a remover: ";
    getline(cin >> ws, nomeProd);

    int resultado = removerProdutoGlobal(sm, nomeProd);

    if (resultado == 1) {
        cout << "[SUCESSO] O produto '" << nomeProd << "' foi removido de todo o supermercado!\n";
    }
    else if (resultado == -1) {
        cout << "[ERRO] Nao e possivel remover o produto '" << nomeProd << "'. A sua area tem uma campanha ativa!\n";
    }
    else {
        cout << "[AVISO] O produto '" << nomeProd << "' nao existe em nenhum sector nem no armazem.\n";
    }
}


/**
 * gere a interface de utilizador para a atualizacao de preco de um produto no armazem.
 * solicita o nome do produto e o novo preco, valida se a entrada numerica e valida
 * para evitar falhas no fluxo de leitura (cin) e reporta o sucesso ou insucesso da operacao.
 * @param sm referencia para a estrutura principal do supermercado
 */
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
        return; // Sai da função antes de estragar a Fila!
    }

    bool sucesso= atualizarPrecoArmazem(sm, nomeProd, novoPreco);

    if (sucesso) {
        cout << "Preco do produto '" << nomeProd << "' atualizado no armazem!\n";
    } else {
        cout << "O produto '" << nomeProd << "' nao existe no armazem.\n";
    }
}


/**
 * gere a interface de utilizador para a criacao e ativacao de uma nova campanha de descontos.
 * solicita a area, a percentagem de desconto e a duracao, aplicando uma validacao defensiva
 * rigorosa em todas as entradas numericas para garantir valores logicos (descontos entre 1-99%
 * e duracoes positivas) antes de submeter a criacao da campanha.
 * @param sm referencia para a estrutura principal do supermercado
 */
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


/**
 * gere a interface de utilizador para a operacao de desfazer a ultima remocao de um produto.
 * tenta invocar o desempilhamento do historico e apresenta uma mensagem de aviso caso
 * nao existam acoes gravadas na pilha para reverter.
 * @param sm referencia para a estrutura principal do supermercado
 */
void interfaceDesfazerRemocao(SuperMercado& sm) {
    cout << "\n--- A TENTAR RECUPERAR O ULTIMO PRODUTO REMOVIDO ---\n";
    if (!desfazerUltimaRemocao(sm)) {
        cout << "[AVISO] Nao existem produtos no historico de remocoes para recuperar.\n";
    }
}


/**
 * gere a interface de utilizador para a criacao e registo de uma nova area de produtos.
 * exibe a lista das areas ja existentes, valida defensivamente a entrada de texto (evitando
 * nomes vazios ou compostos apenas por espacos) e, em caso de sucesso no registo da area,
 * permite ao utilizador abrir imediatamente um sector fisico correspondente na loja,
 * atribuindo-lhe um responsavel e uma capacidade maxima gerada aleatoriamente (entre 5 e 10).
 * @param sm referencia para a estrutura principal do supermercado
 * @param areasAtivas referencia para o ponteiro da lista ligada de areas do sistema
 * @param nAreasAtivas referencia para o contador do total de areas registadas
 */
void interfaceCriarArea(SuperMercado& sm, NoString*& areasAtivas, int& nAreasAtivas) {
    string novaArea;
    bool entradaValida = false;

    // Limpa o '\n' (Enter) deixado no buffer pelo cin do menu
    if (cin.peek() == '\n') {
        cin.ignore();
    }

    // MOSTRAR AS ÁREAS EXISTENTES
    cout << "\n==================================================\n";
    cout << "            CRIAR NOVA AREA DE PRODUTOS            \n";
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

        // Proteção contra nomes vazios
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


/**
 * gere a interface de utilizador para a consulta do histórico de vendas por funcionário.
 * solicita o nome do responsável através do teclado, invoca a função de pesquisa e,
 * caso o gestor não seja encontrado no sistema, emite uma mensagem informativa.
 * @param sm referencia para a estrutura principal do supermercado
 */
void interfaceRegistoVendas(SuperMercado& sm) {
    string nomeResp;
    cout << "-> Introduza o nome do responsavel a pesquisar: ";
    getline(cin >> ws, nomeResp);

    bool sucesso = mostrarRegistoVendas(sm, nomeResp);

    if (!sucesso) {
        cout << "Nenhum sector encontrado com o responsavel: '" << nomeResp << "'\n";
    }
}


/**
 * gere a interface de utilizador para a gravacao do estado atual do supermercado.
 * solicita o nome do ficheiro de destino atraves do teclado, invoca o motor de gravacao
 * e reporta o sucesso da operacao ou uma mensagem de erro caso o ficheiro nao possa ser criado.
 * @param sm referencia para a estrutura principal do supermercado
 */
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


/**
 * gere a interface de utilizador para o carregamento de dados do supermercado.
 * solicita o nome do ficheiro de origem através do teclado, invoca o motor de carregamento
 * e reporta o sucesso da operacao ou uma mensagem de erro caso o ficheiro nao exista ou nao possa ser lido.
 * @param sm referencia para a estrutura principal do supermercado que ira receber os dados
 * @param areasAtivas referencia para o ponteiro da lista ligada de areas do sistema (para sincronizacao)
 * @param nAreasAtivas referencia para o contador do total de areas do sistema
 */
void interfaceCarregar(SuperMercado& sm, NoString*& areasAtivas, int& nAreasAtivas) {
    string ficheiro;
    cout << "-> Introduza o nome do ficheiro a carregar (ex: save1.txt): ";
    getline(cin >> ws, ficheiro);

    if (carregarSupermercado(sm, ficheiro, areasAtivas, nAreasAtivas)) {
        cout << "[SUCESSO] Supermercado carregado com sucesso!\n";
    } else {
        cout << "[ERRO] Ficheiro nao encontrado ou corrompido.\n";
    }
}

/**
 * exibe e gere o menu interativo de gestao do supermercado para o utilizador administrador.
 * @param sm referencia para a estrutura principal do supermercado
 * @param areasAtivas referencia para o ponteiro da lista ligada de areas do sistema
 * @param nAreasAtivas referencia para o contador do total de areas registadas
 */
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
        cout << "(9).Desfazer ultima remocao (UNDO)\n"; // <--- NOVA OPÇÃO AQUI
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
            case 5: interfaceCarregar(sm, areasAtivas, nAreasAtivas); break;
            case 6: imprimirProdutos(sm); break;
                // Nota: uso a assinatura da sua versão mais recente
            case 7: interfaceCriarArea(sm, areasAtivas, nAreasAtivas); break;
            case 8: interfaceRegistoVendas(sm); break;
            case 9: interfaceDesfazerRemocao(sm); break; // <--- NOVA CHAMADA AQUI
            case 0: break;
            default: cout << "[ERRO] Opcao invalida.\n";
        }
    } while (opcaoMenu != 0);
}