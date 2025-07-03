#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include "ativo.hpp"
#include "ordem.hpp"
#include <vector>
#include <string>

class Carteira {
public:
    void limpar();
    void adicionarAtivo(const Ativo& ativo);
    void listarAtivos() const;
    void salvarCSV(const std::string& caminhoArquivo) const;
    void carregarCSV(const std::string& caminhoArquivo);
    // Getter para o vetor de ordens
    const std::vector<Ordem>& getOrdens() const;
    std::vector<Ordem> buscarOrdensPorTicker(const std::string& ticker) const;



    // Busca ativo pelo ticker e corretora, retorna ponteiro ou nullptr
    Ativo* buscarAtivo(const std::string& ticker, const std::string& corretora);

    // Aplica uma ordem (compra ou venda) à carteira; retorna true se sucesso
    bool aplicarOrdem(const Ordem& ordem, TipoAtivo tipoAtivo = TipoAtivo::ACAO);

private:
    std::vector<Ativo> ativos;
    std::vector<Ordem> ordens;
};
    std::string tipoParaString(TipoAtivo tipo);

#endif
