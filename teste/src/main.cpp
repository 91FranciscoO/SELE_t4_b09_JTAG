// The native serial support happens via UART
#include <Arduino.h>

#define TMS 8
#define TCK 9
#define TDI 10
#define TDO 11

void send(boolean valor){
    digitalWrite(TMS,valor);
    digitalWrite(TCK,HIGH);
    delay(1);
    digitalWrite(TCK,LOW);
    delay(1);
}

void setup() {
    Serial.begin(9600); //baud rate de 9600
    

    pinMode(TMS,OUTPUT);
    pinMode(TCK,OUTPUT);
    pinMode(TDI,INPUT);
    pinMode(TDO,OUTPUT);
    Serial.println("Escolha um comando");
    Serial.println("1:Ligar LED");
    Serial.println("1:Desligar LED");
    Serial.println("d:imprimir IDCODE");
    Serial.println("b:estado do botao");
}

void loop() {
    uint8_t a;
    
    if (Serial.available()>0){
        a= Serial.read();
        
        uint32_t b= 0;
        int i = 0;
        // clk and test reset
        send(HIGH); send(HIGH); send(HIGH); send(HIGH); send(HIGH);
        if (a == toascii('d')){
            Serial.println('d');
            send(LOW); send(HIGH); send(HIGH); send(LOW); send(LOW);
            digitalWrite(TDO,HIGH);send(LOW);
            digitalWrite(TDO,LOW);send(LOW);send(LOW);send(LOW);send(HIGH); //envia TDI 00001 (IDCODE)
            send(HIGH);send(LOW);//run-test
            send(HIGH); send(LOW);send(LOW);//shift DR

            while (i<32){
                digitalWrite(TCK,HIGH);
                delay(1);
                
                b |=((uint32_t) (digitalRead(TDI))<<i);
                i++;
                digitalWrite(TCK,LOW);
                delay(1);
            }
            
            Serial.println(b,HEX);
            
        }
        //"extest (00110),"
        if (a == toascii('b')){
            Serial.println('b');
            send(LOW); send(HIGH); send(HIGH); send(LOW); send(LOW);//shift-IR
            digitalWrite(TDO,LOW);send(LOW);
            digitalWrite(TDO,HIGH); send(LOW);
            digitalWrite(TDO,HIGH);send(LOW);
            digitalWrite(TDO,LOW);send(LOW);
            digitalWrite(TDO,LOW);send(HIGH); //envia TDI extest (00110)
            send(HIGH);send(LOW);//UPDATE IR
            send(LOW); send(HIGH);send(LOW);send(LOW);//shift-DR
            while (i<147){
                digitalWrite(TDO,LOW);
                digitalWrite(TCK,HIGH);
                delay(1);
                
                //r |= (digitalRead(TDI)<<1);
                
                i++;
                digitalWrite(TCK,LOW);
                delay(1);
                if (i== 3){
                    if (digitalRead(TDI)==1){
                        Serial.println("Botao esta para cima");

                    }
                    if (digitalRead(TDI)==0){
                        Serial.println("Botao esta para baixo");

                    }

                    //Serial.println(digitalRead(TDI),BIN);
                }
            }

            digitalWrite(TDO,LOW);
            send(HIGH); send(HIGH);
            
        }
        //"extest (00110),"
        if (a == toascii('1')){
            
            send(LOW); send(HIGH); send(HIGH); send(LOW); send(LOW);//shift-IR
            digitalWrite(TDO,LOW);send(LOW);
            digitalWrite(TDO,HIGH); send(LOW);
            digitalWrite(TDO,HIGH);send(LOW);
            digitalWrite(TDO,LOW);send(LOW);
            digitalWrite(TDO,LOW);send(HIGH); //envia TDI extest (00110)
            send(HIGH);send(LOW);//UPDATE IR
            send(LOW); send(HIGH);send(LOW);send(LOW);//shift-DR
            
            while (i<147){
                digitalWrite(TDO,LOW);
                
                i++;
                if(i<126){
                    digitalWrite(TDO,HIGH);

                }

                    
                
                send(LOW);
                

                
                send(LOW);
                i++;
                /*if (i!=129){
                    digitalWrite(TDO,LOW);
                }
                if (i==129){
                    digitalWrite(TDO,HIGH);
                }
                send(LOW);
                i++;*/
            }
            
            digitalWrite(TDO,HIGH);
            send(HIGH); send(HIGH);//update DRb
            Serial.println('1');
        }
        if (a == toascii('0')){
            send(LOW); send(HIGH); send(HIGH); send(LOW); send(LOW);//shift-IR
            digitalWrite(TDO,LOW);send(LOW);
            digitalWrite(TDO,HIGH); send(LOW);
            digitalWrite(TDO,HIGH);send(LOW);
            digitalWrite(TDO,LOW);send(LOW);
            digitalWrite(TDO,LOW);send(HIGH); //envia TDI extest (00110)
            send(HIGH);send(LOW);//UPDATE IR
            send(LOW); send(HIGH);send(LOW);send(LOW);//shift-DR
            
            while (i<294){
                digitalWrite(TDO,LOW);
                send(LOW);
                i++;
            }
            digitalWrite(TDO,LOW);
            send(HIGH); send(HIGH);
            Serial.println('0');
        }
    
    }


        
}