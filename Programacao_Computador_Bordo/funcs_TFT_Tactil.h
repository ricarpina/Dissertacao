void configurar_cor_principal(unsigned char* COR_atual, unsigned char R, unsigned char G, unsigned char B){
    COR_atual[0]=R;
    COR_atual[1]=G;
    COR_atual[2]=B;
    myGLCD.setColor(R,G, B);
}

void configurar_cor_fundo(unsigned char* COR_atual, unsigned char R, unsigned char G, unsigned char B){
    COR_atual[0]=R;
    COR_atual[1]=G;
    COR_atual[2]=B;
     myGLCD.setBackColor(R,G, B);
    }
    
void imprimir_todas_variaveis(){
       configurar_cor_principal(cor_principal_atual,255, 111, 0);

  unsigned char a=(sizeof(can_chars)/sizeof(can_chars[0])),b=((sizeof(can_chars)/sizeof(can_chars[0]))+(sizeof(can_u_ints)/sizeof(can_u_ints[0]))),c=((sizeof(can_chars)/sizeof(can_chars[0]))+(sizeof(can_u_ints)/sizeof(can_u_ints[0]))+(sizeof(can_floats)/sizeof(can_floats[0])));
    for(contador=0;contador<a;contador++){
         imprimir_var(can_chars[contador], contador,0); 
    }
    for(contador=(a);contador<b;contador++){
         imprimir_var(can_u_ints[contador-a], contador,0); 
    }
     for(contador=b;contador<c;contador++){
         imprimir_var(can_floats[contador-b],contador ,1); 
    }
}
// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}

unsigned short int ler_x_tatil(){
  
     unsigned short int yy=myTouch.getY();
   //  Serial.print("Y=");
    // Serial.println(yy,DEC);     
      float x_quad_temp=((float)(-yy))*3.0303;
       return (799+(int)x_quad_temp);
}

 unsigned short int ler_y_tatil(){

     unsigned short int xx=myTouch.getX();
       //   Serial.print("X=");
    // Serial.println(xx,DEC);    
      return (23+479-xx);
}
    
    boolean verificar_contacto_botao(unsigned short int x_contacto,unsigned short int y_contacto,struct botao* bot){
      return ((x_contacto>=bot->coord[0])&(x_contacto<=bot->coord[2])&(y_contacto>=bot->coord[1])&(y_contacto<=bot->coord[3]));
    }

void imprimir_botao(struct botao* bot){
  configurar_cor_principal(cor_principal_atual,bot->cor[0], bot->cor[1], bot->cor[2]);
  myGLCD.fillRoundRect(bot->coord[0], bot->coord[1], bot->coord[2], bot->coord[3]);
  myGLCD.setFont(bot->tipo_letra);
  configurar_cor_principal(cor_principal_atual,bot->cor[3], bot->cor[4], bot->cor[5]);
  configurar_cor_fundo(cor_fundo_atual,bot->cor[0], bot->cor[1], bot->cor[2]);
  unsigned char linha_temp=0, comp_str, cont_temp;
    unsigned short int y_ini = (unsigned short int)((float)((bot->coord[1]+bot->coord[3])/2)+((float)bot->esp_linhas)*((((float)bot->n_linhas)-1.0)*0.5+0.5));

    for(cont_temp=0;cont_temp<bot->n_linhas;cont_temp++){
      comp_str=strlen(bot->texto_botao[cont_temp]);
    //configurar_cor_principal(cor_principal_atual,bot->cor[3], bot->cor[4], bot->cor[5]);
    // 
      myGLCD.print(bot->texto_botao[cont_temp],((bot->coord[0]+bot->coord[2])/2)-(comp_str*8),y_ini-linha_temp*bot->esp_linhas);
      linha_temp++;
     }
}

