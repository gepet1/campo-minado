#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "tabuleiro.h"

// Conta as falhas, para o resumo no final.
int falhas = 0;

// Mostra OK se a condicao for verdadeira e FALHOU caso contrario.
void confere(const std::string& descricao, bool condicao) {
  if (condicao) {
    std::cout << "OK      - " << descricao << '\n';
  } else {
    std::cout << "FALHOU  - " << descricao << '\n';
    falhas++;
  }
}

// Executa uma das impressoes do tabuleiro e devolve, como string, o que ela escreveria na tela.
// Se revelado for true, usa a impressao de depuracao; senao, usa a impressao normal.
std::string capturar(const Tabuleiro& t, bool revelado) {
  std::ostringstream saida;
  std::streambuf* original = std::cout.rdbuf(saida.rdbuf());
  if (revelado)
    t.imprimir_revelado();
  else
    t.imprimir_tabuleiro();
  std::cout.rdbuf(original);
  return saida.str();
}

// Remove os espacos no fim de cada linha e as linhas vazias no comeco e no fim.
// Assim, um espaco sobrando depois do ultimo simbolo nao conta como erro.
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

// Conta quantas vezes um caractere aparece num texto.
int contar(const std::string& texto, char simbolo) {
  int total = 0;
  for (char c : texto)
    if (c == simbolo)
      total++;
  return total;
}

// Compara uma impressao com o texto esperado. Em caso de falha, mostra os dois.
void confere_texto(const std::string& descricao, const std::string& obtido, const std::string& esperado) {
  if (obtido == esperado) {
    std::cout << "OK      - " << descricao << '\n';
  } else {
    std::cout << "FALHOU  - " << descricao << '\n';
    std::cout << "Esperado:\n" << esperado << '\n';
    std::cout << "Obtido:\n" << obtido << "\n\n";
    falhas++;
  }
}

// Le uma impressao (normal ou revelada) e devolve so os simbolos das celulas,
// sem os numeros das bordas: grade[l][c] e o simbolo da linha l, coluna c.
std::vector<std::vector<char>> ler_grade(const std::string& texto) {
  std::vector<std::vector<char>> grade;
  std::istringstream entrada(limpar(texto));
  std::string linha;
  std::getline(entrada, linha);  // descarta a linha com os numeros das colunas
  while (std::getline(entrada, linha)) {
    std::istringstream partes(linha);
    std::string rotulo;
    std::string simbolo;
    partes >> rotulo;  // descarta o numero da linha
    std::vector<char> celulas;
    while (partes >> simbolo)
      celulas.push_back(simbolo[0]);
    grade.push_back(celulas);
  }
  return grade;
}

// Confere, no tabuleiro revelado, se cada celula sem mina mostra exatamente
// a quantidade de '*' nas 8 vizinhas ('.' quando essa quantidade e 0).
bool vizinhas_corretas(const Tabuleiro& t) {
  std::vector<std::vector<char>> g = ler_grade(capturar(t, true));
  int linhas = static_cast<int>(g.size());
  for (int l = 0; l < linhas; l++) {
    int colunas = static_cast<int>(g[l].size());
    for (int c = 0; c < colunas; c++) {
      if (g[l][c] == '*')
        continue;
      int minas = 0;
      for (int dl = -1; dl <= 1; dl++)
        for (int dc = -1; dc <= 1; dc++) {
          int nl = l + dl;
          int nc = c + dc;
          if ((dl != 0 || dc != 0) && nl >= 0 && nl < linhas && nc >= 0 && nc < colunas && g[nl][nc] == '*')
            minas++;
        }
      char esperado = (minas == 0) ? '.' : static_cast<char>('0' + minas);
      if (g[l][c] != esperado)
        return false;
    }
  }
  return true;
}

