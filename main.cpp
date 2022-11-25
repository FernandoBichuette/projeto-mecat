#include "mbed.h"
#include "TextLCD.h"




// Host PC Communication channels
Serial pc(USBTX, USBRX); // tx, rx
 
// I2C Communication
I2C i2c_lcd(I2C_SDA, I2C_SCL); // SDA, SCL >>> NUCLEO: D14,D15
 
//I2C Portexpander PCF8574
TextLCD_I2C lcd(&i2c_lcd, 0x7E, TextLCD::LCD20x4); // I2C bus, PCF8574 Slaveaddress, LCD Type Original = 0x42, Usando I2C scanner = 0x3F



DigitalOut clk(PB_15);


InterruptIn botao_confirma(PC_13);

InterruptIn botao_emergencia(PA_10);

InterruptIn sensor_x_1(PB_9);
InterruptIn sensor_x_2(PC_10);
DigitalIn x_mais(PB_4);
DigitalIn x_menos(PB_3);
DigitalOut enable_x(PC_5);
DigitalOut clkwise_x(PC_6);


InterruptIn sensor_y_1(PC_8);
InterruptIn sensor_y_2(PC_12);
DigitalIn y_mais(PA_8);
DigitalIn y_menos(PB_10);
DigitalOut enable_y(PB_11);
DigitalOut clkwise_y(PB_12);


//InterruptIn sensor_z_1(D8);
//InterruptIn sensor_z_2(D9);
//DigitalIn z_mais(D4);
//DigitalIn z_menos(D5);
//DigitalOut enable_z(PB_4);
//DigitalOut clkwise_z(PB_5);


float delay = 0.1;
float tempo = 950;

int ola;
int quantos_depositos;
int volumes;
int confirma1;

int contador_x1;
int contador_x2;
int curso_x1;  
int curso_x2;
int horario_x;
int referenciar_x;
int outro_lado_x;
int inverter_x;
int fonte_x;
int deposito_x;
int feito_x1;
int feito_x2;


int contador_y1;
int contador_y2;
int curso_y1;  
int curso_y2;
int horario_y;
int referenciar_y;
int outro_lado_y;
int inverter_y;
int fonte_y;
int deposito_y;
int feito_y1;
int feito_y2;


int correcao;



int jog;
int ciclo_jog;

int volta_zero;
int volta_zero2;

int ciclo_da_fonte;


int ciclo_automatico;

int jog_auto;

int quantos;
int volume;
int teste;

int estado_sis = 1;

void fim_de_curso_x_1(){   

    if (contador_x1>0 && contador_x2==0){
        horario_x=1;
        referenciar_x=0;
        outro_lado_x = 1;
        clkwise_x=1;
        if(feito_x1==0){
        horario_x=1;
        referenciar_x=0;
        outro_lado_x = 1;
        clkwise_x=1;
        feito_x1=1;
            for(int i=0;i<400;i++){
                enable_x=0;
                feito_x1=1;
                curso_x1=contador_x1-i;
                clk=!clk;
                wait_us(tempo);
            }
        }
        else{
            enable_x=1;
        }    
        enable_x=1;
       ///printf("\rX1=%d\n",contador1);
    }
   if(contador_x1<100 && sensor_x_1==0 && contador_x2==0){
        horario_x=1;
        enable_x=0;
        referenciar_x=0;
          clkwise_x=1;
        for(int i=0;i<1000;i++){
            clk=!clk;
            wait_us(tempo);
        }
    }
    enable_x=1;
}

void fim_de_curso_x_2(){
    
    if (contador_x2>0 && fonte_x==0){
        outro_lado_x=0;
        correcao = 1;
        inverter_x=1;
        clkwise_x=0;
        if(feito_x2==0){
            for(int i=0;i<400;i++){
                enable_x=0;
                feito_x2=1;
                curso_x2=contador_x2-i;
                clk=!clk;
                wait_us(tempo);
            }
        }
        else{
            enable_x=1;
        }      
    }
    enable_x=1;
}

void fim_de_curso_y_1(){   

    if (contador_y1>0 && contador_y2==0){
        horario_y=1;
        outro_lado_y = 1;
        clkwise_y=1;
        if(feito_y1==0){
            for(int i=0;i<400;i++){
                feito_y1=1;
                enable_y=0;
                curso_y1=contador_y1-i;
                clk=!clk;
                wait_us(tempo);
            }
        }
        else{
            enable_y=1;
        }
    }
   if(contador_y1<100 && sensor_y_1==0 && contador_y2==0){
        horario_y=1;
        enable_y=0;
        referenciar_y=0;
        clkwise_y=1;
        
        for(int i=0;i<1000;i++){
            clk=!clk;
            wait_us(tempo);
        }
    }
    enable_y=1;
}

