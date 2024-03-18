#include <SPI.h>
#include <Wire.h> // Библиотека для работы с шиной I2C
#include <Adafruit_ADS1X15.h> // Библиотека для работы с модулями ADS1115 и ADS1015

// const int analogPin = A5; // Пин съема аналогового сигнала из датчика давления в контуре насоса
// const int analogPin1 = A4; // Пин съема аналогового сигнала из датчика давления в контуре контрольного объема

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
float desired_pressure = abs(-55.0); // целевое значение
float desired_pressure_speed = 0; //скорость изменения давления ПИТО 

float desired_static = 0; // целевое значение статического давления
float desired_static_speed = 0; //скорость изменения давления СТАТИУКА


float pump_stable_pressure = -70;
float minimum_pressure = pump_stable_pressure + 5.0;

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
int delta = 200;

//Задание переменной управляющего сигнала ПИД регуляторов
float PID_control_value_tank = 0;


Adafruit_ADS1115 ads; // Создание объекта ads для общения через него с модулем

// Переменные для хранения значений АЦП каждого канала
int16_t adc0, adc1, adc2, adc3;

//pid controller
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

//Ввод переменной для чтения данных из Serial порта
bool newData = false;
//
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
//
char messageFromPC[numChars] = {0};
//Команды для ввода из порта ПК
const char *set_pressure_command = "set_pressure";
// char *set_KP_command = "set_Kp";
// char *set_KD_command = "set_Kd";
// char *set_KI_command = "set_Ki";
//Переменная для хранения числа из порта ПК
float floatFromPC = 0.0;
//

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

  input_handler();
    
  adc0 = ads.readADC_SingleEnded(0); // Чтение АЦП нулевого канала
  adc1 = ads.readADC_SingleEnded(1); // Чтение АЦП первого канала
  adc2 = ads.readADC_SingleEnded(2); // Чтение АЦП второго канала
  adc3 = ads.readADC_SingleEnded(3);

  
  //Вывод показаний датчиков в Serail порт с интервалом в 200 мс
  if (millis() - timer >= 200) { 
    // Расчёт фактических напряжений на каждом канале
    voltage0= float(adc0) * 0.125 / 1000.0;
    voltage1 = float(adc1) * 0.125 / 1000.0;
    voltage2 = float(adc2) * 0.125 / 1000.0;
    voltage3= float(adc3) * 0.125 / 1000.0;
    
    // //Расчет давлений в контурах системы
    pressure0 = (voltage0 - 2.7) / 0.025; //Давление в насосе
    pressure1 = (voltage1 - 2.7) / 0.025; //Давление в контрольном объеме
    pressure2 = (voltage2 - 2.7) / 0.025; //Давление в мини-объеме
    pressure3 = (voltage3 - 2.7) / 0.025;

    Serial.print(pressure1, 2);
    Serial.print(" ");
    Serial.print(0.00);
    Serial.print(" ");
    Serial.print(0.00);
    Serial.print(" ");
    Serial.println(0.00);

    if (abs(pressure0) < abs(minimum_pressure)) {
      digitalWrite(in1, HIGH);
    }
    
    if (abs(pressure0) >= abs(pump_stable_pressure)) {
        digitalWrite(in1, LOW);
    }
    
    PID_control_value_tank = new_pid(abs(pressure1));
      
    if ((PID_control_value_tank == -1)) {
      decrease_pressure();
    } 
    else if (PID_control_value_tank == 1){
      if (abs(desired_pressure + pressure1) >= 5) {
        highly_increase_pressure();
      } else {
        increase_pressure();//операция по единичному подъему давления      
      }
    } else {
      
      digitalWrite(mosfetPin1, LOW);
      digitalWrite(mosfetPin2, LOW);
      digitalWrite(mosfetPin3, LOW);
      digitalWrite(mosfetPin4, LOW);
      
    }
    timer = millis();
  }     
  
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

void highly_increase_pressure() {
  digitalWrite(mosfetPin1, HIGH);
  digitalWrite(mosfetPin3, HIGH);
  delay(tau*2);
  digitalWrite(mosfetPin3, LOW);
  digitalWrite(mosfetPin1, LOW);  
}

void decrease_pressure() {
  digitalWrite(mosfetPin2, HIGH);
  delay(tau);
  digitalWrite(mosfetPin2, LOW);
  delay(tau + delta);
  digitalWrite(mosfetPin4, HIGH);
  delay(tau);
  digitalWrite(mosfetPin4, LOW);  
}

void parseData() { //split the data into its parts
  char * strtokIndx; //this is used by strtok() as an index

  strtokIndx = strtok(tempChars, " "); //get the firt part - command's name
  strcpy(messageFromPC, strtokIndx);// copy it to messageFromPC

  strtokIndx = strtok(NULL, " ");
  floatFromPC = atof(strtokIndx); //convert this part to a float
}

void applyInputCommand() {
  // Serial.print("Message ");
  // Serial.println(messageFromPC);
  // Serial.print("Desired pressure ");
  // Serial.println(floatFromPC);
  if (strcmp(messageFromPC, set_pressure_command) == 0) {
    if (floatFromPC == 0) {
      //Serial.println("ERROR: INCORRECT PRESSURE UNITS");
    }
    else if (abs(floatFromPC) != 0) {     
      desired_pressure = abs(floatFromPC);
      // tank_regulator.setpoint = abs(desired_pressure);

//      Serial.print(desired_pressure);
//      Serial.print(" ");
//      Serial.print(desired_pressure_speed);
//      Serial.print(" ");
//      Serial.print(desired_static);
//      Serial.print(" ");
//      Serial.println(desired_static_speed);
      // Serial.println(desired_pressure);
      //Serial.print("NEW_DESIRED_PRESSURE_SET = ");
      //Serial.println(desired_pressure);
    }
  }
  else {
    //Serial.println("ERROR: UNKNOWN COMMAND");
    //Serial.println(messageFromPC);
  }
}

void recvData() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }    
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
      
void input_handler() {
  recvData();
    if (newData == true) {
      strcpy(tempChars, receivedChars);
      //this temporary copy is necessary to protect the original data
      //because strtok() used in parseData() replaces the commas with \0
      parseData();
      applyInputCommand();
      newData = false;
    }
}
