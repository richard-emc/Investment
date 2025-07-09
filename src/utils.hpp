// utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "ativo.hpp"

std::string paraMaiusculas(const std::string& str);
std::string padronizarNome(const std::string& entrada);
double lerDouble();
bool dataValida(const std::string& data);
TipoAtivo tipoDeString(const std::string& str);

void buscarValoresOnline(const std::string& ticker, std::map<std::string, double>& historico_valor);

#endif