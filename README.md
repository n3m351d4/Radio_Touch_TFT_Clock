# Radio_Touch_TFT_Clock
![Принципиальная схема](https://pp.userapi.com/c848736/v848736297/1c2947/IHWIIIvTtFA.jpg)
![Структурная схема](https://pp.userapi.com/c848736/v848736297/1c2950/CO4_dJvRcXY.jpg)
![Алгоритм](https://pp.userapi.com/c848736/v848736297/1c2959/YHnNG_kGTiY.jpg)
Так как Arduino Mega работает от напряжения в 5 В, а ИМС Si4735 от 3.3 В, возникает необходимость в их безопасном соединении. Самым простым решением является использование резисторов, но в такой схеме будут возникать задержки. К тому же, логические уровни будут работать только однонаправленно, поэтому, если будет необходимо задать обратное направление, понадобятся лишние провода. Таких проблем можно избежать, используя преобразователь логических уровней.
	Для использования в проекте, был выбран 8-ми канальный преобразователь логических уровней TXS0108E, который работает в диапазоне напряжений от 1.8 до 6 В. Выводы В1-В6 предназначены для включения устройств с 5 В логикой, а B1-B6 – с 3.3 В логикой. На питающий вход VCA подается напряжение одного логического уровня, а на VCB – другого. Направление преобразования распознаётся и выполняется автоматически. Обязательным условием работы данного преобразователя является общий отрицательный вывод для всех источников питания, в том числе и раздельных.
  
