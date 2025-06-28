#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include "ativo.hpp"
#include "ordem.hpp"
#include <vector>

class Carteira {
public:
    void adicionarAtivo(const Ativo& ativo);
    void listarAtivos() const;
    void salvarCSV(const std::string& caminhoArquivo) const;
    void carregarCSV(const std::string& caminhoArquivo);

    // Busca ativo pelo ticker e corretora, retorna ponteiro ou nullptr
    Ativo* buscarAtivo(const std::string& ticker, const std::string& corretora);

    // Aplica uma ordem (compra ou venda) à carteira; retorna true se sucesso
    bool aplicarOrdem(const Ordem& ordem);

private:
    std::vector<Ativo> ativos;
};
    std::string tipoParaString(TipoAtivo tipo);

#endif
