//--Bibliotecas nÃ£o oficiais do arduino Usadas -- UTFT, UTouch, advancedFunctions (para o RTC), due_can

//#include "SPI.h"
#include <UTFT.h>
#include <UTouch.h>
#include "variant.h"
#include <due_can.h>
#include "advancedFunctions.h"

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>*/
/*
//Font selection 
//BVS= Bitstream vera sans, suffix = font size in pixel (height)
#define BVS_13 10
#define BVS_15 12
#define BVS_19 14
#define BVS_22 18
#define BVS_28 22
#define BVS_34 28
#define BVS_43 38
#define BVS_52 53
#define BVS_74 78
#define BVS_112 122

#define FLASH_CS_PIN 52 
#define SPI_RATE 2
//#define Serial SerialUSB
*/
extern prog_uint16_t alta_temp[], logo_deti[], curto_cir[]; //dim_curto=178*150: 
extern uint8_t BigFont[];
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];

struct botao{
	unsigned short int cor[6];
   unsigned short int n_linhas=4;
   unsigned short int coord[4];
     char *texto_botao[10];
   unsigned short int esp_linhas;
   unsigned char* tipo_letra;
};

//UTFT myGLCD(CTE70,25,26,27,28);   // Remember to change the model parameter to suit your display module!
UTFT myGLCD(CTE70);
UTouch myTouch(6,5,32,3,2);

char coord_t[50], texto_vars[50],vazio[12];
int stCurrentLen=0;
int x_quad,y_quad,l_quad=50;

const unsigned int y_ini_vars = 420;
short int contador,cont_dedicado=0;

unsigned char estado=false;
bool card=0;
    //-----VariÃ¡veis-sobre-CAN
char can_chars[4] = {0,0,0,0};
unsigned short int can_u_ints[1]={0};
float can_floats[8]={ 0.0, 0.0 ,0.0 ,0.0 ,0.0, 0.0, 0.0, 0.0};
const unsigned short int id_ini =0x0, id_fin=0xC;
unsigned short int contador_RTR=0;
unsigned char cont_seg=0;
unsigned char b=((sizeof(can_chars)/sizeof(can_chars[0]))+(sizeof(can_u_ints)/sizeof(can_u_ints[0])));
unsigned short int contador_fic=0;

char *variaveis[13]={"Controlador[ON/OFF]","Acelerador[%]","Travao[%]","Sent de Rot[ON/OFF]","Vel de Rotacao[RPM]","I Motor[A]","I Buck[A]","Limite I Motor[A]","V Serie BAT[V]", "V BAT da elet[V]","T motor[*C]","T BAT[*C]","Limite de T[*C]"};
unsigned char indice_can=0, indice_texto=0;

unsigned char cor_principal_atual[3];
unsigned char cor_fundo_atual[3];

unsigned char modo=0;
float factor_espacial =0.1809;  //factor que relaciona pixels com dimensoes em mm

//-----VariÃ¡veis-sobre-consumo-energÃ©tico
unsigned int consumos_ultimos_minutos[6] = {0,0,0,0,0,0}, energia_de_agora=0;
unsigned int consumo_ultimo_segundo=0,consumo_ultimo_minuto=0,cont_temporizador=0, delta_temporizador=0;
unsigned int reg_ultimos_minutos[6] = {0,0,0,0,0,0}, reg_ultimo_minuto=0,reg_ultimo_segundo=0;
int consumo_ultimo_segundos[6]={0,0,0,0,0,0};
bool alerta[2]={0,0};
const unsigned short int tam_FIFO = sizeof(consumos_ultimos_minutos)/sizeof(consumos_ultimos_minutos[0]);

const int led_pin = 13;
bool state = false, iniciacao=1;
unsigned int interruptCtr = 1;
unsigned int temp_int = 1000;
byte canal=0;

struct botao mudar_ecra,on_off,acerca;

unsigned short int y_ini_teclado=150, x_ini_teclado=200;
unsigned char tam_digito[2]={50,32};
String Data, Horas;
unsigned char hora,dia,minuto,segundo,mes,ultimo_dia=200,ultimo_minuto=200;
unsigned int ano;

