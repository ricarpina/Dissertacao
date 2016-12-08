
 void construir_string_nome_fic(char* nome){

 String  nome_fich = "CERP_#";
  nome_fich+=contador_fic;
  nome_fich+=".txt";
  nome_fich.toCharArray(nome,30);
  
}

void printDirectory(File dir, int numTabs) {
    dir = SD.open("/");

   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     
     Serial.print(entry.name());
     if (entry.isDirectory()){
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     }else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
     Serial.println("Lista de ficheiros impressa!");
}

void iniciar_SPI_SD(File* fic, char* nome){
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  
  pinMode(53, OUTPUT);

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }
card=1;
  Serial.println("ok.");
contador_fic=1;
construir_string_nome_fic(nome);
  while(SD.exists(nome)){
    contador_fic++;
    construir_string_nome_fic(nome);
  }
  
  *fic = SD.open("/");
  //printDirectory(fic, 0);

}


void novo_fic(File* fic, char* nome){
  if (card){
if(contador_fic>=99){
    contador_fic=1;
       construir_string_nome_fic(nome);

    while(SD.exists(nome)&&contador_fic<=99){
    
    if(SD.exists(nome)){  
      SD.remove(nome);
      Serial.println("elimina");
        }
        contador_fic++;
            construir_string_nome_fic(nome);
    }
        contador_fic=1;
        construir_string_nome_fic(nome);
}else{           
  digitalWrite(13, HIGH);
   construir_string_nome_fic(nome);
  while(SD.exists(nome)&&contador_fic<=99){
    contador_fic++;
    construir_string_nome_fic(nome);
  }

 *fic=SD.open(nome, FILE_WRITE);
 
 if (*fic){
         fic->println(Data);
        }
        
  fic->close();

}
}
}
void imprimir_todo_fic(File* fic, char* nome){
    *fic = SD.open(nome, FILE_WRITE);
     String temporario;
     char temp;
     while (fic->available()) {
       temporario+=fic->read();
      // temporario+=temp;
       Serial.print(temporario);
     //  temporario="";
     }
  fic->close();
}

void novo_min_fic(File* fic, char* nome){
state=!state;
  //  Serial.println("minuto passou");
    *fic = SD.open(nome, FILE_WRITE);
  //  char nome_temp;
    digitalWrite(led_pin,state);

    //String dataString="Hora: ";
   // dataString+=Horas;
    
    if (*fic){
    fic->println(Horas);
   //  Serial.println(Horas);
    }
    fic->close();
}

void gravar_em_fic(File* fic, char* nome){
  String dataString="";
  short int c_temp;
         *fic = SD.open(nome, FILE_WRITE);
   // Serial.println("loop_gravar_em_fic");

  for(c_temp=(tam_FIFO)-1;c_temp>=0;c_temp--){ //Registo mais antigo primeiro
       dataString+=consumos_ultimos_minutos[c_temp];           
      dataString+=' ';
      dataString+=reg_ultimos_minutos[c_temp];
       if (*fic){
          fic->println(dataString);
     //     Serial.println(dataString);
      }

      dataString="";    
    }
      fic->close();
}
 
