#ifndef Celula_h
#define Celula_h

class Celula{
private:
  bool mina;
  int minas_vizinhas;
  bool revelada;
  bool flag;

public:
  Celula(); // Construtor

  bool tem_mina() const;
  int verificar_vizinhas() const;
  bool esta_revelada() const;
  bool verificar_flag() const;

  void definir_mina(bool x);
  void definir_vizinhas(int x);
  bool revelar();
  bool set_flag();
};

#endif