// -------------------- Порты / Константы ---------------------
#define CLK 10
#define DT 11
#define SW 12

//Переменный резисторы 
#define VR1 13  //Переменный резистор № 1
#define VR2 15  //Переменный резистор № 2
#define VR3 14  //Переменный резистор № 3

//Реле 
#define RL1 13  //Реле № 1
#define RL2 9   //Реле № 2
#define RL3 8   //Реле № 3

//Шим порты на вибромоторы
#define PWM1 7   //Шим порт на вибромотор № 1
#define PWM2 6   //Шим порт на вибромотор № 2
#define PWM3 5   //Шим порт на вибромотор № 3

//Сервоприводы
#define SR1 1   //Сервопривод № 1
#define SR2 2   //Сервопривод № 2
#define SR3 3   //Сервопривод № 3




// -------------------- БИБЛИОТЕКИ ---------------------
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>      // Библиотека для работы с LCD монитором 20х4
LiquidCrystal_I2C lcd(0x27,20,4);   // set the LCD address to 0x27 for a 16 chars and 2 line display

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой / Номера портов - CLK, DT, SW

#include "OneButton.h"
OneButton buttonP1(30, true);
OneButton buttonP2(32, true);
OneButton buttonP3(34, true);
OneButton buttonPause(36, true);
// -------------------- БИБЛИОТЕКИ ---------------------

// -------------------- НАСТРОЙКИ ----------------------------


boolean flagRL1,flagRL2,flagRL3 = false;
boolean flagS1,flagS2,flagS3;   
boolean pickActive1,pickActive2,pickActive3 = true;
int frequency=490;          // частота при которой считывается бусина 
int S1, S2, S3 =0;          // текущая сумма бусин на сенсорах 
int Q1, Q2, Q3, Nx =0;      // Максимальное значение собираемых бусин
int EcMode = 1;             // Режим энеодера / 1=Q1 / 2=Q2 / 3=Q3

// -------------------- НАСТРОЙКИ ----------------------------


void setup() {
  Serial.begin(9600);    //открываем COM порт

  
  buttonP1.attachClick(click1);
  buttonP2.attachClick(click2);
  buttonP3.attachClick(click3);
  buttonPause.attachClick(clickPause);
  //buttonP1.attachDoubleClick(doubleclick1);
  //buttonP1.attachLongPressStart(longPressStart1);
  //buttonP1.attachLongPressStop(longPressStop1);
  //buttonP1.attachDuringLongPress(longPress1);

  //Реле - Обьявляем пины реле ка выходы 
  pinMode(RL1, OUTPUT); 
  digitalWrite(RL1, LOW); // Выключаем реле - посылаем Низкий сигнал
  pinMode(RL2, OUTPUT); 
  digitalWrite(RL2, LOW); // Выключаем реле - посылаем Низкий сигнал
  pinMode(RL3, OUTPUT); 
  digitalWrite(RL3, LOW); // Выключаем реле - посылаем Низкий сигнал
  
  
  //ШИМ ПОРТЫ ДЕЛАЕМ ВЫХОДОМ
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  

  enc1.setType(TYPE2);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  printGUI();
}

