# Minesweeper

A terminal version of the classic Minesweeper game, written in C++. The player must figure out which cells hide a mine and which are safe, until every safe cell has been revealed. This is a study project, made for learning purposes. Hope you enjoy it!

```
Level: Beginner    Mines left: 9

  0 1 2 3 4 5 6 7 8
0 # # # # 1 . . . .
1 # 2 2 2 1 . . . .
2 F 1 . . . . . 1 1
3 # 3 1 1 . . . 1 #
4 # # # 1 . . . 1 #
5 # # # 2 1 1 1 1 #
6 # # # # # # # # #
7 # # # # # # # # #
8 # # # # # # # # #

Command (r L C, f L C, q):
```

## How to compile and play

You need `g++` (C++17) and `make`, on Linux or WSL.

```
make        # compiles the game
make run    # compiles and starts the game
make test   # compiles and runs the automatic tests
make clean  # removes the compiled files
```

Other targets: `make debug` compiles with AddressSanitizer and UndefinedBehaviorSanitizer, and `make experimento` runs the cascade experiment described below.

Passing a number to the game (for example `./campo-minado 42`) uses it as the random seed, so the mines are always placed in the same positions. This is useful for testing.

## Rules

- If you reveal a cell that has a mine, you instantly lose.
- Your first move is always safe: the mines are placed only after it, never on the cell you chose.
- Every safe cell you reveal shows how many mines are in the 8 cells around it (the 3x3 square centered on it).
- You can place a flag on a cell you believe hides a mine. A flag is only a reminder: it does not prove there is a mine there, and a flagged cell cannot be revealed until you remove the flag.
- If a revealed cell has no mines around it, it shows as `.` and its neighbors are revealed automatically. The chain goes on through every neighbor that also has no mines around it and stops at the numbered cells, which are revealed too. Flagged cells are skipped.
- Above the board, "Mines left" shows the number of mines minus the number of flags you placed.
- You win when every safe cell has been revealed, so only the mines stay hidden. If the game hasn't ended yet, there are still safe cells to discover.
- Lines and columns start at 0: the first line, third column is `r 0 2`. The board shows these numbers on its edges.
- You only have one life, so use it well.

## Commands

- `r L C` -> reveals the cell at line L, column C.
- `f L C` -> places a flag on the cell at line L, column C; if there is already a flag there, removes it.
- `q` -> quits the current game and goes back to the menu.

Invalid commands (unknown letters, missing numbers, positions outside the board) show a message and ask again.

## Symbols

- `#` -> hidden cell: it can be either a mine or a safe cell.
- `F` -> a flag you placed.
- `.` -> revealed cell with no mines around it.
- `1` to `8` -> number of mines around that cell, each number in its own color.
- `*` -> a mine, shown only when the game ends.

## Levels

- Beginner -> 9x9 board, 10 mines.
- Medium -> 16x16 board, 40 mines.
- Expert -> 16x30 board, 99 mines.
- Custom -> you choose the size (2 to 30 lines and columns) and the number of mines (at least one cell must stay safe).

## Records

The game measures how long each game takes. When you win on Beginner, Medium or Expert with one of the 5 best times of that level, it asks for your name and saves the time in `recordes.txt`. The menu option "Records" shows the best times of each level, from fastest to slowest.

## Project structure

```
campo-minado/
├── src/
│   ├── celula.h / celula.cpp        Cell ADT
│   ├── tabuleiro.h / tabuleiro.cpp  Board ADT (mines, cascade, win and loss)
│   ├── recordes.h / recordes.cpp    Records (top 5 per level, saved in a file)
│   └── main.cpp                     Menu, game loop and input checking
├── testes/
│   ├── teste_tabuleiro.cpp          Automatic tests with assert
│   └── jogadas.txt                  Scripted input with valid and invalid moves
├── experimento.cpp                  Cascade experiment on a 1000x1000 board
├── Makefile
└── README.md
```

