/*
 * *************************************************
 * SOHOSAI2019 @ TSUKUBA Univ.
 * The Forest of Lights
 * *************************************************
 */

/*
 * I2C通信によって加速度センサ(ADXL345)からデータを取得している。
 * -------------------------
 *  ADXL345 | ESP32-DevKitC
 * -------------------------
 *  VCC     | 3V3
 *  GND     | GND
 *  SCL     | GPIO22
 *  SDA     | GPIO21
 *  -------------------------
 *  
 *  (ref. https://qiita.com/Voltric01i/items/77dd63472820ab12c6dd)
 */
#include <Wire.h>
#include <Arduino.h>

// デバイスアドレス(スレーブ)
uint8_t DEVICE_ADDRESS = 0x53;  

// XYZレジスタ用のテーブル(6byte)
uint8_t RegTbl[6];  

void forward(uint32_t);
void reverse(uint32_t);
void getAccelerationData();

/* 使うピンの定義 */
const int IN1 = 25;
const int IN2 = 26;

/* チャンネルの定義 */
const int CHANNEL_0 = 0;
const int CHANNEL_1 = 1;

const int LEDC_TIMER_BIT = 8;   // PWMの範囲(8bitなら0〜255、10bitなら0〜1023)
const int LEDC_BASE_FREQ = 490; // 周波数(Hz)
const int VALUE_MAX = 255;      // PWMの最大値

void setup() {
  Serial.begin(9600); 

  // マスタとしてI2Cバスに接続する
  Wire.begin();       
  // DATA_FORMAT(データ形式の制御) 
  Wire.beginTransmission(DEVICE_ADDRESS);  
    // DATA_FORMATのアドレス    
  Wire.write(0x31);
    // 「最大分解能モード」 及び 「±16g」 (0x0B == 1011)
  Wire.write(0x0B);  
    // 「10bit固定分解能モード」 及び　「±16g」にする場合 (0x03 == 0011)
    // Wire.write(0x03);
  Wire.endTransmission();
  // POWER_TCL(節電機能の制御) 
  Wire.beginTransmission(DEVICE_ADDRESS);  
    // POWER_CTLのアドレス    
  Wire.write(0x2d);
    // 測定モードにする
  Wire.write(0x08);  
  Wire.endTransmission();

//  pinMode(IN1, OUTPUT); // IN1
//  pinMode(IN2, OUTPUT); // IN2

  // ピンのセットアップ
//  ledcSetup(CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
//  ledcSetup(CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

  // ピンのチャンネルをセット
//  ledcAttachPin(IN1, CHANNEL_0);
//  ledcAttachPin(IN2, CHANNEL_1);
}

void loop() {

  getAccelerationData();

  // データを各XYZの値に変換する(LSB単位)
  int16_t x = (((int16_t)RegTbl[1]) << 8) | RegTbl[0];
  int16_t y = (((int16_t)RegTbl[3]) << 8) | RegTbl[2];
  int16_t z = (((int16_t)RegTbl[5]) << 8) | RegTbl[4];  


  // 各XYZ軸の加速度(m/s^2)を出力する
  Serial.print("X : ");
  Serial.print( x * 0.0392266 );
  Serial.print(" Y : ");
  Serial.print( y * 0.0392266 );
  Serial.print(" Z : ");
  Serial.print( z * 0.0392266 );
  Serial.println(" m/s^2");
  
  delay(1000);
}

//加速度データ取得
void getAccelerationData(){
  // XYZの先頭アドレスに移動する
  Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(0x32);
  Wire.endTransmission();

  // デバイスへ6byteのレジスタデータを要求する
  Wire.requestFrom(DEVICE_ADDRESS, 6);

  // 6byteのデータを取得する
  int i;
  for (i=0; i< 6; i++){
    while (Wire.available() == 0 ){}
    RegTbl[i] = Wire.read();
  }

}
