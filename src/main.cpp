#include <iostream>
#include <filesystem>
#include "carteira.hpp"
#include "inputmanager.hpp"

int main() {
    Carteira minhaCarteira;
    InputManager inputManager;
    const std::string CAMINHO_CARTEIRA = "carteira.csv";
    const std::string CAMINHO_ORDENS = "historico_ordens.csv";

    // Carrega histórico de ordens se existir
    if (std::filesystem::exists(CAMINHO_ORDENS)) {
        inputManager.carregarHistoricoDeOrdens(minhaCarteira, CAMINHO_ORDENS);
        inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
        std::cout << "Histórico de ordens carregado de '" << CAMINHO_ORDENS << "'\n";
    }


    int escolha;
    do {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1 - Inserir ordem manualmente\n";
        std::cout << "2 - Carregar ordens de arquivo CSV\n";
        std::cout << "3 - Listar ativos\n";
        std::cout << "4 - Salvar carteira e histórico de ordens\n";
        std::cout << "5 - Sair\n";
        std::cout << "Escolha: ";
        std::cin >> escolha;

        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Entrada inválida. Tente novamente.\n";
            continue;
        }

        switch(escolha) {
            case 1:
                inputManager.inserirOrdemManual(minhaCarteira); // Função que pede dados da ordem e aplica
                break;

            case 2: {
                std::cin.ignore();
                std::string caminhoOrdens;
                std::cout << "Digite o caminho do arquivo CSV de ordens: ";
                std::getline(std::cin, caminhoOrdens);
                inputManager.carregarHistoricoDeOrdens(minhaCarteira, caminhoOrdens);
                break;
            }

            case 3:
                minhaCarteira.listarAtivos();
                break;

            case 4:
                minhaCarteira.salvarCSV(CAMINHO_CARTEIRA);
                inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
                std::cout << "Carteira e histórico salvos.\n";
                break;

            case 5:
                minhaCarteira.salvarCSV(CAMINHO_CARTEIRA);
                inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
                std::cout << "Saindo... Carteira e histórico salvos.\n";
                break;

            default:
                std::cout << "Opção inválida.\n";
        }
    } while (escolha != 5);

    return 0;
}