void fim_de_curso_y_2(){
    
    if (contador_y2>0 && fonte_y==0){
        outro_lado_y=0;
        correcao = 1;
        inverter_y=1;
        clkwise_y=0;
        if(feito_y2==0){
            for(int i=0;i<400;i++){
                feito_y2=1;
                enable_y=0;
                curso_y2=contador_y2-i;
                clk=!clk;
                wait_us(tempo);
            }
        }
        else{
            enable_y=1;
        }    
    }

    enable_y=1;
}

void confirma(){
    
//    if(teste==0){
//        teste=1;
//      ///  lcd.cls();
//        printf("\n\rfuncionou\n");
//    }

///    if(quantos_depositos>0 && teste==1){
//        lcd.cls();
//        confirma1=1;
//        printf("\n\rFuncionou 1\n");  
//    }

//    if (contador_x1==0 && confirma1==1){
    if (contador_x1==0){
        referenciar_x=1; 
        referenciar_y=1;
      //  printf("\rFuncionou 1\n");       
    }
    
    if ((contador_x2>100)&&(fonte_x==0)&&(fonte_y==0)){
        ciclo_da_fonte=1;
        printf("\rFuncionou 2\n"); 
    }

    if ((fonte_x>100)&&(fonte_y>100)&&(deposito_x==0)&&(deposito_y==0)){
        ciclo_da_fonte=0; 
        jog=1;

    }
    
    if (deposito_x>100 && deposito_y>100){
        printf("\rFuncionou 4\n");
        jog=0;
        ciclo_automatico=1;
        enable_x=1; 
        enable_y=1;
    }

}

void emergencia(){

    
}