#include <SPI.h>
#include <SD.h>
File ficheiro;
char nome_fic[30];

//myRTC rtc;
#include "registos_em_fic.h"
#include "funcs_template.h"
#include "funcs_TFT_Tactil.h"
#include "FIFO.h"
#include "datas.h" 
#include "timers.h"
#include "CAN.h"


void setup()
{
// Initial setup
//myGLCD.SPI_Flash_init(FLASH_CS_PIN,SPI_RATE);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.begin(115200);
  
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.clrScr();
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_EXTREME);
//definir_data();
iniciar_SPI_SD(&ficheiro, nome_fic);
novo_fic(&ficheiro, nome_fic);
novo_min_fic(&ficheiro,nome_fic);
 // imprimir_todo_fic(&ficheiro, nome_fic);  
//printDirectory(ficheiro,0);

  hora=rtc.getHours();
  dia=rtc.getDay();
  minuto=rtc.getMinutes();
  
  for(contador=0;contador<(sizeof(vazio)/sizeof(vazio[0]))-1;contador++){
      vazio[contador]=' ';
  }
  vazio[(sizeof(vazio)/sizeof(vazio[0]))-1]='\0';
  
   //pinMode(14, OUTPUT);
   Timer2_Tick_config(1000);
  Timer1_Tick_config();
  
  // Initialize CAN0 , Set the proper baud rates here
  Can0.begin(CAN_BPS_250K, 8);
  
  //By default there are 7 mailboxes for each device that are RX boxes
  //This sets each mailbox to have an open filter that will accept extended
  //or standard frames
  int filter;
  //extended
  for (filter = 0; filter < 3; filter++){
	Can0.setRXFilter(filter, 0, 0, true);
  }
  //standard
  for (filter = 3; filter < 7; filter++){
	Can0.setRXFilter(filter, 0, 0, false);
  }
  
//  Can0.setNumTXBoxes(7);
//contador_RTR=id_fin-id_ini;
verificar_variaveis_e_receber();
 // Serial.println("Serial finito");
}

void loop(){
 // Serial.println("antes loop princ");
  unsigned short int x_conv,y_conv;
  iniciar_ecra(&mudar_ecra, &acerca,&on_off);
   configurar_cor_fundo(cor_fundo_atual,0, 22, 250);

  CAN_FRAME incoming_main;

  while (true)
  {
   // Serial.println(nome_fic);
    if (Can0.available()> 0){
	Can0.read(incoming_main);  
        processar_dados_recebidos(incoming_main);
      }
      
    if (myTouch.dataAvailable()&!alerta[0]&!alerta[1])
    {
      myTouch.read();
      while(myTouch.dataAvailable());

       x_conv=ler_x_tatil(); y_conv=ler_y_tatil();
      if(modo==3){
        estado_ecra(1);
          modo=0;
          iniciar_ecra(&mudar_ecra, &acerca,&on_off);
        }else if(verificar_contacto_botao(x_conv, y_conv, &mudar_ecra)){
         modo=!modo;
           iniciar_ecra(&mudar_ecra, &acerca,&on_off);
        }else if(verificar_contacto_botao(x_conv, y_conv, &on_off)){
         modo=3;
         estado_ecra(0);

         delay(500);

        }else if(verificar_contacto_botao(x_conv, y_conv, &acerca)){
        if(modo==2){
          modo=0;         
                iniciar_ecra(&mudar_ecra, &acerca,&on_off);
        }else{
        modo=2;
       iniciar_ecra(&mudar_ecra, &acerca,&on_off);
  }
    }
     }
    // Serial.print("contador_RTR=");
      //Serial.println(contador_RTR,DEC);
     
   /*  for(contador=0;contador<2;contador++){
         Serial.print("Alerta["); Serial.print(contador, DEC); Serial.print("] = ");  Serial.println(alerta[contador], DEC);
      }*/
}
}


