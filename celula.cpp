#include <iostream>
#include "celula.h"

Celula::Celula() :
  mina(false),
  minas_vizinhas(0),
  revelada(false),
  flag(false)
{};

bool Celula::tem_mina() const{
  if(mina)
    return true;
  else return false;
}

int Celula::verificar_vizinhas() const{
  return minas_vizinhas;
}

bool Celula::esta_revelada() const{
  if(revelada)
    return true;
  else return false;
}

bool Celula::verificar_flag() const{
  if(flag)
    return true;
  else return false;
}

void Celula::definir_mina(bool x){
  if(x)
    mina=true;
  else mina=false;  
}


void Celula::definir_vizinhas(int x){
  if(x<=8 && x>=0)
    minas_vizinhas=x;
  else {
    std::cerr << "ERRO! Valor invalido para minas vizinhas.";
  }
}

bool Celula::revelar(){
  if(verificar_flag())
    return false;
  else if(!esta_revelada()){
    revelada=true;
    return true;
  }
  else 
    return false;
}

bool Celula::set_flag(){
  if(esta_revelada())
    return false;
  else if(!verificar_flag()){
    flag=true;
    return true;
  }
  else if(verificar_flag()) {
    flag=false;
    return true;
  }
    else return false;    
}

