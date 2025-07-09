#include <iostream>
#include <filesystem>
#include "carteira.hpp"
#include "inputmanager.hpp"
#include "ativo.hpp"
#include "ordem.hpp"
#include "utils.hpp"

int main() {
    Carteira minhaCarteira;
    InputManager inputManager;
    const std::string CAMINHO_CARTEIRA = "carteira.csv";
    const std::string CAMINHO_ORDENS = "historico_ordens.csv";

    // Carrega histórico de ordens se existir
    if (std::filesystem::exists(CAMINHO_ORDENS)) {
        inputManager.carregarHistoricoDeOrdens(minhaCarteira, CAMINHO_ORDENS);
        inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
        minhaCarteira.salvarCarteira(CAMINHO_CARTEIRA);

        std::cout << "Histórico de ordens carregado de '" << CAMINHO_ORDENS << "'\n";
    }

    int escolha;
    do {
        std::cout << "===== Menu =====\n";
        std::cout << "1 - Inserir ordem manualmente\n";
        std::cout << "2 - Listar ativos\n";
        std::cout << "3 - Salvar carteira e histórico de ordens\n";
        std::cout << "4 - Mostrar ordens por ticker\n";
        std::cout << "5 - Registrar valor mensal do ativor\n";
        std::cout << "6 - Inserir provento (dividendo/JCP)\n";
        std::cout << "7 - Atualizar valores mensais automaticamente\n";
        std::cout << "8 - Exibir histórico de valores de um ativo\n";      
        std::cout << "9 - Exibir dividendos por ativo\n";
        std::cout << "10 - Exibir rentabilidade acomulada\n";
        std::cout << "11 - Sair\n";
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
                inputManager.inserirOrdemManual(minhaCarteira);
                minhaCarteira.salvarCarteira(CAMINHO_CARTEIRA);
                inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
                break;

            case 2:
                minhaCarteira.listarAtivos();
                break;

            case 3:
                minhaCarteira.salvarCarteira(CAMINHO_CARTEIRA);
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
            
            case 5: {
                std::string ticker, corretora, mes;
                double valor;
                std::cout << "Ticker: "; std::cin >> ticker;
                std::cout << "Corretora: "; std::cin.ignore(); std::getline(std::cin, corretora);
                std::cout << "Mês (YYYY-MM): "; std::cin >> mes;
                std::cout << "Valor de fechamento: "; std::cin >> valor;
                Ativo* ativo = minhaCarteira.buscarAtivo(paraMaiusculas(ticker), padronizarNome(corretora));
                if (ativo) {
                    ativo->registrarValorMensal(mes, valor);
                    ativo->salvarHistoricoValor(ticker + "_valores.csv");
                    std::cout << "Valor registrado!\n";
                } else {
                    std::cout << "Ativo não encontrado.\n";
                }
                break;
            }
            
            case 6: {
                std::string ticker, corretora, tipoProvento, dataCom, dataPagamento;
                double valor;
                std::cout << "Ticker: "; std::cin >> ticker;
                std::cout << "Corretora: "; std::cin.ignore(); std::getline(std::cin, corretora);

                Ativo* ativo = minhaCarteira.buscarAtivo(paraMaiusculas(ticker), padronizarNome(corretora));
                if (!ativo) {
                    std::cout << "Ativo não encontrado.\n";
                    break;
                }

                std::cout << "Tipo de provento (DIVIDENDO/JCP): ";
                std::getline(std::cin, tipoProvento);
                tipoProvento = paraMaiusculas(tipoProvento);

                std::cout << "Valor do provento: ";
                std::cin >> valor;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Data COM (YYYY-MM-DD): ";
                std::getline(std::cin, dataCom);

                std::cout << "Data de pagamento (YYYY-MM-DD): ";
                std::getline(std::cin, dataPagamento);

                double valorLiquido = valor;
                if (tipoProvento == "JCP") {
                    valorLiquido = valor * 0.85; // desconto de 15%
                    std::cout << "Valor líquido após IR (15%): " << valorLiquido << "\n";
                }

                // Salva o dividendo no mapa de dividendos, usando a data de pagamento como chave
                ativo->proventos[dataPagamento] += valorLiquido;

                // (Opcional) Salvar em arquivo específico de proventos
                std::ofstream arq(ticker + "_proventos.csv", std::ios::app);
                if (arq.is_open()) {
                    arq << tipoProvento << "," << valor << "," << valorLiquido << "," << dataCom << "," << dataPagamento << "\n";
                    arq.close();
                }

                std::cout << "Provento registrado!\n";
                break;
            }
            
            case 7: {
                std::cout << "Funcionalidade de atualização automática ainda não implementada.\n";
                // No futuro: buscarValoresOnline para cada ativo
                break;
            }

            case 8: {
                std::string ticker, corretora;
                std::cout << "Ticker: "; std::cin >> ticker;
                std::cout << "Corretora: "; std::cin.ignore(); std::getline(std::cin, corretora);
                Ativo* ativo = minhaCarteira.buscarAtivo(paraMaiusculas(ticker), padronizarNome(corretora));
                if (ativo) {
                    // Carrega histórico de valores se existir
                    ativo->carregarHistoricoValor(ticker + "_valores.csv");
                    auto rentabilidades = ativo->rentabilidadeMensal();
                    if (rentabilidades.empty()) {
                        std::cout << "Não há dados suficientes para calcular rentabilidade.\n";
                    } else {
                        std::cout << "Rentabilidade mensal para " << ticker << ":\n";
                        for (const auto& [mes, rent] : rentabilidades) {
                            std::cout << mes << ": " << rent << "%\n";
                        }
                    }
                } else {
                    std::cout << "Ativo não encontrado.\n";
                }
                break;
            }
            
            case 9: {
                std::string ticker, corretora;
                std::cout << "Ticker: "; std::cin >> ticker;
                std::cout << "Corretora: "; std::cin.ignore(); std::getline(std::cin, corretora);
                Ativo* ativo = minhaCarteira.buscarAtivo(paraMaiusculas(ticker), padronizarNome(corretora));
                    if (ativo) {
                        std::cout << "proventos para " << ticker;
                        for (const auto& prov : ativo->proventos) {
                            std::cout << "Tipo: " << prov.tipo
                            << " | Bruto: " << prov.valorBruto
                            << " | Líquido: " << prov.valorLiquido
                            << " | Data COM: " << prov.dataCom
                            << " | Pagamento: " << prov.dataPagamento << "\n";
                        }
                    } else {
                        std::cout << "Ativo não encontrado.\n";
                    }
                    break;
            } 
            
            case 10: {
                std::string ticker, corretora;
                std::cout << "Ticker: "; std::cin >> ticker;
                std::cout << "Corretora: "; std::cin.ignore(); std::getline(std::cin, corretora);
                Ativo* ativo = minhaCarteira.buscarAtivo(paraMaiusculas(ticker), padronizarNome(corretora));
                if (ativo && !ativo->historico_valor.empty()) {
                    auto& hist = ativo->historico_valor;
                    auto primeiro = hist.begin();
                    auto ultimo = std::prev(hist.end());
                    double rent = ((ultimo->second - primeiro->second) / primeiro->second) * 100.0;
                    std::cout << "Rentabilidade acumulada de " << primeiro->first << " até " << ultimo->first << ": " << rent << "%\n";
                } else {
                    std::cout << "Ativo não encontrado ou sem histórico.\n";
                }
                break;
            }

            case 11:
                minhaCarteira.salvarCarteira(CAMINHO_CARTEIRA);
                inputManager.salvarHistoricoOrdens(CAMINHO_ORDENS, minhaCarteira.getOrdens());
                std::cout << "Saindo... Carteira e histórico salvos.\n";
                break;

            default:
                std::cout << "Opção inválida.\n";
        }
    } while (escolha != 11);

    return 0;
}