int main() {
  // Na tela: o tabuleiro Iniciante revelado com a semente 42, para conferir a olho.
  std::cout << "Tabuleiro Iniciante revelado (9x9, 10 minas, semente 42):\n\n";
  Tabuleiro iniciante(9, 9, 10, 42);
  iniciante.imprimir_revelado();
  std::cout << '\n';

  // Parte 1 (03/10): a impressao normal mostra tudo oculto, mesmo com minas.

  // Teste 1: um tabuleiro 3x3.
  confere_texto("impressao normal do 3x3 continua oculta", limpar(capturar(Tabuleiro(3, 3, 1, 42), false)),
                "  0 1 2\n"
                "0 # # #\n"
                "1 # # #\n"
                "2 # # #");

  // Teste 2: 2 linhas e 5 colunas, para pegar linhas e colunas trocadas.
  confere_texto("impressao normal com 2 linhas e 5 colunas", limpar(capturar(Tabuleiro(2, 5, 1, 42), false)),
                "  0 1 2 3 4\n"
                "0 # # # # #\n"
                "1 # # # # #");

  // Teste 3: o menor tabuleiro possivel.
  confere_texto("impressao normal do 1x1", limpar(capturar(Tabuleiro(1, 1, 0, 42), false)),
                "  0\n"
                "0 #");

  // Teste 4: o Iniciante completo, que tem 10 minas escondidas.
  confere_texto("impressao normal do Iniciante nao mostra as minas", limpar(capturar(iniciante, false)),
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

  // Parte 2 (06/10): sorteio das minas e impressao revelada.

  // Teste 5: o Iniciante tem exatamente 10 minas.
  confere("Iniciante com semente 42 tem exatamente 10 minas", contar(capturar(iniciante, true), '*') == 10);

  // Teste 6: a mesma semente gera o mesmo tabuleiro.
  Tabuleiro a(9, 9, 10, 42);
  Tabuleiro b(9, 9, 10, 42);
  confere("mesma semente (42) gera o mesmo tabuleiro", capturar(a, true) == capturar(b, true));

  // Teste 7: sementes diferentes geram tabuleiros diferentes.
  Tabuleiro c(9, 9, 10, 7);
  confere("sementes diferentes (42 e 7) geram tabuleiros diferentes", capturar(a, true) != capturar(c, true));

  // Teste 8: com muitas minas, posicoes repetidas aparecem com frequencia.
  Tabuleiro especialista(16, 30, 99, 42);
  confere("Especialista (16x30) tem exatamente 99 minas", contar(capturar(especialista, true), '*') == 99);

  // Teste 9: varias sementes no Iniciante, todas com exatamente 10 minas.
  bool todas_com_10 = true;
  for (unsigned int semente = 1; semente <= 20; semente++) {
    Tabuleiro t(9, 9, 10, semente);
    if (contar(capturar(t, true), '*') != 10)
      todas_com_10 = false;
  }
  confere("sementes de 1 a 20 geram, todas, exatamente 10 minas", todas_com_10);

  // Teste 10: tabuleiro cheio (6 minas em 6 celulas).
  confere_texto("tabuleiro 2x3 cheio de minas, revelado", limpar(capturar(Tabuleiro(2, 3, 6, 42), true)),
                "  0 1 2\n"
                "0 * * *\n"
                "1 * * *");

  // Teste 11: tabuleiro sem minas. Todas as celulas tem 0 vizinhas, entao aparecem como '.'.
  confere_texto("tabuleiro 2x3 sem minas, revelado", limpar(capturar(Tabuleiro(2, 3, 0, 42), true)),
                "  0 1 2\n"
                "0 . . .\n"
                "1 . . .");

  // Parte 3 (09/10): contagem das minas vizinhas.
  // Cada numero do tabuleiro revelado e comparado com a contagem de '*' em volta dele.

  // Teste 12: o Iniciante, com cantos, bordas e meio.
  confere("vizinhas do Iniciante (semente 42) batem com os '*' em volta", vizinhas_corretas(iniciante));

  // Teste 13: o Especialista, que nao e quadrado e tem muitas minas.
  confere("vizinhas do Especialista batem com os '*' em volta", vizinhas_corretas(especialista));

  // Teste 14: um tabuleiro quase cheio, em que quase toda vizinha e mina.
  confere("vizinhas do 3x3 com 8 minas batem com os '*' em volta", vizinhas_corretas(Tabuleiro(3, 3, 8, 42)));

  // Teste 15: varias sementes no Iniciante.
  bool todas_corretas = true;
  for (unsigned int semente = 1; semente <= 20; semente++) {
    Tabuleiro t(9, 9, 10, semente);
    if (!vizinhas_corretas(t))
      todas_corretas = false;
  }
  confere("vizinhas corretas com as sementes de 1 a 20", todas_corretas);

  // Parte 4 (10/10): revelar, bandeira e impressao conforme o estado de cada celula.
  // O teste usa o tabuleiro revelado para saber onde ha celulas sem mina.
  Tabuleiro jogo(9, 9, 10, 42);
  std::vector<std::vector<char>> mapa = ler_grade(capturar(jogo, true));
  int sl = -1, sc = -1;  // primeira celula sem mina, em ordem de leitura
  int tl = -1, tc = -1;  // ultima celula sem mina, em ordem de leitura
  for (int l = 0; l < 9; l++)
    for (int c = 0; c < 9; c++)
      if (mapa[l][c] != '*') {
        if (sl == -1) {
          sl = l;
          sc = c;
        }
        tl = l;
        tc = c;
      }

  // Teste 16: revelar uma celula sem mina mostra o numero dela (ou '.') e mais nada.
  jogo.revelacao(sl, sc);
  std::string normal = capturar(jogo, false);
  confere("revelar uma celula sem mina mostra o simbolo dela",
          ler_grade(normal)[sl][sc] == mapa[sl][sc] && contar(normal, '#') == 80);

  // Teste 17: por bandeira numa celula oculta mostra F e diminui as minas restantes.
  jogo.colocar_flag(tl, tc);
  confere("bandeira numa celula oculta aparece como F", ler_grade(capturar(jogo, false))[tl][tc] == 'F');
  confere("com uma bandeira, minas_faltando() passa de 10 para 9", jogo.minas_faltando() == 9);

  // Teste 18: uma celula com bandeira nao pode ser revelada.
  jogo.revelacao(tl, tc);
  confere("revelar uma celula com bandeira nao muda nada", ler_grade(capturar(jogo, false))[tl][tc] == 'F');

  // Teste 19: por bandeira de novo tira a bandeira.
  jogo.colocar_flag(tl, tc);
  confere("tirar a bandeira volta a mostrar #", ler_grade(capturar(jogo, false))[tl][tc] == '#');
  confere("sem bandeiras, minas_faltando() volta a 10", jogo.minas_faltando() == 10);

  // Teste 20: nao se pode por bandeira numa celula ja revelada.
  jogo.colocar_flag(sl, sc);
  confere("bandeira numa celula revelada nao muda nada",
          ler_grade(capturar(jogo, false))[sl][sc] == mapa[sl][sc] && jogo.minas_faltando() == 10);

  // Teste 21: revelar de novo uma celula ja revelada nao muda nada.
  std::string antes = capturar(jogo, false);
  jogo.revelacao(sl, sc);
  confere("revelar de novo uma celula revelada nao muda nada", capturar(jogo, false) == antes);

  // Teste 22: revelar fora do tabuleiro nao muda nada (nem trava o programa).
  jogo.revelacao(-1, 0);
  jogo.revelacao(9, 0);
  jogo.revelacao(0, -1);
  jogo.revelacao(0, 9);
  confere("revelar fora do tabuleiro nao muda nada", capturar(jogo, false) == antes);

  // Teste 23: bandeira fora do tabuleiro. Este e o ultimo teste porque, se o
  // colocar_flag nao conferir a posicao, o programa pode travar aqui.
  std::cout << "(testando bandeira fora do tabuleiro; se o programa parar logo depois desta linha, "
               "o colocar_flag nao confere a posicao)" << std::endl;
  jogo.colocar_flag(-1, 0);
  jogo.colocar_flag(9, 0);
  jogo.colocar_flag(0, -1);
  jogo.colocar_flag(0, 9);
  confere("bandeira fora do tabuleiro nao muda nada",
          capturar(jogo, false) == antes && jogo.minas_faltando() == 10);

  // Resumo final.
  std::cout << '\n';
  if (falhas == 0)
    std::cout << "Todos os testes passaram.\n";
  else
    std::cout << falhas << " teste(s) falharam.\n";

  return 0;
}
