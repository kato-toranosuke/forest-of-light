/*
   Title of work: "The Forest of Light"
   Venue: SOHO-SAI @ University of Tsukuba
   Date: 2019/11/03,04
   Author of program: Kato Toranosuke
*/
/*
 * https://www.petitmonte.com/robot/howto_kxr94_2050.html
 * KXR94-2050
 */

// "#define x 10" とすることで"定数"を定義できます。この例では整数の10が格納された"x"という定数を定義しました。
// Red, Green, Blueの各LEDに対して制御信号をOutputするPin番号を定義
#define R_PIN 10
#define G_PIN 6
#define B_PIN 3

// X, Y, Z軸の加速度を取得するPin番号を定義
#define X_ACCL A2
#define Y_ACCL A3
#define Z_ACCL A5

//#define MAX_ACCL 4.00
//#define MIN_ACCL -4.0

// 地球の重力である1Gの加速度(m/s^2)を定義
const float ms2 = 9.80665;
// 電源電圧5V時のオフセット電圧(0G = 2.5V = 2500mV)
const float offset_voltage = 2500.0;

// 加速度を格納する変数
float accl_x, accl_y, accl_z;

// setup関数は(Arduinoの電源が入ってから)最初に一回だけ実行される
void setup() {
  pinMode(R_PIN, OUTPUT); // R_PINをOutput Pin(=出力ピン)に指定。以下同様。
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  Serial.begin(9600); // Serial通信のレートを指定。今回は9600bps(bit per second)。ここはどうでもいい所。気になったら調べて。
}

// loop関数は電源が入っている間、繰り返し実行される
void loop() {
  // X,Y,Z加速度を取得
  get_accl();

  // 取得した加速度をSerialモニタに出力。
  Serial.print("X: ");
  Serial.print(accl_x);
  Serial.print(", Y: ");
  Serial.print(accl_y);
  Serial.print(", Z: ");
  Serial.println(accl_z);

  // LEDを制御するPinに制御信号(明るさの値)を出力
  // 第2引数と第3引数の値を変えることで、色の変化の具合を調整できます。これは実際にやってみながら調整する必要がある。
  analogWrite(R_PIN, accl2duty(accl_x, -7, -10));
  analogWrite(G_PIN, accl2duty(accl_y, -5, -8));
  analogWrite(B_PIN, accl2duty(accl_z, 2, 6));

  // 次のループに入るまで100ms待機する
  delay(100);
}

// 加速度センサからX,Y,Z軸の加速度を取得し、グローバル変数に格納する関数
void get_accl() {
  // XYZの電圧(mV)を取得する
  float x =  (analogRead(X_ACCL) / 1024.0) * 5.0 * 1000;
  float y =  (analogRead(Y_ACCL) / 1024.0) * 5.0 * 1000;
  float z =  (analogRead(Z_ACCL) / 1024.0) * 5.0 * 1000;

  // XYZからオフセット電圧を引いた電圧を求める
  x = x - offset_voltage;
  y = y - offset_voltage;
  z = z - offset_voltage;

  // XYZから重力を求める
  float xg = x / 1000.0;
  float yg = y / 1000.0;
  float zg = z / 1000.0;

  // XYZの重力から加速度(m/s^2)を算出 & 変数に格納
  accl_x = xg * ms2;
  accl_y = yg * ms2;
  accl_z = zg * ms2;

  return;
}

// 取得した加速度の値を基に、LEDの明るさを制御するための信号(値)を返す関数
int accl2duty(float accl, float MAX_ACCL, float MIN_ACCL) {
  int duty = 0;

  // 取得した加速度の値を一定の範囲(MIN_ACCL ~ MAX_ACCL)の値に収める
  if (accl > MAX_ACCL)
    accl = MAX_ACCL;
  else if (accl < MIN_ACCL)
    accl = MIN_ACCL;

  // duty比(0~255)を計算
  duty = 255 * (accl - MIN_ACCL) / (MAX_ACCL - MIN_ACCL);

  return duty;
}
