
#include "mbed.h"
#include "TextLCD.h"


#include <cstdio>
#include <new>
#include <vector>

#include <iostream>
using namespace std;


// Host PC Communication channels
Serial pc(USBTX, USBRX); // tx, rx
 
// I2C Communication

I2C i2c_lcd(I2C_SDA, I2C_SCL); // SDA, SCL >>> NUCLEO: D14,D15

//I2C Portexpander PCF8574
TextLCD_I2C lcd(&i2c_lcd, 0x7E, TextLCD::LCD20x4); // I2C bus, PCF8574 Slaveaddress, LCD Type Original = 0x42, Usando I2C scanner = 0x3F


DigitalOut clk(PB_15);


InterruptIn botao_confirma(PA_11);
InterruptIn botao_chega(PB_6);

InterruptIn botao_emergencia(PA_10);

InterruptIn sensor_x_1(PC_9);

DigitalIn x_mais(PB_4);
DigitalIn x_menos(PB_3);
DigitalOut enable_x(PC_5);
DigitalOut clkwise_x(PC_6);


InterruptIn sensor_y_1(PB_7);

DigitalIn y_mais(PB_10);
DigitalIn y_menos(PA_8);
DigitalOut enable_y(PB_12);
DigitalOut clkwise_y(PB_11);


InterruptIn sensor_z_1(PA_12);
DigitalIn z_menos(PC_7);
DigitalIn z_mais(PA_9);
DigitalOut enable_z(PB_13);
DigitalOut clkwise_z(PB_14);
//DigitalOut pipeta(PC_4);


int tempo = 950;


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
int jog;
int ciclo_emergencia;
int voltar_origem;

 
int curso_x = 49019;
int referenciar_x;
int fonte_x;
int deposito_x=0;



int curso_y=17220;
int referenciar_y;
int fonte_y;
int deposito_y;


  
int curso_z = 7450;
int referenciar_z;
int fonte_z;
int deposito_z;

int n;
int define_volume;
int define_posicao;


int posicao[9][4] = {};

int posicao_x[9]={};
int posicao_y[9]={};
int posicao_z[9]={};
int salva_volume[9]={};


int x1;
int x2;
int x3;
int x4;
int x5;
int x6;
int x7;
int x8;
int x9;

int y1;
int y2;
int y3;
int y4;
int y5;
int y6;
int y7;
int y8;
int y9;

int z1;
int z2;
int z3;
int z4;
int z5;
int z6;
int z7;
int z8;
int z9;


int v1;
int v2;
int v3;
int v4;
int v5;
int v6;
int v7;
int v8;
int v9;

//int posicao_x[8]={deposito_x};
//int posicao_y[8]={deposito_y};
//int posicao_z[8]={deposito_z};
//int salva_volume[8]={volume};

void vermelho(){
    
    if(index>1 && volume>0){
        ciclo=6;
    }
    
}

void emergencia(){
    
    while(1){           
        lcd.cls();
        lcd.locate(4,2);
        lcd.printf("Emergencia");
        wait(3);
    }   
    ///NVIC_SystemReset();
}

