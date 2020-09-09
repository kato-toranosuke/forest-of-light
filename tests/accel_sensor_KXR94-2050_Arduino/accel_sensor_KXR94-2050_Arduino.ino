/*
 * https://www.petitmonte.com/robot/howto_kxr94_2050.html
 */

void setup() {
  Serial.begin(9600);
}
 
void loop() {
  // 地球の重力である1Gの加速度(m/s^2)
  float ms2 = 9.80665;
  
  // 電源電圧5V時のオフセット電圧(0G = 2.5V = 2500mV)
  float offset_voltage = 2500.0;
 
  // XYZの電圧(mV)を取得する
  float x =  (analogRead(A2) / 1024.0) * 5.0 * 1000;
  float y =  (analogRead(A3) / 1024.0) * 5.0 * 1000;
  float z =  (analogRead(A5) / 1024.0) * 5.0 * 1000;
 
  // XYZからオフセット電圧を引いた電圧を求める
  x = x - offset_voltage;
  y = y - offset_voltage;
  z = z - offset_voltage;
 
  // XYZから重力を求める
  float xg = x / 1000.0;
  float yg = y / 1000.0;
  float zg = z / 1000.0;
 
  // XYZの重力から加速度(m/s^2)を算出して出力する
  Serial.print("X : ");
  Serial.print(xg * ms2);
  Serial.print(" Y : ");
  Serial.print(yg * ms2);
  Serial.print(" Z : ");
  Serial.print(zg * ms2);
  Serial.println(" m/s^2");
 
  delay(100);
}
