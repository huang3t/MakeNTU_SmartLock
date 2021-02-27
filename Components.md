# Components introduction

## Arduino接線
* RFID:
    * SDA: 10
    * MOSI: 11(固定)
    * CLOCK: 13(固定)
    * MISO: 12(固定)
    * RST: A0
* Blue tooth
    * Rx: 6(藍芽的Rx)
    * Tx: 7(藍芽的Tx)
* Hall_pin: A1, GND
* buzzer_pin: 9, GND
* stepper:
    * pin1: 2
    * pin2: 3
    * pin3: 4
    * pin4: 5
* LED_pin: 8, 5V


## Code
* RFID
    * [reference 1](http://milktea132.blogspot.com/2018/02/arduino-18-rfid-rc522led.html)
    * [reference 2](https://swf.com.tw/?p=930)
* Blue tooth
    * reference: 電資工程入門
    * 手機端控制APP: 利用[APP Inventor](https://blog.cavedu.com/2014/04/04/%E9%9B%99a%E8%A8%88%E5%8A%83-part1%EF%BC%9Aapp-inventor-%E7%B6%93%E7%94%B1%E8%97%8D%E7%89%99%E6%8E%A7%E5%88%B6-arduino-led-%E4%BA%AE%E6%BB%85/)寫出簡單的控制程式
    * 回傳值在code有註解，利用char command儲存手機傳值，每次void loop就重設command為'd'(default)，避免重複執行指令
* buzzer
    * [reference](http://yhhuang1966.blogspot.com/2016/09/arduino_17.html)
    * include"note.h"是define音階用的，為了讓蜂鳴器唱出小蜜蜂^^
    * buzzer_check(): 在RFID成功讀取卡片資料時逼~一聲
    * buzzer_find(int): 唱出小蜜蜂，利用void loop唱出陣列中儲存的音符(變數i 紀錄唱到哪個音符)，因為如果用while讓蜂鳴器唱，此時無法讀取藍芽傳得值
* LED
    * reference: 常識(?
    * 利用void loop，及變數flash紀錄上一個迴圈的亮暗，製造閃爍效果，同buzzer_find()，若在void loop中使用while，就無法讀藍芽的回傳值，除非把BT.read()寫在每個while但這樣很麻煩
* Hall
    * [reference](http://bruce-iot.blogspot.com/2016/05/iot29-arduino.html)
    * 有磁場回傳1，沒有就0，就這麼簡單^^
    * millis(): 內建函式，回傳從arduino開機到現在的ms，最多記錄到49天後會[rollover](https://www.baldengineer.com/arduino-how-do-you-reset-millis.html)
    * 變數unsigned long lock_interval: 預計停下多久要自動上鎖 
    * 變數unsigned long previous_time: 紀錄上次時間停下的時間，霍爾模組有讀值就reset到millis()，當millis() - previous_time >= lock_interval 時就上鎖

* Stepper motor
    * Cheap step motor: 2相4線
    * ULN2003模組 + 擴充板
