#include <iostream>
#include <sstream>
#include <string>
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

// Conta quantos '*' (minas) aparecem num texto.
int contar_minas(const std::string& texto) {
  int total = 0;
  for (char c : texto)
    if (c == '*')
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

int main() {
  // Na tela: o tabuleiro Iniciante revelado com a semente 42, para conferir a olho.
  // Ele e o unico tabuleiro revelado impresso, entao o grep do roteiro deve contar 10.
  std::cout << "Tabuleiro Iniciante revelado (9x9, 10 minas, semente 42):\n\n";
  Tabuleiro iniciante(9, 9, 10, 42);
  iniciante.imprimir_revelado();
  std::cout << '\n';

  // Parte 1 (03/10): a impressao normal continua mostrando tudo oculto, mesmo com minas.

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
  confere("Iniciante com semente 42 tem exatamente 10 minas", contar_minas(capturar(iniciante, true)) == 10);

  // Teste 6: a mesma semente gera o mesmo tabuleiro.
  Tabuleiro a(9, 9, 10, 42);
  Tabuleiro b(9, 9, 10, 42);
  confere("mesma semente (42) gera o mesmo tabuleiro", capturar(a, true) == capturar(b, true));

  // Teste 7: sementes diferentes geram tabuleiros diferentes.
  Tabuleiro c(9, 9, 10, 7);
  confere("sementes diferentes (42 e 7) geram tabuleiros diferentes", capturar(a, true) != capturar(c, true));

  // Teste 8: com muitas minas, as posicoes repetidas aparecem com frequencia.
  // Se o sorteio nao conferir se a celula ja tem mina, a contagem fica abaixo de 99.
  Tabuleiro especialista(16, 30, 99, 42);
  confere("Especialista (16x30) tem exatamente 99 minas", contar_minas(capturar(especialista, true)) == 99);

  // Teste 9: varias sementes no Iniciante, todas com exatamente 10 minas.
  bool todas_com_10 = true;
  for (unsigned int semente = 1; semente <= 20; semente++) {
    Tabuleiro t(9, 9, 10, semente);
    if (contar_minas(capturar(t, true)) != 10)
      todas_com_10 = false;
  }
  confere("sementes de 1 a 20 geram, todas, exatamente 10 minas", todas_com_10);

  // Teste 10: tabuleiro cheio (6 minas em 6 celulas). O sorteio precisa terminar
  // e a impressao revelada precisa seguir o mesmo formato da normal.
  confere_texto("tabuleiro 2x3 cheio de minas, revelado", limpar(capturar(Tabuleiro(2, 3, 6, 42), true)),
                "  0 1 2\n"
                "0 * * *\n"
                "1 * * *");

  // Teste 11: tabuleiro sem minas. Todas as celulas tem 0 vizinhas, entao aparecem como '.'.
  confere_texto("tabuleiro 2x3 sem minas, revelado", limpar(capturar(Tabuleiro(2, 3, 0, 42), true)),
                "  0 1 2\n"
                "0 . . .\n"
                "1 . . .");

  // Resumo final.
  std::cout << '\n';
  if (falhas == 0)
    std::cout << "Todos os testes passaram.\n";
  else
    std::cout << falhas << " teste(s) falharam.\n";

  return 0;
}
