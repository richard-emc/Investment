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
        std::cout << "===== Menu =====\n";
        std::cout << "1 - Inserir ordem manualmente\n";
        std::cout << "2 - Listar ativos\n";
        std::cout << "3 - Salvar carteira e histórico de ordens\n";
        std::cout << "4 - Mostrar ordens por ticker\n";
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

            case 2:
                minhaCarteira.listarAtivos();
                break;

            case 3:
                minhaCarteira.salvarCSV(CAMINHO_CARTEIRA);
                inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
                std::cout << "Carteira e histórico salvos.\n";
                break;

            case 4: {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpa o buffer
                std::string tickerConsulta;
                std::cout << "Digite o ticker para mostrar ordens: ";
                std::getline(std::cin, tickerConsulta);
                tickerConsulta = paraMaiusculas(tickerConsulta);
                auto ordensTicker = minhaCarteira.buscarOrdensPorTicker(tickerConsulta);
                if (ordensTicker.empty()) {
                    std::cout << "Nenhuma ordem encontrada para o ticker " << tickerConsulta << ".\n";
                } else {
                    std::cout << "Ordens para " << tickerConsulta << ":\n";
                for (const auto& ordem : ordensTicker) {
                    std::cout << (ordem.tipo == TipoOrdem::COMPRA ? "COMPRA" : "VENDA")
                          << " | Corretora: " << ordem.corretora
                          << " | Quantidade: " << ordem.quantidade
                          << " | Preço: " << ordem.preco
                          << " | Data: " << ordem.data << "\n";
                    }
                }
                break;
                }


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

