// DS3231 - Version: Latest 
#include <DS3231.h>

 //DS3231 - Version: Latest 
// Si4735 rst 22 - 20 sdio  21 sclk Version: Latest 
#include <Adafruit_GFX.h>     //Библиотеки вывода на дисплей
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>      //Библиотеки тачскрина
#include <Wire.h>             
#include <Si4735.h>           //Библиотека радио
Si4735 radio;
DS3231  rtc(24, 26); //sda scl
static uint8_t prevHour=0;                                            //Переменная для сохранения секунд
static uint8_t prevMin=0;                                            //Переменная для сохранения секунд
static uint8_t prevSec=0;                                            //Переменная для сохранения секунд
static uint8_t prevDate=0;        
//Переменная для сохранения секунд


#define LCD_CS A3             // Chip Select - аналоговый пин 3
#define LCD_CD A2             // Command/Data - аналоговый пин 2
#define LCD_WR A1             // LCD запись - аналоговый пин 1
#define LCD_RD A0             // LCD чтение - аналоговый пин 0
#define LCD_RESET A4
#define YP A3                 // Пины для сенсора, первые два могут быть только аналоговыми, вторые - любыми, аналоговыми или цифровыми 
#define XM A2             
#define YM 9
#define XP 8

#define	BLACK   0x0000        //Цвета
#define	BLUE    0x001F
#define	RED     0xF800
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define WHITE   0xFFFF
#define PURPLE  0x780F      
#define OLIVE   0x7BE0      

#define BUTTON_X 40            //Вид кнопки
#define BUTTON_Y 180
#define BUTTON_W 60
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2

#define TEXT_X 10             //Текст-бокс для времени и станции
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 100
#define TEXT_TSIZE 3
#define TEXT_TCOLOR MAGENTA
#define TEXT_LEN 12           // Запись данных, хранимых в текст-боксе

char textfield[TEXT_LEN+3] = "";
uint8_t textfield_i=0;
char textfielddate[TEXT_LEN+4] = "";
uint8_t textfielddate_i=0;

#define TS_MINX 150           //Координаты для тачскрина
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define MINPRESSURE 10        //Сила нажатия для тачскрина
#define MAXPRESSURE 1000

Tim t;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);       //Назначение пинов
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button buttons[9];                                       //Создание кнопок
char buttonlabels[9][5] = 
    {"AM", "FM", "", 
    "Mute", "+", "-",
    "UnM", "S+", "S-"};
uint16_t buttoncolors[9] =
    {BLUE, BLUE, BLUE,                                                //Назначение цветов
    OLIVE, OLIVE, OLIVE, 
    PINK, PINK, PINK};

void setup(void) {
  Wire.begin();
  rtc.begin(); //Включение часов
 

  Serial.begin(9600);                                                       //Запускаем радио
  radio.begin();
  radio.setMode(FM);
  radio.tuneFrequency(10660);
  
    tft.reset();        //Настройка дисплея
    uint16_t identifier = tft.readID(); 
    if(identifier==0x0101)
    identifier=0x9341;
  tft.begin(identifier);  //Включение дисплея
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  
    for (uint8_t row=0; row<3; row++) {                                     //Отрисовка кнопок
      for (uint8_t col=0; col<3; col++) {
        buttons[col + row*3].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, buttoncolors[col+row*3], WHITE,
                  buttonlabels[col + row*3], BUTTON_TEXTSIZE); 
        buttons[col + row*3].drawButton();
      }
    }
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);                      //Создание строки состояния
}


void loop() {
  t = rtc.getTim();  
  digitalWrite(13, HIGH);                                                   //Конфигурация тачскрина, получение координаты нажатия
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {                            //Получение точки нажатия
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
   }
   for (uint8_t b=0; b<9; b++) {                                          //Опрос кнопок
    if (buttons[b].contains(p.x, p.y)) {
      buttons[b].press(true);  
    } else {
      buttons[b].press(false);
    }
  }
   for (uint8_t b=0; b<9; b++) {                                               //Если кнопка не нажата, то сохраняет обычный вид
    if (buttons[b].justReleased()) {
        buttons[b].drawButton();
    }
    if (buttons[b].justPressed()) {                                       //Смена цвета кнопки при нажатии
        buttons[b].drawButton(true);
        delay(500);                                                         //Чтобы ничего не мигало зря



                          if (b==0) {
                            radio.setMode(AM);
                            delay(500);} 
                          if (b==1) {
                            radio.setMode(FM);
                            delay(500);} 
                           if (b==3) {
                            radio.mute();
                            delay(500);} 
                          if (b==4) {
                            radio.volumeUp();
                            delay(500);} 
                          if (b==5) {
                            radio.volumeDown();
                            delay(500);} 
                          if (b==6) {
                            radio.unmute();
                            delay(500);}                        
                          if (b==7) {
                            radio.seekUp();
                            delay(500);}
                          if (b==8) {
                            radio.seekDown();
                            delay(500);}}  }
                          
                           

  tft.fillRect(20, 20, 90, 20, BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20); tft.println(t.hour, DEC);
  tft.setCursor(40, 20); tft.println(":");
  tft.setCursor(50, 20); tft.println(t.min, DEC) ;
   tft.setCursor(70, 20); tft.println(":");
  tft.setCursor(80, 20); tft.println(t.sec, DEC);

 tft.setCursor(20, 50); 
 tft.println("08.12.2018");
 
tft.setTextColor(WHITE);
tft.setCursor(15, 80);
tft.setTextSize(2);
tft.print("Temperature:");  tft.print(rtc.getTemp());  tft.print("C");

}
