#include "mbed.h"
#include "TextLCD.h"
#include <cstdlib>
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

InterruptIn botao_emergencia(PA_10);

InterruptIn sensor_x_1(PC_10);
InterruptIn sensor_x_2(PB_7);
DigitalIn x_mais(PA_8);
DigitalIn x_menos(PB_10);
DigitalOut enable_x(PC_5);
DigitalOut clkwise_x(PC_6);


InterruptIn sensor_y_1(PC_8);
InterruptIn sensor_y_2(PC_12);
DigitalIn y_mais(PB_4);
DigitalIn y_menos(PB_3);

DigitalOut enable_y(PB_12);
DigitalOut clkwise_y(PB_11);

InterruptIn sensor_z_1(PB_6);
InterruptIn sensor_z_2(PC_12);
DigitalIn z_mais(PC_7);
DigitalIn z_menos(PA_9);
DigitalOut enable_z(PB_13);
DigitalOut clkwise_z(PB_14);
DigitalOut pipeta(PC_4);


float delay = 0.1;
float tempo = 950;


int ola;
int quantos_depositos;
int confirma1;
int pote;
int volume;
int index=1;
int referenciar;
int ciclo;
int fim_referenciamento;
int fc;
int ciclo_emergencia;

int contador_x1;
int contador_x2;
int curso_x1;  
int curso_x2;
int referenciar_x;
int fonte_x;
int deposito_x=0;


int contador_y1;
int contador_y2;
int curso_y1;  
int curso_y2;
int referenciar_y;
int fonte_y;
int deposito_y;


int contador_z1;
int contador_z2;
int curso_z1;  
int curso_z2;
int referenciar_z;
int fonte_z;
int deposito_z;



int define_volume;
int define_posicao;
int chega;

int posicao[][4] = {};

//int salvapos(int pos_x, int pos_y, int vol){
//        
//    int posicao[index][pos_x][pos_y][vol];
//    index =index+1;
//    return posicao;
//}

void vermelho(){
    
    chega=1;
}

void emergencia(){
        
    ciclo_emergencia=!ciclo_emergencia;

}

