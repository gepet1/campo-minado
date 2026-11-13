// Testes automaticos do jogo. Para compilar e rodar: make test
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "recordes.h"
#include "tabuleiro.h"

// Tabuleiro 3x3 com minas em (0,0) e (0,1):
//   * * 1
//   2 2 1
//   . . .
void teste_vizinhas() {
  Tabuleiro tab(3, 3, {{0,0}, {0,1}});
  assert(tab.celula(0,2).verificar_vizinhas()==1);   // canto
  assert(tab.celula(2,2).verificar_vizinhas()==0);   // canto
  assert(tab.celula(1,0).verificar_vizinhas()==2);   // borda
  assert(tab.celula(2,1).verificar_vizinhas()==0);   // borda
  assert(tab.celula(1,1).verificar_vizinhas()==2);   // meio
  assert(tab.minas_faltando()==2);
  std::cout << "teste_vizinhas: OK\n";
}

void teste_posicao() {
  Tabuleiro tab(3, 4, {{0,0}});
  assert(tab.verificar_posicao(0,0));
  assert(tab.verificar_posicao(2,3));
  assert(!tab.verificar_posicao(3,0));
  assert(!tab.verificar_posicao(0,4));
  assert(!tab.verificar_posicao(-1,2));
  assert(!tab.revelacao(5,5));   // fora do tabuleiro: nada acontece
  assert(tab.celulas_reveladas()==0);
  std::cout << "teste_posicao: OK\n";
}

void teste_derrota() {
  Tabuleiro tab(3, 3, {{1,1}});
  assert(!tab.derrota());
  assert(tab.revelacao(1,1));
  assert(tab.derrota());
  assert(!tab.vitoria());
  std::cout << "teste_derrota: OK\n";
}

// Tabuleiro 5x5 com a linha 2 inteira de minas. Revelar (0,0) abre as linhas 0 e 1,
// mas a cascata nao atravessa as minas, entao as linhas 3 e 4 continuam escondidas.
void teste_cascata() {
  Tabuleiro tab(5, 5, {{2,0}, {2,1}, {2,2}, {2,3}, {2,4}});
  assert(tab.revelacao(0,0));
  assert(tab.celulas_reveladas()==10);
  assert(tab.celula(1,2).esta_revelada());
  assert(tab.celula(1,2).verificar_vizinhas()==3);
  assert(!tab.celula(2,2).esta_revelada());
  assert(!tab.celula(3,0).esta_revelada());
  assert(!tab.vitoria() && !tab.derrota());

  assert(tab.revelacao(4,4));
  assert(tab.celulas_reveladas()==20);
  assert(tab.vitoria());
  std::cout << "teste_cascata: OK\n";
}

// A cascata nao revela celulas com bandeira.
void teste_cascata_bandeira() {
  Tabuleiro tab(5, 5, {{4,4}});
  assert(tab.colocar_flag(0,2));
  assert(tab.revelacao(0,0));
  assert(!tab.celula(0,2).esta_revelada());
  assert(tab.celula(0,2).verificar_flag());
  assert(tab.celulas_reveladas()==23);
  assert(!tab.vitoria());

  assert(!tab.revelacao(0,2));     // com bandeira nao pode revelar
  assert(tab.colocar_flag(0,2));   // tira a bandeira
  assert(tab.revelacao(0,2));
  assert(tab.vitoria());
  std::cout << "teste_cascata_bandeira: OK\n";
}

void teste_bandeira() {
  Tabuleiro tab(3, 3, {{0,0}, {2,2}});
  assert(tab.minas_faltando()==2);
  assert(tab.colocar_flag(0,0));
  assert(tab.minas_faltando()==1);
  assert(tab.colocar_flag(0,0));   // tira a bandeira
  assert(tab.minas_faltando()==2);
  assert(!tab.colocar_flag(3,3));  // fora do tabuleiro
  assert(tab.minas_faltando()==2);

  assert(tab.revelacao(1,1));
  assert(!tab.colocar_flag(1,1));  // celula ja revelada
  assert(!tab.revelacao(1,1));     // revelar de novo nao conta
  assert(tab.celulas_reveladas()==1);
  std::cout << "teste_bandeira: OK\n";
}

