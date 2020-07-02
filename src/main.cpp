/* WARNING: the code below will make you cry;
*           a safety pig is provided below for your benefit 
*                          _
*  _._ _..._ .-',     _.._(`))
* '-. `     '  /-._.-'    ',/
*    )         \            '.
*   / _    _    |             \
*  |  a    a    /              |
*  \   .-.                     ;  
*   '-('' ).-'       ,'       ;
*      '-;           |      .'
*         \           \    /
*         | 7  .__  _.-\   \
*         | |  |  ``/  /`  /
*        /,_|  |   /,_/   /
*           /,_/      '`-'
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <TimeLib.h>

#define PIN 14

//timezone(UTC+x)
#define timezone 3

//Wi-Fi SSID and password
#define ssid  "something"
#define password "something"

//colors (gradient)
#define GRAD0_R 255
#define GRAD0_G 0
#define GRAD0_B 0
#define GRAD1_R 255
#define GRAD1_G 0
#define GRAD1_B 31
#define GRAD2_R 255
#define GRAD2_G 0
#define GRAD2_B 51
#define GRAD3_R 255
#define GRAD3_G 0
#define GRAD3_B 68
#define GRAD4_R 255
#define GRAD4_G 0
#define GRAD4_B 86
#define GRAD5_R 253
#define GRAD5_G 0
#define GRAD5_B 103
#define GRAD6_R 244
#define GRAD6_G 0
#define GRAD6_B 121
#define GRAD7_R 233
#define GRAD7_G 0
#define GRAD7_B 139
#define GRAD8_R 218
#define GRAD8_G 0
#define GRAD8_B 157
#define GRAD9_R 200
#define GRAD9_G 0
#define GRAD9_B 174
#define GRAD10_R 177
#define GRAD10_G 0
#define GRAD10_B 191
#define GRAD11_R 148
#define GRAD11_G 0
#define GRAD11_B 206
#define GRAD12_R 109
#define GRAD12_G 0
#define GRAD12_B 220

Adafruit_NeoPixel strip = Adafruit_NeoPixel(140, PIN, NEO_GRB + NEO_KHZ800);

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org");

byte i, r;
byte a[5];
boolean curState[28];
byte hours, minutes, cMinute = 100, br;
int brightness;
int inputVal, prev_inputVal = 2000;
unsigned long eTime;

void setDigit(byte place, byte digit);
void setVerticalColorGradient(byte line, byte red, byte green, byte blue);

void setup() {
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  timeClient.begin();
  timeClient.setTimeOffset(timezone*3600);  
  strip.begin();
  strip.show();
  for(i=0;i<5;i++) a[i] = 140;
}

void loop() {
  inputVal = analogRead(A0);
  if (abs(inputVal - prev_inputVal) > 15){
    prev_inputVal = inputVal;
    brightness = (inputVal-20)/5;
    if(brightness < 0) brightness = 0;
    br = brightness + 55;
    strip.setBrightness(br);
  }
  timeClient.update();
  eTime = timeClient.getEpochTime();
  hours = hour(eTime);
  minutes = minute(eTime);
  if(minutes != cMinute){
    setDigit(0, hours/10);
    setDigit(1, hours%10);
    setDigit(2, minutes/10);
    setDigit(3, minutes%10);
    cMinute = minutes;
  }
  
  setVerticalColorGradient(0, GRAD0_R, GRAD0_G, GRAD0_B);
  setVerticalColorGradient(1, GRAD1_R, GRAD1_G, GRAD1_B);
  setVerticalColorGradient(2, GRAD2_R, GRAD2_G, GRAD2_B);
  setVerticalColorGradient(3, GRAD3_R, GRAD3_G, GRAD3_B);
  setVerticalColorGradient(4, GRAD4_R, GRAD4_G, GRAD4_B);
  setVerticalColorGradient(5, GRAD5_R, GRAD5_G, GRAD5_B);
  setVerticalColorGradient(6, GRAD6_R, GRAD6_G, GRAD6_B);
  setVerticalColorGradient(7, GRAD7_R, GRAD7_G, GRAD7_B);
  setVerticalColorGradient(8, GRAD8_R, GRAD8_G, GRAD8_B);
  setVerticalColorGradient(9, GRAD9_R, GRAD9_G, GRAD9_B);
  setVerticalColorGradient(10, GRAD10_R, GRAD10_G, GRAD10_B);
  setVerticalColorGradient(11, GRAD11_R, GRAD11_G, GRAD11_B);
  setVerticalColorGradient(12, GRAD12_R, GRAD12_G, GRAD12_B);
  for(i=0; i<strip.numPixels(); i++){
    if(!curState[i/5]) strip.setPixelColor(i, 0);
  }
  r = random(0, 150);
  if(r == 42){
    if(a[0] > 139) a[0] = 0;
    else if(a[1] > 139) a[1] = 0;
    else if(a[2] > 139) a[2] = 0;
    else if(a[3] > 139) a[3] = 0;
    else if(a[4] > 139) a[4] = 0;
  }
  for(i=0; i<5; i++){
    if(a[i] < 140){
      if(curState[a[i]/5]) strip.setPixelColor(a[i], 255, 162, 0);
      if(a[i] != 139 && curState[(a[i]+1)/5]) strip.setPixelColor(a[i]+1, 255, 162, 0);
    }
    a[i]++;
  }
  strip.show();
  delay(30);
}


void setDigit(byte place, byte digit){ //drawing digits
  boolean st_a = false, st_b = false,  st_c = false,  st_d = false,  st_e = false,  st_f = false,  st_g = false; 
  switch (digit) {
  case 0:
    st_a = true;
    st_b = true;
    st_c = true;
    st_d = true;
    st_e = true;
    st_f = true;
    st_g = false;
    break;
  case 1:
    st_a = false;
    st_b = true;
    st_c = true;
    st_d = false;
    st_e = false;
    st_f = false;
    st_g = false;
    break;
    case 2:
    st_a = true;
    st_b = true;
    st_c = false;
    st_d = true;
    st_e = true;
    st_f = false;
    st_g = true;
    break;
    case 3:
    st_a = true;
    st_b = true;
    st_c = true;
    st_d = true;
    st_e = false;
    st_f = false;
    st_g = true;
    break;
    case 4:
    st_a = false;
    st_b = true;
    st_c = true;
    st_d = false;
    st_e = false;
    st_f = true;
    st_g = true;
    break;
    case 5:
    st_a = true;
    st_b = false;
    st_c = true;
    st_d = true;
    st_e = false;
    st_f = true;
    st_g = true;
    break;
    case 6:
    st_a = true;
    st_b = false;
    st_c = true;
    st_d = true;
    st_e = true;
    st_f = true;
    st_g = true;
    break;
    case 7:
    st_a = true;
    st_b = true;
    st_c = true;
    st_d = false;
    st_e = false;
    st_f = false;
    st_g = false;
    break;
    case 8:
    st_a = true;
    st_b = true;
    st_c = true;
    st_d = true;
    st_e = true;
    st_f = true;
    st_g = true;
    break;
  default:
    st_a = true;
    st_b = true;
    st_c = true;
    st_d = true;
    st_e = false;
    st_f = true;
    st_g = true;
    break;
  }
    curState[0+place*7] = st_g;
    curState[1+place*7] = st_b;
    curState[2+place*7] = st_a;
    curState[3+place*7] = st_f;
    curState[4+place*7] = st_e;
    curState[5+place*7] = st_d;
    curState[6+place*7] = st_c;
}

void setVerticalColorGradient(byte line, byte red, byte green, byte blue){
  switch (line){
    case 0:
      strip.setPixelColor(25, red, green, blue);
      strip.setPixelColor(26, red, green, blue);
      strip.setPixelColor(27, red, green, blue);
      strip.setPixelColor(28, red, green, blue);
      strip.setPixelColor(29, red, green, blue);

      strip.setPixelColor(60, red, green, blue);
      strip.setPixelColor(61, red, green, blue);
      strip.setPixelColor(62, red, green, blue);
      strip.setPixelColor(63, red, green, blue);
      strip.setPixelColor(64, red, green, blue);

      strip.setPixelColor(95, red, green, blue);
      strip.setPixelColor(96, red, green, blue);
      strip.setPixelColor(97, red, green, blue);
      strip.setPixelColor(98, red, green, blue);
      strip.setPixelColor(99, red, green, blue);

      strip.setPixelColor(130, red, green, blue);
      strip.setPixelColor(131, red, green, blue);
      strip.setPixelColor(132, red, green, blue);
      strip.setPixelColor(133, red, green, blue);
      strip.setPixelColor(134, red, green, blue);
      break;
    case 1:
      strip.setPixelColor(24, red, green, blue);
      strip.setPixelColor(30, red, green, blue);
      strip.setPixelColor(59, red, green, blue);
      strip.setPixelColor(65, red, green, blue);
      strip.setPixelColor(94, red, green, blue);
      strip.setPixelColor(100, red, green, blue);
      strip.setPixelColor(129, red, green, blue);
      strip.setPixelColor(135, red, green, blue);
      break;
    case 2:
      strip.setPixelColor(23, red, green, blue);
      strip.setPixelColor(31, red, green, blue);
      strip.setPixelColor(58, red, green, blue);
      strip.setPixelColor(66, red, green, blue);
      strip.setPixelColor(93, red, green, blue);
      strip.setPixelColor(101, red, green, blue);
      strip.setPixelColor(128, red, green, blue);
      strip.setPixelColor(136, red, green, blue);
      break;
    case 3:
      strip.setPixelColor(22, red, green, blue);
      strip.setPixelColor(32, red, green, blue);
      strip.setPixelColor(57, red, green, blue);
      strip.setPixelColor(67, red, green, blue);
      strip.setPixelColor(92, red, green, blue);
      strip.setPixelColor(102, red, green, blue);
      strip.setPixelColor(127, red, green, blue);
      strip.setPixelColor(137, red, green, blue);
      break;
    case 4:
      strip.setPixelColor(21, red, green, blue);
      strip.setPixelColor(33, red, green, blue);
      strip.setPixelColor(56, red, green, blue);
      strip.setPixelColor(68, red, green, blue);
      strip.setPixelColor(91, red, green, blue);
      strip.setPixelColor(103, red, green, blue);
      strip.setPixelColor(126, red, green, blue);
      strip.setPixelColor(138, red, green, blue);
      break;
    case 5:
      strip.setPixelColor(20, red, green, blue);
      strip.setPixelColor(34, red, green, blue);
      strip.setPixelColor(55, red, green, blue);
      strip.setPixelColor(69, red, green, blue);
      strip.setPixelColor(90, red, green, blue);
      strip.setPixelColor(104, red, green, blue);
      strip.setPixelColor(125, red, green, blue);
      strip.setPixelColor(139, red, green, blue);
      break;
    case 6:
      strip.setPixelColor(0, red, green, blue);
      strip.setPixelColor(1, red, green, blue);
      strip.setPixelColor(2, red, green, blue);
      strip.setPixelColor(3, red, green, blue);
      strip.setPixelColor(4, red, green, blue);

      strip.setPixelColor(35, red, green, blue);
      strip.setPixelColor(36, red, green, blue);
      strip.setPixelColor(37, red, green, blue);
      strip.setPixelColor(38, red, green, blue);
      strip.setPixelColor(39, red, green, blue);

      strip.setPixelColor(70, red, green, blue);
      strip.setPixelColor(71, red, green, blue);
      strip.setPixelColor(72, red, green, blue);
      strip.setPixelColor(73, red, green, blue);
      strip.setPixelColor(74, red, green, blue);

      strip.setPixelColor(105, red, green, blue);
      strip.setPixelColor(106, red, green, blue);
      strip.setPixelColor(107, red, green, blue);
      strip.setPixelColor(108, red, green, blue);
      strip.setPixelColor(109, red, green, blue);
      break;
    case 7:
      strip.setPixelColor(19, red, green, blue);
      strip.setPixelColor(5, red, green, blue);
      strip.setPixelColor(54, red, green, blue);
      strip.setPixelColor(40, red, green, blue);
      strip.setPixelColor(89, red, green, blue);
      strip.setPixelColor(75, red, green, blue);
      strip.setPixelColor(124, red, green, blue);
      strip.setPixelColor(110, red, green, blue);
      break;
    case 8:
      strip.setPixelColor(18, red, green, blue);
      strip.setPixelColor(6, red, green, blue);
      strip.setPixelColor(53, red, green, blue);
      strip.setPixelColor(41, red, green, blue);
      strip.setPixelColor(88, red, green, blue);
      strip.setPixelColor(76, red, green, blue);
      strip.setPixelColor(123, red, green, blue);
      strip.setPixelColor(111, red, green, blue);
      break;
    case 9:
      strip.setPixelColor(17, red, green, blue);
      strip.setPixelColor(7, red, green, blue);
      strip.setPixelColor(52, red, green, blue);
      strip.setPixelColor(42, red, green, blue);
      strip.setPixelColor(87, red, green, blue);
      strip.setPixelColor(77, red, green, blue);
      strip.setPixelColor(122, red, green, blue);
      strip.setPixelColor(112, red, green, blue);
      break;
    case 10:
      strip.setPixelColor(16, red, green, blue);
      strip.setPixelColor(8, red, green, blue);
      strip.setPixelColor(51, red, green, blue);
      strip.setPixelColor(43, red, green, blue);
      strip.setPixelColor(86, red, green, blue);
      strip.setPixelColor(78, red, green, blue);
      strip.setPixelColor(121, red, green, blue);
      strip.setPixelColor(113, red, green, blue);
      break;
    case 11:
      strip.setPixelColor(15, red, green, blue);
      strip.setPixelColor(9, red, green, blue);
      strip.setPixelColor(50, red, green, blue);
      strip.setPixelColor(44, red, green, blue);
      strip.setPixelColor(85, red, green, blue);
      strip.setPixelColor(79, red, green, blue);
      strip.setPixelColor(120, red, green, blue);
      strip.setPixelColor(114, red, green, blue);
      break;
    default:
      strip.setPixelColor(10, red, green, blue);
      strip.setPixelColor(11, red, green, blue);
      strip.setPixelColor(12, red, green, blue);
      strip.setPixelColor(13, red, green, blue);
      strip.setPixelColor(14, red, green, blue);

      strip.setPixelColor(45, red, green, blue);
      strip.setPixelColor(46, red, green, blue);
      strip.setPixelColor(47, red, green, blue);
      strip.setPixelColor(48, red, green, blue);
      strip.setPixelColor(49, red, green, blue);

      strip.setPixelColor(80, red, green, blue);
      strip.setPixelColor(81, red, green, blue);
      strip.setPixelColor(82, red, green, blue);
      strip.setPixelColor(83, red, green, blue);
      strip.setPixelColor(84, red, green, blue);

      strip.setPixelColor(115, red, green, blue);
      strip.setPixelColor(116, red, green, blue);
      strip.setPixelColor(117, red, green, blue);
      strip.setPixelColor(118, red, green, blue);
      strip.setPixelColor(119, red, green, blue);
      break;
  }
}
