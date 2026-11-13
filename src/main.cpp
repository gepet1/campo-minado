#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "recordes.h"
#include "tabuleiro.h"

struct Nivel {
  std::string nome;
  int linhas;
  int colunas;
  int minas;
};

const Nivel NIVEIS[3] = {
  {"Beginner", 9, 9, 10},
  {"Medium", 16, 16, 40},
  {"Expert", 16, 30, 99}
};

const std::string ARQUIVO_RECORDES = "recordes.txt";

// Comando digitado pelo jogador durante a partida.
// tipo vale 'r' (revelar), 'f' (bandeira), 'q' (sair) ou 'x' (comando invalido).
struct Comando {
  char tipo;
  int linha;
  int coluna;
  std::string erro;
};

void limpar_tela() {
  std::cout << "\033[2J\033[H";
}

void esperar_enter() {
  std::string linha;
  std::cout << "\nPress Enter to continue...";
  std::getline(std::cin, linha);
}

// Le um numero inteiro entre minimo e maximo, sozinho na linha.
// Repete a pergunta ate a resposta ser valida. Retorna false se a entrada terminar.
bool ler_inteiro(const std::string& pergunta, int minimo, int maximo, int& valor) {
  std::string linha;
  while(true) {
    std::cout << pergunta;
    if(!std::getline(std::cin, linha))
      return false;

    std::istringstream entrada(linha);
    std::string sobra;
    if(entrada >> valor && !(entrada >> sobra) && valor>=minimo && valor<=maximo)
      return true;

    std::cout << "Please type a number from " << minimo << " to " << maximo << ".\n";
  }
}

// Separa a linha digitada em comando, linha e coluna, e verifica o formato.
Comando interpretar(const std::string& texto) {
  Comando cmd = {'x', -1, -1, ""};
  std::istringstream entrada(texto);
  std::string letra, sobra;

  if(!(entrada >> letra)) {
    cmd.erro = "Type a command: r L C, f L C or q.";
    return cmd;
  }

  if(letra=="q" || letra=="Q") {
    if(entrada >> sobra)
      cmd.erro = "The q command does not take numbers.";
    else
      cmd.tipo = 'q';
    return cmd;
  }

  if(letra!="r" && letra!="R" && letra!="f" && letra!="F") {
    cmd.erro = "Unknown command \"" + letra + "\". Use r L C, f L C or q.";
    return cmd;
  }

  if(!(entrada >> cmd.linha >> cmd.coluna)) {
    cmd.erro = "Line and column must be two whole numbers. Example: r 3 4";
    return cmd;
  }

  if(entrada >> sobra) {
    cmd.erro = "Too many values. Example: r 3 4";
    return cmd;
  }

  cmd.tipo = (letra=="r" || letra=="R") ? 'r' : 'f';
  return cmd;
}

// Executa um comando valido no tabuleiro e devolve a mensagem para o jogador
// (vazia quando a jogada deu certo).
std::string executar(Tabuleiro& tab, const Comando& cmd) {
  if(!tab.verificar_posicao(cmd.linha, cmd.coluna)) {
    return "Position out of the board. Lines go from 0 to " + std::to_string(tab.num_linhas()-1)
         + " and columns from 0 to " + std::to_string(tab.num_colunas()-1) + ".";
  }

  if(cmd.tipo=='r') {
    if(tab.celula(cmd.linha, cmd.coluna).verificar_flag())
      return "This cell has a flag. Remove it with f L C before revealing it.";
    if(!tab.revelacao(cmd.linha, cmd.coluna))
      return "This cell is already revealed.";
  }
  else {
    if(!tab.colocar_flag(cmd.linha, cmd.coluna))
      return "A revealed cell cannot receive a flag.";
  }
  return "";
}

void mostrar_situacao(const Tabuleiro& tab, const std::string& nome_nivel) {
  std::cout << "Level: " << nome_nivel << "    Mines left: " << tab.minas_faltando() << "\n\n";
}