To play the scripted moves: `make && ./campo-minado < testes/jogadas.txt`.

## Cascade experiment: recursion vs. queue

The simplest way to write the cascade is with recursion: revealing a cell with no mines around it calls the same function for each neighbor. That works on the normal levels, but `experimento.cpp` builds a 1000x1000 board with a single mine and reveals a corner, which opens almost one million cells in one move. With a recursive cascade the program crashes with a segmentation fault: each call waits for the next one, the chain of calls becomes hundreds of thousands deep, and the call stack (8 MB by default on Linux) runs out.

The current version uses a breadth-first search with `std::queue`: cells waiting to be opened go into a queue stored on the heap, so there is no deep chain of calls. On the same board it reveals all 999,999 safe cells in a few tens of milliseconds.

## What I learned

- Designing abstract data types (`Celula`, `Tabuleiro`, `Recordes`) and using composition between them.
- Using `std::vector` for a matrix whose size is only known while the program runs.
- Generating random positions with `std::mt19937` and a seed, and making the game reproducible.
- Reading user input safely with `std::getline` and `std::istringstream`.
- The limits of recursion, and replacing it with a breadth-first search.
- Measuring time with `std::chrono` and saving data in a text file.
- Writing tests with `assert`, using a board with mines in known positions.
- Organizing the build with a Makefile and finding memory errors with sanitizers.
- Using Git and GitHub: commits, issues, tags and releases.

## TADs (para o programador)

### Célula

Guarda o estado de uma posição do tabuleiro.

- Dados privados: se tem mina, o número de minas vizinhas, se está revelada e se tem bandeira.
- Operações públicas:
  - consultar se tem mina, o número de minas vizinhas, se está revelada e se tem bandeira;
  - colocar mina e definir o número de minas vizinhas, usadas pelo tabuleiro ao montar o jogo;
  - revelar;
  - colocar ou tirar a bandeira.
- Na tela, uma célula não revelada aparece como `#` (ou `F`, se tiver bandeira); uma célula revelada mostra o número de minas vizinhas (ou `.`, se for zero).

### Tabuleiro

Guarda as células numa matriz feita com `vector`, o que permite criar tabuleiros de tamanhos diferentes para cada nível.

- Dados privados: a matriz de células, o número total de minas, o número de bandeiras colocadas, o número de células reveladas, se o jogador revelou uma mina, se as minas já foram sorteadas e a semente.
- Operações privadas:
  - sortear as minas na primeira jogada, escolhendo posições aleatórias distintas, diferentes da célula escolhida, até completar exatamente o número de minas do nível;
  - calcular, para cada célula, o número de minas nas 8 vizinhas;
  - revelar em cascata, usando uma fila (busca em largura).
- Operações públicas:
  - verificar se uma posição (linha, coluna) está dentro do tabuleiro;
  - consultar o número de linhas e de colunas, uma célula (só para leitura) e quantas células já foram reveladas;
  - imprimir o tabuleiro, com os números das linhas e das colunas nas bordas, alinhados mesmo com dois dígitos;
  - imprimir o tabuleiro revelado, com todas as minas, ao fim da partida;
  - informar quantas minas faltam (minas menos bandeiras);
  - revelar uma célula, com a revelação em cascata;
  - colocar ou tirar uma bandeira;
  - informar se o jogador venceu;
  - informar se o jogador perdeu.
- Há um segundo construtor, usado nos testes, que recebe as posições das minas em vez de sorteá-las.

### Recordes

Guarda os 5 melhores tempos de cada nível e os lê e grava no arquivo `recordes.txt`.

- Dados privados: o nome do arquivo e, para cada nível, a lista de recordes (nome e tempo) em ordem crescente de tempo.
- Operações públicas:
  - carregar e salvar o arquivo;
  - informar se um tempo entraria na lista de um nível;
  - adicionar um tempo, mantendo a lista ordenada e com no máximo 5 recordes;
  - consultar e imprimir a lista de um nível.
