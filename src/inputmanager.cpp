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
#include <regex>

bool dataValida(const std::string& data);

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

bool dataValida(const std::string& data) {
    // Expressão regular para YYYY-MM-DD
    std::regex re(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(data, re)) return false;

    int ano, mes, dia;
    char sep1, sep2;
    std::istringstream iss(data);
    if (!(iss >> ano >> sep1 >> mes >> sep2 >> dia)) return false;
    if (sep1 != '-' || sep2 != '-') return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false; // Para simplificação, não verifica meses de 30/31 dias ou ano bissexto
    return true;
}

void InputManager::inserirOrdemManual(Carteira& carteira) {
    Ordem ordem;
    std::string tipoStr, tipoAtivoStr;

    std::string data;
    do {
        std::cout << "Data (YYYY-MM-DD): ";
        std::cin >> data;
        if (!dataValida(data)) {
            std::cout << "Data inválida! Tente novamente.\n";
        }
    } while (!dataValida(data));
    ordem.data = data;

    std::cout << "Ticker: ";
    std::cin >> ordem.ticker;
    ordem.ticker = paraMaiusculas(ordem.ticker);

    std::cout << "Tipo (COMPRA/VENDA): ";
    std::cin >> tipoStr;
    tipoStr = paraMaiusculas(tipoStr);
    if (tipoStr == "COMPRA") ordem.tipo = TipoOrdem::COMPRA;
    else if (tipoStr == "VENDA") ordem.tipo = TipoOrdem::VENDA;
    else {
        std::cerr << "Tipo inválido!\n";
        return;
    }

    std::cout << "Corretora: ";
    std::cin.ignore();
    std::getline(std::cin, ordem.corretora);
    ordem.corretora = padronizarNome(ordem.corretora);

    if (ordem.tipo == TipoOrdem::COMPRA) {
        std::cout << "Tipo do ativo (ACAO/ETF/FII/FIIAGRO/FIIINFRA/REIT/TESOURO/BDR/CRIPTO): ";
        std::cin >> tipoAtivoStr;
        tipoAtivoStr = paraMaiusculas(tipoAtivoStr);
        try {
            ordem.tipoAtivo = tipoDeString(tipoAtivoStr);
        } catch (const std::exception& e) {
            std::cout << "Tipo de ativo inválido.\n";
            return;
        }
    } else { // VENDA
        // Busca o ativo na carteira para obter o tipo
        Ativo* ativo = carteira.buscarAtivo(ordem.ticker, ordem.corretora);
        if (!ativo) {
            std::cerr << "Ativo não encontrado para venda.\n";
            return;
        }
        ordem.tipoAtivo = ativo->tipo;
    }


    std::cout << "Quantidade: ";
    ordem.quantidade = lerDouble();
    
    std::cout << "Preço: ";
    ordem.preco = lerDouble();
    
    // Aplica ordem na carteira
    if (carteira.aplicarOrdem(ordem, ordem.tipoAtivo)) {
        std::cout << "Ordem aplicada com sucesso!\n";
    } else {
        std::cerr << "⚠️ Ordem não aplicada. Verifique os dados.\n";
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
        std::string tipoStr, ticker, tipoAtivoStr, corretora, quantidadeStr, precoStr, data;

        std::getline(ss, tipoStr, ',');
        std::getline(ss, ticker, ',');
        std::getline(ss, tipoAtivoStr, ',');
        std::getline(ss, corretora, ',');
        std::getline(ss, quantidadeStr, ',');
        std::getline(ss, precoStr, ',');
        std::getline(ss, data);

        if (!dataValida(data)) {
        std::cerr << "⚠️ Data inválida na linha: " << linha << "\n";
        continue;
        }

        try {
            Ordem ordem;
            ordem.tipo = (paraMaiusculas(tipoStr) == "COMPRA") ? TipoOrdem::COMPRA : TipoOrdem::VENDA;
            ordem.ticker = paraMaiusculas(ticker);
            ordem.tipoAtivo = tipoDeString(paraMaiusculas(tipoAtivoStr));
            ordem.corretora = padronizarNome(corretora);
            ordem.quantidade = std::stod(quantidadeStr);
            std::replace(precoStr.begin(), precoStr.end(), ',', '.');
            ordem.preco = std::stod(precoStr);
            ordem.data = data;

            if (!carteira.aplicarOrdem(ordem, ordem.tipoAtivo)) {
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

    arquivo << "TIPO,TICKER,TIPOAtIVO,CORRETORA,QUANTIDADE,PRECO,DATA\n";
    for (const auto& ordem : ordens) {
        arquivo << (ordem.tipo == TipoOrdem::COMPRA ? "COMPRA" : "VENDA") << ","
                << ordem.ticker << ","
                << tipoParaString(ordem.tipoAtivo) << ","
                << ordem.corretora << ","
                << ordem.quantidade << ","
                << ordem.preco << ","
                << ordem.data << "\n";
    }
}

void InputManager::carregarCarteira(Carteira& carteira, const std::string& caminhoArquivo) {
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
        std::string ticker, tipoStr, corretora, qtdStr, precoStr;

        if (!std::getline(ss, ticker, ',')) continue;
        if (!std::getline(ss, tipoStr, ',')) continue;
        if (!std::getline(ss, corretora, ',')) continue;
        if (!std::getline(ss, qtdStr, ',')) continue;
        if (!std::getline(ss, precoStr, ',')) continue;

        try {
            double quantidade = std::stod(qtdStr);
            // Troca vírgula por ponto antes de converter
            std::replace(precoStr.begin(), precoStr.end(), ',', '.');
            double preco_medio = std::stod(precoStr);

            Ativo novoAtivo;
            novoAtivo.ticker = paraMaiusculas(ticker);
            novoAtivo.tipo = tipoDeString(paraMaiusculas(tipoStr));
            novoAtivo.corretora = padronizarNome(corretora);
            novoAtivo.quantidade = quantidade;
            novoAtivo.preco_medio = preco_medio;
            carteira.adicionarAtivo(novoAtivo);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao ler linha do CSV: " << linha << "\n";
            continue;
        }
    }

    std::cout << "✅ Arquivo carregado com sucesso!\n";
    carteira.salvarCarteira("carteira.csv");
    std::cout << "💾 Carteira atualizada e salva automaticamente.\n";

}





