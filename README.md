# Campo Minado

Uma versão para terminal do clássico jogo Campo Minado, escrita em C++. O jogador precisa descobrir quais células escondem uma mina e quais são seguras, até que todas as células seguras tenham sido reveladas. Este é um projeto de estudo, feito com fins de aprendizado. Espero que goste!

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

## Como compilar e jogar

É preciso ter o `g++` (C++17) e o `make`, no Linux ou no WSL.

```
make        # compila o jogo
make run    # compila e inicia o jogo
make test   # compila e roda os testes automáticos
make clean  # apaga os arquivos compilados
```

Outros alvos: `make debug` compila com AddressSanitizer e UndefinedBehaviorSanitizer, e `make experimento` roda o experimento da cascata descrito mais abaixo.

Ao passar um número para o jogo (por exemplo, `./campo-minado 42`), ele é usado como semente, e as minas são colocadas sempre nas mesmas posições. Isso é útil para testes.

## Regras

- Se você revelar uma célula que tem mina, perde na hora.
- A primeira jogada é sempre segura: as minas só são colocadas depois dela, e nunca na célula escolhida.
- Cada célula segura revelada mostra quantas minas existem nas 8 células ao seu redor (o quadrado 3x3 centrado nela).
- Você pode colocar uma bandeira numa célula que acredita esconder uma mina. A bandeira é apenas um lembrete: ela não prova que há uma mina ali, e uma célula com bandeira não pode ser revelada até que a bandeira seja retirada.
- Se uma célula revelada não tem minas ao redor, ela aparece como `.` e suas vizinhas são reveladas automaticamente. A revelação continua por todas as vizinhas que também não têm minas ao redor e para nas células com números, que também são reveladas. Células com bandeira são ignoradas.
- Acima do tabuleiro, "Mines left" mostra o número de minas menos o número de bandeiras colocadas.
- Você vence quando todas as células seguras forem reveladas, ou seja, quando só as minas continuarem escondidas. Se o jogo ainda não terminou, ainda há células seguras a descobrir.
- Linhas e colunas começam em 0: a primeira linha, terceira coluna é `r 0 2`. O tabuleiro mostra esses números nas bordas.
- Você só tem uma vida, então use-a bem.

## Comandos

- `r L C` -> revela a célula da linha L, coluna C.
- `f L C` -> coloca uma bandeira na célula da linha L, coluna C; se já houver uma bandeira, ela é retirada.
- `q` -> sai da partida atual e volta ao menu.

Comandos inválidos (letras desconhecidas, números faltando, posições fora do tabuleiro) mostram uma mensagem e o jogo pede um novo comando.

## Símbolos

- `#` -> célula escondida: pode ser uma mina ou uma célula segura.
- `F` -> uma bandeira colocada por você.
- `.` -> célula revelada sem minas ao redor.
- `1` a `8` -> número de minas ao redor daquela célula, cada número com sua própria cor.
- `*` -> uma mina, mostrada apenas quando o jogo termina.

## Níveis

- Iniciante (Beginner) -> tabuleiro 9x9, 10 minas.
- Intermediário (Medium) -> tabuleiro 16x16, 40 minas.
- Especialista (Expert) -> tabuleiro 16x30, 99 minas.
- Personalizado (Custom) -> você escolhe o tamanho (2 a 30 linhas e colunas) e o número de minas (pelo menos uma célula precisa ficar sem mina).

## Recordes

O jogo mede o tempo de cada partida. Quando você vence no Iniciante, no Intermediário ou no Especialista com um dos 5 melhores tempos do nível, o jogo pede seu nome e salva o tempo em `recordes.txt`. A opção "Records" do menu mostra os melhores tempos de cada nível, do mais rápido para o mais lento.

## Estrutura do projeto

```
campo-minado/
├── src/
│   ├── celula.h / celula.cpp        TAD Célula
│   ├── tabuleiro.h / tabuleiro.cpp  TAD Tabuleiro (minas, cascata, vitória e derrota)
│   ├── recordes.h / recordes.cpp    Recordes (5 melhores por nível, salvos em arquivo)
│   └── main.cpp                     Menu, laço do jogo e verificação da entrada
├── testes/
│   ├── teste_tabuleiro.cpp          Testes automáticos com assert
│   └── jogadas.txt                  Entrada pronta com jogadas válidas e inválidas
├── experimento.cpp                  Experimento da cascata num tabuleiro 1000x1000
├── Makefile
└── README.md
```

Para rodar as jogadas prontas: `make && ./campo-minado < testes/jogadas.txt`.

## Experimento da cascata: recursão x fila

A forma mais simples de escrever a cascata é com recursão: revelar uma célula sem minas ao redor chama a mesma função para cada vizinha. Isso funciona nos níveis normais, mas o `experimento.cpp` cria um tabuleiro 1000x1000 com uma única mina e revela um canto, o que abre quase um milhão de células numa só jogada. Com uma cascata recursiva, o programa termina com segmentation fault: cada chamada espera a próxima terminar, a cadeia de chamadas chega a centenas de milhares de níveis, e a pilha de chamadas (8 MB por padrão no Linux) se esgota.

A versão atual usa uma busca em largura com `std::queue`: as células que ainda precisam ser abertas entram numa fila guardada no heap, então não existe uma cadeia longa de chamadas. No mesmo tabuleiro, ela revela as 999.999 células seguras em algumas dezenas de milissegundos.

## O que aprendi

- Projetar tipos abstratos de dados (`Celula`, `Tabuleiro`, `Recordes`) e usar composição entre eles.
- Usar `std::vector` para uma matriz cujo tamanho só é conhecido durante a execução.
- Gerar posições aleatórias com `std::mt19937` e uma semente, tornando o jogo reproduzível.
- Ler a entrada do usuário com segurança usando `std::getline` e `std::istringstream`.
- Os limites da recursão e como substituí-la por uma busca em largura.
- Medir tempo com `std::chrono` e salvar dados num arquivo de texto.
- Escrever testes com `assert`, usando um tabuleiro com minas em posições conhecidas.
- Organizar a compilação com um Makefile e encontrar erros de memória com sanitizers.
- Usar Git e GitHub: commits, issues, tags e releases.

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
