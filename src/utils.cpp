#include "utils.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>
#include <regex>

//Padronizar nomes de ativos e corretoras
std::string paraMaiusculas(const std::string& str) {
    std::string resultado = str;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return resultado;
}

// Padroniza nomes de corretoras, capitalizando a primeira letra de cada palavra
std::string padronizarNome(const std::string& entrada) {
    std::stringstream ss(entrada);
    std::string palavra;
    std::string resultado;

    while (ss >> palavra) {
        if (!palavra.empty()) {
            palavra[0] = std::toupper(palavra[0]);
            for (size_t i = 1; i < palavra.size(); ++i)
                palavra[i] = std::tolower(palavra[i]);
            if (!resultado.empty()) resultado += " ";
            resultado += palavra;
        }
    }
    return resultado;
}

// Lê um double do usuário, aceitando vírgula ou ponto
double lerDouble() {
    std::string entrada;
    std::getline(std::cin, entrada);
    // Trocar vírgula por ponto
    std::replace(entrada.begin(), entrada.end(), ',', '.');

    std::stringstream ss(entrada);
    double valor;
    ss >> valor;
    if (ss.fail()) {
        throw std::invalid_argument("Número inválido: " + entrada);
    }
    return valor;
}

// Valida data no formato YYYY-MM-DD
bool dataValida(const std::string& data) {
    // Expressão regular para YYYY-MM-DD
    std::regex re(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(data, re)) return false;

    int ano, mes, dia;
    char sep1, sep2;
    std::istringstream iss(data);
    if (!(iss >> ano >> sep1 >> mes >> sep2 >> dia)) return false;
    if (sep1 != '-' || sep2 != '-') return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false; // Para simplificação, não verifica meses de 30/31 dias ou ano bissexto
    return true;
}

// Converte string para enum TipoAtivo
TipoAtivo tipoDeString(const std::string& str) {
    if (str == "ACAO") return TipoAtivo::ACAO;
    if (str == "ETF") return TipoAtivo::ETF;
    if (str == "FII") return TipoAtivo::FII;
    if (str == "FIIAGRO") return TipoAtivo::FIIAGRO;
    if (str == "FIIINFRA") return TipoAtivo::FIIINFRA;
    if (str == "REIT") return TipoAtivo::REIT;
    if (str == "TESOURO") return TipoAtivo::TESOURO;
    if (str == "BDR") return TipoAtivo::BDR;
    if (str == "CRIPTO") return TipoAtivo::CRIPTO;
    throw std::invalid_argument("Tipo de ativo inválido: " + str);
}