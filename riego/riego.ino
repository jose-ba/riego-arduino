#include <LiquidCrystal.h>

const int pinSensorHumedad = 0;
const int pinBomba = 2;
const int intervaloBombaFuncionando = 5000; // ms (3s)
const double intervaloMuestreo = 120000; // ms (5m) 300000 por defecto
const int umbralHumedad = 50; // % de humedad por debajo del cual se activará la bomba
const int medidasATomar = 10;
int valorHumedad = 100; // Lo inicializo a 100% por defecto para que la bomba no funcione
int porcentajeHumedad = 100;
int valorSensor = 250;
int medidasTomadas = 0;
int media = -1;
float aaa = 0;

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
  
  if(medidasTomadas == 0){   
    media = porcentajeHumedad;
  } else {
    media += porcentajeHumedad;
  }

  if(medidasTomadas >= (medidasATomar-1)){    
    Serial.println(" ");
    aaa = (media/(medidasTomadas+1));
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
      lcd.setCursor(0, 1);
      lcd.print("esperando 1h.");
      delay(3600000);
    } else {
      digitalWrite(pinBomba, HIGH);
    }
    medidasTomadas =0;
    Serial.println(" ");
    Serial.println("--------------------------------");
    Serial.println(" ");
  } else {
      medidasTomadas++;  
  }
  
  delay(intervaloMuestreo);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Midiendo... ");
  delay(3000);
}
