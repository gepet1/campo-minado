# Minesweeper

A terminal version of the classic Minesweeper game, written in C++. The player must figure out which cells hide a mine and which are safe, until every safe cell has been revealed. This is a study project, made for learning purposes. Hope you enjoy it!

## Rules

- If you reveal a cell that has a mine, you instantly lose.
- Every safe cell you reveal shows how many mines are in the 8 cells around it (the 3x3 square centered on it).
- You can place a flag on a cell you believe hides a mine. A flag is only a reminder: it does not prove there is a mine there, and a flagged cell cannot be revealed until you remove the flag.
- If a revealed cell has no mines around it, it shows as `.` and its neighbors are revealed automatically. The chain goes on through every neighbor that also has no mines around it and stops at the numbered cells, which are revealed too.
- Above the board, "Mines left" shows the number of mines minus the number of flags you placed.
- You win when every safe cell has been revealed, so only the mines stay hidden. If the game hasn't ended yet, there are still safe cells to discover.
- Lines and columns start at 0: the first line, third column is `r 0 2`. The board shows these numbers on its edges.
- You only have one life, so use it well.

## Commands

- `r L C` -> reveals the cell at line L, column C.
- `f L C` -> places a flag on the cell at line L, column C; if there is already a flag there, removes it.
- `q` -> quits the game.

## Symbols

- `#` -> hidden cell: it can be either a mine or a safe cell.
- `F` -> a flag you placed.
- `.` -> revealed cell with no mines around it.
- `1` to `8` -> number of mines around that cell.
- `*` -> a mine, shown only when the game ends.

## Levels

- Beginner -> 9x9 board, 10 mines.
- Medium -> 16x16 board, 40 mines.
- Expert -> 16x30 board, 99 mines.

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

- Dados privados: a matriz de células, o número total de minas, o número de bandeiras colocadas e se o jogador revelou uma mina.
- Operações privadas:
  - verificar se uma posição (linha, coluna) está dentro do tabuleiro;
  - sortear as minas, escolhendo posições aleatórias distintas até completar exatamente o número de minas do nível;
  - calcular, para cada célula, o número de minas nas 8 vizinhas.
- Operações públicas:
  - imprimir o tabuleiro, com os números das linhas e das colunas nas bordas;
  - informar quantas minas faltam (minas menos bandeiras);
  - revelar uma célula, com a revelação em cascata;
  - colocar ou tirar uma bandeira;
  - informar se o jogador venceu;
  - informar se o jogador perdeu.
