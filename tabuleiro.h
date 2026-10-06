#ifndef Tabuleiro_h
#define Tabuleiro_h

#include <vector>
#include "celula.h"

class Tabuleiro {
private:
  std::vector<std::vector<Celula>> matriz; 
  int num_minas;
  int num_bandeiras;
  bool revelou_mina;
  
  bool verificar_posicao(int linha,int coluna) const;
  void sortear_mina();
  int calcular_vizinhas();
public:
  Tabuleiro(int linhas,int coluas, int minas);
  
  void imprimir_tabuleiro() const; 
  int minas_faltando();
  void revelacao(int linha,int coluna);
  bool colocar_flag(int linha,int coluna);
  bool vitoria();
  bool derrota();
};

#endif