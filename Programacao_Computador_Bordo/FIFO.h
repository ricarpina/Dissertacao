
void inserir_novo_elemento(unsigned int *pos_ini, unsigned char tam, unsigned int elem){
  int abc;
  for (abc=tam-2;abc>=0;abc--){
    pos_ini[abc+1]=pos_ini[abc];
  }
  pos_ini[0]=elem;

/*
  Serial.print("vec=  ");
    for (i=0;i<tam;i++){
        Serial.println(pos_ini[i],DEC);
      
    }*/
  
}


void somar_novos_registos(unsigned int *vec, unsigned char tam,unsigned int *ener_agora){
 
	inserir_novo_elemento(vec,tam,*ener_agora);
    *ener_agora = 0;
}

