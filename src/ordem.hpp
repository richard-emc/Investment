#ifndef ORDEM_HPP
#define ORDEM_HPP

#include <string>
#include "ativo.hpp"

enum class TipoOrdem {
    COMPRA,
    VENDA
};

struct Ordem {
    std::string data;    // Exemplo: "2025-06-28"
    std::string ticker;
    TipoOrdem tipo;
    std::string corretora;
    double quantidade;
    double preco;

    std::string toCSV() const {
        return data + "," + ticker + "," + (tipo == TipoOrdem::COMPRA ? "COMPRA" : "VENDA") + "," +
               corretora + "," + std::to_string(quantidade) + "," + std::to_string(preco);
    }

    static Ordem fromCSV(const std::string& linha);
};

#endif
