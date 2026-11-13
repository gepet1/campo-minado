#ifndef Tabuleiro_h
#define Tabuleiro_h

#include <vector>
#include <utility>
#include "celula.h"

class Tabuleiro {
private:
  std::vector<std::vector<Celula>> matriz;
  int num_minas;
  int num_bandeiras;
  int num_reveladas;
  bool revelou_mina;
  bool minas_sorteadas;   // as minas so sao colocadas na primeira jogada
  unsigned int semente;

  void sortear_mina(int linha_segura, int coluna_segura);
  void calcular_vizinhas();
  void cascata(int linha, int coluna);
  void imprimir_celula(int linha, int coluna, bool mostrar_tudo) const;
  void imprimir(bool mostrar_tudo) const;

public:
  // Construtor usado no jogo: as minas sao sorteadas na primeira revelacao.
  Tabuleiro(int linhas, int colunas, int minas, unsigned int semente);

  // Construtor usado nos testes: as minas ficam nas posicoes informadas.
  Tabuleiro(int linhas, int colunas, const std::vector<std::pair<int,int>>& posicoes_minas);

  int num_linhas() const;
  int num_colunas() const;
  bool verificar_posicao(int linha, int coluna) const;
  const Celula& celula(int linha, int coluna) const;
  int celulas_reveladas() const;
  int minas_faltando() const;

  bool revelacao(int linha, int coluna);
  bool colocar_flag(int linha, int coluna);
  bool vitoria() const;
  bool derrota() const;

  void imprimir_tabuleiro() const;
  void imprimir_revelado() const;
};

#endif
