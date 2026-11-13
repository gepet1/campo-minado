#include <iostream>
#include <iomanip>
#include <queue>
#include <random>
#include <string>
#include "tabuleiro.h"

// Deslocamentos que levam de uma celula ate cada uma das 8 vizinhas.
const int vizinho_coluna[] = {1,-1,0,0,-1,1,1,-1};
const int vizinho_linha[]  = {0,0,-1,1,1,-1,1,-1};

// Codigos de cor ANSI usados na impressao.
const std::string COR_NORMAL = "\033[0m";
const std::string COR_BANDEIRA = "\033[1;33m";
const std::string COR_MINA = "\033[1;31m";
const std::string COR_NUMERO[9] = {
  "",           // 0 nao e impresso como numero
  "\033[94m",   // 1 azul
  "\033[32m",   // 2 verde
  "\033[91m",   // 3 vermelho
  "\033[34m",   // 4 azul escuro
  "\033[31m",   // 5 vermelho escuro
  "\033[36m",   // 6 ciano
  "\033[35m",   // 7 roxo
  "\033[90m"    // 8 cinza
};

// Retorna quantos digitos o numero n tem (n >= 0).
int digitos(int n) {
  int quantidade = 1;
  while(n >= 10) {
    n /= 10;
    quantidade++;
  }
  return quantidade;
}

Tabuleiro::Tabuleiro(int linhas, int colunas, int minas, unsigned int semente)
  : matriz(linhas, std::vector<Celula>(colunas)), num_minas(minas),
    num_bandeiras(0), num_reveladas(0), revelou_mina(false),
    minas_sorteadas(false), semente(semente) {}

Tabuleiro::Tabuleiro(int linhas, int colunas, const std::vector<std::pair<int,int>>& posicoes_minas)
  : matriz(linhas, std::vector<Celula>(colunas)), num_minas(0),
    num_bandeiras(0), num_reveladas(0), revelou_mina(false),
    minas_sorteadas(true), semente(0) {
  for(std::size_t i=0; i<posicoes_minas.size(); i++) {
    int linha = posicoes_minas[i].first;
    int coluna = posicoes_minas[i].second;
    if(verificar_posicao(linha,coluna) && !matriz[linha][coluna].tem_mina()) {
      matriz[linha][coluna].definir_mina(true);
      num_minas++;
    }
  }
  calcular_vizinhas();
}

// Sorteia as minas em posicoes distintas, sem usar a posicao da primeira jogada.
void Tabuleiro::sortear_mina(int linha_segura, int coluna_segura) {
  std::mt19937 mt {semente};
  std::uniform_int_distribution<> line{0,num_linhas()-1};
  std::uniform_int_distribution<> column{0,num_colunas()-1};

  for(int i=1; i<=num_minas; i++) {
    int posY = line(mt); int posX = column(mt);
    bool posicao_segura = (posY==linha_segura && posX==coluna_segura);
    if(!matriz[posY][posX].tem_mina() && !posicao_segura)
      matriz[posY][posX].definir_mina(true);
    else
      i--;
  }
  minas_sorteadas = true;
}

void Tabuleiro::calcular_vizinhas() {
  for(int i=0; i<num_linhas(); i++) {
    for(int j=0; j<num_colunas(); j++) {
      int bombas=0;
      for(int k=0; k<8; k++) {
        int dy = i + vizinho_linha[k];
        int dx = j + vizinho_coluna[k];
        if(verificar_posicao(dy,dx) && matriz[dy][dx].tem_mina())
          bombas++;
      }
      matriz[i][j].definir_vizinhas(bombas);
    }
  }
}

int Tabuleiro::num_linhas() const {
  return static_cast<int>(matriz.size());
}

int Tabuleiro::num_colunas() const {
  return static_cast<int>(matriz[0].size());
}

bool Tabuleiro::verificar_posicao(int linha, int coluna) const {
  return (linha>=0 && linha<num_linhas()) && (coluna>=0 && coluna<num_colunas());
}

const Celula& Tabuleiro::celula(int linha, int coluna) const {
  return matriz[linha][coluna];
}

int Tabuleiro::celulas_reveladas() const {
  return num_reveladas;
}

int Tabuleiro::minas_faltando() const {
  return num_minas-num_bandeiras;
}

