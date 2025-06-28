#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "carteira.hpp"
#include <string>

class InputManager {
public:
    static void inserirManual(Carteira& carteira);
    static void carregarCSV(Carteira& carteira, const std::string& caminhoArquivo);
};

#endif