void fim_de_curso_z_1(){   
    
    
    switch (fc) {
        case 0:
            referenciar=2;
            clkwise_z=1;
            for(int i=0;i<400;i++){
                enable_z=0;
                curso_z1=contador_z1-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            enable_z=1;
            break;

    }
    
     switch (ciclo) {
        case 0:
            enable_x=1;
            enable_y=1;
            enable_z=0;
            for(int i=0;i<400;i++){
                clkwise_z=1;
                clk=!clk;
                wait_us(tempo);
            }
            break;
    }
    
}



void fim_de_curso_x_1(){   
    
    
    switch (fc) {
        case 1:
            referenciar=3;
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
    
     switch (ciclo) {
        case 0:
            enable_y=1;
            enable_x=0;
            for(int i=0;i<400;i++){
                clkwise_x=1;
                clk=!clk;
                wait_us(tempo);
            }
            break;
    }
    
}


void fim_de_curso_y_1(){  

    switch (fc) {
        case 2:
            referenciar=4;
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
     
     switch (ciclo) {
        case 0:
            enable_y=0;
            enable_x=1;
            for(int i=0;i<400;i++){
                clkwise_y=1;
                clk=!clk;
                wait_us(tempo);
            }
            break;
        }    
}

void fim_de_curso_x_2(){
     
    switch (fc) {
        case 3:
            referenciar=5;
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
    switch (ciclo) {
        case 0:
            enable_y=1;
            enable_x=0;
            for(int i=0;i<400;i++){
                clkwise_x=0;
                clk=!clk;
                wait_us(tempo);
            }
            break;
    }
     
 
}

void fim_de_curso_y_2(){

    switch (fc) {
        case 4:
            referenciar=6;
            clkwise_y=0;
            for(int i=0;i<400;i++){
                curso_y2=contador_y2-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            
            enable_y=1;            
            break;
        }    
     
     switch (ciclo) {
        case 0:
            enable_y=0;
            enable_x=1;
            for(int i=0;i<400;i++){
                clkwise_y=0;
                clk=!clk;
                wait_us(tempo);
            }
            break;
        }    


}


void fim_de_curso_z_2(){

    switch (fc) {
        case 5:
            clkwise_z=0;
            for(int i=0;i<400;i++){
                curso_z2=contador_z2-i;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            
            enable_z=1;
            fim_referenciamento=1;
            ciclo=2;                  
            break;
        }    
     
     switch (ciclo) {
        case 0:
            enable_x=1;
            enable_y=1;
            enable_z=0;
            for(int i=0;i<400;i++){
                clkwise_z=0;
                clk=!clk;
                wait_us(tempo);
            }
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
        ciclo=3;
    }    


    if ((fonte_x>100)&&(fonte_y>100)){
        ciclo=4;
        define_volume=1;

    }
    
    if(chega==0 && index<=9){
        
        if (define_volume==1){
            define_posicao=2;
        }

        
        if (define_posicao==2){

            posicao[index][0] = deposito_x;
            posicao[index][1] = deposito_y;
            posicao[index][2] = deposito_z;
            posicao[index][3] = volume;
            index=index+1;
            
            define_volume=1;

        }
    }
    
    if (chega == 1 || index==9){
        ciclo=5;
    }
}




int main(){
    
    botao_confirma.fall(&confirma);
    botao_chega.fall(&vermelho);
    botao_emergencia.rise(&emergencia);

    sensor_x_1.fall(&fim_de_curso_x_1);
    sensor_x_2.fall(&fim_de_curso_x_2);

    sensor_y_2.fall(&fim_de_curso_y_1);
    sensor_y_1.fall(&fim_de_curso_y_2);

        
    lcd.setBacklight(TextLCD::LightOn);

    enable_z=1;

    while (1){
            
        while(ciclo_emergencia==1){           
                lcd.cls();
                lcd.locate(4,2);
                lcd.printf("Emergencia");
                wait(3);
        }
        

        while(ciclo_emergencia==0){
            
            enable_x=1; 
            enable_y=1;
            enable_z=1;
            switch (ciclo) {
                case 0:
                    while(ciclo==0){
                        
                        lcd.cls();
                        wait_us(10);
                        lcd.cls();
                        lcd.locate(8,1);
                        lcd.printf("Ola");
                        
                        wait(3);
                        
                        lcd.cls();
                        wait_us(10);
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
                    wait_us(10);
                    lcd.cls();
                    lcd.locate(2,1);
                    lcd.printf("Referenciando...");

                    switch (referenciar) {
                        case 1:
                            enable_z=0;
                            while(referenciar==1){  
                                contador_z1 ++;
                                clk=!clk;
                                wait_us(tempo);
                            }
                            break;

                        case 2:
                            enable_x=0; 
                            while(referenciar==2){  
                                contador_x1 ++;
                                clk=!clk;
                                wait_us(tempo);
                            }
                            break;
                    
                        case 3: 
                            enable_y=0;
                            while(referenciar==3){
                                contador_y1 ++;
                                clk=!clk;
                                wait_us(tempo);
                            
                            }
                            break;

                        case 4: 
                            enable_x=0;
                            clkwise_x=1;           
                            while(referenciar==4) {
                                contador_x2 ++;
                                clk=!clk;
                                wait_us(tempo);               
                            }
                            break;
                
                    
                        case 5:
                            clkwise_y=1;           
                            enable_y=0;    
                            while(referenciar==5) {
                                contador_y2 ++;
                                clk=!clk;
                                wait_us(tempo);               
                            }
                            break;

                        case 6:
                            clkwise_z=1;           
                            enable_z=0;    
                            while(referenciar==6) {
                                contador_z2 ++;
                                clk=!clk;
                                wait_us(tempo);               
                            }
                            break;    

                        }
                        break;

                case 2:
                    while(ciclo==2){
                        lcd.cls();
                        wait_us(10);
                        lcd.cls();
                        lcd.locate(2,1);
                        lcd.printf("Referenciamento");
                        lcd.locate(5,2);
                        lcd.printf("concluido");
                        wait(3);

                        lcd.cls();
                        wait_us(10);
                        lcd.cls();
                        lcd.locate(3,1);
                        lcd.printf("Aperte confirma");
                        lcd.locate(2,2);
                        lcd.printf("para definir a");
                        lcd.locate(8,3);
                        lcd.printf("fonte");
                        wait(3);
                    } 
                    break;

                case 3:          
                    
                    lcd.cls();
                    wait_us(10);
                    lcd.cls();
                    lcd.locate(3,1);
                    lcd.printf("FonteX",fonte_x);
                    lcd.locate(3,2);
                    lcd.printf("Fonte Y",fonte_y);
                    lcd.locate(3,3);
                    lcd.printf("Fonte Z",fonte_z);
                    wait(5);
                    
                    while(ciclo==3){
                        enable_x=1;
                        enable_y=1;
                        enable_z=1;                        

                        while(x_mais==0){ 
                            if(fonte_x<curso_x2){
                                enable_x=0;
                                fonte_x++;
                                clkwise_x=0;             
                                clk=!clk;
                                wait_us(tempo);


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
                            }
                                
                            else{
                                enable_y=1;
                            }
                        }
//
                        while(z_mais==0){ 
                            if(fonte_z<curso_z2){
                                enable_z=0;
                                fonte_z++;
                                clkwise_z=0;             
                                clk=!clk;
                                wait_us(tempo);
                            }
                            else{
                                enable_z=1;
                            }
                        }    

                            
                        while(z_menos==0){
                            if(fonte_z>0){
                                enable_z=0;
                                fonte_z--;
                                clkwise_z=1;        
                                clk=!clk;
                                wait_us(tempo);
                            }
                                
                            else{
                                enable_z=1;
                            }
                        }

                    } 
                    break;
                                
                case 4:
                    switch (define_volume) {
                    
                        case 1:
                            volume = 0;
                            while(define_volume==1){
                                
                                lcd.cls();
                                lcd.locate(3,1);  
                                lcd.printf("Defina o ");
                                lcd.locate(3,2);  
                                lcd.printf("volume %d",index);
                                lcd.locate(8,3);  
                                lcd.printf("%d",volume);
                                wait(0.5);

                                while(y_mais==0){
                                    volume++; 
                                    wait(0.3);
                                }

                                while(y_menos==0 && volume<=0){
                                    volume--;
                                    wait(0.3);
                                }
                            }
                            break;
                            
                        case 2:
        
                            while(define_posicao==2){
                                
                                lcd.cls();
                                wait_us(10);
                                lcd.cls();
                                lcd.locate(3,1);
                                lcd.printf("Deposito X",deposito_x);
                                lcd.locate(3,2);
                                lcd.printf("Deposito Y",deposito_x); 
                                lcd.locate(3,3);
                                lcd.printf("Deposito Z",deposito_x); 
                                wait(0.5);

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
                              while(z_mais==0){
                                  if(deposito_z<curso_z2){
                                      enable_z=0;
                                      clkwise_z=0;        
                                      deposito_z ++;
                                      clk=!clk;
                                      wait_us(tempo);     
                                    }
                                  
                                  else{
                                      enable_z=1;
                                    }
                                }

                              while(z_menos==0){
                                  if(deposito_z>0){
                                      enable_z=0;
                                      clkwise_z=1;        
                                      deposito_z--;
                                      clk=!clk;
                                      wait_us(tempo);         
                                    }

                                  else{      
                                      enable_z=1;
                                    }
                                }
                            }
                            break;                          
                        
             
                case 5:   
                    
                    lcd.cls();
                    wait_us(10);
                    lcd.cls();
                    lcd.locate(3,3);
                    lcd.printf("Ciclo");
                    lcd.locate(3,4);
                    lcd.printf("Automatico");        
                    wait(0.5);    

                    for(int i=1;i<=index;i++){
                        for(int j=0;j<=2*posicao[i][3];j++){
                            enable_z=1;
                            for(int n=0;n<=abs(fonte_x-posicao[i][0]);n++){
                                if ((fonte_x-posicao[i][0])<0){
                                    enable_x=0; 
                                    clkwise_x=0;
                                    clk=!clk;
                                    wait_us(tempo);
                                }
                                else{
                                    enable_x=0; 
                                    clkwise_x=1;
                                    clk=!clk;
                                    wait_us(tempo);   
                                }
                            }     
                            wait(0.2);
                            for(int n=0;n<=abs(fonte_y-posicao[i][1]);n++){
                                enable_x=1;
                                if ((fonte_y-posicao[i][1])<0){
                                    enable_y=0; 
                                    clkwise_y=0;
                                    clk=!clk;
                                    wait_us(tempo);
                                }
                                else{
                                    enable_y=0; 
                                    clkwise_y=1;
                                    clk=!clk;
                                    wait_us(tempo);   
                                }
                            }     
                            wait(0.2);
                            for(int n=0;n<=abs(fonte_z-posicao[i][2]);n++){
                                enable_y=1;
                                if ((fonte_y-posicao[i][2])<0){
                                    enable_z=0; 
                                    clkwise_z=0;
                                    clk=!clk;
                                    wait_us(tempo);
                                }
                                else{
                                    enable_z=0; 
                                    clkwise_z=1;
                                    clk=!clk;
                                    wait_us(tempo);   
                                }
                            }
                            wait(0.2);
                        }
                    }
                    
                    break;       
                }
                
                
            }    
           
        }            
    }
}
