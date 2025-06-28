#include "carteira.hpp"
#include <iostream>

void Carteira::adicionarAtivo(const Ativo& ativo) {
    ativos.push_back(ativo);
}

void Carteira::listarAtivos() const {
    for (const auto& ativo : ativos) {
        std::cout << "Ticker: " << ativo.ticker
                  << " | Tipo: " << static_cast<int>(ativo.tipo)
                  << " | Corretora: " << ativo.corretora
                  << " | Quantidade: " << ativo.quantidade
                  << " | Preço Médio: R$ " << ativo.preco_medio << "\n";
    }
}