int main(){
    
    botao_confirma.fall(&confirma);
    
    sensor_x_1.fall(&fim_de_curso_x_1);
    sensor_x_2.fall(&fim_de_curso_x_2);

    sensor_y_1.fall(&fim_de_curso_y_1);
    sensor_y_2.fall(&fim_de_curso_y_2);



    //botao_emergencia.fall(&emergencia);
    //botao_emergencia.mode(PullDown);
        
    lcd.setBacklight(TextLCD::LightOn);

   //while(estado_sis==0){
   //     lcd.cls();
    //    lcd.locate(6,1);
    //    lcd.printf("Emergencia");
  //     wait(0.2);
  //  }

    while (1){
    

    //    if(ola==0){
   //         ola=1;
   //         while(teste==0){
   //             lcd.cls();
   //             lcd.locate(8,1);
   //             lcd.printf("Ola");
                
    //            wait(3);
                
   //             lcd.cls();
   //             lcd.locate(3,1);
   //             lcd.printf("Aperte confirma");
   //             lcd.locate(3,2);
   //             lcd.printf("para iniciar o");
   //             lcd.locate(6,3);
  //              lcd.printf("processo");
   //            wait(6);
                
  //         }
   //     }


  //      if(teste==1){
  //          lcd.cls();
  //          lcd.locate(3,1);  
  //          lcd.printf("Quantidade de");
  //          lcd.locate(3,2);  
  //          lcd.printf("recipientes %d",quantos_depositos);
//
 //           while(confirma1==0){
 //               while(x_mais==0 && quantos_depositos<=8){
  //                  quantos_depositos++;
 ///                   lcd.cls();
  //                  lcd.locate(3,1); 
  //                  lcd.printf("Quantidade de");
  //                  lcd.locate(3,2);
//                    lcd.printf("recipientes %d",quantos_depositos);
 //                   wait(0.5);
  //              }
//
//                while(x_menos==0 && quantos_depositos>0){
//                    quantos_depositos--;
 //                   lcd.cls();
 //                   lcd.locate(3,1); 
 //                   lcd.printf("Quantidade de");
  //                  lcd.locate(3,2);  
 //                   lcd.printf("recipientes %d",quantos_depositos);
  //                  wait(0.5);
//                }
 //            }
 //       }

        if (referenciar_x==1 || referenciar_y==1){
            
            while(horario_x==0){
                referenciar_x=0;
                clkwise_x=0;
                enable_x=0;     
                contador_x1 ++;
                clk=!clk;
                wait_us(tempo);
                //printf("\rX1=%d",contador1);
                }
            while(horario_y==0){
                referenciar_y=0;
                clkwise_y=0;
                enable_y=0; 
                contador_y1 ++;
                clk=!clk;
                wait_us(tempo);
                //printf("\rX1=%d",contador1);
            }

        }        
        
        
        if (outro_lado_x==1 || outro_lado_y==1){

            while(inverter_x==0) {
                enable_x=0;
                clkwise_x=1;           
                contador_x2 ++;
                clk=!clk;
                wait_us(tempo);               
            }
            while(inverter_y==0) {
                enable_y=0;
                clkwise_y=1;           
                contador_y2 ++;
                clk=!clk;
                wait_us(tempo);               
            }

        }
        
        if(ciclo_da_fonte==1 || fonte_x==0 || fonte_y==0){
            while(ciclo_da_fonte==1){

                while(x_mais==0){ 
                    if(fonte_x<curso_x2){
                        enable_x=0;
                        fonte_x++;
                        clkwise_x=0;             
                        clk=!clk;
                        wait_us(tempo);
                        //printf("\rfonte_x=%d\n",fonte_x);
                    }
                    else{
                        enable_x=1;
                    }
                }    
   
                    
                while(x_menos==0){
                    if(fonte_x>0){
                        enable_x=0;
                        fonte_x--;
                        clkwise_x=1;        
                        clk=!clk;
                        wait_us(tempo);
                        //printf("\rfonte_x=%d\n",fonte_x);
                    }
                        
                    else{
                        enable_x=1;
                    }
                }
                while(y_mais==0){ 
                    if(fonte_y<curso_y2){
                        enable_y=0;
                        fonte_y++;
                        clkwise_y=0;             
                        clk=!clk;
                        wait_us(tempo);
                        //printf("\rfonte_x=%d\n",fonte_x);
                    }
                    else{
                        enable_y=1;
                    }
                }    
   
                    
                while(y_menos==0){
                    if(fonte_y>0){
                        enable_y=0;
                        fonte_y--;
                        clkwise_y=1;        
                        clk=!clk;
                        wait_us(tempo);
                        //printf("\rfonte_x=%d\n",fonte_x);
                    }
                        
                    else{
                        enable_y=1;
                    }
                }
            } 
        }            

        if (jog==1 && deposito_x==0 && deposito_y==0){
            if (volta_zero==0){
                volta_zero=1;
                for(int j = 0; j <fonte_x; ++j){
                    enable_x=0;
                    clkwise_x=1;
                    clk=!clk;
                    wait_us(tempo);
                }
                enable_x=1;
                
                for(int j = 0; j <fonte_y; ++j){
                    enable_y=0;
                    clkwise_y=1;
                    clk=!clk;
                    wait_us(tempo);
                }
                enable_y=1;

            }
            while(jog==1){

                while(x_mais==0){
                    if(deposito_x<curso_x2){
                        enable_x=0;
                        clkwise_x=0;        
                        deposito_x ++;
                        clk=!clk;
                        wait_us(tempo);     
                    }
                    else{
                        enable_x=1;
                    }
                }
                
                while(x_menos==0){
                    if(deposito_x>0){
                        enable_x=0;
                        clkwise_x=1;        
                        deposito_x--;
                        clk=!clk;
                        wait_us(tempo);         
                    }
                    else{      
                        enable_x=1;
                    }
                }

                while(y_mais==0){
                    if(deposito_y<curso_y2){
                        enable_y=0;
                        clkwise_y=0;        
                        deposito_y ++;
                        clk=!clk;
                        wait_us(tempo);     
                    }
                    else{
                        enable_y=1;
                    }
                }
                while(y_menos==0){
                    if(deposito_y>0){
                        enable_y=0;
                        clkwise_y=1;        
                        deposito_y--;
                        clk=!clk;
                        wait_us(tempo);         
                    }
                    else{      
                        enable_y=1;
                    }
                }            
            
            }

        }    
        
        if(fonte_x>0 && deposito_x>0 && fonte_y>0 && deposito_y>0){
  
            if((fonte_x-deposito_x)<0 && (fonte_y-deposito_y)<0){ 
            
                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
                    enable_x=0; 
                    clkwise_x=1;
                    clk=!clk;
                    wait_us(tempo);
                }
                for(int i = 0; i <abs(fonte_y-deposito_x); ++i){
                    enable_y=0; 
                    clkwise_y=1;
                    clk=!clk;
                    wait_us(tempo);
                }

                wait(0.5);

                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
                    clkwise_x=0;
                    clk=!clk;
                    wait_us(tempo);
                }

                for(int i = 0; i <abs(fonte_y-deposito_y); ++i){
                    clkwise_y=0;
                    clk=!clk;
                    wait_us(tempo);
                }                

            }    
            
            if((fonte_x-deposito_x)>0 && (fonte_y-deposito_y)>0){

                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
                    enable_x=0;
                    clkwise_x=0;
                    clk=!clk;
                    wait_us(tempo);
                }
                for(int i = 0; i <abs(fonte_y-deposito_y); ++i){
                    enable_y=0;
                    clkwise_y=0;
                    clk=!clk;
                    wait_us(tempo);
                }
                 wait(0.5);

                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
                    clkwise_x=1;
                    clk=!clk;
                    wait_us(tempo);
                }
                for(int i = 0; i <abs(fonte_y-deposito_y); ++i){
                    clkwise_y=1;
                    clk=!clk;
                    wait_us(tempo);
                }

            } 
            






        }






    }            
}       

