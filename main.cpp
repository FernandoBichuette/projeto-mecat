#include "mbed.h"

DigitalOut clkwise(D7);
DigitalOut clk(D8);
DigitalOut enable(D9);


InterruptIn referenciamento(D2);
Serial pc(USBTX,USBRX);

InterruptIn sensor1(D3);
InterruptIn sensor2(D4);

////InterruptIn emergencia(D1);
DigitalIn x_mais(D5);
DigitalIn x_menos(D6);


float delay = 0.1;
float tempo = 950;

int contador1;
int contador2;
int curso1;  
int curso2;
int horario;
int inverter;
int correcao;
int outro_lado;

int referenciar;
int x;
int jog;
int ciclo_jog;

int volta_zero;
int volta_zero2;
int fonte;
int ciclo_da_fonte;

int deposito;
int ciclo_automatico;

int jog_auto;

void fim_de_curso1(){   

    if (contador1>0 && contador2==0){
        horario=1;
        outro_lado = 1;
        clkwise=1;
        for(int i=0;i<400;i++){
            curso1=contador1-i;
            clk=!clk;
            wait_us(tempo);
        }
        enable=1;
       ///printf("\rX1=%d\n",contador1);
    }

}

void fim_de_curso2(){
    
    if (contador2>0 && fonte==0){
        outro_lado=0;
        correcao = 1;
        inverter=1;
        clkwise=0;
        for(int i=0;i<400;i++){
            curso2=contador2-i;
            clk=!clk;
            wait_us(tempo);
        }
        enable=1;
        //printf("\rX1=%d\n",contador2);
    }

}


void confirma()
{
    if (contador1==0){
        enable=1;
        referenciar=1; 
        printf("\rFuncionou 1\n");       
    }
    
    if ((contador2>100)&&(fonte==0)){
        ciclo_da_fonte=1;
        printf("\rFuncionou 2\n"); 
    }

    if ((fonte>100)&&(deposito==0)){
        ciclo_da_fonte=0; 
        jog=1;
        printf("\rFuncionou 3\n");
        printf("\rF=%d\n",fonte);
        printf("\rD=%d\n",deposito);
        printf("\rciclo_da_fonte=%d\n",ciclo_da_fonte);
        if (volta_zero==0){
            volta_zero=1;
            for(int j = 0; j <fonte; ++j){
                enable=0;
                clkwise=1;
                clk=!clk;
                wait_us(tempo);
            }
            enable=1;
        }
    }
    
    if (deposito>100){
        printf("\rFuncionou 4\n");
        jog=0;
        ciclo_automatico=1;
        printf("\rC=%d\n",curso2);
        printf("\rF=%d\n",fonte);
        printf("\rD=%d\n",deposito);
        enable=1; 
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
            enable=0;
            while(horario==0){     
                contador1 ++;
                clk=!clk;
                wait_us(tempo);
                //printf("\rX1=%d",contador1);
                }      
            }
        
        if (outro_lado!=0){
            enable=0;
            clkwise=1;
            while(inverter==0) {           
                contador2 ++;
                clk=!clk;
                wait_us(tempo);               
                //printf("\n\rX2=%d",contador2);
                    }
                }

        
        if(ciclo_da_fonte==1 && fonte==0){
            enable=0;
            while(ciclo_da_fonte==1){

                while(x_mais==0){ 
                    if(fonte<curso2){
                        enable=0;
                        fonte++;
                        clkwise=0;             
                        clk=!clk;
                        wait_us(tempo);
                        //printf("\rfonte=%d\n",fonte);
                    }
                    else{
                        enable=1;
                    }
                }    
   
                    
                while(x_menos==0){
                    if(fonte>0){
                        enable=0;
                        fonte--;
                        clkwise=1;        
                        clk=!clk;
                        wait_us(tempo);
                        //printf("\rfonte=%d\n",fonte);
                    }
                        
                    else{
                        enable=1;
                    }
                }

            } 
        }            

        if (jog==1 && deposito==0){
            while(jog==1){

                while(x_mais==0){
                    if(deposito<curso2){
                        enable=0;
                        clkwise=0;        
                        deposito ++;
                        clk=!clk;
                        wait_us(tempo);
                          
                    }
                    else{
                        enable=1;
                    }
                }
                while(x_menos==0){
                    if(deposito>0){
                        enable=0;
                        clkwise=1;        
                        deposito--;
                        clk=!clk;
                        wait_us(tempo);
                            
                    }

                    else{
                        
                        enable=1;
                    }
                }
            }

        }    
        
        if(fonte>0 && deposito>0 ){      
            if((fonte-deposito)<0){
                for(int i = 0; i <abs(fonte-deposito); ++i){
                    clkwise=0;
                    clk=!clk;
                    wait_us(tempo);
                    //printf("\rX=%d\n",abs(fonte-deposito));
                }
    
                wait(0.5);
    

                for(int i = 0; i <abs(fonte-deposito); ++i){
                    clkwise=1;
                    clk=!clk;
                    wait_us(tempo);
                    //printf("\rX=%d\n",abs(fonte-deposito));
                }

            }    
            
            if((fonte-deposito)>0){
                for(int i = 0; i <abs(fonte-deposito); ++i){
                    clkwise=1;
                    clk=!clk;
                    wait_us(tempo);
                    //printf("\rX=%d\n",i);
                }

                 wait(0.5);

                for(int i = 0; i <abs(fonte-deposito); ++i){
                    clkwise=0;
                    clk=!clk;
                    wait_us(tempo);
                    //printf("\rX=%d\n",i);
                }


            } 
            






        }






    }            
}       
