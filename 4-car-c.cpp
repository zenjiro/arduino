#include <Arduino.h>
#include <Servo.h>
#include <IRremoteExt.h>  // mBlock拡張用の赤外線ライブラリ

// ==========================================
// ピン配置（定数定義）
// ==========================================
const int PIN_SERVO      = 2;   // サーボモーター
const int PIN_BUTTON     = 4;   // トグルボタン（スイッチ）
const int PIN_MOTOR_PWM  = 5;   // モーター速度制御（PWM出力）
const int PIN_TRIG       = 7;   // 超音波距離センサー (Trig)
const int PIN_ECHO       = 8;   // 超音波距離センサー (Echo)
const int PIN_IR_REC     = 10;  // 赤外線受信モジュール
const int PIN_MOTOR_IN1  = 11;  // モーター方向指定 1
const int PIN_MOTOR_IN2  = 12;  // モーター方向指定 2

// ==========================================
// グローバル変数
// ==========================================
Servo myServo;

bool isRunning   = false;  // 走行フラグ（true: 走行, false: 停止）
int maxSpeed     = 255;    // 最高速度設定（0 ~ 255）
int currentAngle = 90;     // サーボ角度（0 ~ 180）

// ==========================================
// 関数プロトタイプ宣言
// ==========================================
float getDistance(int trigPin, int echoPin);
char getIrKey();

// ==========================================
// セットアップ（初期化）
// ==========================================
void setup() {
  // ピンモードの設定
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_MOTOR_PWM, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  // モーターの進行方向設定（前進固定）
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);

  // サーボモーターの初期化
  myServo.attach(PIN_SERVO);
  myServo.write(currentAngle);  // 初期角度: 正面（90度）

  // 赤外線受信の初期化
  beginIRremote(PIN_IR_REC);
}

// ==========================================
// メインループ
// ==========================================
void loop() {
  // 1. センサー値・入力の取得
  float distance = getDistance(PIN_TRIG, PIN_ECHO);
  char irKey     = getIrKey();

  // 2. ボタン操作 または リモコン「OK」で走行/停止切り替え
  bool isButtonPressed = (digitalRead(PIN_BUTTON) == LOW);
  if (isButtonPressed || irKey == 'O') {
    isRunning = !isRunning;  // ON/OFFを反転（トグル）
    maxSpeed  = 255;         // 速度リセット
    delay(300);              // チャタリング防止用デレイ
  }

  // 3. リモコン入力による動作制御
  if (irKey != -1) {
    switch (irKey) {
      // --- 停止・スピードプリセット ---
      case 0:
        isRunning = false;
        break;
      case 1:
        isRunning = true;
        maxSpeed  = 96;
        break;
      case 2:
        isRunning = true;
        maxSpeed  = 192;
        break;
      case 3:
        isRunning = true;
        maxSpeed  = 255;
        break;

      // --- スピード微調整（▲/▼キー） ---
      case 'U':
        maxSpeed = constrain(maxSpeed + 16, 64, 255);
        break;
      case 'D':
        maxSpeed = constrain(maxSpeed - 16, 64, 255);
        break;

      // --- ステアリング微調整（◄/►キー） ---
      case 'L':
        currentAngle = constrain(currentAngle - 15, 0, 180);
        break;
      case 'R':
        currentAngle = constrain(currentAngle + 15, 0, 180);
        break;

      // --- ステアリング固定位置指定 ---
      case 4:
        currentAngle = 0;    // 左いっぱい
        break;
      case 5:
        currentAngle = 90;   // 正面
        break;
      case 6:
        currentAngle = 180;  // 右いっぱい
        break;

      default:
        break;
    }
  }

  // 4. 超音波センサーによる自動速度制御
  int targetSpeed = maxSpeed;
  if (distance < 20.0) {
    // 20cm未満のときは距離に応じて減速（5cm以下で速度0）
    targetSpeed = map(constrain(distance, 5, 20), 5, 20, 0, maxSpeed);
  }

  // 5. モーターとサーボへの出力
  if (isRunning) {
    analogWrite(PIN_MOTOR_PWM, targetSpeed);
    myServo.write(currentAngle);
  } else {
    analogWrite(PIN_MOTOR_PWM, 0);
    myServo.write(90);  // 停止時は正面に戻す
  }

  delay(100);  // ループ周期調整
}

// ==========================================
// 超音波距離計測関数 (cm単位で返却)
// ==========================================
float getDistance(int trigPin, int echoPin) {
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  // 30,000マイクロ秒（約5m分）をタイムアウトに設定
  unsigned long duration = pulseIn(echoPin, HIGH, 30000);
  return (float)duration / 58.0;
}

// ==========================================
// 赤外線リモコン読み取り関数
// ==========================================
char getIrKey() {
  handleIRremote();
  unsigned long rawValue = getPressedIRremote();

  if (rawValue != 0xFFFFFFFF) {
    switch (rawValue) {
      case 0xFF38C7: return 'O';  // OK
      case 0xFF6897: return '*';
      case 0xFFB04F: return '#';
      case 0xFF18E7: return 'U';  // Up
      case 0xFF4AB5: return 'D';  // Down
      case 0xFF10EF: return 'L';  // Left
      case 0xFF5AA5: return 'R';  // Right
      case 0xFFA25D: return 1;
      case 0xFF629D: return 2;
      case 0xFFE21D: return 3;
      case 0xFF22DD: return 4;
      case 0xFF02FD: return 5;
      case 0xFFC23D: return 6;
      case 0xFFE01F: return 7;
      case 0xFFA857: return 8;
      case 0xFF906f: return 9;
      case 0xFF9867: return 0;
      default: break;
    }
  }
  return -1;  // 押されていない場合
}