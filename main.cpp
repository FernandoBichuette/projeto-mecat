#include "mbed.h"
#include "TextLCD.h"
#include <vector>


// Host PC Communication channels
Serial pc(USBTX, USBRX); // tx, rx
 
// I2C Communication

I2C i2c_lcd(I2C_SDA, I2C_SCL); // SDA, SCL >>> NUCLEO: D14,D15

//I2C Portexpander PCF8574
TextLCD_I2C lcd(&i2c_lcd, 0x7E, TextLCD::LCD20x4); // I2C bus, PCF8574 Slaveaddress, LCD Type Original = 0x42, Usando I2C scanner = 0x3F



DigitalOut clk(PB_15);


InterruptIn botao_confirma(USER_BUTTON);
InterruptIn botao_chega(D2);

DigitalIn botao_emergencia(PA_10);

InterruptIn sensor_x_1(PB_7);
InterruptIn sensor_x_2(PC_10);
DigitalIn x_mais(PB_4);
DigitalIn x_menos(PB_3);


DigitalOut enable_x(PC_5);
DigitalOut clkwise_x(PC_6);


InterruptIn sensor_y_1(PC_8);
InterruptIn sensor_y_2(PC_12);
DigitalIn y_mais(PA_8);
DigitalIn y_menos(PB_10);


DigitalOut enable_y(PB_12);
DigitalOut clkwise_y(PB_11);


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
int confirma1;
int pote;
int volume;
int index;
int referenciar;
int ciclo;
int fim_referenciamento;
int fc;

int contador_x1;
int contador_x2;
int curso_x1;  
int curso_x2;
int horario_x;
int referenciar_x;
int outro_lado_x;
int inverter_x;
int fonte_x;
int deposito_x=0;
int fc_x1;
int fc_x2;


int contador_y1;
int contador_y2;
int curso_y1;  
int curso_y2;
int horario_y;
int referenciar_y;
int outro_lado_y;
int inverter_y;
int fonte_y;
int deposito_y=0;
int fc_y1;
int fc_y2;


int correcao;



int jog;
int ciclo_jog;

int volta_zero;
int volta_zero2;

int ciclo_da_fonte;


int ciclo_automatico;

int jog_auto;

int quantos;

int teste;

int define_volume;
int define_posicao;
int chega;

int posicao[][3] = {};

//int salvapos(int pos_x, int pos_y, int vol){
//        
//    int posicao[index][pos_x][pos_y][vol];
//    index =index+1;
//    return posicao;
//}

void vermelho(){
    
    chega=1;
}