// Revela a celula escolhida. Retorna false se a posicao for invalida,
// se a celula ja estiver revelada ou se tiver bandeira.
bool Tabuleiro::revelacao(int linha, int coluna) {
  if(!verificar_posicao(linha,coluna))
    return false;
  if(matriz[linha][coluna].esta_revelada() || matriz[linha][coluna].verificar_flag())
    return false;

  if(!minas_sorteadas) {
    sortear_mina(linha,coluna);
    calcular_vizinhas();
  }

  matriz[linha][coluna].revelar();
  num_reveladas++;

  if(matriz[linha][coluna].tem_mina())
    revelou_mina=true;
  else if(matriz[linha][coluna].verificar_vizinhas()==0)
    cascata(linha,coluna);

  return true;
}

// Revelacao em cascata feita com uma fila (busca em largura).
// Cada celula sem minas vizinhas entra na fila, e suas vizinhas sao reveladas.
// Celulas com bandeira ou ja reveladas sao ignoradas, pois revelar() retorna false.
void Tabuleiro::cascata(int linha, int coluna) {
  std::queue<std::pair<int,int>> fila;
  fila.push({linha,coluna});

  while(!fila.empty()) {
    int l = fila.front().first;
    int c = fila.front().second;
    fila.pop();

    for(int k=0; k<8; k++) {
      int dy = l + vizinho_linha[k];
      int dx = c + vizinho_coluna[k];
      if(verificar_posicao(dy,dx) && !matriz[dy][dx].tem_mina() && matriz[dy][dx].revelar()) {
        num_reveladas++;
        if(matriz[dy][dx].verificar_vizinhas()==0)
          fila.push({dy,dx});
      }
    }
  }
}

bool Tabuleiro::colocar_flag(int linha, int coluna) {
  if(verificar_posicao(linha,coluna) && !matriz[linha][coluna].esta_revelada()) {
    if(!matriz[linha][coluna].verificar_flag())
      num_bandeiras++;
    else
      num_bandeiras--;
    matriz[linha][coluna].set_flag();
    return true;
  }
  return false;
}

// O jogador vence quando todas as celulas sem mina foram reveladas.
bool Tabuleiro::vitoria() const {
  int celulas_seguras = num_linhas()*num_colunas() - num_minas;
  return !revelou_mina && minas_sorteadas && num_reveladas==celulas_seguras;
}

bool Tabuleiro::derrota() const {
  return revelou_mina;
}

// Imprime o simbolo de uma celula, completando com espacos a esquerda
// para que as colunas de dois digitos fiquem alinhadas.
void Tabuleiro::imprimir_celula(int linha, int coluna, bool mostrar_tudo) const {
  const Celula& atual = matriz[linha][coluna];
  std::cout << std::string(digitos(num_colunas()-1)-1, ' ');

  if(!mostrar_tudo && atual.verificar_flag())
    std::cout << COR_BANDEIRA << 'F' << COR_NORMAL;
  else if(!mostrar_tudo && !atual.esta_revelada())
    std::cout << '#';
  else if(atual.tem_mina())
    std::cout << COR_MINA << '*' << COR_NORMAL;
  else if(atual.verificar_vizinhas()==0)
    std::cout << '.';
  else {
    int num = atual.verificar_vizinhas();
    std::cout << COR_NUMERO[num] << num << COR_NORMAL;
  }
  std::cout << ' ';
}

void Tabuleiro::imprimir(bool mostrar_tudo) const {
  int largura_linha = digitos(num_linhas()-1);
  int largura_coluna = digitos(num_colunas()-1);

  std::cout << std::string(largura_linha+1, ' ');
  for(int j=0; j<num_colunas(); j++)
    std::cout << std::setw(largura_coluna) << j << ' ';
  std::cout << '\n';

  for(int i=0; i<num_linhas(); i++) {
    std::cout << std::setw(largura_linha) << i << ' ';
    for(int j=0; j<num_colunas(); j++)
      imprimir_celula(i,j,mostrar_tudo);
    std::cout << '\n';
  }
}

// Mostra o tabuleiro como o jogador o ve durante a partida.
void Tabuleiro::imprimir_tabuleiro() const {
  imprimir(false);
}

// Mostra o tabuleiro inteiro, com todas as minas, ao fim da partida.
void Tabuleiro::imprimir_revelado() const {
  imprimir(true);
}
