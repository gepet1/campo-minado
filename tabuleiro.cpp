#include <iostream>
#include "tabuleiro.h"
#include <vector>
#include <random>

Tabuleiro::Tabuleiro(int linhas, int colunas, int minas, unsigned int semente)
  : matriz(linhas, std::vector<Celula>(colunas)), num_minas(minas),
    num_bandeiras(0), revelou_mina(false) {
  sortear_mina(semente);
  calcular_vizinhas();
}

void Tabuleiro::sortear_mina(unsigned int semente) {
  std::mt19937 mt {semente};
  std::uniform_int_distribution<> line{0,static_cast<int>(matriz.size())-1};
  std::uniform_int_distribution<> column{0,static_cast<int>(matriz[0].size())-1};
   
  for(int i=1; i<=num_minas; i++) {
    int posY = line(mt); int posX = column(mt);
    if(!matriz[posY][posX].tem_mina())
      matriz[posY][posX].definir_mina(true);
    else
      i--;  
  }

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

void Tabuleiro::imprimir_revelado() const {
  std::cout << "  ";
  for(std::size_t i=0; i<matriz[0].size(); i++)
    std::cout << i << ' ';
  std::cout << '\n';
 
  for(std::size_t i=0; i<matriz.size(); i++) {
    std::cout << i << ' ';
    for(std::size_t j=0; j<matriz[0].size(); j++) {
      if(matriz[i][j].tem_mina())
        std::cout << "* ";
      else if(matriz[i][j].verificar_vizinhas()==0)
        std::cout << ". ";
      else
        std::cout << matriz[i][j].verificar_vizinhas() << ' ';    
    }
    std::cout << '\n';
  }    
}

bool Tabuleiro::verificar_posicao(int linha, int coluna) const {
  int x = static_cast<int>(matriz[0].size());
  int y = static_cast<int>(matriz.size());

  if((linha>=0 && linha<y) && (coluna>=0 && coluna<x))
    return true;
  return false;  

}

void Tabuleiro::calcular_vizinhas() {
  int x[]={1,-1,0,0,-1,1,1,-1};
  int y[]={0,0,-1,1,1,-1,1,-1};

  int linhas = static_cast<int>(matriz.size());
  int colunas = static_cast<int>(matriz[0].size());

  for(int i=0; i<linhas; i++) {
    for(int j=0; j<colunas; j++) {
      int bombas=0;
      int dx,dy;
      for(int k=0; k<8;k++) {
        dx=x[k]+j;
        dy=y[k]+i;  
        if(verificar_posicao(dy,dx) && matriz[dy][dx].tem_mina())
          bombas++;
      } 
    matriz[i][j].definir_vizinhas(bombas);
    }
  }
}

void Tabuleiro::revelacao(int linha, int coluna) {
  if(verificar_posicao(linha,coluna)){
    if(matriz[linha][coluna].revelar() && matriz[linha][coluna].tem_mina())
      revelou_mina=true;
  }
}

bool Tabuleiro::colocar_flag(int linha, int coluna) {
  if(!matriz[linha][coluna].esta_revelada()) {
    matriz[linha][coluna].set_flag();
    num_bandeiras++;
    return true;
  }
  return false;  
}


int Tabuleiro::minas_faltando() const {
  return num_minas-num_bandeiras;
}