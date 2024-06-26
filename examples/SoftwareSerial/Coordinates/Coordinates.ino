// ПРИМЕР ВЫВОДИТ КООРДИНАТЫ 5 РАЗ В СЕКУНДУ:      //
                                                   //
const uint8_t pinRX = 8;                           //  Определяем вывод RX (программного UART) на плате Arduino к которому подключён вывод TX модуля. Номер вывода можно изменить.
const uint8_t pinTX = 9;                           //  Определяем вывод TX (программного UART) на плате Arduino к которому подключён вывод RX модуля. Номер вывода можно изменить.
                                                   //
#include <SoftwareSerial.h>                        //  Подключаем библиотеку для работы с программным UART, до подключения библиотек iarduino_GPS_NMEA и iarduino_GPS_ATGM336.
#include <iarduino_GPS_NMEA.h>                     //  Подключаем библиотеку для расшифровки строк протокола NMEA получаемых по UART.
#include <iarduino_GPS_ATGM336.h>                  //  Подключаем библиотеку для настройки параметров работы GPS модуля ATGM336.
                                                   //
SoftwareSerial       SerialGPS(pinRX, pinTX);      //  Объявляем объект SerialGPS   для работы с функциями и методами библиотеки SoftwareSerial, указав выводы RX и TX Arduino.
iarduino_GPS_NMEA    gps;                          //  Объявляем объект gps         для работы с функциями и методами библиотеки iarduino_GPS_NMEA.
iarduino_GPS_ATGM336 SettingsGPS;                  //  Объявляем объект SettingsGPS для работы с функциями и методами библиотеки iarduino_GPS_ATGM336.
                                                   //
void setup(){                                      //
//   Инициируем работу библиотек:                  //
     Serial.begin(9600);                           //  Инициируем работу с аппаратной шиной UART для вывода данных в монитор последовательного порта на скорости 9600 бит/сек.
     SettingsGPS.begin(SerialGPS);                 //  Инициируем работу с GPS модулем по указанной шине UART. Функция сама определит текущую скорость GPS модуля ATGM336 (вместо программной шины, можно указывать аппаратные: Serial, Serial1, Serial2, Serial3).
     gps.begin(SerialGPS);                         //  Инициируем расшифровку строк NMEA указав объект используемой шины UART (вместо программной шины, можно указывать аппаратные: Serial, Serial1, Serial2, Serial3).
//   Настраиваем работу модуля:                    //
     SettingsGPS.baudrate(9600);                   //  Устанавливаем скорость передачи данных модулем и скорость работы шины UART в 9600 бит/сек.
     SettingsGPS.system(GPS_GP, GPS_GL);           //  Указываем что данные нужно получать от спутников навигационных систем GPS (GPS_GP) и Glonass (GPS_GL).
     SettingsGPS.composition(NMEA_RMC);            //  Указываем что каждый пакет данных NMEA должен содержать только одну строку и этой строкой является идентификатор RMC (рекомендуемый минимум навигационных данных).
     SettingsGPS.updaterate(10);                   //  Указываем обновлять данные 10 раз в секунду. Функция gps.read() читает данные в 2 раза медленнее чем их выводит модуль.
}                                                  //
                                                   //
void loop(){                                       //
     gps.read();                                   //  Читаем данные.
     if( gps.errPos ){                             //
         Serial.print("Координаты недостоверны."); //  Выводим сообщение об ошибке.
     }else{                                        //
         Serial.print("Широта: ");                 //
         Serial.print(gps.latitude,5);             //  Выводим широту в градусах.
         Serial.print("°, ");                      //
         Serial.print("Долгота: ");                //
         Serial.print(gps.longitude,5);            //  Выводим долготу в градусах.
         Serial.print("°.");                       //
     }   Serial.print("\r\n");                     //  Завершаем строку.
}                                                  //
                                                   //
//   ПРИМЕЧАНИЕ:                                   //  ВАЖНО:
//   В скетче определён состав пакета NMEA         //  Уменьшение идентификаторов в составе пакета NMEA приводит
//   обращением к функции composition();           //  к уменьшению количества данных содержащихся в нём.
//   Но информация о широте и долготе содержится   //  Например, строка идентификатора NMEA_RMC не содержит высоту и данные о спутниках.
//   не только в идентификаторе NMEA_RMC, а в:     //  Рекомендуется использовать следующий состав пакета NMEA:
//   NMEA_GGA - данные о последнем местоположений. //  SettingsGPS.composition(NMEA_RMC); - все данные, кроме высоты и данных о спутниках.
//   NMEA_GLL - географические координаты.         //  SettingsGPS.composition(NMEA_RMC, NMEA_GGA); - все данные кроме данных о спутниках.
//                                                 //  SettingsGPS.composition(NMEA_RMC, NMEA_GGA, NMEA_GSA, NMEA_GSV); - все данные.