// Joga uma partida completa. Retorna true se o jogador venceu
// e, nesse caso, guarda em tempo a duracao da partida em segundos.
bool jogar(Tabuleiro& tab, const std::string& nome_nivel, double& tempo) {
  auto inicio = std::chrono::steady_clock::now();
  std::string mensagem;

  while(!tab.vitoria() && !tab.derrota()) {
    limpar_tela();
    mostrar_situacao(tab, nome_nivel);
    tab.imprimir_tabuleiro();
    std::cout << '\n';
    if(!mensagem.empty())
      std::cout << mensagem << '\n';
    std::cout << "Command (r L C, f L C, q): ";

    std::string linha;
    if(!std::getline(std::cin, linha))
      return false;

    Comando cmd = interpretar(linha);
    if(cmd.tipo=='q')
      return false;
    else if(cmd.tipo=='x')
      mensagem = cmd.erro;
    else
      mensagem = executar(tab, cmd);
  }

  auto fim = std::chrono::steady_clock::now();
  tempo = std::chrono::duration<double>(fim-inicio).count();

  limpar_tela();
  mostrar_situacao(tab, nome_nivel);
  tab.imprimir_revelado();
  std::cout << '\n';

  if(tab.derrota()) {
    std::cout << "BOOM! You revealed a mine. Game over.\n";
    esperar_enter();
    return false;
  }

  std::cout << "Congratulations, you won in " << std::fixed << std::setprecision(1)
            << tempo << " seconds!\n";
  return true;
}

// Pergunta o nome do jogador e guarda o tempo, se ele entrar entre os 5 melhores do nivel.
void registrar_recorde(Recordes& recordes, const std::string& nivel, double tempo) {
  if(!recordes.entra_na_lista(nivel, tempo)) {
    std::cout << "Your time did not reach the top " << Recordes::MAXIMO << " of this level.\n";
    return;
  }

  std::cout << "New record! Type your name: ";
  std::string nome;
  std::getline(std::cin, nome);
  std::istringstream limpa(nome);
  limpa >> std::ws;
  std::getline(limpa, nome);
  if(nome.empty())
    nome = "Anonymous";

  int posicao = recordes.adicionar(nivel, nome, tempo);
  recordes.salvar();
  std::cout << nome << " is now number " << posicao << " in the " << nivel << " records.\n";
}

void mostrar_recordes(const Recordes& recordes) {
  limpar_tela();
  std::cout << "RECORDS (top " << Recordes::MAXIMO << " per level)\n\n";
  for(int i=0; i<3; i++) {
    recordes.imprimir(NIVEIS[i].nome);
    std::cout << '\n';
  }
}

// Le as dimensoes e o numero de minas do nivel personalizado.
// Retorna false se a entrada terminar.
bool ler_personalizado(Nivel& nivel) {
  nivel.nome = "Custom";
  if(!ler_inteiro("Number of lines (2 to 30): ", 2, 30, nivel.linhas))
    return false;
  if(!ler_inteiro("Number of columns (2 to 30): ", 2, 30, nivel.colunas))
    return false;

  // Pelo menos uma celula precisa ficar sem mina, pois a primeira jogada e sempre segura.
  int maximo = nivel.linhas*nivel.colunas - 1;
  return ler_inteiro("Number of mines (1 to " + std::to_string(maximo) + "): ", 1, maximo, nivel.minas);
}

void mostrar_menu() {
  limpar_tela();
  std::cout << "MINESWEEPER\n\n";
  for(int i=0; i<3; i++) {
    std::cout << i+1 << " - " << NIVEIS[i].nome << " (" << NIVEIS[i].linhas << "x"
              << NIVEIS[i].colunas << ", " << NIVEIS[i].minas << " mines)\n";
  }
  std::cout << "4 - Custom\n";
  std::cout << "5 - Records\n";
  std::cout << "0 - Exit\n\n";
}

// Se um numero for passado na linha de comando (ex.: ./campo-minado 42),
// ele e usado como semente, e as minas saem sempre nas mesmas posicoes.
int main(int argc, char* argv[]) {
  bool semente_fixa = false;
  unsigned int semente_escolhida = 0;
  if(argc>1) {
    std::istringstream argumento(argv[1]);
    if(argumento >> semente_escolhida)
      semente_fixa = true;
  }

  std::random_device aleatorio;
  Recordes recordes(ARQUIVO_RECORDES);
  recordes.carregar();

  while(true) {
    mostrar_menu();
    int opcao;
    if(!ler_inteiro("Choose an option: ", 0, 5, opcao))
      break;

    if(opcao==0)
      break;

    if(opcao==5) {
      mostrar_recordes(recordes);
      esperar_enter();
      continue;
    }

    Nivel nivel;
    if(opcao==4) {
      if(!ler_personalizado(nivel))
        break;
    }
    else
      nivel = NIVEIS[opcao-1];

    unsigned int semente = semente_fixa ? semente_escolhida : aleatorio();
    Tabuleiro tab(nivel.linhas, nivel.colunas, nivel.minas, semente);

    double tempo = 0;
    if(jogar(tab, nivel.nome, tempo)) {
      if(opcao==4)
        std::cout << "Records are kept only for the standard levels.\n";
      else
        registrar_recorde(recordes, nivel.nome, tempo);
      esperar_enter();
    }
  }

  std::cout << "\nSee you next time!\n";
  return 0;
}
