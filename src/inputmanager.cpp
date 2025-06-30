#include "inputmanager.hpp"
#include "carteira.hpp"
#include "ativo.hpp"
#include "ordem.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <limits>



std::string paraMaiusculas(const std::string& str) {
    std::string resultado = str;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return resultado;
}

// Função para ler um número com virgula ou ponto e retornar como double
double lerDouble() {
    std::string entrada;
    std::getline(std::cin, entrada);
    // Trocar vírgula por ponto
    std::replace(entrada.begin(), entrada.end(), ',', '.');

    std::stringstream ss(entrada);
    double valor;
    ss >> valor;
    if (ss.fail()) {
        throw std::invalid_argument("Número inválido: " + entrada);
    }
    return valor;
}

// Função que capitaliza as palavras
std::string padronizarNome(const std::string& entrada) {
    std::stringstream ss(entrada);
    std::string palavra;
    std::string resultado;

    while (ss >> palavra) {
        if (!palavra.empty()) {
            palavra[0] = std::toupper(palavra[0]);
            for (size_t i = 1; i < palavra.size(); ++i)
                palavra[i] = std::tolower(palavra[i]);
            if (!resultado.empty()) resultado += " ";
            resultado += palavra;
        }
    }
    return resultado;
}

TipoAtivo tipoDeString(const std::string& str) {
    if (str == "ACAO") return TipoAtivo::ACAO;
    if (str == "ETF") return TipoAtivo::ETF;
    if (str == "FII") return TipoAtivo::FII;
    if (str == "FIIAGRO") return TipoAtivo::FIIAGRO;
    if (str == "FIIINFRA") return TipoAtivo::FIIINFRA;
    if (str == "REIT") return TipoAtivo::REIT;
    if (str == "TESOURO") return TipoAtivo::TESOURO;
    if (str == "BDR") return TipoAtivo::BDR;
    if (str == "CRIPTO") return TipoAtivo::CRIPTO;
    throw std::invalid_argument("Tipo de ativo inválido: " + str);
}

void InputManager::inserirOrdemManual(Carteira& carteira) {
    Ordem ordem;
    std::string tipoStr;

    std::cout << "Data (YYYY-MM-DD): ";
    std::cin >> ordem.data;

    std::cout << "Ticker: ";
    std::cin >> ordem.ticker;
    // Transformar ticker em maiúsculo (exemplo)
    std::transform(ordem.ticker.begin(), ordem.ticker.end(), ordem.ticker.begin(), ::toupper);

    std::cout << "Tipo (COMPRA/VENDA): ";
    std::cin >> tipoStr;
    std::transform(tipoStr.begin(), tipoStr.end(), tipoStr.begin(), ::toupper);
    if (tipoStr == "COMPRA") ordem.tipo = TipoOrdem::COMPRA;
    else if (tipoStr == "VENDA") ordem.tipo = TipoOrdem::VENDA;
    else {
        std::cerr << "Tipo inválido!\n";
        return;
    }

    std::cout << "Corretora: ";
    std::cin.ignore();
    std::getline(std::cin, ordem.corretora);

    std::cout << "Quantidade: ";
    std::cin >> ordem.quantidade;

    std::cout << "Preço: ";
    std::cin >> ordem.preco;

    // Aplica ordem na carteira
    if (carteira.aplicarOrdem(ordem)) {
        std::cout << "Ordem aplicada com sucesso!\n";
    }
}

void InputManager::carregarHistoricoDeOrdens(Carteira& carteira, const std::string& caminhoHistorico) {
    std::ifstream arquivo(caminhoHistorico);
    if (!arquivo.is_open()) {
        std::cerr << "❌ Não foi possível abrir o histórico de ordens.\n";
        return;
    }

    carteira.limpar(); // Limpa ordens anteriores
    

    std::string linha;
    std::getline(arquivo, linha); // Ignora o cabeçalho

    while (std::getline(arquivo, linha)) {
        std::istringstream ss(linha);
        std::string tipoStr, ticker, corretora, quantidadeStr, precoStr, data;

        std::getline(ss, tipoStr, ',');
        std::getline(ss, ticker, ',');
        std::getline(ss, corretora, ',');
        std::getline(ss, quantidadeStr, ',');
        std::getline(ss, precoStr, ',');
        std::getline(ss, data);

        try {
            Ordem ordem;
            ordem.tipo = (paraMaiusculas(tipoStr) == "COMPRA") ? TipoOrdem::COMPRA : TipoOrdem::VENDA;
            ordem.ticker = paraMaiusculas(ticker);
            ordem.corretora = padronizarNome(corretora);
            ordem.quantidade = std::stod(quantidadeStr);
            ordem.preco = std::stod(precoStr);
            ordem.data = data;

            if (!carteira.aplicarOrdem(ordem)) {
                std::cerr << "⚠️ Ordem ignorada (não aplicada): " << linha << "\n";
            }

        } catch (const std::exception& e) {
            std::cerr << "❌ Erro ao processar linha: " << linha << " → " << e.what() << "\n";
        }
    }

    std::cout << "✅ Histórico de ordens carregado e aplicado com sucesso.\n";
}

void InputManager::salvarHistoricoOrdens(const std::string& caminhoArquivo, const std::vector<Ordem>& ordens) {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar histórico de ordens.\n";
        return;
    }

    arquivo << "TIPO,TICKER,CORRETORA,QUANTIDADE,PRECO,DATA\n";
    for (const auto& ordem : ordens) {
        arquivo << (ordem.tipo == TipoOrdem::COMPRA ? "COMPRA" : "VENDA") << ","
                << ordem.ticker << ","
                << ordem.corretora << ","
                << ordem.quantidade << ","
                << ordem.preco << ","
                << ordem.data << "\n";
    }
}

void InputManager::carregarCSV(Carteira& carteira, const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "❌ Não foi possível abrir o arquivo.\n";
        return;
    }

    std::string linha;
    std::string cabecalho;
    std::getline(arquivo, cabecalho);  // descarta o cabeçalho

    while (std::getline(arquivo, linha)) {
        std::istringstream ss(linha);
        std::string ticker, tipoStr, corretora;
        double quantidade, preco;

        std::getline(ss, ticker, ',');
        std::getline(ss, tipoStr, ',');
        std::getline(ss, corretora, ',');
        corretora = padronizarNome(corretora);
        ss >> quantidade;
        ss.ignore(); // ignora a vírgula
        ss >> preco;

        try {
            Ativo ativo = {
                ticker,
                tipoDeString(tipoStr),
                corretora,
                quantidade,
                preco
            };
            carteira.adicionarAtivo(ativo);
        } catch (const std::exception& e) {
            std::cerr << "Erro na linha: " << linha << " → " << e.what() << "\n";
        }
    }

    std::cout << "✅ Arquivo carregado com sucesso!\n";
    carteira.salvarCSV("carteira.csv");
    std::cout << "💾 Carteira atualizada e salva automaticamente.\n";

}