void fim_de_curso_z_1(){   
    
    
    switch (fc) {
        case 0:
            referenciar=2;
            clkwise_z=0;
            for(int i=0;i<400;i++){
                enable_z=0;
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

void fim_de_curso_y_1(){  

    switch (fc) {
        case 1:
            referenciar=3;
            clkwise_y=0; 
            for(int i=0;i<400;i++){
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
            clkwise_y=1;
            for(int i=0;i<400;i++){
                clkwise_y=0;
                clk=!clk;
                wait_us(tempo);
            }
            break;
        }    
}

void fim_de_curso_x_1(){   
    
    
    switch (fc) {
        case 2:
            referenciar=4;
            clkwise_x=1;
            for(int i=0;i<400;i++){
                enable_x=0;
                clk=!clk;
                wait_us(tempo);
            }
            fc++;
            fim_referenciamento=1;
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



void confirma(){

    if (referenciar==0){
        referenciar=1;
        ciclo = 1;
    }
        
        
    if (fim_referenciamento==1){
        referenciar=0;
        ciclo=2;
    }    


    if (jog ==1){
        ciclo=3;
    }
   
    if(ciclo==3 && define_volume==1 && index<9){
        ciclo++;
        
    }

        
    if (ciclo==4 && define_posicao==1 && index<9){

//        posicao[n][0] = deposito_x;
//        posicao[n][1] = deposito_y;
//        posicao[n][2] = deposito_z;
//        posicao[n][3] = volume;
//        
//        posicao_x[0] =1000;
//        posicao_y[0] = 2000;
//       posicao_z[0] = 3000;
//        salva_volume[0] = 2;
    
//        n++;
        printf("\r\n Entrou ");
        ciclo=5;
        
    }
    
    
}



int main(){
    
    botao_confirma.fall(&confirma);
    botao_chega.fall(&vermelho);
    botao_emergencia.rise(&emergencia);

    sensor_x_1.fall(&fim_de_curso_x_1);
    

    sensor_y_1.fall(&fim_de_curso_y_1);
    

    sensor_z_1.fall(&fim_de_curso_z_1);
    
        
    lcd.setBacklight(TextLCD::LightOn);


    enable_x=1;
    enable_y=1;
    enable_z=1;
    volume = 0;
            
    while(1){

        switch (ciclo) {
            case 0:
                while(ciclo==0 && jog==0){
                    
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
                    wait(4);                
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
                        clkwise_z=1;
                        while(referenciar==1){  
                            clk=!clk;
                            wait_us(tempo);
                        }
                        break;
                
                    case 2: 
                        enable_y=0;
                        clkwise_y=1;
                        while(referenciar==2){
                            clk=!clk;
                            wait_us(tempo);
                        
                        }
                        break;
                    
                    case 3:
                        enable_x=0;
                        clkwise_x=0; 
                        while(referenciar==3){  
                            clk=!clk;
                            wait_us(tempo);
                        }
                        break;    

                    
                    case 4:

                        while(referenciar==4){
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

                    }
                    break;
                                    

            case 2:          

                while(ciclo==2){
                    jog=1;
                    enable_x=1;
                    enable_y=1;
                    enable_z=1;                        
                    
                    lcd.cls();
                    lcd.locate(2,1);
                    lcd.printf("Fonte X %i",fonte_x);
                    lcd.locate(2,2);
                    lcd.printf("Fonte Y %i",fonte_y);
                    lcd.locate(2,3);
                    lcd.printf("Fonte Z %i",fonte_z);
                    wait(0.4);

                    while(x_mais==0){ 
                        

                        if(fonte_x<curso_x){
                            enable_x=0;
                            fonte_x++;
                            clkwise_x=1;             
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
                            clkwise_x=0;        
                            clk=!clk;
                            wait_us(tempo);
                        }
                            
                        else{
                            enable_x=1;
                        }
                    }
                    while(y_mais==0){
                        
                        if(fonte_y<curso_y){
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
                    while(z_menos==0){

                        if(fonte_z<curso_z){
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

                        
                    while(z_mais==0){
                        
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
            case 3:
                
                if(voltar_origem == 0){
                    lcd.cls();
                    lcd.locate(3,1);
                    lcd.printf("Voltando para");
                    lcd.locate(5,2);
                    lcd.printf("a origem");

                    for(int z=0; z<(fonte_z);z++){
                        enable_x=1;
                        enable_y=1;
                        enable_z=0;
                        clkwise_z=1;        
                        clk=!clk;
                        wait_us(tempo);
                    }
                        
                    for(int x=0; x<(fonte_x);x++){
                        enable_x=0;
                        enable_y=1;
                        enable_z=1;
                        clkwise_x=0;        
                        clk=!clk;
                        wait_us(tempo);
                    }
                    
                    for(int y=0; y<(fonte_y);y++){
                        enable_x=1;
                        enable_y=0;
                        enable_z=1;
                        enable_y=0;
                        clkwise_y=1;        
                        clk=!clk;
                        wait_us(tempo);
                    }
                    voltar_origem=1;
                }    
            
                if (voltar_origem==1){
                    lcd.cls();
                    lcd.locate(3,1);  
                    lcd.printf("Defina o volume");
                    lcd.locate(3,2);  
                    lcd.printf("no pote %i",index);
                    lcd.locate(3,3);  
                    lcd.printf("%i  ml",volume);

                    index++;
                    
                    while(ciclo==3){
                        
                        lcd.cls();
                        lcd.locate(3,1);  
                        lcd.printf("Defina o volume");
                        lcd.locate(3,2);  
                        lcd.printf("no pote %i",index);
                        lcd.locate(3,3);  
                        lcd.printf("%i  ml",volume);
                        wait(0.8);

                        while(z_mais==0){
                            v1++; 
                            wait(0.2);
                        }

                        while(z_menos==0){
                            if(volume>=0){
                                v1--;
                                wait(0.2);
                            }

                        }
                    }
                }
                break;
////                        
                        
            case 4:
                
                lcd.cls();
                wait_us(10);
                lcd.cls();
                lcd.locate(3,1);
                lcd.printf("Deposito X %i",deposito_x);
                lcd.locate(3,2);
                lcd.printf("Deposito Y %i",deposito_y); 
                lcd.locate(3,3);
                lcd.printf("Deposito Z %i",deposito_z); 
                

                define_volume=0;
                define_posicao=1;   
                
                while(ciclo==4){
                    enable_x=1;
                    enable_y=1;
                    enable_z=1;
                    

                    lcd.cls();
                    wait_us(10);
                    lcd.cls();
                    lcd.locate(3,1);
                    lcd.printf("Deposito X %i",deposito_x);
                    lcd.locate(3,2);
                    lcd.printf("Deposito Y %i",deposito_y); 
                    lcd.locate(3,3);
                    lcd.printf("Deposito Z %i",deposito_z); 
                    wait(0.8);
                    
                    while(x_mais==0){
                        if(deposito_x<curso_x){
                            enable_x=0;
                            clkwise_x=1;        
                            x1 ++;
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
                            clkwise_x=0;        
                            x1--;
                            clk=!clk;
                            wait_us(tempo);         
                        }
                        else{      
                            enable_x=1;
                        }
                    }

                    while(y_mais==0){
                        if(deposito_y<curso_y){
                            enable_y=0;
                            clkwise_y=0;        
                            y1 ++;
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
                            y1--;
                            clk=!clk;
                            wait_us(tempo);         
                        }

                        else{      
                            enable_y=1;
                        }
                    }            
                    while(z_menos==0){
                        if(deposito_z<curso_z){
                            enable_z=0;
                            clkwise_z=0;        
                            z1 ++;
                            clk=!clk;
                            wait_us(tempo);     
                        }
                        
                        else{
                            enable_z=1;
                        }
                    }

                    while(z_mais==0){
                        if(deposito_z>0){
                            enable_z=0;
                            clkwise_z=1;        
                            z1--;
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
                    lcd.locate(3,1);  
                    lcd.printf("Defina o volume");
                    lcd.locate(3,2);  
                    lcd.printf("no pote %i",index);
                    lcd.locate(3,3);  
                    lcd.printf("%i  ml",volume);

                    index++;
                    
                    while(ciclo==3){
                        
                        lcd.cls();
                        lcd.locate(3,1);  
                        lcd.printf("Defina o volume");
                        lcd.locate(3,2);  
                        lcd.printf("no pote %i",index);
                        lcd.locate(3,3);  
                        lcd.printf("%i  ml",volume);
                        wait(0.8);

                        while(z_mais==0){
                            v1++; 
                            wait(0.2);
                        }

                        while(z_menos==0){
                            if(volume>=0){
                                v1--;
                                wait(0.2);
                            }

                        }
                    }
                }
                break;
////                        
                        
            case 4:
                
                lcd.cls();
                wait_us(10);
                lcd.cls();
                lcd.locate(3,1);
                lcd.printf("Deposito X %i",deposito_x);
                lcd.locate(3,2);
                lcd.printf("Deposito Y %i",deposito_y); 
                lcd.locate(3,3);
                lcd.printf("Deposito Z %i",deposito_z); 
                

                define_volume=0;
                define_posicao=1;   
                
                while(ciclo==4){
                    enable_x=1;
                    enable_y=1;
                    enable_z=1;
                    

                    lcd.cls();
                    wait_us(10);
                    lcd.cls();
                    lcd.locate(3,1);
                    lcd.printf("Deposito X %i",deposito_x);
                    lcd.locate(3,2);
                    lcd.printf("Deposito Y %i",deposito_y); 
                    lcd.locate(3,3);
                    lcd.printf("Deposito Z %i",deposito_z); 
                    wait(0.8);
                    
                    while(x_mais==0){
                        if(deposito_x<curso_x){
                            enable_x=0;
                            clkwise_x=1;        
                            x1 ++;
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
                            clkwise_x=0;        
                            x1--;
                            clk=!clk;
                            wait_us(tempo);         
                        }
                        else{      
                            enable_x=1;
                        }
                    }

                    while(y_mais==0){
                        if(deposito_y<curso_y){
                            enable_y=0;
                            clkwise_y=0;        
                            y1 ++;
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
                            y1--;
                            clk=!clk;
                            wait_us(tempo);         
                        }

                        else{      
                            enable_y=1;
                        }
                    }            
                    while(z_menos==0){
                        if(deposito_z<curso_z){
                            enable_z=0;
                            clkwise_z=0;        
                            z1 ++;
                            clk=!clk;
                            wait_us(tempo);     
                        }
                        
                        else{
                            enable_z=1;
                        }
                    }

                    while(z_mais==0){
                        if(deposito_z>0){
                            enable_z=0;
                            clkwise_z=1;        
                            z1--;
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

                posicao[n][0] = deposito_x;
                posicao[n][1] = deposito_y;
                posicao[n][2] = deposito_z;
                posicao[n][3] = volume;
                n++;
                printf("\r\nEntrou 1");
                
                break;
            
            case 6:   

                enable_x=1;
                enable_y=1;
                enable_z=1;
                
                lcd.cls();
                wait_us(10);
                lcd.cls();
                lcd.locate(3,2);
                lcd.printf("Ciclo");
                lcd.locate(3,3);
                lcd.printf("Automatico");        
                wait(0.5);    

                for (int i = 0; i < 2; i++){
                    cout << "\rX: "<< posicao[i][0] << endl;
                    cout << "\rY: "<< posicao[i][1] << endl;
                    cout << "\rZ: "<< posicao[i][2]<< endl;
                    cout << "\rV: "<< posicao[i][3] << endl;
                    wait(0.5);
                }


                for(int i=0;i<=index;i++){
                    for(int j=0;j<=salva_volume[i];j++){
                        
                        for(int n=0;n<=posicao_z[i];n++){
//                            printf("\r\nEntrou Z1 %i",contadorz++);    
                            enable_x=1;
                            enable_y=1;
                            enable_z=0;
                            clkwise_z=1;
                            clk=!clk;
                            wait_us(tempo); 
                            //pipeta=!pipeta;
                        }
                        wait(0.2);                        
    
                        for(int n=0;n<=abs(fonte_x-posicao_x[i]);n++){
//                            printf("\r\nEntrou X1 %i",contadorx++); 
                            enable_x=0;
                            enable_y=1;
                            enable_z=1;
                            if ((fonte_x-posicao_x[i])<0){ 
                                clkwise_x=1;
                            }
                            else{
                                clkwise_x=0;  
                            }
                            clk=!clk;
                            wait_us(tempo);  
                        }     
                        wait(0.2);
                        for(int n=0;n<=abs(fonte_y-posicao_y[i]);n++){
//                            printf("\r\nEntrou Y1 %i",contadory++); 
                            enable_x=1;
                            enable_y=0;
                            enable_z=1;

                            if ((fonte_y-posicao_y[i])<0){
                                clkwise_y=0;
                            }
                            else{ 
                                clkwise_y=1;  
                            }
                            clk=!clk;
                            wait_us(tempo);  
                        }     
                        wait(0.2);
                        for(int n=0;n<=fonte_z;n++){
//                            printf("\r\nEntrou Z2"); 
                            enable_x=1;
                            enable_y=1;
                            enable_z=0;

                            clkwise_z=0;
                            clk=!clk;
                            wait_us(tempo); 
                            //pipeta=!pipeta;
                        }
                        wait(0.2);
                        
                        for(int n=0;n<=fonte_z;n++){
                            enable_x=1;
                            enable_y=1;
                            enable_z=0;

                            clkwise_z=1;
                            clk=!clk;
                            wait_us(tempo); 
                            //pipeta=!pipeta;
                        }
                        wait(0.2);
                        
                        for(int n=0;n<=abs(fonte_x-posicao_x[i]);n++){
                            enable_x=0;
                            enable_y=1;
                            enable_z=1;
                            if ((fonte_x-posicao_x[i])<0){ 
                                clkwise_x=0;
                            }
                            else{
                                clkwise_x=1;  
                            }
                            clk=!clk;
                            wait_us(tempo);  
                        }     
                        wait(0.2);
                        for(int n=0;n<=abs(fonte_y-posicao_y[i]);n++){
                            enable_x=1;
                            enable_y=0;
                            enable_z=1;

                            if ((fonte_y-posicao_y[i])<0){
                                clkwise_y=1;
                            }
                            else{ 
                                clkwise_y=0;  
                            }
                            clk=!clk;
                            wait_us(tempo);  
                        }
                        wait(0.2);                    
                        
                        for(int n=0;n<=posicao_z[i];n++){
                            enable_x=1;
                            enable_y=1;
                            enable_z=0;
                            clkwise_z=0;
                            clk=!clk;
                            wait_us(tempo); 
                            //pipeta=!pipeta;
                        }
                        wait(0.2);                     
                    }

                }
                lcd.cls();
                wait_us(10);
                lcd.cls();
                lcd.locate(3,1);
                lcd.printf("Ciclo");
                lcd.locate(3,2);
                lcd.printf("Automatico");        
                wait(0.5); 
                break;       
            }
            
            
        }    
           
                    
    }
      
