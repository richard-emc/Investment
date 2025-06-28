#include "carteira.hpp"
#include "ativo.hpp"

int main() {
    Carteira carteira;

    Ativo a1 = {"PETR4", TipoAtivo::ACAO, "Clear", 100, 28.5};
    Ativo a2 = {"IVVB11", TipoAtivo::ETF, "XP", 10, 270.0};

    carteira.adicionarAtivo(a1);
    carteira.adicionarAtivo(a2);

    carteira.listarAtivos();

    return 0;
}
