
void processar_dados_recebidos(CAN_FRAME &frame){
        unsigned short int k=0;
      
 /*   if(frame.id==0x4){
      printFrame(frame);
    }*/
  unsigned char tipo=0;
             union{
                  float floating;
                  unsigned int integer;
          }uniao;
           uniao.integer=0;
           
           if(frame.id==0x4){
             
             tipo=0;
              indice_can=0x4-frame.id;
              can_u_ints[indice_can]=0;
             for (k = 0; k < frame.length; k++) {
                can_u_ints[indice_can]=can_u_ints[indice_can]|((frame.data.bytes[k])<<(8*k));
              }
              //  Serial.print("frame ID=0x19  ");  
                uniao.integer=can_u_ints[indice_can];
             //   Serial.println(can_u_ints[indice_can], DEC);
           //Serial.print(can_u_ints[0], DEC);
           indice_texto=indice_can+(sizeof(can_chars)/sizeof(can_chars[0]));

          }else if((frame.id>=id_ini)&(frame.id<=0x3)){
            
            if(frame.id==id_ini&frame.data.bytes[0]==1){
              if(dia!=ultimo_dia){
              ultimo_dia=dia;
              novo_fic(&ficheiro,nome_fic);
              }
             if(iniciacao==0)
              novo_min_fic(&ficheiro, nome_fic);
              Timer1_Tick_config();
            }else if(frame.id==id_ini&frame.data.bytes[0]==0){
              Timer1_Tick_disable();
            }
            tipo=0;
           indice_can=frame.id-id_ini;
           can_chars[indice_can]=frame.data.bytes[0];
           uniao.integer=can_chars[indice_can];
           indice_texto=indice_can;
            
        }else if((frame.id>=0x5)&(frame.id<=id_fin)){
     //     Serial.println("recebido float");
            tipo=1;
            indice_can=frame.id-0x5;
           //   unsigned char a;
              uniao.integer=0;
            for(k=0;k<frame.length;k++){
              uniao.integer= uniao.integer|((frame.data.bytes[k])<<(8*k));
            }
                  
            if(frame.id==0x5 | frame.id==0x8){
              if(can_floats[indice_can]!=uniao.floating){
                calcula_consumo_res_timer();
                can_floats[indice_can]=uniao.floating;
                }
            }else{
              can_floats[indice_can]=uniao.floating;
            }
              indice_texto=indice_can+(sizeof(can_chars)/sizeof(can_chars[0]))+(sizeof(can_u_ints)/sizeof(can_u_ints[0]));
        }
        //  digitalWrite(led_pin, frame.id==0x1A);
          if((frame.id>=id_ini & frame.id<=id_fin)&(modo==0) & alerta[0]==0 & alerta[1]==0){
            if(tipo){
              imprimir_var(uniao.floating, indice_texto,tipo); 
            }else if(!tipo){
             imprimir_var(uniao.integer, indice_texto,tipo); 
          }
        }
           
           
            if((can_floats[0]>=can_floats[2])&!alerta[0]&contador_RTR==(id_fin-id_ini+1)){
              
              alerta[0]=1;
                 if(alerta[1]==0){
                myGLCD.clrScr();
                myGLCD.clrScr();
              }
          myGLCD.drawBitmap(280, 190, 89, 75, curto_cir,1);
           configurar_cor_principal(cor_principal_atual,255,242,0);
              myGLCD.print("Problema de",230,320);
              myGLCD.print("Sobreintensidade!",230,290);
              imprimir_var(can_floats[2],b+2,1);
            }
            
            if(((can_floats[5]>=can_floats[7])|(can_floats[6]>=can_floats[7]))&!alerta[1]&contador_RTR==(id_fin-id_ini+1)){
              alerta[1]=1;
              if(alerta[0]==0){
                myGLCD.clrScr();
                myGLCD.clrScr();
              }
          myGLCD.drawBitmap(600, 10, 85, 75, alta_temp,1);
            configurar_cor_principal(cor_principal_atual,255,242,0);
              myGLCD.print("Problema de",510,140);
              myGLCD.print("Sobreaquecimento!",510,120);
              imprimir_var(can_floats[7],b+7,1);
          }
          
            if((can_floats[0]<can_floats[2])&alerta[0]&contador_RTR==(id_fin-id_ini+1)){
              alerta[0]=0;
              
           if(!alerta[0] & !alerta[1])
            iniciar_ecra(&mudar_ecra, &acerca,&on_off);
              }
          if((can_floats[5]<can_floats[7])&(can_floats[6]<can_floats[7])&alerta[1]&contador_RTR==(id_fin-id_ini+1)){
             alerta[1]=0;
             
           if(!alerta[0] & !alerta[1])
            iniciar_ecra(&mudar_ecra, &acerca,&on_off);
          }
}

void printFrame(CAN_FRAME &frame){
   Serial.print("rtr: ");
   Serial.println(frame.rtr, DEC);
   Serial.print("ID: 0x");
   Serial.println(frame.id, HEX);
   Serial.print(" Len: ");
   Serial.println(frame.length);
   Serial.print(" Data: 0x");
   for (int count = 0; count < frame.length; count++) {
       Serial.print(frame.data.bytes[count], HEX);
       Serial.print(" ");
   }
   Serial.print("\r\n");
}

bool verificar_barramento_e_processar(CAN_FRAME& verificar){
  unsigned short int id_inicial=verificar.id;
  bool retorno=0;
//delay(1000);
    if(Can0.available()>0){
      /*
Serial.print("ANTES:");
Serial.println(id_inicial,HEX);*/
	Can0.read(verificar);
       retorno=verificar.id==id_inicial;
/*
Serial.print("DEPOIS:");
Serial.println(verificar.id,HEX);*/  
   // verificar->id--;
//printFrame(verificar);
        processar_dados_recebidos(verificar);

  return retorno;
  }
}

void verificar_variaveis_e_receber(){
  CAN_FRAME tempo;

  bool var_temp=false;
  unsigned short int id_t=id_ini;
  contador_RTR=0;
        tempo.rtr=0;
  while(contador_RTR<=(id_fin-id_ini)){
 //   Serial.println(CAN_MCR_MRTR,DEC);
  /*  for(contador=0;contador<7;contador++){
    Can0.mailbox_tx_frame(contador);
    }*/
Serial.println("loop do can");
      tempo.length=0;
      tempo.rtr=1;
      tempo.id=id_t;
    
   /* interruptCtr=0;
    while(interruptCtr==0);*/
      while(Can0.sendFrame(tempo)==0);

    var_temp = verificar_barramento_e_processar(tempo);
//    printFrame(tempo);
    if(var_temp){
      var_temp=false;
      contador_RTR++;
      id_t++;
    }
    /*
Serial.print("id=0x");
Serial.println(id_t,HEX);
*/
    }
   //         Serial.println("fim loop do can");



}




