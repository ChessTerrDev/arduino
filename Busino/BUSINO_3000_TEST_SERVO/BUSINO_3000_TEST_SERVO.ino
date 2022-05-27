// Подклоючаем библиотеку Servo
#include <Servo.h> 

//Сервоприводы
#define SR1 1   //Сервопривод № 1
#define SR2 2   //Сервопривод № 2
#define SR3 3   //Сервопривод № 3

// Создаем объект
Servo Servo1;
Servo Servo2;
Servo Servo3;

void setup() {
  // Нам нужно подключить сервопривод к используемому номеру пина
  Servo1.attach(SR1);
  Servo2.attach(SR2);
  Servo3.attach(SR3);
}

void loop(){
  // 0 градусов
  Servo1.write(0);
  Servo2.write(0);
  Servo3.write(0);
  delay(1000);
  // 90 градусов
  Servo1.write(90);
  Servo2.write(90);
  Servo3.write(90);
  delay(1000);

}
