/*LEDCの設定*/
#define LEDC_CHANNEL_R 0  // 0~15 channel
#define LEDC_CHANNEL_G 2
#define LEDC_CHANNEL_B 4
#define LEDC_TIMER_BIT 8 // PWMの範囲(0~255)。dutyビット長とも言う。
#define LEDC_BASE_FREQ 256000 // 周波数
#define VALUE_MAX 255

/*ピンの定義*/
const int PIN_R = 25;
const int PIN_G = 26;
const int PIN_B = 27;

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // ledc使用の為のピンのセットアップ
  ledcSetup(LEDC_CHANNEL_R, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(LEDC_CHANNEL_G, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(LEDC_CHANNEL_B, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

  // ピンのチャンネルをセット
  ledcAttachPin(PIN_R, LEDC_CHANNEL_R);
  ledcAttachPin(PIN_G, LEDC_CHANNEL_G);
  ledcAttachPin(PIN_B, LEDC_CHANNEL_B);
}

void loop() {
  static uint8_t brightness = 0;
  int diff = 1;

  ledcWrite(LEDC_CHANNEL_R, brightness);
  ledcWrite(LEDC_CHANNEL_G, brightness);
  ledcWrite(LEDC_CHANNEL_B, brightness);

  if (brightness == 0)
  {
    diff = 1;
  }else if (brightness == 255)
  {
    diff = -1;
  }

  brightness += diff;
  
  delay(10);
}
