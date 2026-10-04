#include <iostream>
#include <string>
#include "celula.h"

// Mostra o resultado de um teste: OK se a condicao for verdadeira, FALHOU caso contrario.
// Tambem conta as falhas, para o resumo no final.
int falhas = 0;

void confere(const std::string& descricao, bool condicao) {
  if (condicao) {
    std::cout << "OK      - " << descricao << '\n';
  } else {
    std::cout << "FALHOU  - " << descricao << '\n';
    falhas++;
  }
}

int main() {
  // Teste 1: uma celula recem-criada deve comecar vazia, oculta e sem bandeira.
  Celula c;
  confere("celula nova nao tem mina", c.tem_mina() == false);
  confere("celula nova tem 0 minas vizinhas", c.verificar_vizinhas() == 0);
  confere("celula nova nao esta revelada", c.esta_revelada() == false);
  confere("celula nova nao tem bandeira", c.verificar_flag() == false);

  // Teste 2: colocar e tirar a mina.
  c.definir_mina(true);
  confere("depois de definir_mina(true), tem mina", c.tem_mina() == true);
  c.definir_mina(false);
  confere("depois de definir_mina(false), nao tem mina", c.tem_mina() == false);

  // Teste 3: definir o numero de minas vizinhas.
  c.definir_vizinhas(3);
  confere("depois de definir_vizinhas(3), a consulta devolve 3", c.verificar_vizinhas() == 3);

  // Teste 4: um valor invalido (fora de 0 a 8) nao deve alterar o numero guardado.
  c.definir_vizinhas(9);
  confere("definir_vizinhas(9) e ignorado e o valor continua 3", c.verificar_vizinhas() == 3);
  c.definir_vizinhas(-1);
  confere("definir_vizinhas(-1) e ignorado e o valor continua 3", c.verificar_vizinhas() == 3);

  // Teste 5: a bandeira alterna a cada chamada.
  c.set_flag();
  confere("depois de um set_flag, tem bandeira", c.verificar_flag() == true);
  c.set_flag();
  confere("depois de dois set_flag, nao tem bandeira", c.verificar_flag() == false);

  // Teste 6: uma celula com bandeira nao pode ser revelada.
  c.set_flag();
  confere("revelar com bandeira devolve false", c.revelar() == false);
  confere("com bandeira, a celula continua oculta", c.esta_revelada() == false);

  // Teste 7: sem bandeira, a celula e revelada.
  c.set_flag();
  confere("revelar sem bandeira devolve true", c.revelar() == true);
  confere("depois de revelar, a celula esta revelada", c.esta_revelada() == true);

  // Teste 8: revelar de novo uma celula ja revelada devolve false.
  confere("revelar uma celula ja revelada devolve false", c.revelar() == false);

  // Resumo final.
  std::cout << '\n';
  if (falhas == 0)
    std::cout << "Todos os testes passaram.\n";
  else
    std::cout << falhas << " teste(s) falharam.\n";

  return 0;
}
