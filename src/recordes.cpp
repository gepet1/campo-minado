#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "recordes.h"

Recordes::Recordes(const std::string& arquivo) : arquivo(arquivo) {}

// Le o arquivo de recordes. Cada linha tem o formato: nivel tempo nome
// O nome fica no fim da linha para poder conter espacos.
// Se o arquivo ainda nao existir, a tabela continua vazia.
void Recordes::carregar() {
  tabela.clear();
  std::ifstream entrada(arquivo);
  std::string linha;

  while(std::getline(entrada, linha)) {
    std::istringstream partes(linha);
    std::string nivel, nome;
    double tempo;
    if(partes >> nivel >> tempo) {
      partes >> std::ws;
      std::getline(partes, nome);
      adicionar(nivel, nome, tempo);
    }
  }
}

void Recordes::salvar() const {
  std::ofstream saida(arquivo);
  for(const auto& par : tabela) {
    for(const Recorde& r : par.second)
      saida << par.first << ' ' << r.tempo << ' ' << r.nome << '\n';
  }
}

bool Recordes::entra_na_lista(const std::string& nivel, double tempo) const {
  std::vector<Recorde> atual = lista(nivel);
  return atual.size()<static_cast<std::size_t>(MAXIMO) || tempo<atual.back().tempo;
}

int Recordes::adicionar(const std::string& nivel, const std::string& nome, double tempo) {
  std::vector<Recorde>& atual = tabela[nivel];

  // Procura a primeira posicao com tempo maior. Em caso de empate,
  // quem fez o tempo primeiro continua na frente.
  std::size_t pos = 0;
  while(pos<atual.size() && atual[pos].tempo<=tempo)
    pos++;

  if(pos>=static_cast<std::size_t>(MAXIMO))
    return 0;

  atual.insert(atual.begin()+pos, Recorde{nome, tempo});
  if(atual.size()>static_cast<std::size_t>(MAXIMO))
    atual.pop_back();

  return static_cast<int>(pos)+1;
}

std::vector<Recorde> Recordes::lista(const std::string& nivel) const {
  auto it = tabela.find(nivel);
  if(it==tabela.end())
    return {};
  return it->second;
}

void Recordes::imprimir(const std::string& nivel) const {
  std::vector<Recorde> melhores = lista(nivel);
  std::cout << nivel << ":\n";
  if(melhores.empty()) {
    std::cout << "  no records yet\n";
    return;
  }
  for(std::size_t i=0; i<melhores.size(); i++) {
    std::cout << "  " << i+1 << ". " << std::fixed << std::setprecision(1)
              << std::setw(7) << melhores[i].tempo << "s  " << melhores[i].nome << '\n';
  }
}