void fim_de_curso_x_1(){   
    
    
    switch (fc) {
        case 0:
            referenciar=2;
            clkwise_x=1;
            for(int i=0;i<400;i++){
                enable_x=0;
               
                curso_x1=contador_x1-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            enable_x=1;
            break;

    }

    
        
    //   while(sensor_x_1==0){
    //        horario_x=1;
    //        enable_x=0;
    //        referenciar_x=0;
    //        clkwise_x=1;
    //        clk=!clk;
    //        wait_us(tempo);
    //    }
    
}

void fim_de_curso_x_2(){
     
    switch (fc) {
        case 2:
            referenciar=4;
            clkwise_x=0;
            for(int i=0;i<400;i++){
                curso_x2=contador_x2-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            enable_x=1;
            break;
            
    }

    //   while(sensor_x_2==0){
    //        outro_lado_x=0;
    //        correcao = 1;
    //        inverter_x=1;
    //        enable_x=0;
    //        clkwise_x=0;
    //        clk=!clk;
    //        wait_us(tempo);
    //    }

     
 
}



void fim_de_curso_y_1(){  

    switch (fc) {
        case 1:
            referenciar=3;
            clkwise_y=1; 
            for(int i=0;i<400;i++){
                curso_y1=contador_y1-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            enable_y=1;
            break;
    }    
    
}

void fim_de_curso_y_2(){

    switch (fc) {
        case 3:
            clkwise_y=0;
            for(int i=0;i<400;i++){
                curso_y2=contador_y2-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            
            enable_y=1;
            fim_referenciamento=1;

            lcd.cls();
            lcd.locate(2,1);
            lcd.printf("Referenciamento");
            lcd.locate(5,2);
            lcd.printf("concluido");
            wait(3);
            lcd.cls();
            lcd.locate(3,1);
            lcd.printf("Aperte confirma");
            lcd.locate(2,2);
            lcd.printf("para definir a");
            lcd.locate(8,3);
            lcd.printf("fonte");
            wait(5);                  
            break;
    }    
  
}




void confirma(){

    if (referenciar==0){
        referenciar=1;
        ciclo = 1;
    }
        
        
    if (fim_referenciamento==1){
        referenciar=0;
        printf("\n\r entrou");
        ciclo=2;
    }    


    if ((fonte_x>100)&&(fonte_y>100)){
        ciclo_da_fonte=0; 
        volta_zero=1;

    }
    
    if(chega==0 && index<9){
        if (define_volume==1){
            define_posicao=1;
            define_volume=0;

        }

        
        if (define_posicao==1){
            define_posicao=0;
            define_volume=1;
            //volta_zero=1;
            index=index+1;
            posicao[index][0] = deposito_x;
            posicao[index][1] = deposito_y;
            posicao[index][2] = volume;

        }
    }
    
    if (chega == 1 || index==9){
        define_volume=0;
        define_volume=0;
    }
}





int main(){
    
    botao_confirma.fall(&confirma);
    botao_chega.fall(&vermelho);
    
    sensor_x_1.fall(&fim_de_curso_x_1);
    sensor_x_2.fall(&fim_de_curso_x_2);

    sensor_y_2.fall(&fim_de_curso_y_1);
    sensor_y_1.fall(&fim_de_curso_y_2);

        
    lcd.setBacklight(TextLCD::LightOn);


//    while(botao_emergencia==1){
//        lcd.cls();
//        lcd.locate(4,2);
//        lcd.printf("Emergencia");
//        wait(3);
//    }

//    while (botao_emergencia==0){
    while (1){
    
       switch (ciclo) {
            case 0:
                while(ciclo==0){
                    lcd.cls();
                    lcd.locate(8,1);
                    lcd.printf("Ola");
                    
                    wait(3);
                    
                    lcd.cls();
                    lcd.locate(3,1);
                    lcd.printf("Aperte confirma");
                    lcd.locate(3,2);
                    lcd.printf("para iniciar o");
                    lcd.locate(3,3);
                    lcd.printf("referenciamento");
                    wait(5);                
                }
                break;
       
            case 1:
                lcd.cls();
                lcd.locate(2,1);
                lcd.printf("Referenciando...");

                switch (referenciar) {
                    case 1:
                        enable_x=0; 
                        enable_y=1;
                        while(referenciar==1){  
                            contador_x1 ++;
                            clk=!clk;
                            wait_us(tempo);
                        }
                        break;
                
                    case 2:
                        enable_x=1; 
                        enable_y=0;
                        while(referenciar==2){
                            contador_y1 ++;
                            clk=!clk;
                            wait_us(tempo);
                        
                        }
                        break;

                    case 3: 

                        enable_x=0;
                        clkwise_x=1;           
                        enable_y=1;
                    
                        while(referenciar==3) {
                            contador_x2 ++;
                            clk=!clk;
                            wait_us(tempo);               
                        }
                        break;
            
                
                    case 4:
                        enable_x=1;
                        clkwise_y=1;           
                        enable_y=0;    
                        while(referenciar==4) {
                            contador_y2 ++;
                            clk=!clk;
                            wait_us(tempo);               
                        }
                        break;

                }
                break;
            
            case 2:          
            
                while(ciclo==2){
                    lcd.cls();
                    lcd.locate(3,1);
                    lcd.printf("X",fonte_x);
                    lcd.locate(3,2);
                    lcd.printf("Y",fonte_y);
//                    lcd.locate(3,3);
//                    lcd.printf("Z",fonte_Z);
                    wait(5);           

                    while(x_mais==0){ 
                        if(fonte_x<curso_x2){
                            enable_x=0;
                            enable_y=1;
                            fonte_x++;
                            clkwise_x=0;             
                            clk=!clk;
                            wait_us(tempo);


                        }
                        else{
                            enable_x=1;
                            enable_y=1;
                        }
                    }    

                        
                    while(x_menos==0){
                        if(fonte_x>0){
                            enable_x=0;
                            enable_y=1;
                            fonte_x--;
                            clkwise_x=1;        
                            clk=!clk;
                            wait_us(tempo);
                            //printf("\rfonte_x=%d\n",fonte_x);
                        }
                            
                        else{
                            enable_x=1;
                            enable_y=1;
                        }
                    }
                    while(y_mais==0){ 
                        if(fonte_y<curso_y2){
                            enable_x=1;
                            enable_y=0;
                            fonte_y++;
                            clkwise_y=0;             
                            clk=!clk;
                            wait_us(tempo);
                            //printf("\rfonte_x=%d\n",fonte_x);
                        }
                        else{
                            enable_x=1;
                            enable_y=1;
                        }
                    }    

                        
                    while(y_menos==0){
                        if(fonte_y>0){
                            enable_x=1;
                            enable_y=0;
                            fonte_y--;
                            clkwise_y=1;        
                            clk=!clk;
                            wait_us(tempo);
                            //printf("\rfonte_x=%d\n",fonte_x);
                        }
                            
                        else{
                            enable_x=1;
                            enable_y=1;
                        }
                    }
                } 
                break;
                              
//        if(ciclo==3){
//            if (volta_zero==1){
//                volta_zero=0;
//                for(int j = 0; j <fonte_x; ++j){
//                    enable_x=0;
//                    clkwise_x=1;
//                    clk=!clk;
//                    wait_us(tempo);
//                }
//                enable_x=1;
//                
//                for(int j = 0; j <fonte_y; ++j){
//                    enable_y=0;
//                    clkwise_y=1;
//                    clk=!clk;
//                    wait_us(tempo);
//                }
//                enable_y=1;
//                define_volume = 1;
//
//            }
//            
//
//            if(define_volume==1){
                
//                lcd.cls();
//                lcd.locate(3,1);  
//                lcd.printf("Defina o ");
//                lcd.locate(3,2);  
//                lcd.printf("volume %d",index+1);
//                lcd.locate(8,3);  
//                lcd.printf("%d",volume);
//
//                while(define_volume==1){
//
//                    while(y_mais==0){
//                        volume++;
//                        lcd.cls();
//                        lcd.locate(3,1);  
//                        lcd.printf("Defina o ");
//                        lcd.locate(3,2);  
//                        lcd.printf("volume %d",index+1);
//                        lcd.locate(8,3);  
//                        lcd.printf("%d",volume);
//                        wait(0.5);
//                    }

//                    while(y_menos==0 && volume<=0){
//                        volume--;
//                        lcd.cls();
//                        lcd.locate(3,1);  
//                        lcd.printf("Defina o ");
//                        lcd.locate(3,2);  
//                        lcd.printf("volume %d",index+1);
//                        lcd.locate(8,3);  
//                        lcd.printf("%d",volume);
//                        wait(0.5);
//                    }
//                }
                
//            }

//            if(define_posicao==1){

//                while(define_posicao==1){

//                    while(x_mais==0){
//                        if(deposito_x<curso_x2){
//                            enable_x=0;
//                            clkwise_x=0;        
//                            deposito_x ++;
//                            clk=!clk;
//                            wait_us(tempo);     
//                        }
//                        else{
//                            enable_x=1;
//                        }
//                    }
                    
//                    while(x_menos==0){
//                        if(deposito_x>0){
//                            enable_x=0;
//                            clkwise_x=1;        
//                            deposito_x--;
//                            clk=!clk;
//                            wait_us(tempo);         
//                        }
//                        else{      
//                            enable_x=1;
//                        }
//                    }

//                    while(y_mais==0){
//                        if(deposito_y<curso_y2){
//                            enable_y=0;
//                            clkwise_y=0;        
//                            deposito_y ++;
//                            clk=!clk;
//                            wait_us(tempo);     
//                        }
//                        else{
//                            enable_y=1;
//                        }
//                    }
//                    while(y_menos==0){
//                        if(deposito_y>0){
//                            enable_y=0;
//                            clkwise_y=1;        
//                            deposito_y--;
//                            clk=!clk;
//                            wait_us(tempo);         
//                        }
//                        else{      
//                            enable_y=1;
//                        }
//                    }            
//                
//                }
//            }    
//        
//        //        if(fonte_x>0 && deposito_x>0 && fonte_y>0 && deposito_y>0){
//        //  
//        //            if((fonte_x-deposito_x)<0 && (fonte_y-deposito_y)<0){ 
//        //            
//        //                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
//        //                    enable_x=0; 
        //                    clkwise_x=1;
        //                    clk=!clk;
        //                    wait_us(tempo);
        //                }
        //                for(int i = 0; i <abs(fonte_y-deposito_x); ++i){
        //                    enable_y=0; 
        //                    clkwise_y=1;
        //                    clk=!clk;
        //                    wait_us(tempo);
        //                }
        //
        //                wait(0.5);
        //
        //                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
        //                    clkwise_x=0;
        //                    clk=!clk;
        //                    wait_us(tempo);
        //                }
        //
        //                for(int i = 0; i <abs(fonte_y-deposito_y); ++i){
        //                    clkwise_y=0;
        //                    clk=!clk;
        //                    wait_us(tempo);
        //                }                
        //
        //            }    
        //            
        //            if((fonte_x-deposito_x)>0 && (fonte_y-deposito_y)>0){
        //
        //                for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
        //                    enable_x=0;
        //                    clkwise_x=0;
        //                    clk=!clk;
        //                    wait_us(tempo);
        //                }
        //                for(int i = 0; i <abs(fonte_y-deposito_y); ++i){
        //                    enable_y=0;
        //                    clkwise_y=0;
        //                    clk=!clk;
        //                    wait_us(tempo);
        //                }
        //                 wait(0.5);

        //              for(int i = 0; i <abs(fonte_x-deposito_x); ++i){
        //                  clkwise_x=1;
        //                  clk=!clk;
        //                  wait_us(tempo);
        //              }
        //              for(int i = 0; i <abs(fonte_y-deposito_y); ++i){
        //                  clkwise_y=1;
        //                  clk=!clk;
        //                  wait_us(tempo);
        //              }
        //
        //          } 
        //          
        //
        //
        //
        //
        //
        //
        //      }
        }
    }            
}      

