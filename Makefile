# Define o compilador
CXX = g++

# Define flags para o compilador (avisos + C++17)
CXXFLAGS = -Wall -std=c++17

# Arquivos-fonte
SRC = src/main.cpp src/carteira.cpp src/inputmanager.cpp src/ordem.cpp src/ativo.hpp

# Nome do programa final
EXEC = carteira

# Regra principal
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)

# Regra para limpar os arquivos compilados
clean:
	rm -f $(EXEC)
