#include <iostream>
#include <sstream>
#include <string>
#include "tabuleiro.h"

// Conta as falhas, para o resumo no final.
int falhas = 0;

// Executa a impressao do tabuleiro e devolve, como string, o que ela escreveria na tela.
// Para isso, a saida do cout e desviada temporariamente para uma string.
std::string capturar_impressao(const Tabuleiro& t) {
  std::ostringstream saida;
  std::streambuf* original = std::cout.rdbuf(saida.rdbuf());
  t.imprimir_tabuleiro();
  std::cout.rdbuf(original);
  return saida.str();
}

// Remove os espacos no fim de cada linha e as linhas vazias no comeco e no fim.
// Assim, um espaco sobrando depois do ultimo # nao conta como erro.
std::string limpar(const std::string& texto) {
  std::istringstream entrada(texto);
  std::string linha;
  std::string resultado;
  while (std::getline(entrada, linha)) {
    while (!linha.empty() && linha.back() == ' ')
      linha.pop_back();
    if (linha.empty() && resultado.empty())
      continue;
    resultado += linha + '\n';
  }
  while (!resultado.empty() && resultado.back() == '\n')
    resultado.pop_back();
  return resultado;
}

// Compara a impressao do tabuleiro com o texto esperado e mostra OK ou FALHOU.
// Em caso de falha, mostra o esperado e o obtido, um abaixo do outro.
void confere_impressao(const std::string& descricao, const Tabuleiro& t, const std::string& esperado) {
  std::string obtido = limpar(capturar_impressao(t));
  if (obtido == esperado) {
    std::cout << "OK      - " << descricao << '\n';
  } else {
    std::cout << "FALHOU  - " << descricao << '\n';
    std::cout << "Esperado:\n" << esperado << '\n';
    std::cout << "Obtido:\n" << obtido << "\n\n";
    falhas++;
  }
}

int main() {
  // Parte 1: o tabuleiro Iniciante na tela, para conferir a olho.
  std::cout << "Tabuleiro Iniciante (9x9, 10 minas):\n\n";
  Tabuleiro iniciante(9, 9, 10);
  iniciante.imprimir_tabuleiro();
  std::cout << '\n';

  // Parte 2: comparacao automatica com o formato do roteiro.

  // Teste 1: um tabuleiro 3x3, igual ao desenho do roteiro.
  confere_impressao("tabuleiro 3x3", Tabuleiro(3, 3, 1),
                    "  0 1 2\n"
                    "0 # # #\n"
                    "1 # # #\n"
                    "2 # # #");

  // Teste 2: 2 linhas e 5 colunas. Se linhas e colunas estiverem trocadas
  // no construtor ou na impressao, este teste falha.
  confere_impressao("tabuleiro com 2 linhas e 5 colunas", Tabuleiro(2, 5, 1),
                    "  0 1 2 3 4\n"
                    "0 # # # # #\n"
                    "1 # # # # #");

  // Teste 3: o menor tabuleiro possivel.
  confere_impressao("tabuleiro 1x1", Tabuleiro(1, 1, 0),
                    "  0\n"
                    "0 #");

  // Teste 4: o tabuleiro Iniciante completo.
  confere_impressao("tabuleiro Iniciante 9x9", iniciante,
                    "  0 1 2 3 4 5 6 7 8\n"
                    "0 # # # # # # # # #\n"
                    "1 # # # # # # # # #\n"
                    "2 # # # # # # # # #\n"
                    "3 # # # # # # # # #\n"
                    "4 # # # # # # # # #\n"
                    "5 # # # # # # # # #\n"
                    "6 # # # # # # # # #\n"
                    "7 # # # # # # # # #\n"
                    "8 # # # # # # # # #");

  // Resumo final.
  std::cout << '\n';
  if (falhas == 0)
    std::cout << "Todos os testes passaram.\n";
  else
    std::cout << falhas << " teste(s) falharam.\n";

  return 0;
}
