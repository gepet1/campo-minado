# Campo Minado

Minha versão do clássico Campo Minado, para jogar no terminal, feita em C++. O objetivo é descobrir todas as células sem mina sem explodir nenhuma. Fiz este projeto para praticar o que estou aprendendo na faculdade de Ciência da Computação. Espero que goste!

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

## Como jogar

Você precisa do `g++` e do `make` instalados (eu uso no WSL). Dentro da pasta do projeto, digite:

```
make run
```

Também dá para rodar os testes com `make test` e apagar os arquivos compilados com `make clean`.

## Regras

- Se revelar uma célula com mina, você perde.
- A primeira jogada nunca tem mina.
- Cada célula sem mina mostra quantas minas existem nas 8 células em volta dela.
- Você pode marcar com uma bandeira as células onde acha que tem mina. A bandeira é só um lembrete, e uma célula marcada não pode ser revelada até você tirar a bandeira.
- Quando uma célula não tem nenhuma mina em volta, as vizinhas dela são reveladas sozinhas, e isso continua até chegar nas células com números.
- "Mines left", acima do tabuleiro, mostra quantas minas faltam marcar (minas menos bandeiras).
- Você vence quando revelar todas as células sem mina.
- As linhas e colunas começam do 0. Os números delas aparecem nas bordas do tabuleiro.

## Comandos

- `r L C` -> revela a célula da linha L e coluna C.
- `f L C` -> coloca uma bandeira na célula da linha L e coluna C (ou tira, se já tiver uma).
- `q` -> sai da partida e volta ao menu.

Se você digitar algo errado, o jogo avisa e pede de novo.

## Símbolos

- `#` -> célula ainda escondida.
- `F` -> bandeira.
- `.` -> célula sem minas em volta.
- `1` a `8` -> quantas minas tem em volta (cada número tem uma cor).
- `*` -> mina (só aparece quando o jogo acaba).

## Níveis

- Iniciante (Beginner) -> 9x9, 10 minas.
- Intermediário (Medium) -> 16x16, 40 minas.
- Especialista (Expert) -> 16x30, 99 minas.
- Personalizado (Custom) -> você escolhe o tamanho e quantas minas quer.

## Recordes

O jogo marca quanto tempo você levou para vencer. Se for um dos 5 melhores tempos do nível, ele pede seu nome e guarda no arquivo `recordes.txt`. Dá para ver os recordes pela opção "Records" do menu.

## Como o projeto está organizado

```
campo-minado/
├── src/          código do jogo
├── testes/       testes e uma lista de jogadas para testar
├── experimento.cpp
├── Makefile
└── README.md
```

## Curiosidade: a revelação em cascata

Quando você revela uma célula sem minas em volta, o jogo precisa abrir as vizinhas dela, depois as vizinhas das vizinhas, e assim por diante. Dá para fazer isso com recursão (uma função que chama ela mesma), mas num tabuleiro gigante, de 1000x1000, a função chama ela mesma tantas vezes que o programa fecha com erro. Por isso usei uma fila: o jogo vai guardando as células que ainda precisa abrir e trata uma de cada vez. O arquivo `experimento.cpp` testa exatamente esse caso (rode com `make experimento`).

## O que aprendi

- Separar o programa em partes (célula, tabuleiro e recordes), cada uma com sua função.
- Usar `vector` para montar tabuleiros de tamanhos diferentes.
- Sortear as minas com números aleatórios.
- Tratar o que o usuário digita sem o programa quebrar.
- Que recursão nem sempre é a melhor escolha.
- Salvar e ler dados de um arquivo.
- Testar o código com `assert`.
- Usar Makefile, Git e GitHub.

## TADs (para o programador)

### Célula

Guarda o estado de uma posição do tabuleiro: se tem mina, quantas minas tem em volta, se já foi revelada e se tem bandeira. Dá para consultar e mudar essas informações, revelar a célula e colocar ou tirar a bandeira.

### Tabuleiro

Guarda as células numa matriz feita com `vector`. Ele sorteia as minas depois da primeira jogada, calcula quantas minas cada célula tem em volta, revela as células (com a cascata), coloca e tira bandeiras, imprime o tabuleiro e diz se o jogador ganhou ou perdeu.

### Recordes

Guarda os 5 melhores tempos de cada nível, em ordem, e salva tudo no arquivo `recordes.txt`.
