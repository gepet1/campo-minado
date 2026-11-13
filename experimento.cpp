// Experimento: mede a cascata num tabuleiro 1000x1000 com uma unica mina.
// Revelar o canto (0,0) abre quase o tabuleiro inteiro numa so jogada.
// Para compilar e rodar: make experimento
#include <chrono>
#include <iostream>
#include "tabuleiro.h"

int main() {
  const int N = 1000;
  Tabuleiro tab(N, N, {{N-1, N-1}});

  auto inicio = std::chrono::steady_clock::now();
  tab.revelacao(0, 0);
  auto fim = std::chrono::steady_clock::now();
  double ms = std::chrono::duration<double, std::milli>(fim-inicio).count();

  std::cout << "Tabuleiro " << N << "x" << N << " com 1 mina\n";
  std::cout << "Celulas reveladas: " << tab.celulas_reveladas() << " de " << N*N-1 << '\n';
  std::cout << "Vitoria: " << (tab.vitoria() ? "sim" : "nao") << '\n';
  std::cout << "Tempo da cascata: " << ms << " ms\n";
  return 0;
}
