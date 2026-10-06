#include <iostream>
#include "tabuleiro.h"
#include <vector>

Tabuleiro::Tabuleiro(int linhas,int colunas, int minas) {
  for(int i=0; i<linhas; i++) {
    matriz.push_back(std::vector<Celula> ());
    for(int j=0; j<colunas; j++)
      matriz[i].push_back(Celula());
  }
  num_minas=minas;
  num_bandeiras=0;
  revelou_mina=false;
}

void Tabuleiro::imprimir_tabuleiro() const {
  std::cout << "  ";
  for(std::size_t i=0; i<matriz[0].size(); i++)
    std::cout << i << ' ';
  std::cout << '\n';

  for(std::size_t i=0; i<matriz.size(); i++) {
    std::cout << i << ' ';
    for(std::size_t j=0; j<matriz[0].size(); j++) {
      if(matriz[i][j].verificar_flag())
        std::cout << "F ";
      else if(!matriz[i][j].esta_revelada())
        std::cout << "# ";
      else {
        int num = matriz[i][j].verificar_vizinhas();
        if(num==0)
          std::cout << ". ";
        else
          std::cout << num << ' ';  
      }  
    }
    std::cout << '\n'; 
  }
}

