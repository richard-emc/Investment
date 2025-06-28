#include <iostream>
#include <filesystem>
#include "carteira.hpp"
#include "inputmanager.hpp"

int main() {
    Carteira minhaCarteira;
    InputManager inputManager;
    int escolha;
    const std::string CAMINHO_PADRAO = "carteira.csv";

    // 🔁 Carregar carteira automaticamente, se o arquivo existir
    if (std::filesystem::exists(CAMINHO_PADRAO)) {
        inputManager.carregarCSV(minhaCarteira, CAMINHO_PADRAO);
        std::cout << "📂 Carteira carregada automaticamente de '" << CAMINHO_PADRAO << "'.\n\n";
    } else {
        std::cout << "📁 Nenhum arquivo de carteira encontrado. Começando com carteira vazia.\n\n";
    }   


    while (true) {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1 - Inserir ativo manualmente\n";
        std::cout << "2 - Carregar ativos de arquivo CSV\n";
        std::cout << "3 - Listar ativos\n";
        std::cout << "4 - Salvar carteira em CSV\n";   
        std::cout << "5 - Sair\n";
        std::cout << "Escolha: ";
        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Entrada inválida, tente novamente.\n";
            continue;
        }

        switch (escolha) {
            case 1:
                inputManager.inserirManual(minhaCarteira);
                break;
            case 2: {
                std::string caminho;
                std::cout << "Digite o caminho do arquivo CSV: ";
                std::cin.ignore(); // limpa o '\n'
                std::getline(std::cin, caminho);
                inputManager.carregarCSV(minhaCarteira, caminho);
                break;
            }
            case 3:
                minhaCarteira.listarAtivos();  // Implemente essa função para mostrar os ativos
                break;
            case 4:{
                std::string caminhoSalvar;
                std::cout << "Digite o caminho para salvar a carteira em CSV: ";
                std::cin.ignore();
                std::getline(std::cin, caminhoSalvar);
                minhaCarteira.salvarCSV(caminhoSalvar);
                break;
            }
            case 5:{
                minhaCarteira.salvarCSV(CAMINHO_PADRAO);
                std::cout << "💾 Carteira salva automaticamente em '" << CAMINHO_PADRAO << "'.\n";
                std::cout << "Saindo...\n";
                return 0;
            }
            default:
                std::cout << "Opção inválida.\n";
        }
    }
}
