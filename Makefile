CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
DEBUGFLAGS = -g -fsanitize=address,undefined

FONTES = src/celula.cpp src/tabuleiro.cpp src/recordes.cpp
CABECALHOS = $(wildcard src/*.h)
EXEC = campo-minado

all: $(EXEC)

$(EXEC): $(FONTES) src/main.cpp $(CABECALHOS)
	$(CXX) $(CXXFLAGS) $(FONTES) src/main.cpp -o $(EXEC)

run: $(EXEC)
	./$(EXEC)

debug:
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(FONTES) src/main.cpp -o $(EXEC)-debug

test:
	$(CXX) $(CXXFLAGS) -Isrc $(FONTES) testes/teste_tabuleiro.cpp -o teste
	./teste

experimento:
	$(CXX) $(CXXFLAGS) -O2 -Isrc src/celula.cpp src/tabuleiro.cpp experimento.cpp -o experimento
	./experimento

clean:
	rm -f $(EXEC) $(EXEC)-debug teste experimento

.PHONY: all run debug test experimento clean