void loop() {
  
  buttonP1.tick();
  buttonP2.tick();
  buttonP3.tick();
  buttonPause.tick();

  


  // -------------------- ШИМ СИГНАЛ НА ВИБРОМОТОРЫ ----------------------------
  // ШИМ на пин, 1023/4 = 255 - перевели диапазон
  if (flagRL1) {
   analogWrite(PWM1, analogRead(VR1) / 4);
  }
  if (flagRL2) {
    analogWrite(PWM2, analogRead(VR2) / 4);
  }
  if (flagRL3) {
    analogWrite(PWM3, analogRead(VR3) / 4);
  }
  
  
  

  
// -------------------- ЧИТАЕМ ДАТЧИКИ ОПТОВОРОТ ----------------------------
  /*if(pickActive1) {
    if (analogRead(1) >= frequency) {
      flagS1 = true;
    }
    if (analogRead(1) < frequency && flagS1) {
      flagS1 = false;
      S1++;
    }
  }
  
  if(pickActive2) {
    if (analogRead(2) >= frequency) {
      flagS2 = true;
    }
    if (analogRead(2) < frequency && flagS2) {
      flagS2 = false;
      S2++;
    }
  }

  if(pickActive3) {
    if (analogRead(3) >= frequency) {
      flagS2 = true;
    }
    if (analogRead(3) < frequency && flagS3) {
      flagS3 = false;
      S3++;
    }
  }*/
 
// -------------------- ЧИТАЕМ ДАТЧИКИ ОПТОВОРОТ ----------------------------


// -------------------- УПРАВЛЯЕМ СБОРОМ ------------------------------------
/*
  //----Первый поток----

  //Если собралось нужное колличество бусин
  if (S1 == Q1){
    Serial.println("Выключаем реле Вибромотора");
    Serial.println("Поварачиваем сервопривод - (Закрываем)");
    Serial.println("Выводим на экран статус 'Пауза'");
    //Поварачиваем сервопривод
  }

  //Если нажата кнопка 'начать новый сбор' 
  if (S1 == Q1){
    S1 = 0; //Обнуляем насчитанное количество бусин
    Serial.println("Поварачиваем сервопривод - (открываем)");
    Serial.println("Включаем реле Вибромотора");
    Serial.println("Выводим на экран статус 'Сбор'");
    //Поварачиваем сервопривод
  }

  //Если кнопка 'начать новый сбор' была зажата в течении 5 сек
  if (1=1 && pickActive1){
    S1 = 0; //Обнуляем насчитанное количество бусин
    pickActive1 = false; //запрещаем счет потока
    Serial.println("Поварачиваем сервопривод - (открываем)");
    Serial.println("Включаем реле Вибромотора на максимум");
    Serial.println("Выводим на экран статус 'Сброс'");
    //Поварачиваем сервопривод
  }
  

  //----Второй поток----
  */
// -------------------- УПРАВЛЯЕМ СБОРОМ ------------------------------------


// -------------------- ОБРАБОТКА ЭНКОДЕРА ----------------------------

  // обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();

  //По одиночному клику меняем режим 
  if (enc1.isClick())
  {
    lcd.clear();        // очищаем дисплей
    if (EcMode == 3) EcMode = 1;
    else EcMode++;
    printGUI();
  }

  // Долгое зажатие кнопки - отчистка максимального количества бусин 
  if (enc1.isHolded()) {
    lcd.clear();        // очищаем дисплей
    if (EcMode == 1) Q1=0;
    if (EcMode == 2) Q2=0;
    if (EcMode == 3) Q3=0;
     printGUI();
  }
    
  if(enc1.isTurn())
  {
    
    if (enc1.isLeft()) {
      if (EcMode == 1) Q1 +=5;
      if (EcMode == 2) Q2 +=5;
      if (EcMode == 3) Q3 +=5;
    }

    if (enc1.isRight()) {
      if (EcMode == 1) Q1 -=5;
      if (EcMode == 2) Q2 -=5;
      if (EcMode == 3) Q3 -=5;
    }

    if (enc1.isLeftH()) {
      if (EcMode == 1) Q1 +=50;
      if (EcMode == 2) Q2 +=50;
      if (EcMode == 3) Q3 +=50;
    }

    if (enc1.isRightH()) {
      if (EcMode == 1) Q1 -=50;
      if (EcMode == 2) Q2 -=50;
      if (EcMode == 3) Q3 -=50;
    }

    Q1= (Q1<0) ? 0 : Q1;
    Q2= (Q2<0) ? 0 : Q2;
    Q3= (Q3<0) ? 0 : Q3;

    printGUI(); //ВЫВОД НА ЭКРАН
  }
// -------------------- ОБРАБОТКА ЭНКОДЕРА ----------------------------
}




void printGUI() {

// -------------------- ВЫВОД НА ЭКРАН --------------------------------

  //Первая линия - Счет бусин
  lcd.setCursor(0,0);   lcd.print("[---- | ---- | ----]"); // 0,1 / 0,8 / 0,15
  lcd.setCursor(1,0);   lcd.print(S1);
  lcd.setCursor(8,0);   lcd.print(S1);
  lcd.setCursor(15,0);  lcd.print(S1);

  //Вторая линия - максимальное кольчество бусин, ограничение

  switch (EcMode) {
    case 1: lcd.setCursor(0,1);   lcd.print("[----]| ---- | ----|"); // 0,1 / 0,8 / 0,15
      break;
    case 2: lcd.setCursor(0,1);   lcd.print("|---- |[----]| ----|"); // 0,1 / 0,8 / 0,15
      break;
    case 3: lcd.setCursor(0,1);   lcd.print("|---- | ---- |[----]"); // 0,1 / 0,8 / 0,15
      break;
  }
  lcd.setCursor(1,1);   lcd.print(Q1);
  lcd.setCursor(8,1);   lcd.print(Q2);
  lcd.setCursor(15,1);  lcd.print(Q3);
  
  
  //Третья линия - Статус каждого из трех потоков 
  lcd.setCursor(0,2); lcd.print("      |      |      "); // Счет / Пауза / Сброс / 

  
  
  lcd.setCursor(4,3);
  lcd.print("BUSINO 3000!");
  
// -------------------- ВЫВОД НА ЭКРАН --------------------------------
}

void click1() {
  Serial.println("Нажата Первоя кнопка");
  
  if (flagRL1) {
    digitalWrite(RL1,LOW); // Выключаем реле - посылаем низкий уровень сигнала
    flagRL1 = false; 
  } else {
    digitalWrite(RL1, HIGH); // Включаем реле - посылаем низкий уровень сигнала
    flagRL1 = true;
  }

} 
void click2() {
  Serial.println("Нажата Вторая кнопка");

  if (flagRL2) {
    digitalWrite(RL2,LOW); // Выключаем реле - посылаем низкий уровень сигнала
    flagRL2 = false; 
  } else {
    digitalWrite(RL2, HIGH); // Включаем реле - посылаем низкий уровень сигнала
    flagRL2 = true;
  }
} 
void click3() {
  Serial.println("Нажата Третья кнопка");
  
    if (flagRL3) {
    digitalWrite(RL3,LOW); // Выключаем реле - посылаем низкий уровень сигнала
    flagRL3 = false; 
  } else {
    digitalWrite(RL3, HIGH); // Включаем реле - посылаем низкий уровень сигнала
    flagRL3 = true;
  }
} 
void clickPause() {
  Serial.println("Нажата кнопка Пауза / Плей");
}
