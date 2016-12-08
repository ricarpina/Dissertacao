
struct botao_numero{
   unsigned short int x_ini, x_fin, y_sup, y_inf;
};
struct botao_numero teclado_num[10];


void atualizar_horas(){

    Horas="";
           hora=rtc.getHours();
          minuto=rtc.getMinutes();;
          segundo=rtc.getSeconds();

         if(hora<10)
            Horas+="0";
          Horas+=hora;
          Horas+=":";

        if(minuto<10)
            Horas+="0";
            Horas+=minuto;
           Horas+=":";
            
           if(segundo<10)
            Horas+="0";
           Horas+=segundo;
  
}

void atualizar_data(){
        dia=rtc.getDay();
        mes=rtc.getMonth();
        ano=rtc.getYear();
          Data="";

         if(dia<10)
            Data+="0";
          Data+=dia;
          Data+="/";

           if(mes<10)
            Data+="0";
          Data+=mes;
          Data+="/";
          Data+=ano;

}

unsigned char ler_valor_digitado(){
	bool var = false;
	unsigned char i,j,num=0;
        unsigned short int h_bot=tam_digito[0]*1.5,l_bot=tam_digito[1]*1.5, x_centro,y_centro,x_conv,y_conv,x,y;

           y=y_ini_teclado-(h_bot+5);
                y_centro=y+(tam_digito[0]/2);

           while(!var){
             if(myTouch.dataAvailable()){
                   myTouch.read();
                 while(myTouch.dataAvailable());
                 x_conv=ler_x_tatil();
                 y_conv=ler_y_tatil();
                 myGLCD.print("            ",650,400);
          //  myGLCD.printNumI(x_conv,650,400,4);
           // myGLCD.printNumI(y_conv,720,400,4);
                                        num=0;
              while(!var & num<=9){
              if((x_conv>=teclado_num[num].x_ini) & (x_conv<=teclado_num[num].x_fin) & (y_conv>=teclado_num[num].y_inf) & (y_conv<=teclado_num[num].y_sup)){
                      var=true;        
                }else{
	        	num++;
		}
		}
                  }
                }
	return num;
}

unsigned short int ler_numero(unsigned char n_digitos, unsigned short x_p, unsigned short y_p){

	unsigned short int retorno=0, val_temp, x_p_ini=x_p;
		unsigned int mult=1, a;		
		for(a=0;a<(n_digitos-1);a++){
			mult*=10;
		}
              	myGLCD.print("                ",x_p_ini,y_p);

		for(a=n_digitos;a>0;a--){
				val_temp=ler_valor_digitado();
				retorno+=val_temp*mult;
				myGLCD.printNumI(val_temp,x_p,y_p,1);
				x_p+=20;
				mult/=10;
		}
return retorno;
}

unsigned char nDias(unsigned char m, unsigned int a){
	if(m == 2 && rtc.isLeapYear(a)){
			return 29;
					}
		else if (m == 2 && !rtc.isLeapYear(a)) {
  			 return 28;						
				}
			else if(m%2 == 0){
				return 31;
				}
				else{
			return 30;
			}
}

void definir_data(){

	      configurar_cor_principal(cor_principal_atual,255,255,255);
	
		//imprimir teclado
		myGLCD.setFont(SevenSegNumFont);
			int x,y;
                unsigned short int h_bot=tam_digito[0]*1.5,l_bot=tam_digito[1]*1.5, x_centro,y_centro;
		unsigned char i,j,num=0;
                	
                y=y_ini_teclado-(h_bot+5);
                y_centro=y+(tam_digito[0]/2);
                
                myGLCD.drawRect(x_ini_teclado,y_centro-h_bot,x_ini_teclado+l_bot,y_centro);
                myGLCD.printNumI(num,x_ini_teclado+(tam_digito[1]/3),y+(tam_digito[0]/3),1);
		teclado_num[num].x_ini=x_ini_teclado; teclado_num[num].x_fin=x_ini_teclado+l_bot; teclado_num[num].y_sup=y_centro; teclado_num[num].y_inf=y_centro-h_bot;
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
			num++;
			x=x_ini_teclado+h_bot*j;
			y=y_ini_teclado+(h_bot+5)*i;
                        x_centro=x;
                        y_centro=y+(tam_digito[0]/2);

			myGLCD.drawRect(x_centro,y_centro-(h_bot),x_centro+(l_bot),y_centro);
			myGLCD.printNumI(num,x_centro+(tam_digito[1]/3),y_centro-(tam_digito[0]/3),1);
		teclado_num[num].x_ini=x_centro; teclado_num[num].x_fin=x_centro+l_bot; teclado_num[num].y_sup=y_centro; teclado_num[num].y_inf=y_centro-h_bot;
                        }
		}

	        myGLCD.setFont(BigFont);

        //   myGLCD.Set_character_spacing(1); 
         //   myGLCD.Put_Text("Coldtears electronics",5,400,BVS_34);
	        myGLCD.print("Configurar data e hora", x_ini_teclado, 420);
		myGLCD.print("Digitar Ano", x_ini_teclado, 400);

			x=450,y=400;
	     ano=ler_numero(4, x, y);
               myGLCD.printNumI(ano, 10, 100,4);
		myGLCD.print("               ", x_ini_teclado, 400);
		myGLCD.print("Digitar mes", x_ini_teclado, 400);
				
		 mes=ler_numero(2, x, y);
		if(mes>12){
			mes=12;
                 }

		myGLCD.print("               ", x_ini_teclado, 400);
		myGLCD.print("Digitar dia", x_ini_teclado, 400);
                  
                  dia=ler_numero(2, x, y);
                  if(dia>nDias(mes,ano)){
                      dia=nDias(mes,ano);
                  }/*else if(dia==0){
                      dia=1;
                  }*/
                 
                 myGLCD.print("               ", x_ini_teclado, 400);
		 myGLCD.print("Digitar hora", x_ini_teclado, 400);
                  hora=ler_numero(2, x, y);
              
              /*   if(hora>=24){
                     hora=0;
                 }*/

                 myGLCD.print("               ", x_ini_teclado, 400);
		 myGLCD.print("Digitar minuto", x_ini_teclado, 400); 
                  minuto =ler_numero(2, x, y);
          /*       if(minuto>=60){
                     minuto=59;
                 }*/
                 myGLCD.clrScr();
                 myGLCD.clrScr();
             //  RTC=myRTC();
                     rtc.setDate(ano, mes, dia);
                rtc.setTime(hora, minuto); //set here the time
               
                 atualizar_horas();
                atualizar_data();
                ultimo_minuto=minuto;
                ultimo_dia=dia;
}


