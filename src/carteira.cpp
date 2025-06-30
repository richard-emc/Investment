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

bool Carteira::aplicarOrdem(const Ordem& ordem) {
    Ativo* ativo = buscarAtivo(ordem.ticker, ordem.corretora);

    if (ordem.tipo == TipoOrdem::COMPRA) {
        if (ativo) {
            ativo->atualizarCompra(ordem.quantidade, ordem.preco);
        } else {
            // Criar novo ativo na carteira
            Ativo novo;
            novo.ticker = ordem.ticker;
            novo.corretora = ordem.corretora;
            novo.quantidade = ordem.quantidade;
            novo.preco_medio = ordem.preco;
            // Tipo idealmente viria da ordem ou input do usuário (pode ajustar)
            novo.tipo = TipoAtivo::ACAO;  // padrão ou parametrizar
            adicionarAtivo(novo);
        }
            // Se aplicada com sucesso, adiciona a ordem no histórico
        ordens.push_back(ordem);
        return true;
        } else if (ordem.tipo == TipoOrdem::VENDA) {
    if (!ativo) {
        std::cerr << "Erro: Tentando vender ativo não existente.\n";
        return false;
    }
    if (!ativo->atualizarVenda(ordem.quantidade)) {
        std::cerr << "Erro: Quantidade a vender maior que disponível.\n";
        return false;
    }
    // Adiciona a ordem de venda no histórico
    ordens.push_back(ordem);
    // Se quantidade zerada, pode remover ativo (implementação opcional)
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

// Retorna referência constante para o vetor de ordens
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
                << std::fixed << std::setprecision(2) << ativo.preco_medio
                << "\n";
    }
}

