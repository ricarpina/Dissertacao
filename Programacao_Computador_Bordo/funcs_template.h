template <class T>

void imprimir_var(T var, unsigned char index, byte type){
unsigned short int y_pos[2], x_pos=215;

  if (index<8){
    y_pos[0]=y_ini_vars-25*(index*2);
    y_pos[1]=y_ini_vars-25*(index*2+1);

  }else{
    x_pos=543;
     y_pos[0]=y_ini_vars-25*((index-5)*2)+150;
  y_pos[1]=y_ini_vars-25*((index-5)*2+1)+150;
}        myGLCD.setColor(255, 111, 0);
        myGLCD.setBackColor(0, 0, 0);
    myGLCD.print(vazio,x_pos,y_pos[1]);
          myGLCD.print(variaveis[index],x_pos,y_pos[0]);
         if(type)
           myGLCD.printNumF(var,2,x_pos,y_pos[1],',',5);
         else
              myGLCD.printNumI(var,x_pos,y_pos[1],6);

      myGLCD.setColor(cor_principal_atual[0], cor_principal_atual[1], cor_principal_atual[2]);
      myGLCD.setBackColor(cor_fundo_atual[0], cor_fundo_atual[1], cor_fundo_atual[2]);

}
