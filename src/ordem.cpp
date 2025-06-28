#include "ordem.hpp"
#include <sstream>
#include <stdexcept>

Ordem Ordem::fromCSV(const std::string& linha) {
    std::istringstream ss(linha);
    std::string data, ticker, tipoStr, corretora, qtdStr, precoStr;

    std::getline(ss, data, ',');
    std::getline(ss, ticker, ',');
    std::getline(ss, tipoStr, ',');
    std::getline(ss, corretora, ',');
    std::getline(ss, qtdStr, ',');
    std::getline(ss, precoStr, ',');

    Ordem ordem;
    ordem.data = data;
    ordem.ticker = ticker;
    ordem.corretora = corretora;
    ordem.quantidade = std::stod(qtdStr);
    ordem.preco = std::stod(precoStr);

    if (tipoStr == "COMPRA") ordem.tipo = TipoOrdem::COMPRA;
    else if (tipoStr == "VENDA") ordem.tipo = TipoOrdem::VENDA;
    else throw std::invalid_argument("Tipo de ordem inválido: " + tipoStr);

    return ordem;
}
