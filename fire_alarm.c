#include <Arduino.h>
#define led 2
#define mq2 13
#define uS_TO_S_FACTOR 1000000  //tương tác với ngoại vi mỗi micro giây
#define TIME_TO_SLEEP  5        // ESP32 sẽ rơi vào trạng thái ngủ mỗi 5 giây

//Năng lượng tiêu thụ trong trạng thái ngủ của ESP32 10 µA
RTC_DATA_ATTR int bootCount = 0; // ghi lại số lần thức dậy

//hàm kiểm tra lí do thức dậy của ESP32
//Start
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
 
  wakeup_reason = esp_sleep_get_wakeup_cause();// gọi hàm esp_sleep_get_wakeup_cause() để kiểm tra lí do thức dậy
  switch(wakeup_reason)
  {
    //RTC_IO thường liên quan đến xử lý sự kiện từ các tín hiệu ngoại vi hoặc cảm biến, trong khi RTC_CNTL chủ yếu liên quan đến quản lý và kiểm soát năng lượng của vi điều khiển ESP32. 
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;//ESP32 đã thức dậy từ trạng thái ngủ sâu do một tín hiệu ngoại vi (external signal) được nhận thông qua RTC_IO (Real-Time Clock Input/Output)
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;// ESP32 đã thức dậy do một tín hiệu ngoại vi được nhận thông qua RTC_CNTL (Real-Time Clock Control).
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;// ESP32 được thiết kế để kết nối với các cảm biến cảm ứng (touch sensors), chỉ cần chạm vào thì nó sẽ nhận tính hiệu và gọi ESP32 dậy
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;// ESP32 đã thức dậy do chương trình chạy trên ULP co-processor.
// ULP là một bộ xử lý siêu tiết kiệm năng lượng tích hợp trong vi điều khiển ESP32.
// Chương trình chạy trên ULP được thiết kế để chạy ở mức tiêu thụ năng lượng thấp, cho phép vi điều khiển tiếp tục hoạt động ở trạng thái ngủ sâu trong khi ULP vẫn thực hiện các nhiệm vụ cần thiết.
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
//End

void setup(){
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(GPIO_NUM_4,OUTPUT);
  pinMode(GPIO_NUM_18,OUTPUT);

  pinMode(led,OUTPUT);
  pinMode(GPIO_NUM_26,INPUT);
  delay(1000);
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();// gọi phương thức để in ra lí do thức dậy
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  // " Seconds");
   esp_sleep_enable_ext0_wakeup(GPIO_NUM_26,0);
   while(digitalRead(GPIO_NUM_26)==0)
   {
    Serial.println(digitalRead(GPIO_NUM_26));
    digitalWrite(GPIO_NUM_2,1);
    digitalWrite(GPIO_NUM_18,1);
    digitalWrite(GPIO_NUM_4,1);
    delay(100);
    digitalWrite(GPIO_NUM_2,0);
    digitalWrite(GPIO_NUM_18,0);
    digitalWrite(GPIO_NUM_4,0);
    delay(100);
    if(digitalRead(GPIO_NUM_26)==1) break;
   }
  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}
void loop()
{
  //This is not going to be called
  // Serial.println(analogRead(mq2));
  // digitalWrite(led,1);
  // delay(100);
  // Serial.println(analogRead(mq2));
  // digitalWrite(led,0);
  // delay(100);

}

