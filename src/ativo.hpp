#ifndef ATIVO_HPP
#define ATIVO_HPP

#include <string>
#include <map>

enum class TipoAtivo {
    ACAO,
    ETF,
    REIT,
    FII,
    FIIAGRO,
    FIIINFRA,
    TESOURO,
    BDR,
    CRIPTO
};

struct Ativo {
    std::string ticker;
    TipoAtivo tipo;
    std::string corretora;
    double quantidade;
    double preco_medio;

    void comprar(double qtd, double preco) {
        double totalInvestido = preco_medio * quantidade + preco * qtd;
        quantidade += qtd;
        preco_medio = totalInvestido / quantidade;
    }

        // Retorna lucro/prejuízo da venda
    double vender(double qtd, double preco) {
        double lucroPrejuizo = (preco - preco_medio) * qtd;
        quantidade -= qtd;
        return lucroPrejuizo;
    }
    // Histórico de preço total por mês: {"2025-05": 1200.0}
    std::map<std::string, double> historico_valor;

    // Dividendos por mês: {"2025-05": 45.0}
    std::map<std::string, double> dividendos;

};

#endif
