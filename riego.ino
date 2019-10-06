const int pinSensorHumedad = 0;
const int pinBomba = 2;
const int intervaloBombaFuncionando = 1000; // ms (3s)
const double intervaloMuestreo = 120000; // ms (5m) 300000 por defecto
const int umbralHumedad = 35; // % de humedad por debajo del cual se activará la bomba
const int medidasATomar = 10;
int valorHumedad = 100; // Lo inicializo a 100% por defecto para que la bomba no funcione
int porcentajeHumedad = 100;
int valorSensor = 250;
int medidasTomadas = 0;
int media = -1;
float aaa = 0;


void setup() {  
  Serial.begin(9600);
  pinMode(pinBomba, OUTPUT);
  digitalWrite(pinBomba, HIGH);
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
      delay(intervaloBombaFuncionando);
      digitalWrite(pinBomba, HIGH);
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
}
