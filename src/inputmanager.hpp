#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "carteira.hpp"
#include <string>

class InputManager {
public:
    void inserirOrdemManual(Carteira& carteira);
    void carregarCarteira(Carteira& carteira, const std::string& caminhoArquivo);
    void carregarHistoricoDeOrdens(Carteira& carteira, const std::string& caminhoHistorico);
    void salvarHistoricoOrdens(const std::string& caminhoArquivo, const std::vector<Ordem>& ordens);
};

std::string paraMaiusculas(const std::string& str);

#endif