void teste_vitoria() {
  Tabuleiro tab(2, 2, {{0,0}});
  assert(tab.revelacao(0,1));
  assert(tab.revelacao(1,0));
  assert(!tab.vitoria());
  assert(tab.revelacao(1,1));
  assert(tab.vitoria());
  assert(!tab.derrota());
  std::cout << "teste_vitoria: OK\n";
}

// A primeira jogada nunca encontra uma mina, para varias sementes.
void teste_primeira_jogada() {
  for(unsigned int semente=0; semente<300; semente++) {
    Tabuleiro tab(9, 9, 10, semente);
    assert(tab.revelacao(4,4));
    assert(!tab.derrota());

    int minas=0;
    for(int i=0; i<9; i++)
      for(int j=0; j<9; j++)
        if(tab.celula(i,j).tem_mina())
          minas++;
    assert(minas==10);
  }

  // Com 80 minas em 81 celulas, a unica celula segura e a da primeira jogada.
  for(unsigned int semente=0; semente<20; semente++) {
    Tabuleiro tab(9, 9, 80, semente);
    assert(tab.revelacao(0,8));
    assert(tab.vitoria());
  }
  std::cout << "teste_primeira_jogada: OK\n";
}

void teste_recordes_ordem() {
  Recordes rec("testes/recordes_teste.txt");
  assert(rec.adicionar("Beginner", "Ana", 30.0)==1);
  assert(rec.adicionar("Beginner", "Bia", 10.0)==1);
  assert(rec.adicionar("Beginner", "Caio", 20.0)==2);
  assert(rec.adicionar("Medium", "Davi", 50.0)==1);

  std::vector<Recorde> lista = rec.lista("Beginner");
  assert(lista.size()==3);
  assert(lista[0].nome=="Bia" && lista[1].nome=="Caio" && lista[2].nome=="Ana");
  assert(rec.lista("Medium").size()==1);
  assert(rec.lista("Expert").empty());
  std::cout << "teste_recordes_ordem: OK\n";
}

void teste_recordes_top5() {
  Recordes rec("testes/recordes_teste.txt");
  double tempos[] = {40, 10, 70, 20, 60, 30, 50};
  for(double t : tempos)
    rec.adicionar("Expert", "Jogador " + std::to_string(static_cast<int>(t)), t);

  std::vector<Recorde> lista = rec.lista("Expert");
  assert(lista.size()==5);
  assert(lista[0].tempo==10 && lista[4].tempo==50);

  assert(!rec.entra_na_lista("Expert", 55));
  assert(rec.adicionar("Expert", "Lento", 55)==0);
  assert(rec.entra_na_lista("Expert", 15));
  assert(rec.adicionar("Expert", "Rapido", 15)==2);
  assert(rec.lista("Expert").back().tempo==40);
  std::cout << "teste_recordes_top5: OK\n";
}

void teste_recordes_arquivo() {
  const std::string arquivo = "testes/recordes_teste.txt";
  Recordes rec(arquivo);
  rec.adicionar("Beginner", "Maria Clara", 12.5);
  rec.adicionar("Beginner", "Joao", 8.25);
  rec.salvar();

  Recordes lido(arquivo);
  lido.carregar();
  std::vector<Recorde> lista = lido.lista("Beginner");
  assert(lista.size()==2);
  assert(lista[0].nome=="Joao" && lista[0].tempo==8.25);
  assert(lista[1].nome=="Maria Clara");   // nome com espaco
  std::remove(arquivo.c_str());
  std::cout << "teste_recordes_arquivo: OK\n";
}

int main() {
  teste_vizinhas();
  teste_posicao();
  teste_derrota();
  teste_cascata();
  teste_cascata_bandeira();
  teste_bandeira();
  teste_vitoria();
  teste_primeira_jogada();
  teste_recordes_ordem();
  teste_recordes_top5();
  teste_recordes_arquivo();
  std::cout << "\nTodos os testes passaram.\n";
  return 0;
}
