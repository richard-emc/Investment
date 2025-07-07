#include "ativo.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

void Ativo::salvarHistoricoValor(const std::string& caminho) const {
    std::ofstream arq(caminho);
    if (!arq.is_open()) return;
    arq << "MES,VALOR\n";
    for (const auto& [mes, valor] : historico_valor) {
        arq << mes << "," << valor << "\n";
    }
}

void Ativo::carregarHistoricoValor(const std::string& caminho) {
    std::ifstream arq(caminho);
    if (!arq.is_open()) return;
    std::string linha;
    std::getline(arq, linha); // cabeçalho
    while (std::getline(arq, linha)) {
        size_t pos = linha.find(',');
        if (pos == std::string::npos) continue;
        std::string mes = linha.substr(0, pos);
        double valor = std::stod(linha.substr(pos+1));
        historico_valor[mes] = valor;
    }
}

    // Registra o valor mensal do ativo
    void Ativo::registrarValorMensal(const std::string& mes, double valor) {
    historico_valor[mes] = valor;
}

    std::vector<std::pair<std::string, double>> Ativo::rentabilidadeMensal() const {
    std::vector<std::pair<std::string, double>> resultado;
    if (historico_valor.size() < 2) return resultado;

    // Ordena os meses
    std::vector<std::string> meses;
    for (const auto& par : historico_valor) meses.push_back(par.first);
    std::sort(meses.begin(), meses.end());

    for (size_t i = 1; i < meses.size(); ++i) {
        double valorAnterior = historico_valor.at(meses[i-1]);
        double valorAtual = historico_valor.at(meses[i]);
        if (valorAnterior != 0) {
            double rent = ((valorAtual - valorAnterior) / valorAnterior) * 100.0;
            resultado.emplace_back(meses[i], rent);
        }
    }
    return resultado;
}