void desenhar_grafico(){
            
            unsigned short int x_actual=0, y_actual=0,xx=350,x_ini=410, y_ini=390,l_grafico_mm=60,y_val,y_val_reg, tam=(tam_FIFO);
          unsigned short int l_grafico=(unsigned short int)((float)l_grafico_mm/factor_espacial);
          unsigned char multiplicador=144,cont_temp;
          float passo_mm=10.0;
          unsigned short int passo=(unsigned short int)(passo_mm/factor_espacial);
          
         configurar_cor_principal(cor_principal_atual,0,0,0);
          configurar_cor_fundo(cor_fundo_atual,0,0,0);
          myGLCD.fillRect(x_ini-(unsigned int)((float)passo*3.5), y_ini-2,799,0);
        configurar_cor_principal(cor_principal_atual,255,255,255);

          myGLCD.print("Legendas",xx-2*passo,y_ini+50);
          myGLCD.drawRect(xx-2*passo,y_ini+5,799,479);
          myGLCD.drawRect(xx+(passo/2),y_ini+5,xx+(unsigned short int)((4.5)*(float)passo),479);
          myGLCD.print("Regene.",xx+(passo/2)+10,y_ini+50);
          myGLCD.print("Consumo",xx+passo*5-10,y_ini+50);

        configurar_cor_principal(cor_principal_atual,21,149,0);
          myGLCD.fillRect(xx+passo*3, y_ini+20, xx+passo*4, y_ini+20+passo);
        configurar_cor_principal(cor_principal_atual,0, 22, 250);
          myGLCD.drawRoundRect(xx+passo*7, y_ini+20, xx+passo*8, y_ini+20+passo);
          
        configurar_cor_principal(cor_principal_atual,0, 22, 250);
          configurar_cor_fundo(cor_fundo_atual,0,0,0);
          myGLCD.fillRect(x_ini,y_ini-passo,x_ini+1,y_ini-l_grafico);
          myGLCD.fillRect(x_ini,y_ini-l_grafico,x_ini+l_grafico,y_ini-l_grafico+2);
        configurar_cor_principal(cor_principal_atual,255, 255, 255);
          myGLCD.print("Energia",x_ini-(unsigned int)((float)passo*3.5), y_ini-passo);
          myGLCD.print("[J]",x_ini-passo*2, y_ini-18-passo);
          myGLCD.print("Tempo [s]", x_ini+passo*4, y_ini-l_grafico-(  passo/2));
    //      myGLCD.printNumI(l_grafico/tam, 20 ,20, 1);
          myGLCD.printNumI(0,x_ini-2,y_ini-l_grafico-5,1);
    for(cont_temp=0;cont_temp<tam;cont_temp++){
          x_actual=x_ini+cont_temp*passo;
          y_actual=y_ini-l_grafico+cont_temp*passo;
        configurar_cor_principal(cor_principal_atual,255, 255, 255);
          myGLCD.drawLine(x_ini,y_actual ,x_ini+10,y_actual);
          myGLCD.drawLine(x_actual,y_ini-l_grafico,x_actual,y_ini-l_grafico+10);
          myGLCD.printNumI(cont_temp*multiplicador,x_ini-passo, y_actual+10,1);
          myGLCD.printNumI(cont_temp+1,x_actual+passo-3,y_ini-l_grafico-6,1);
          
          y_val=y_ini-l_grafico+(short int)((passo_mm/(float)multiplicador)*(float)consumos_ultimos_minutos[cont_temp]/factor_espacial);
           y_val_reg=y_ini-l_grafico+(short int)((passo_mm/(float)multiplicador)*(float)reg_ultimos_minutos[cont_temp]/factor_espacial);
           
          if(((y_val-(y_ini-l_grafico))>=0)&(y_val<=(l_grafico))){
          configurar_cor_principal(cor_principal_atual,0, 22, 250);
            myGLCD.drawRoundRect(x_actual,y_ini-l_grafico,x_actual+(l_grafico/tam),y_val);
          //  myGLCD.printNumI(consumos_ultimos_minutos[cont_temp],700,300-cont_temp*18,4);
        }
        
        if(((y_val_reg-(y_ini-l_grafico))>=0)&(y_val_reg<=(l_grafico))){
          configurar_cor_principal(cor_principal_atual,21,149,0);
            myGLCD.fillRoundRect(x_actual+1,y_ini-l_grafico,x_actual+(l_grafico/tam)-1,y_val_reg);
            //delay(100);  
            configurar_cor_principal(cor_principal_atual,255,255,255);
          configurar_cor_fundo(cor_fundo_atual,0,0,0);
        }
    }
}


