#include <LiquidCrystal.h>
#include <math.h>

const int pinSensorHumedad = 0;
const int pinBomba = 2;
const int intervaloBombaFuncionando = 5000; // ms (3s)
const double intervaloMuestreo = 120000; //120000 // ms (5m) 300000 por defecto
const int umbralHumedad = 50; // % de humedad por debajo del cual se activará la bomba
const int medidasATomar = 10;
const double esperaRiego = 7200000; //7200000
int valorHumedad = 100; // Lo inicializo a 100% por defecto para que la bomba no funcione
int porcentajeHumedad = 100;
int valorSensor = 250;
int medidasTomadas = 0;
int media = -1;
float aaa = 0;
float sumaMedidas = 0;
double esperaRestante;

// Pines lcd
const int pinRs = 34;
const int pinEnable = 35;
const int pinD4 = 30;
const int pinD5 = 31;
const int pinD6 = 32;
const int pinD7 = 33;

//LiquidCrystal lcd(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(pinRs, pinEnable, pinD4,pinD5,pinD6,pinD7);

void setup() {     
  Serial.begin(9600);
  pinMode(pinBomba, OUTPUT);
  pinMode(pinRs, OUTPUT);
  pinMode(pinEnable, OUTPUT);
  pinMode(pinD4, OUTPUT);
  pinMode(pinD5, OUTPUT);
  pinMode(pinD6, OUTPUT);
  pinMode(pinD7, OUTPUT);  
  digitalWrite(pinBomba, HIGH);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Iniciando...");
  delay(5000);
  valorSensor = analogRead(pinSensorHumedad);
  
  
}

void loop() {     
  valorSensor = analogRead(pinSensorHumedad);
  porcentajeHumedad = map(valorSensor, 590, 250, 0,100); // Los valores del sensor los he sacado haciendo Serial.printl del sensor directamente (590 = seco, 250 = en agua)
  
  Serial.print("Sensor: ");
  Serial.print(valorSensor);
  Serial.print("\t");
  Serial.print("% humedad: ");
  Serial.println(porcentajeHumedad);

  // Escribo al LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("H. actual:  ");
  if(porcentajeHumedad > 0){
    lcd.print(" ");
  }
  lcd.print(porcentajeHumedad);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("H. obj.:     ");
  lcd.print(umbralHumedad);
  lcd.print("%");
  
  if(sumaMedidas == 0){   
    sumaMedidas = porcentajeHumedad;
  } else {
    sumaMedidas += porcentajeHumedad;
  }
  medidasTomadas++;

  /*Serial.print("Medidas tomadas ");
  Serial.println(medidasTomadas);

  Serial.print("Suma ");
  Serial.println(sumaMedidas);*/

  if(medidasTomadas >= (medidasATomar)){    
    Serial.println(" ");
    aaa = (sumaMedidas/medidasTomadas);
    medidasTomadas = 0;
    Serial.print("Media de las últimas ");
    Serial.print(medidasATomar);
    Serial.print(" medidas: ");
    Serial.println(aaa);
    if((aaa < umbralHumedad) && (aaa > 10) && (aaa < 90)) {    
      Serial.println(" ");
      Serial.println("\t>>> Riego activado <<<");
      digitalWrite(pinBomba, LOW);
      lcd.clear();
      lcd.setCursor(0,0);     
      lcd.print("Regando...");
      delay(intervaloBombaFuncionando);
      digitalWrite(pinBomba, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Se ha regado,");
      esperaRestante = esperaRiego;
      while(esperaRestante > 0){
        lcd.setCursor(0, 1);
        lcd.print("esperando ");
        lcd.print(round(esperaRestante/1000));
        delay(1000);
        esperaRestante = esperaRestante - 1000;
      }      
    } else {
      digitalWrite(pinBomba, HIGH);
    }
    sumaMedidas = 0;
    medidasTomadas =0;
    Serial.println(" ");
    Serial.println("--------------------------------");
    Serial.println(" ");
  }
  
  delay(intervaloMuestreo);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Midiendo... ");
  delay(3000);
}
