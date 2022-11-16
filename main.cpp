#include "mbed.h"
#include "mbed2/299/platform/wait_api.h"
#include <cstdint>
#include <cstdlib>

DigitalOut clkwise(D7);
DigitalOut clk(D8);
DigitalOut enable(D10);


InterruptIn referenciamento(D2);
Serial pc(USBTX,USBRX);

InterruptIn sensor1(D3);
InterruptIn sensor2(D4);

InterruptIn emergencia(D13);
DigitalIn x_mais(D5);
DigitalIn x_menos(D6);


float delay = 0.1;
float tempo = 0.001;

int contador1;
int contador2;
int curso;  
int horario;
int inverter;
int correcao;

int referenciar;
int x;
int jog;
int ciclo_jog;

int volta_zero;
int retorno;
int fonte;

int deposito;
int ciclo_automatico;

int jog_auto;

void fim_de_curso1()

{   
    
    inverter = 1;
    printf("\rX1=%d\n",contador1);

}

void fim_de_curso2(){
    correcao = 1;
    printf("\n\rX2=%d\n",contador2);
    

}


void confirma()
{
    if (contador1==0){
        enable=1;
        referenciar=1;        
    }
    
    if ((contador2>100)&&(fonte==0)){
        enable = 1;
        ciclo_jog=1; 
    }

    if ((fonte>100)&&(deposito==0)){
       jog=1; 
    }
    
    if (deposito>100){
        volta_zero=0;
        jog=0;
        ciclo_automatico=1; 
    }



}

int main(){
    
    referenciamento.fall(&confirma);
    sensor1.fall(&fim_de_curso1);
    sensor2.fall(&fim_de_curso2);
    
    
    while (1){
            
        if (referenciar!=0){
            clkwise=0;
            referenciar=0;
            while(1){     
                contador1 ++;
                clk=!clk;
                wait(tempo);
                //printf("\rX1=%d",contador1);
                }      
            }
        
        if (inverter!=0){
            clkwise=1;
            inverter=0;
            while(1) {           
                contador2 ++;
                clk=!clk;
                wait(tempo);               
                //printf("\n\rX2=%d",contador2);
                    }
                }

        if (correcao!=0){
                correcao = 0;
                for(int i = 0; i <100; ++i){
                    clkwise=0;
                    curso=contador2-i;
                    clk=!clk;
                    wait(tempo);
                    printf("\rcurso=%d\n",curso);
                }
                enable=0;
            }
        
        if(ciclo_jog!=0){
            while(1){
                if(0<=fonte<=curso){
                    enable=1;
                    while(x_mais==0){
                        clkwise=0;        
                        fonte ++;
                        clk=!clk;
                        wait(tempo);
                        printf("\rfonte=%d\n",fonte);    

                    }
                    while(x_menos==0){
                        clkwise=1;        
                        fonte --;
                        clk=!clk;
                        wait(tempo);
                        printf("\rfonte=%d\n",fonte);    

                    }
                }
                
                else{
                    enable=0;
                }
             }
        }     

        if (jog!=0){
            if (volta_zero==0){
                volta_zero = 1; 
                for(int j = 0; j <=(curso-fonte); ++j){
                    clkwise=1;
                    clk=!clk;
                    wait(tempo);
                    printf("\rX2=%d\n",j);
                }
            }
            while(1){
                if(0<=deposito<=curso){
                    enable=1;
                    while(x_mais==0){
                        clkwise=0;        
                        deposito ++;
                        clk=!clk;
                        wait(tempo);
                        printf("\rdeposito=%d\n",deposito);    

                    }
                    while(x_menos==0){
                        clkwise=1;        
                        deposito --;
                        clk=!clk;
                        wait(tempo);
                        printf("\rdeposito=%d\n",deposito);    

                    }
                }
                
                else{
                    
                    enable=0;
                }
            }

        }    
        
        if(ciclo_automatico!=0){
            if (volta_zero==0){
                volta_zero = 1; 
                for(int i = 0; i <=(curso-deposito); ++i){
                    clkwise=1;
                    clk=!clk;
                    wait(tempo);
                    printf("\rX=%d\n",i);
                }
            }
            if((fonte-deposito)<=0){
                for(int i = 0; i <=abs(fonte-deposito); ++i){
                    clkwise=0;
                    clk=!clk;
                    wait(tempo);
                    printf("\rX=%d\n",i);
                }
    
                wait(0.5);
    

                for(int i = 0; i <=abs(fonte-deposito); ++i){
                    clkwise=1;
                    clk=!clk;
                    wait(tempo);
                    printf("\rX=%d\n",i);
                }

            }    
            
            if((fonte-deposito)>=0){
                for(int i = 0; i <=abs(fonte-deposito); ++i){
                    clkwise=1;
                    clk=!clk;
                    wait(tempo);
                    printf("\rX=%d\n",i);
                }

                 wait(0.5);

                for(int i = 0; i <=abs(fonte-deposito); ++i){
                    clkwise=0;
                    clk=!clk;
                    wait(tempo);
                    printf("\rX=%d\n",i);
                }


            } 
            






        }






    }            
}       