void iniciar_ecra(struct botao* bot, struct botao* sobre,struct botao* on_off){

                 myGLCD.clrScr();
                 myGLCD.clrScr();
                       //  Serial.println("Aqui");

        myGLCD.setFont(BigFont);
      configurar_cor_fundo(cor_fundo_atual,0, 0, 0);
    configurar_cor_principal(cor_principal_atual,255, 255, 255);
      myGLCD.print("Opcoes",50,445);

    bot->coord[0]=0;
        bot->coord[3]=420;
    bot->coord[1]=bot->coord[3]-125;
    bot->coord[2]=bot->coord[0]+200;

      bot->esp_linhas=18;

    if(modo==0){
      imprimir_todas_variaveis();
  configurar_cor_principal(cor_principal_atual,0, 22, 250);
    
    unsigned short int y_temp=480, x_temp=210;
   int y2=y_temp-50;
    int x2=799;
    myGLCD.drawLine(540, y2, 540, 0);

     myGLCD.fillRect(x_temp, y_temp, x2, y2);
      myGLCD.drawRect(x_temp,y2,x2,0);
  configurar_cor_principal(cor_principal_atual,255, 255, 255); //fundo cizeno escuro

// Draw some random filled, rounded rectangles
   configurar_cor_fundo(cor_fundo_atual,0, 22, 250);
   
    myGLCD.print("Variaveis em Tempo Real", 315, 460);

       configurar_cor_fundo(cor_fundo_atual,0, 0, 0);
   configurar_cor_principal(cor_principal_atual,255, 111, 0);
       
  bot->cor[0]=21;
  bot->cor[1]=149;
  bot->cor[2]=0;
  bot->cor[3]= 255;
  bot->cor[4]=255;
  bot->cor[5]=255;
  bot->texto_botao[0]="Ver";
  bot->texto_botao[1]="consumos";
  bot->texto_botao[2]="do";
  bot->texto_botao[3]="automovel";
  bot->n_linhas=4;
  bot->tipo_letra=BigFont;

    }else if(modo==1|modo==2){
      
      bot->cor[0]=0;
      bot->cor[1]=22;
      bot->cor[2]=250;
      
      bot->cor[3]=255;
      bot->cor[4]=255;
      bot->cor[5]=255;
      
      bot->texto_botao[0]="Ver";
      bot->texto_botao[1]="variaveis";
      bot->texto_botao[2]="em tempo";
      bot->texto_botao[3]="real";
      bot->n_linhas=4;
      bot->tipo_letra=BigFont;
    if(modo==1)
      desenhar_grafico();
    }
    if(modo==2){
        myGLCD.setFont(SmallFont);
       unsigned short int x_=240;
              configurar_cor_fundo(cor_fundo_atual,0, 0, 0);
   configurar_cor_principal(cor_principal_atual,255, 255, 255);
       myGLCD.print("Trabalho realizado no ambito da dissertacao:",x_,400);            
       myGLCD.print("'Desenvolvimento de um Carro Eletrico Puro: Instrumentacao e",x_, 380);
       myGLCD.print("Registos Energeticos'",x_, 360);
       myGLCD.print("Ano: 2014/2015",x_,340);
       myGLCD.print("Controlador de motor eletrico inicialmente desenvolvido por",x_,320);
       myGLCD.print("Miguel Bento (2013)",x_,300);
       myGLCD.print("Controlador de motor eletrico alterado por Ricardo Pina",x_,280);
       myGLCD.print("Computador de bordo desenvolvido por Ricardo Pina",x_,260);
       myGLCD.print("Comunicacao entre Controlador e Comp. de Bordo efetuada segundo",x_,240);
       myGLCD.print("protocolo CAN 2.0A a 250 kb/s",x_,220);
       myGLCD.print("Balancos energeticos registados em Cartao SD",x_,200);
      myGLCD.drawBitmap(x_, 50, 183, 45, logo_deti,2);
    }
    imprimir_botao(bot);

  on_off->cor[0]=234;
  on_off->cor[1]=2;
  on_off->cor[2]=0;
  on_off->cor[3]= 255;
  on_off->cor[4]=255;
  on_off->cor[5]=255;
  on_off->texto_botao[0]="Limpar";
  on_off->texto_botao[1]="ecra";
  on_off->n_linhas=2;
  on_off->esp_linhas=18;
  on_off->coord[0]=0;
  on_off->coord[3]=bot->coord[1]-10;
  on_off->coord[2]=on_off->coord[0]+bot->coord[2]-bot->coord[0];
  on_off->coord[1]=on_off->coord[3]-80;
  on_off->tipo_letra=BigFont;  
  imprimir_botao(on_off);
  
  sobre->cor[0]=129;
  sobre->cor[1]=129;
  sobre->cor[2]=129;
  sobre->cor[3]= 255;
  sobre->cor[4]=255;
  sobre->cor[5]=255;
  sobre->texto_botao[0]="Informacao";
  sobre->texto_botao[1]="sobre";
  sobre->texto_botao[2]="prototipo";
  sobre->texto_botao[3]="desenvolvido";
  sobre->n_linhas=4;
  sobre->esp_linhas=18;
    sobre->coord[0]=0;
  sobre->coord[3]=on_off->coord[1]-10;
  sobre->coord[2]=sobre->coord[0]+on_off->coord[2]-on_off->coord[0];
  sobre->coord[1]=sobre->coord[3]-120;
  sobre->tipo_letra=BigFont;
  if(modo!=2)
    imprimir_botao(sobre);
}

void estado_ecra(bool estado){
 // digitalWrite(28,HIGH);
  if(estado){

     //   myGLCD.lcdOn();
        myGLCD.Wake();
        delay(500);
    }else{
         myGLCD.clrScr();
         myGLCD.clrScr();
        // myGLCD.lcdOff();
        myGLCD.Sleep();
  }
//digitalWrite(28,LOW);
}
