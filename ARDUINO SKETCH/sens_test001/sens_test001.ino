/**
  ТЕСТОВЫЙ СКЕТЧ СЕНСОРНОГО ВЫКЛЮЧАТЕЛЯ С ПРЕРЫВАНИЯМИ НА NRF_LPCOMP
*/
bool button_flag;
bool sens_flag;
bool send_flag;
bool detection;
bool nosleep;
byte timer;
unsigned long SLEEP_TIME = 21600000; //6 hours
unsigned long oldmillis;
unsigned long newmillis;
unsigned long interrupt_time;
unsigned long SLEEP_TIME_W;
uint16_t currentBatteryPercent;
uint16_t batteryVoltage = 0;
uint16_t battery_vcc_min = 2400;
uint16_t battery_vcc_max = 3000;

#define MY_RADIO_NRF5_ESB
//#define MY_PASSIVE_NODE
#define MY_NODE_ID 30
#define MY_PARENT_NODE_ID 0
#define MY_PARENT_NODE_IS_STATIC
#define MY_TRANSPORT_UPLINK_CHECK_DISABLED
#define IRT_PIN 3 //(PORT0,  gpio 5)
#include <MySensors.h>
// see https://www.mysensors.org/download/serial_api_20
#define SENS_CHILD_ID 0
#define CHILD_ID_VOLT 254
MyMessage sensMsg(SENS_CHILD_ID, V_VAR1);
//MyMessage voltMsg(CHILD_ID_VOLT, V_VOLTAGE);


void preHwInit() {
  sleep(2000);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);
  pinMode(MODE_PIN, INPUT);
  pinMode(SENS_PIN, INPUT);
}

void before()
{
  NRF_POWER->DCDCEN = 1;
  NRF_UART0->ENABLE = 0;
  sleep(1000);
  digitalWrite(BLUE_LED, LOW);
  sleep(150);
  digitalWrite(BLUE_LED, HIGH);
}

void presentation()  {
  sendSketchInfo("EFEKTA Sens 1CH Sensor", "1.1");
  present(SENS_CHILD_ID, S_CUSTOM, "SWITCH STATUS");
  //present(CHILD_ID_VOLT, S_MULTIMETER, "Battery");
}

void setup() {
  digitalWrite(BLUE_LED, LOW);
  sleep(100);
  digitalWrite(BLUE_LED, HIGH);
  sleep(200);
  digitalWrite(BLUE_LED, LOW);
  sleep(100);
  digitalWrite(BLUE_LED, HIGH);
  lpComp();
  detection = false;
  SLEEP_TIME_W = SLEEP_TIME;
  pinMode(31, OUTPUT);
  digitalWrite(31, HIGH);
  /*
    while (timer < 10) {
    timer++;
    digitalWrite(GREEN_LED, LOW);
    wait(5);
    digitalWrite(GREEN_LED, HIGH);
    wait(500);
    }
    timer = 0;
  */
  sleep(7000);
  while (timer < 3) {
    timer++;
    digitalWrite(GREEN_LED, LOW);
    sleep(15);
    digitalWrite(GREEN_LED, HIGH);
    sleep(85);
  }
  timer = 0;
  sleep(1000);
}

