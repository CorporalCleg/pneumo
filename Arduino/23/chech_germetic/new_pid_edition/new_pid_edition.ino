#include <SPI.h>
#include <Wire.h> // Библиотека для работы с шиной I2C
#include <Adafruit_ADS1X15.h> // Библиотека для работы с модулями ADS1115 и ADS1015



const int mosfetPin1 = 2; //Определение цифрового ПИН для управления соленоидом №1
const int mosfetPin2 = 3; //Определение цифрового ПИН для управления соленоидом №2
const int mosfetPin3 = 6; //Определение цифрового ПИН для управления соленоидом №3 (промежуточный)
const int mosfetPin4 = 7;

uint32_t timer = 0;   // переменная таймера для вывода значений

float voltage0; 
float pressure0;
float voltage1; // Значение напряжения на датчике давления в контуре насоса
float pressure1; // Показания давления датчика в контуре насоса
float voltage2; // Значение напряжения на датчике давления в контуре контрольного объема
float pressure2; // Показания давления датчика в контуре контрольного объема
float voltage3; // Значение напряжения на датчике давления в контуре мини-объема
float pressure3; // Показания давления датчика в контуре мини-объема

//Определение требуемого давления в контролируемом объеме
float desired_pressure = abs(-65.0); // целевое значение

float pump_stable_pressure = -70;
float minimum_pressure = pump_stable_pressure + 25.0;

//Определение ПИН-а для управления РЕЛЕ
int in1 = 5; // Канал IN1 реле будет управляться пином 5
int dt_in_ms = 25; //Определение периода формирования команд ПИД регуляторами


//Коэффициенты регулирования ПИД контрольного объема
float Kp = 120.0;
float Ki = 0.0;
float Kd = 0.3;
float sum_error = 0.0;
float min_out = -1.0;
float max_out = 1.0;
float prev_error = 0.0;
int tau = 50;
int delta = 400;

//side var. to open gates correctly
int first_time = 1;
int flag = 0;

//Задание переменной управляющего сигнала ПИД регуляторов
float PID_control_value_tank = 0;

Adafruit_ADS1115 ads; // Создание объекта ads для общения через него с модулем

// Переменные для хранения значений АЦП каждого канала
int16_t adc0, adc1, adc2, adc3;


float new_pid(float p){
    float error, output;
    
    error = desired_pressure - p;
    sum_error += error;                   
    output = Kp * error + Ki * sum_error + Kd * (error - prev_error)  / dt_in_ms;
    
    if (output < 0) {
      output /= 10.0;
    }

    prev_error = output;

    if (output < min_out) {
      return min_out;
    } else if (output > max_out){
      return max_out;      
    } else
      return output;
}


void setup() {
  pinMode(in1, OUTPUT);    // Установка пина 5 как выход
  digitalWrite(in1, LOW); //Закрытие канала IN1 реле

  pinMode(mosfetPin1, OUTPUT); //Устанновка пина mosfetPin1 как выхода
  pinMode(mosfetPin2, OUTPUT); //Устанновка пина mosfetPin2 как выхода
  pinMode(mosfetPin3, OUTPUT); //Устанновка пина mosfetPin3 как выхода
  pinMode(mosfetPin4, OUTPUT);

  //Продуваем контур при включении
  digitalWrite(mosfetPin1, HIGH);//Открытие соленоида №1 для соединения контуров насоса и контрольного объема
  digitalWrite(mosfetPin2, HIGH);//Открытие соленоида №2 для соединения контуров насоса и контрольного объема
  digitalWrite(mosfetPin3, HIGH);//Открытие соленоида №3 для соединения контуров насоса и контрольного объема
  digitalWrite(mosfetPin4, HIGH);//Открытие соленоида №4 для соединения контуров насоса и контрольного объема
  
  delay(2000);//Ожидание в течение 2х секунд для продувки контуров после перезапуска системы
  
  digitalWrite(mosfetPin1, LOW);//Открытие соленоида №1 для соединения контуров насоса и контрольного объема
  digitalWrite(mosfetPin2, LOW);//Закрытие соленоида №2
  digitalWrite(mosfetPin3, LOW);//Закрытие соленоида №3
  digitalWrite(mosfetPin4, LOW);//Закрытие соленоида №4
  // Установка коэффициента усиления 1
  ads.setGain(GAIN_ONE);
  ads.begin(); // Инициализация модуля ADS1115

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
}
// the loop routine runs over and over again forever:
void loop() {

  adc0 = ads.readADC_SingleEnded(0); // Чтение АЦП нулевого канала
  adc1 = ads.readADC_SingleEnded(1); // Чтение АЦП первого канала
  adc2 = ads.readADC_SingleEnded(2); // Чтение АЦП второго канала
  adc3 = ads.readADC_SingleEnded(3);

  // Расчёт фактических напряжений на каждом канале
  voltage0= float(adc0) * 0.125 / 1000.0;
  voltage1 = float(adc1) * 0.125 / 1000.0;
  voltage2 = float(adc2) * 0.125 / 1000.0;
  voltage3= float(adc3) * 0.125 / 1000.0;
  
  //Расчет давлений в контурах системы
  pressure0 = (voltage0 - 2.7) / 0.025; //Давление в насосе
  pressure1 = (voltage1 - 2.7) / 0.025; //Давление в контрольном объеме
  pressure2 = (voltage2 - 2.7) / 0.025; //Давление в мини-объеме
  pressure3 = (voltage3 - 2.7) / 0.025;
  
  //Вывод показаний датчиков в Serail порт с интервалом в 500 мс
  if (millis() - timer >= 200) { 
    // Serial.print("The pressure on first sensor is "); // print text
    Serial.print("Pressure in the pump:");
    Serial.println(pressure0, 1); // print pressure reading
    Serial.print("Pressure in the tank:");
    Serial.println(pressure1, 2);
    Serial.print("Pressure in the mini-volume:");
    Serial.println(pressure2, 2);
    Serial.print("Pressure in the mini-volume2:");
    Serial.println(pressure3, 3);
    timer = millis();
  }    

  if(abs(pressure0) < 80){
    if (abs(pressure0) < abs(minimum_pressure)) {
      digitalWrite(in1, HIGH);
      flag = 0;
      first_time = 1;
    }
    if (abs(pressure0) >= abs(pump_stable_pressure)) {
        digitalWrite(in1, LOW);
        flag = 1;
    }
  }
    PID_control_value_tank = new_pid(abs(pressure1));
    
    if (flag && first_time) {
      digitalWrite(mosfetPin1, HIGH);
      digitalWrite(mosfetPin2, HIGH);
      digitalWrite(mosfetPin3, HIGH);
      digitalWrite(mosfetPin4, LOW);
      first_time = 0;
    } else {
      digitalWrite(mosfetPin1, LOW);
      digitalWrite(mosfetPin2, LOW);
      digitalWrite(mosfetPin3, LOW);
      digitalWrite(mosfetPin4, LOW);
    } 
    delay(500);
}  

void increase_pressure() {
  digitalWrite(mosfetPin1, HIGH);
  delay(tau);
  digitalWrite(mosfetPin1, LOW);
  delay(tau + delta);
  digitalWrite(mosfetPin3, HIGH);
  delay(tau);
  digitalWrite(mosfetPin3, LOW);  
}

void decrease_pressure() {
  delay(tau);
  digitalWrite(mosfetPin2, LOW);
  delay(tau + delta);
  //digitalWrite(mosfetPin4, HIGH);
  delay(tau);
  digitalWrite(mosfetPin4, LOW);  
}
