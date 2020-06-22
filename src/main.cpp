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
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Dusk2Dawn.h>

#define PIN 14

//night color
#define r_night 230
#define g_night 118
#define b_night 33

//location for sunset & sinrise
//current location is Moscow
#define latitude 55.7075
#define longitude 37.6038

//timezone(UTC+x)
#define timezone 3

//Wi-Fi SSID and password
#define ssid  "something"
#define password "something"

//day colors (gradient)
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

Dusk2Dawn location(latitude, longitude, timezone);

byte i, r;
byte a[5];
boolean curState[28], night;
byte hours, minutes, cDay = 100, cMinute = 100, br;
int flicker, r1, g1, b1, brightness;
int sSet, sRise, hrmin, inputVal, prev_inputVal = 2000;
unsigned long eTime;

void setDigit(byte place, byte digit);

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.begin();
  timeClient.begin();
  timeClient.setTimeOffset(timezone*3600);  
  strip.begin();
  strip.show();
  for(i=0;i<5;i++) a[i] = 140;
}

void loop() {
  timeClient.update();
  ArduinoOTA.handle();
  eTime = timeClient.getEpochTime();
   if(cDay != day(eTime)){
    sSet = location.sunset(year(eTime), month(eTime), day(eTime), false);
    sRise = location.sunrise(year(eTime), month(eTime), day(eTime), false);
    cDay = day(eTime);
  }
  hours = hour(eTime);
  minutes = minute(eTime);
  if(minutes != cMinute){
    hrmin = (hours*60)+minutes;
    if(hrmin > sSet || hrmin < sRise) night = true;
    else night = false;
    setDigit(0, hours/10);
    setDigit(1, hours%10);
    setDigit(2, minutes/10);
    setDigit(3, minutes%10);
    cMinute = minutes;
  }
  inputVal = analogRead(A0);
  if (abs(inputVal - prev_inputVal) > 15){
    prev_inputVal = inputVal;
    brightness = (inputVal-20)/5;
    if(brightness < 0) brightness = 0;
    br = brightness + 55;
    strip.setBrightness(br);
  }
  if(night){
    for(i=0; i<strip.numPixels(); i++) {
    flicker = random(0,55);
    r1 = r_night-flicker;
    g1 = g_night-flicker;
    b1 = b_night-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
      strip.setPixelColor(i,r1,g1, b1);
    }
    for(i=0; i<strip.numPixels(); i++){
      if(!curState[i/5]) strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(random(10,113));
  } else {
  //Middle line L6
    strip.setPixelColor(0, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(1, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(2, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(3, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(4, GRAD6_R, GRAD6_G, GRAD6_B);

    strip.setPixelColor(35, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(36, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(37, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(38, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(39, GRAD6_R, GRAD6_G, GRAD6_B);

    strip.setPixelColor(70, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(71, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(72, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(73, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(74, GRAD6_R, GRAD6_G, GRAD6_B);

    strip.setPixelColor(105, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(106, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(107, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(108, GRAD6_R, GRAD6_G, GRAD6_B);
    strip.setPixelColor(109, GRAD6_R, GRAD6_G, GRAD6_B);

    //Top line L12
    strip.setPixelColor(10, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(11, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(12, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(13, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(14, GRAD12_R, GRAD12_G, GRAD12_B);

    strip.setPixelColor(45, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(46, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(47, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(48, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(49, GRAD12_R, GRAD12_G, GRAD12_B);

    strip.setPixelColor(80, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(81, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(82, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(83, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(84, GRAD12_R, GRAD12_G, GRAD12_B);

    strip.setPixelColor(115, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(116, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(117, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(118, GRAD12_R, GRAD12_G, GRAD12_B);
    strip.setPixelColor(119, GRAD12_R, GRAD12_G, GRAD12_B);

    //Bottom line L0
    strip.setPixelColor(25, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(26, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(27, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(28, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(29, GRAD0_R, GRAD0_G, GRAD0_B);

    strip.setPixelColor(60, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(61, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(62, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(63, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(64, GRAD0_R, GRAD0_G, GRAD0_B);

    strip.setPixelColor(95, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(96, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(97, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(98, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(99, GRAD0_R, GRAD0_G, GRAD0_B);

    strip.setPixelColor(130, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(131, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(132, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(133, GRAD0_R, GRAD0_G, GRAD0_B);
    strip.setPixelColor(134, GRAD0_R, GRAD0_G, GRAD0_B);

    //L1
    strip.setPixelColor(24, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(30, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(59, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(65, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(94, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(100, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(129, GRAD1_R, GRAD1_G, GRAD1_B);
    strip.setPixelColor(135, GRAD1_R, GRAD1_G, GRAD1_B);

    //L2
    strip.setPixelColor(23, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(31, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(58, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(66, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(93, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(101, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(128, GRAD2_R, GRAD2_G, GRAD2_B);
    strip.setPixelColor(136, GRAD2_R, GRAD2_G, GRAD2_B);

    //L3
    strip.setPixelColor(22, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(32, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(57, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(67, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(92, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(102, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(127, GRAD3_R, GRAD3_G, GRAD3_B);
    strip.setPixelColor(137, GRAD3_R, GRAD3_G, GRAD3_B);

    //L4
    strip.setPixelColor(21, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(33, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(56, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(68, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(91, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(103, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(126, GRAD4_R, GRAD4_G, GRAD4_B);
    strip.setPixelColor(138, GRAD4_R, GRAD4_G, GRAD4_B);
    
    //L5
    strip.setPixelColor(20, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(34, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(55, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(69, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(90, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(104, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(125, GRAD5_R, GRAD5_G, GRAD5_B);
    strip.setPixelColor(139, GRAD5_R, GRAD5_G, GRAD5_B);
    
    //L7
    strip.setPixelColor(19, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(5, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(54, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(40, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(89, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(75, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(124, GRAD7_R, GRAD7_G, GRAD7_B);
    strip.setPixelColor(110, GRAD7_R, GRAD7_G, GRAD7_B);

    //L8
    strip.setPixelColor(18, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(6, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(53, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(41, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(88, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(76, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(123, GRAD8_R, GRAD8_G, GRAD8_B);
    strip.setPixelColor(111, GRAD8_R, GRAD8_G, GRAD8_B);

    //L9
    strip.setPixelColor(17, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(7, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(52, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(42, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(87, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(77, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(122, GRAD9_R, GRAD9_G, GRAD9_B);
    strip.setPixelColor(112, GRAD9_R, GRAD9_G, GRAD9_B);

    //L10
    strip.setPixelColor(16, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(8, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(51, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(43, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(86, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(78, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(121, GRAD10_R, GRAD10_G, GRAD10_B);
    strip.setPixelColor(113, GRAD10_R, GRAD10_G, GRAD10_B);

    //L11
    strip.setPixelColor(15, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(9, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(50, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(44, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(85, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(79, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(120, GRAD11_R, GRAD11_G, GRAD11_B);
    strip.setPixelColor(114, GRAD11_R, GRAD11_G, GRAD11_B);


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