void loop() {

  if (detection) {
    if (digitalRead(MODE_PIN) == 1 && button_flag == 0 && digitalRead(SENS_PIN) == 0) {
      //back side button detection
      button_flag = 1;
      nosleep = 1;
    }
    if (digitalRead(MODE_PIN) == 1 && button_flag == 1 && digitalRead(SENS_PIN) == 0) {
      digitalWrite(RED_LED, LOW);
      wait(10);
      digitalWrite(RED_LED, HIGH);
      wait(50);
    }
    if (digitalRead(MODE_PIN) == 0 && button_flag == 1 && digitalRead(SENS_PIN) == 0) {
      nosleep = 0;
      button_flag = 0;
      digitalWrite(RED_LED, HIGH);
      lpComp_reset();
    }

    if (digitalRead(SENS_PIN) == 1 && sens_flag == 0 && digitalRead(MODE_PIN) == 0) {
      //sens detection
      sens_flag = 1;
      nosleep = 1;
      newmillis = millis();
      interrupt_time = newmillis - oldmillis;
      SLEEP_TIME_W = SLEEP_TIME_W - interrupt_time;
      if (send(sensMsg.set(detection))) {
        send_flag = 1;
      }
    }
    if (digitalRead(SENS_PIN) == 1 && sens_flag == 1 && digitalRead(MODE_PIN) == 0) {
      if (send_flag == 1) {
        while (timer < 10) {
          timer++;
          digitalWrite(GREEN_LED, LOW);
          wait(20);
          digitalWrite(GREEN_LED, HIGH);
          wait(30);
        }
        timer = 0;
      } else {
        while (timer < 10) {
          timer++;
          digitalWrite(RED_LED, LOW);
          wait(20);
          digitalWrite(RED_LED, HIGH);
          wait(30);
        }
        timer = 0;
      }
    }
    if (digitalRead(SENS_PIN) == 0 && sens_flag == 1 && digitalRead(MODE_PIN) == 0) {
      sens_flag = 0;
      nosleep = 0;
      send_flag = 0;
      digitalWrite(GREEN_LED, HIGH);
      sleep(500);
      lpComp_reset();
    }
    if (SLEEP_TIME_W < 60000) {
      SLEEP_TIME_W = SLEEP_TIME;
      sendBatteryStatus();
    }
  }
  else {
    //if (detection == -1) {
    SLEEP_TIME_W = SLEEP_TIME;
    sendBatteryStatus();
  }
  if (nosleep == 0) {
    oldmillis = millis();
    sleep(SLEEP_TIME_W);
  }
}

void sendBatteryStatus() {
  wait(20);
  batteryVoltage = hwCPUVoltage();
  wait(2);

  if (batteryVoltage > battery_vcc_max) {
    currentBatteryPercent = 100;
  }
  else if (batteryVoltage < battery_vcc_min) {
    currentBatteryPercent = 0;
  } else {
    currentBatteryPercent = (100 * (batteryVoltage - battery_vcc_min)) / (battery_vcc_max - battery_vcc_min);
  }

  sendBatteryLevel(currentBatteryPercent, 1);
  wait(2000, C_INTERNAL, I_BATTERY_LEVEL);
  //send(powerMsg.set(batteryVoltage), 1);
  //wait(2000, 1, V_VAR1);
}

void lpComp() {
  NRF_LPCOMP->PSEL = IRT_PIN;
  NRF_LPCOMP->ANADETECT = 1;
  NRF_LPCOMP->INTENSET = B0100;
  NRF_LPCOMP->ENABLE = 1;
  NRF_LPCOMP->TASKS_START = 1;
  NVIC_SetPriority(LPCOMP_IRQn, 15);
  NVIC_ClearPendingIRQ(LPCOMP_IRQn);
  NVIC_EnableIRQ(LPCOMP_IRQn);
}

void s_lpComp() {
  if ((NRF_LPCOMP->ENABLE) && (NRF_LPCOMP->EVENTS_READY)) {
    NRF_LPCOMP->INTENCLR = B0100;
  }
}

void r_lpComp() {
  NRF_LPCOMP->INTENSET = B0100;
}

#if __CORTEX_M == 0x04
#define NRF5_RESET_EVENT(event)                                                 \
  event = 0;                                                                   \
  (void)event
#else
#define NRF5_RESET_EVENT(event) event = 0
#endif

extern "C" {
  void LPCOMP_IRQHandler(void) {
    detection = true;
    NRF5_RESET_EVENT(NRF_LPCOMP->EVENTS_UP);
    NRF_LPCOMP->EVENTS_UP = 0;
    MY_HW_RTC->CC[0] = (MY_HW_RTC->COUNTER + 2);
  }
}

void lpComp_reset () {
  s_lpComp();
  detection = false;
  NRF_LPCOMP->EVENTS_UP = 0;
  r_lpComp();
}
