#define SSD1306_RST    PIN_B2
#include <18F452.h>
#device PASS_STRINGS = IN_RAM
#fuses HS,  ,  NOLVP
#use delay(clock = 20MHz)
#use fast_io(B)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Initialize I2C

// Include SSD1306 OLED driver source code
#define SSD1306_I2C_ADDRESS 0x78
#include <SSD1306_OLED.c> 



//#use RS232(BAUD = 9600, BITS = 8, PARITY = N, XMIT = PIN_C6, RCV = PIN_C7)
char buffer[10];// Posición  de fila
char buffer2[10];//Posición  de columna
int valorPot1=0 , valorPot2=0  , dire=1 , posbx=15 , posby=15;
boolean over=false;//verificar si choque erroneo
void monvimientoCircle(int dire , int x , int y);// se encarga de mover la bola
boolean validarChoqueCorrecrto(posbx , int x );//verifica si la bola al tocar el borde coencide con la raqueta
void posLinea(int x , int y){
 SSD1306_DrawLine(x , y , x  , y + 10 );
}
int cambioPot(int id , int columna ){
 set_adc_channel(id);
 int  valor = read_adc();
 delay_us(3);
 valor=valor/5;
 posLinea(columna , valor);
 return valor;
 
}
main(){

 //setup_adc_ports(sAN1|sAN2);
// setup_adc(ADC_CLOCK_DIV_2);
 //|AN7
 setup_adc_ports(all_analog);
 setup_adc(ADC_CLOCK_DIV_2);

 // la dirección puede ser I2C addr = 0x7A 0 0x78
  SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

  delay_ms(100);
  SSD1306_ClearDisplay();
  SSD1306_Display();
  posLinea(10 , 10);//poscion de la linea
  SSD1306_Display();
  delay_ms(100);
  SSD1306_ClearDisplay();
  posLinea(20 , 20);
    SSD1306_Display();
    
    //leo valores del potenciometro
     
  while(true){
  SSD1306_ClearDisplay();
  valorPot1= cambioPot(0 , 0);
  valorPot2= cambioPot(0 , 127);
  monvimientoCircle(dire , posbx , posby);
 
if(over){// si perdio
SSD1306_ClearDisplay();
SSD1306_DrawText(40 , 30 , "game over");

      sprintf(buffer, "columna = %u", posbx);                      // Convierte el valor entero a una cadena
      SSD1306_DrawText(40, 40, buffer, 1); 
      sprintf(buffer, "fila = %u", posby);                        // Convierte el valor entero a una cadena
      SSD1306_DrawText(40, 40, buffer, 1); 
      sprintf(buffer2, "pot= %u", valorPot1);                      // Convierte el valor entero a una cadena
      SSD1306_DrawText(40, 50, buffer2, 1); 
      
SSD1306_Display();
delay_ms(10000);
over=false;
}
    }    
}

boolean validarChoqueCorrecrto(int pot , int x ){
over=true;
for(int i = (pot) ; i<= (pot + 15) ; i++){
if(x==i){
over=false;
break;
}

}
return over;
}
void monvimientoCircle(int dir , x , y){
switch(dire){
case 1: 
//derecha ariba


SSD1306_FillCircle(x , y, 3);
 SSD1306_Display();
delay_ms(5);
posby--;
posbx++;

break;
case 2: 
//derecha abajo
SSD1306_FillCircle(x, y, 3);
 SSD1306_Display();
delay_ms(5);

posby++;
posbx++;
break;
case 3: 
//izquierda ariba
SSD1306_FillCircle(x, y, 3);
 SSD1306_Display();
delay_ms(5);

posby--;
posbx--;
break;
case 4: 
//izquierda abajo
SSD1306_FillCircle(x, y, 3);
 SSD1306_Display();
delay_ms(5);

posby++;
posbx--;
break;
}

if(y<5  && dire==1){
//derecha subiendo a derecha bajando
dire=2;
}
else if(y>59 && dire ==2){
//derecha bajando a derecha subiendo
dire=1;
}

else if(x > 122 && dire==1 ){
//derecha subiendo a chocar con borde a izquierda subiendo
dire=3;
over=validarChoqueCorrecrto( valorPot2 ,  posby  );
}else if(y < 5 && dire==3){
//izquierda subiendo a izquierda bajando
dire=4;
}
else if(y > 59 && dire==4){
//izquierda bajando a izquierda subiendo
dire=3;
}
else if(x < 7 && dire==3){
//izquierda subiendo a derecha subiendo
dire=1;
over=validarChoqueCorrecrto( valorPot1 ,  posby  );
}
else if(x < 7 && dire==4){
//izquierda bajando a derecha bajando
dire=2;
over=validarChoqueCorrecrto( valorPot1 ,  posby  );
}
else if(x > 122  && dire==2){

dire=4;
over=validarChoqueCorrecrto( valorPot2 ,  posby  );
}
}
