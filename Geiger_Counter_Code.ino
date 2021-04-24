


#include <Wire.h>

#include <LiquidCrystal_I2C.h> //从库管理器添加1602I2C库

#include <SPI.h>

#define LOG_PERIOD 15000  
#define MAX_PERIOD 60000  

 

unsigned long counts;     // GM 变量

unsigned long cpm;        // CPM 变量

unsigned int multiplier;  //设置换算变量

unsigned long previousMillis;  //测量时间

float usv;

 

LiquidCrystal_I2C lcd(0x27, 16, 2); //设置LCD地址为 0x27 （1602显示器）

 

void tube_impulse() {      //自加

  counts++;

}

 

void setup()

{

  counts = 0;

  cpm = 0;    //计数

  multiplier = MAX_PERIOD / LOG_PERIOD;      //计算乘数，取决于周期

  //Serial.begin(9600);

  attachInterrupt(0, tube_impulse, FALLING); //中断为下降沿触发

 

  //////////////////

  lcd.init();                     

 

  // 输出数据到屏幕

  lcd.backlight();

  lcd.setCursor(5, 0);

  lcd.print("Boot...");  //写一个开机boot加载的画面，提供一个盖革管启动的时间。

   lcd.setCursor(0, 1);

  for(int i=0;i<16;i++)

  {

  lcd.write(0xff);

  delay(250);

  }

}

 

void loop()

{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > LOG_PERIOD) {

    previousMillis = currentMillis;

    cpm = counts * multiplier;//得出技术次数

    usv = float(cpm) / 151;//带入公式计算出辐射强度

    lcd.clear();

    lcd.print("CPM=");

    lcd.print(cpm);//输出cpm值

    lcd.setCursor(0, 1);//第二行

    lcd.print(usv);

    lcd.print(" uSv/h");//输出强度值

    counts = 0;//复位

    if (usv >= 10)

    {

      lcd.setCursor(9, 0);

      lcd.print("Danger!");//如果辐射大于10则显示危险

      delay(0.1);

    }

    else if (usv < 10 && usv >= 0.52)

      {

        lcd.setCursor(10, 0);

        lcd.print("Unsafe");//在0.52-10这个范围显示不安全

        delay(0.1);

      }

      else if (usv < 0.52)

        {

          lcd.setCursor(10, 0);

          lcd.print("Safety");//在这个值以下显示安全

          delay(0.1);

        }

      }

 

}
