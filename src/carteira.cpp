#include "carteira.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

void Carteira::limpar() {
    ativos.clear();
    ordens.clear();
}

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

Ativo* Carteira::buscarAtivo(const std::string& ticker, const std::string& corretora) {
    for (auto& ativo : ativos) {
        if (ativo.ticker == ticker && ativo.corretora == corretora) {
            return &ativo;
        }
    }
    return nullptr;
}

bool Carteira::aplicarOrdem(const Ordem& ordem, TipoAtivo tipoAtivo) {
    Ativo* ativo = buscarAtivo(ordem.ticker, ordem.corretora);

    if (ordem.tipo == TipoOrdem::COMPRA) {
        if (ativo) {
            ativo->comprar(ordem.quantidade, ordem.preco);
        } else {
            // Cria novo ativo com o tipo informado
            Ativo novoAtivo;
            novoAtivo.ticker = ordem.ticker;
            novoAtivo.tipo = tipoAtivo;
            novoAtivo.corretora = ordem.corretora;
            novoAtivo.quantidade = 0;
            novoAtivo.preco_medio = 0;
            novoAtivo.comprar(ordem.quantidade, ordem.preco);
            ativos.push_back(novoAtivo);
        }
        ordens.push_back(ordem);
        return true;
    } else if (ordem.tipo == TipoOrdem::VENDA) {
        if (!ativo) {
            std::cout << "Ativo não encontrado para venda.\n";
            return false;
        }
        if (ordem.quantidade > ativo->quantidade) {
            std::cout << "Quantidade insuficiente para venda.\n";
            return false;
        }

        double lucroPrejuizo = ativo->vender(ordem.quantidade, ordem.preco);
        std::cout << "Lucro/Prejuízo nesta venda: " << doubleToStringBR(lucroPrejuizo) << "\n";

        // Remove ativo se zerou
        if (ativo->quantidade == 0) {
            ativos.erase(std::remove_if(ativos.begin(), ativos.end(),
                [&](const Ativo& a) {
                    return a.ticker == ativo->ticker && a.corretora == ativo->corretora;
                }), ativos.end());
        }

        ordens.push_back(ordem);
        return true;
    }
    return false;
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

std::vector<Ordem> Carteira::buscarOrdensPorTicker(const std::string& ticker) const {
    std::vector<Ordem> resultado;
    for (const auto& ordem : ordens) {
        if (ordem.ticker == ticker) {
            resultado.push_back(ordem);
        }
    }
    return resultado;
}

const std::vector<Ordem>& Carteira::getOrdens() const {
    return ordens;
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
                << doubleToStringBR(ativo.preco_medio)
                << "\n";
    }
}

