#include "inputmanager.hpp"
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

void InputManager::inserirManual(Carteira& carteira) {
    std::string ticker, tipoStr, corretora;
    double quantidade, preco;

    std::cout << "Ticker: ";
    std::cin >> ticker;
    ticker = paraMaiusculas(ticker);

    std::cout << "Tipo (ACAO/ETF/FII/FIIAGRO/FIIINFRA/REIT/TESOURO/BDR/CRIPTO): ";
    std::cin >> tipoStr;
    tipoStr = paraMaiusculas(tipoStr);

    std::cout << "Corretora: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string inputCorretora;
    std::getline(std::cin, inputCorretora);
    corretora = padronizarNome(inputCorretora);

    std::cout << "Quantidade: ";
    quantidade = lerDouble();

    std::cout << "Preço médio: ";
    preco = lerDouble();

    try {
        Ativo novoAtivo = {
            ticker,
            tipoDeString(tipoStr),
            corretora,
            quantidade,
            preco
        };
        carteira.adicionarAtivo(novoAtivo);
        std::cout << "✅ Ativo adicionado com sucesso!\n";
        carteira.salvarCSV("carteira.csv");
        std::cout << "💾 Carteira salva automaticamente em 'carteira.csv'.\n";

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
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



