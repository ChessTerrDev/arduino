
//int frequency=490;          // частота при которой считывается бусина 


void setup() {
  Serial.begin(9600);    //открываем COM порт
  //pinMode(7, OUTPUT);
}

void loop() {
  

// -------------------- ЧИТАЕМ ДАТЧИКИ ОПТОВОРОТ ----------------------------

  //показать значение на датчиках 
  Serial.print("sensors: ["); 
  Serial.print(analogRead(0));  Serial.print("][");
  Serial.print(analogRead(1));  Serial.print("]["); 
  Serial.print(analogRead(2));  Serial.print("]");
  Serial.println(" ");

}
