#include "carteira.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

// Função para formatar double com vírgula decimal
std::string doubleToStringBR(double valor, int casasDecimais = 2) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(casasDecimais) << valor;
    std::string str = ss.str();
    // trocar ponto por vírgula
    std::replace(str.begin(), str.end(), '.', ',');
    return str;
}

std::string tipoParaString(TipoAtivo tipo) {
    switch (tipo) {
        case TipoAtivo::ACAO: return "ACAO";
        case TipoAtivo::ETF: return "ETF";
        case TipoAtivo::REIT: return "REIT";
        case TipoAtivo::FII: return "FII";
        case TipoAtivo::FIIAGRO: return "FIIAGRO";
        case TipoAtivo::FIIINFRA: return "FIIINFRA";
        case TipoAtivo::TESOURO: return "TESOURO";
        case TipoAtivo::BDR: return "BDR";
        case TipoAtivo::CRIPTO: return "CRIPTO";
        default: return "Desconhecido";
    }
}

void Carteira::adicionarAtivo(const Ativo& ativo) {
    ativos.push_back(ativo);
}

void Carteira::listarAtivos() const {
    for (const auto& ativo : ativos) {
        std::cout << "Ticker: " << ativo.ticker
                  << " | Tipo: " << tipoParaString(ativo.tipo)
                  << " | Corretora: " << ativo.corretora
                  << " | Quantidade: " << ativo.quantidade
                  << " | Preço médio: " << doubleToStringBR(ativo.preco_medio) << "\n";

    }
}

void Carteira::salvarCSV(const std::string& caminhoArquivo) const {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar.\n";
        return;
    }

    arquivo << "TICKER,TIPO,CORRETORA,QUANTIDADE,PRECOMEDIO\n";

    for (const auto& ativo : ativos) {
        arquivo << ativo.ticker << ","
                << tipoParaString(ativo.tipo) << ","
                << ativo.corretora << ","
                << ativo.quantidade << ","
                << std::fixed << std::setprecision(2) << ativo.preco_medio
                << "\n";
    }
}

