void Timer1_Tick_config(){
        
        canal=0;
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk(TC3_IRQn);//IRQ=ID_TC3
        TC_Configure(TC1, canal, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/1; //128 because we selected TIMER_CLOCK4 above
       // TC_SetRA(TC1, channel, rc/2); //50% high, 50% low
        TC_SetRC(TC1, canal, rc);// 2+1
        TC_Start(TC1, canal);
        TC1->TC_CHANNEL[canal].TC_IER=TC_IER_CPCS;
        TC1->TC_CHANNEL[canal].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(TC3_IRQn);
}

void Timer2_Tick_config(unsigned int t_mseg){
  
  temp_int = t_mseg;
  /* turn on the timer clock in the power management controller */
  pmc_set_writeprotect(false);		 // disable write protection for pmc registers
  pmc_enable_periph_clk(ID_TC7);	 // enable peripheral clock TC7

  /* we want wavesel 01 with RC */
  TC_Configure(/* clock */TC2,/* channel */1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4); 
  uint32_t rc = VARIANT_MCK/128/1; //128 because we selected TIMER_CLOCK4 above
 
  TC_SetRC(TC2, 1, rc*(t_mseg/1000)); //Interrup��es de 1 em 1 segundo
  TC_Start(TC2, 1);

  // enable timer interrupts on the timer
  TC2->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;   // IER = interrupt enable register
  TC2->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;  // IDR = interrupt disable register

  /* Enable the interrupt in the nested vector interrupt controller */
  /* TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number (=(1*3)+1) for timer1 channel1 */
  NVIC_EnableIRQ(TC7_IRQn);
}

void calcula_consumo_res_timer(){

    if(TC1->TC_CHANNEL[canal].TC_CV>=cont_temporizador){
    delta_temporizador=TC1->TC_CHANNEL[canal].TC_CV-cont_temporizador;
       // digitalWrite(led_pin, LOW);
    }else{
    delta_temporizador=(VARIANT_MCK/128)-cont_temporizador;
 //   digitalWrite(led_pin, HIGH);
    }

 //   Serial.print("TC1->TC_CHANNEL[canal].TC_CV=");
 //   Serial.println(TC1->TC_CHANNEL[canal].TC_CV,DEC);
 //    Serial.print("cont_temporizador=");
  //  Serial.println(cont_temporizador,DEC);
    
      cont_temporizador=TC1->TC_CHANNEL[canal].TC_CV;
    TC_GetStatus(TC1, 0);

//Timer1_Tick_config();
         // TC1->TC_CHANNEL[canal].TC_CV=0;

        /*
 Serial.print("TC1->TC_CHANNEL[canal].TC_CV=");
 Serial.println(TC1->TC_CHANNEL[canal].TC_CV,DEC);
*/
    int ener_temp=(int)((can_floats[3]*can_floats[0])*(((double)delta_temporizador)/(double)(VARIANT_MCK/128))); //E[J]=V[A]*I[A]*t[s];
    /*
   Serial.print("delta_temporizador=");
    Serial.println(delta_temporizador,DEC);
     Serial.print("VARIANT_MCK=");
    Serial.println(VARIANT_MCK,DEC);*/
    
        if(ener_temp>=0){
            consumo_ultimo_segundo+=ener_temp;
            consumo_ultimo_minuto+=ener_temp;
          }else{
            reg_ultimo_segundo+=(unsigned int)ener_temp*-1;
            reg_ultimo_minuto+=(unsigned int)ener_temp*-1;
          }
}

 void Timer2_Tick_disable(){
 
  TC_Stop(TC2, 1);

  // enable timer interrupts on the timer
  TC2->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;   // IER = interrupt enable register
  TC2->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;  // IDR = interrupt disable register

  /* Enable the interrupt in the nested vector interrupt controller */
  NVIC_DisableIRQ(TC7_IRQn); 
 
 }

 
void TC3_Handler(){
      TC_GetStatus(TC1, 1);
  TC1->TC_CHANNEL[canal].TC_CV=VARIANT_MCK/128/1; //128 because we selected TIMER_CLOCK4 above
    calcula_consumo_res_timer();
     //      uint32_t rc = VARIANT_MCK/128/1; //128 because we selected TIMER_CLOCK4 above
       // TC_SetRA(TC1, channel, rc/2); //50% high, 50% low
      //  TC_SetRC(TC1, canal, rc);// 2+1
}

void TC7_Handler(){

  
  TC_GetStatus(TC2, 1);
  
//Serial.print("TC1->TC_CHANNEL[canal].TC_CV=");
//Serial.println(TC1->TC_CHANNEL[canal].TC_CV,DEC);
  // interruptCtr++;
          calcula_consumo_res_timer();
        
      cont_seg++;
  //  Serial.print("elem= ");
   // Serial.print(reg_ultimo_segundo,DEC);

  somar_novos_registos(consumos_ultimos_minutos, tam_FIFO,&consumo_ultimo_segundo);
  somar_novos_registos(reg_ultimos_minutos,tam_FIFO,&reg_ultimo_segundo);

   cont_temporizador=0;
      
    if(cont_seg==tam_FIFO){
      if(modo==1){
            desenhar_grafico();
          }

        cont_seg=0;
     if(can_chars[0]==1)
        gravar_em_fic(&ficheiro, nome_fic);
    }
            atualizar_horas();  

      //para agora regista-se de um em 1 minuto
        if(ultimo_minuto!=minuto){
        ultimo_minuto=minuto;
        iniciacao=0;
            novo_min_fic(&ficheiro,nome_fic);
        // imprimir_todo_fic(&ficheiro,nome_fic);
        }
        if(dia!=rtc.getDay()){
          atualizar_data();
     if(can_chars[0]==1){
         ultimo_dia=dia;
          novo_fic(&ficheiro,nome_fic);
          
          }
        }
    
        if(modo!=3){
         ano=rtc.getYear();
         mes=rtc.getMonth();
         dia=rtc.getDay();
          configurar_cor_fundo(cor_fundo_atual,0,0,0);
        configurar_cor_principal(cor_principal_atual,21,149,0);
        
          myGLCD.print(Data,0,20);
         
          myGLCD.print(Horas,0,50);
        }
        
      
          configurar_cor_principal(cor_principal_atual,255, 111, 0);

    if(alerta[1]){
              configurar_cor_fundo(cor_fundo_atual,0,0,0);
                 configurar_cor_principal(cor_principal_atual,255, 111, 0);
              myGLCD.setFont(BigFont);
      if(can_floats[6]>=can_floats[7]){
                 imprimir_var(can_floats[6],b+6 ,1); 
                }
                if(can_floats[5]>=can_floats[7]){
                 imprimir_var(can_floats[5],b+5,1); 
                }
     }
     
        if(alerta[0]){
              configurar_cor_fundo(cor_fundo_atual,0,0,0);
   configurar_cor_principal(cor_principal_atual,255, 111, 0);
              myGLCD.setFont(BigFont);
      if(can_floats[0]>=can_floats[2]){
                 imprimir_var(can_floats[0],b ,1); 
      }
     }
 }


 
  void Timer1_Tick_disable(){
 
  TC_Stop(TC1, 1);
  // enable timer interrupts on the timer
  TC1->TC_CHANNEL[canal].TC_IER=TC_IER_CPCS;   // IER = interrupt enable register
  TC1->TC_CHANNEL[canal].TC_IDR=~TC_IER_CPCS;  // IDR = interrupt disable register
  /* Enable the interrupt in the nested vector interrupt controller */
  NVIC_DisableIRQ(TC3_IRQn); 
 
 }

