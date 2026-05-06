//
// Created by carlos on 06/05/2026.
//

#ifndef PROJETO2EDA_MENU_H
#define PROJETO2EDA_MENU_H


#include <string>
#include "supermercado.h" //importar structs todas

// Declarações das funções de Interface
void interfaceRemoverProduto(SuperMercado& sm);
void interfaceAtualizarPreco(SuperMercado& sm);
void interfaceIniciarCampanha(SuperMercado& sm);
void interfaceCriarArea(NoString*& areasAtivas, int& nAreasAtivas);
void interfaceRegistoVendas(SuperMercado& sm);

// função principal do Menu
void menuGestao(SuperMercado& sm, NoString*& areasAtivas, int& nAreasAtivas);
#endif //PROJETO2EDA_MENU_H