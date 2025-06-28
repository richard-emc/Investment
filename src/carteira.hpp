#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include "ativo.hpp"
#include <vector>

class Carteira {
public:
    void adicionarAtivo(const Ativo& ativo);
    void listarAtivos() const;

private:
    std::vector<Ativo> ativos;
};

#endif
