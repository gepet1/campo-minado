#ifndef Recordes_h
#define Recordes_h

#include <map>
#include <string>
#include <vector>

struct Recorde {
  std::string nome;
  double tempo;   // em segundos
};

class Recordes {
private:
  std::string arquivo;
  std::map<std::string, std::vector<Recorde>> tabela;   // nivel -> melhores tempos

public:
  static const int MAXIMO = 5;   // quantidade de recordes guardados por nivel

  Recordes(const std::string& arquivo);

  void carregar();
  void salvar() const;

  // Informa se o tempo entraria na lista do nivel.
  bool entra_na_lista(const std::string& nivel, double tempo) const;

  // Insere o tempo na lista do nivel, mantendo a ordem crescente e apenas os 5 melhores.
  // Retorna a posicao alcancada (1 a 5) ou 0 se o tempo nao entrou na lista.
  int adicionar(const std::string& nivel, const std::string& nome, double tempo);

  std::vector<Recorde> lista(const std::string& nivel) const;
  void imprimir(const std::string& nivel) const;
};

#endif
