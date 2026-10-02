#include <Arduino.h>
#include <Servo.h>
#include <IRremote.hpp>

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
bool isReversing = false;  // 後退フラグ
int maxSpeed     = 255;    // 最高速度設定（0 ~ 255）
int currentAngle = 90;     // サーボ角度（0 ~ 180）

// ==========================================
// 関数プロトタイプ宣言
// ==========================================
float getDistance(int trigPin, int echoPin);
char getIrKey(bool &isIrRepeat);

// ==========================================
// セットアップ（初期化）
// ==========================================
void setup() {
  // ピンモードの設定
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_MOTOR_PWM, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  // モーターの初期方向設定（前進）
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);

  // サーボモーターの初期化
  myServo.attach(PIN_SERVO);

  // 赤外線受信の初期化
  IrReceiver.begin(PIN_IR_REC, DISABLE_LED_FEEDBACK);
}

// ==========================================
// メインループ
// ==========================================
void loop() {
  // 1. センサー値・入力の取得
  float distance = getDistance(PIN_TRIG, PIN_ECHO);
  bool isIrRepeat = false;
  char irKey = getIrKey(isIrRepeat);

  // 2. ボタン操作 または リモコン「OK」で走行/停止切り替え
  bool isButtonPressed = (digitalRead(PIN_BUTTON) == LOW);
  if (isButtonPressed || irKey == 'O') {
    isRunning = !isRunning;  // ON/OFFを反転（トグル）
    delay(300);              // チャタリング防止用デレイ
  }

  // 3. リモコン入力による動作制御
  if (irKey != -1) {
    switch (irKey) {
      // --- 数字キー: 走行方向・停止とステアリング位置 ---
      case 0:
        isRunning = false;
        break;
      case 1:
        isRunning = true;
        isReversing = false;
        maxSpeed = 255;
        currentAngle = 0;
        break;
      case 2:
        isRunning = true;
        isReversing = false;
        maxSpeed = 255;
        currentAngle = 90;
        break;
      case 3:
        isRunning = true;
        isReversing = false;
        maxSpeed  = 255;
        currentAngle = 180;
        break;
      case 4:
        isRunning = false;
        currentAngle = 0;
        break;
      case 5:
        isRunning = false;
        currentAngle = 90;
        break;
      case 6:
        isRunning = false;
        currentAngle = 180;
        break;
      case 7:
        isRunning = true;
        isReversing = true;
        maxSpeed = 255;
        currentAngle = 0;
        break;
      case 8:
        isRunning = true;
        isReversing = true;
        maxSpeed = 255;
        currentAngle = 90;
        break;
      case 9:
        isRunning = true;
        isReversing = true;
        maxSpeed = 255;
        currentAngle = 180;
        break;

      // --- スピード微調整（▲/▼キー） ---
      case 'U':
        if (!isRunning) {
          if (!isIrRepeat) {
            maxSpeed = 64;
            isReversing = false;
            isRunning = true;
          }
        } else if (isReversing && maxSpeed <= 64) {
          isRunning = false;
          isReversing = false;
        } else if (isReversing) {
          maxSpeed = constrain(maxSpeed - 16, 64, 255);
        } else {
          maxSpeed = constrain(maxSpeed + 16, 64, 255);
        }
        break;
      case 'D':
        if (!isRunning) {
          if (!isIrRepeat) {
            maxSpeed = 64;
            isReversing = true;
            isRunning = true;
          }
        } else if (isReversing) {
          maxSpeed = constrain(maxSpeed + 16, 64, 255);
        } else if (maxSpeed <= 64) {
          isRunning = false;
        } else {
          maxSpeed = constrain(maxSpeed - 16, 64, 255);
        }
        break;

      // --- ステアリング微調整（◄/►キー） ---
      case 'L':
        currentAngle = constrain(currentAngle - 15, 0, 180);
        break;
      case 'R':
        currentAngle = constrain(currentAngle + 15, 0, 180);
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
  digitalWrite(PIN_MOTOR_IN1, isReversing ? HIGH : LOW);
  digitalWrite(PIN_MOTOR_IN2, isReversing ? LOW : HIGH);
  myServo.write(currentAngle);
  if (isRunning) {
    analogWrite(PIN_MOTOR_PWM, targetSpeed);
  } else {
    analogWrite(PIN_MOTOR_PWM, 0);
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
char getIrKey(bool &isIrRepeat) {
  isIrRepeat = false;
  if (!IrReceiver.decode()) return -1;

  isIrRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
  const uint16_t command = IrReceiver.decodedIRData.command;
  IrReceiver.resume();
  if (isIrRepeat && command != 0x18 && command != 0x52 &&
      command != 0x08 && command != 0x5A) return -1;

  switch (command) {
    case 0x1C: return 'O';
    case 0x16: return '*';
    case 0x0D: return '#';
    case 0x18: return 'U';
    case 0x52: return 'D';
    case 0x08: return 'L';
    case 0x5A: return 'R';
    case 0x45: return 1;
    case 0x46: return 2;
    case 0x47: return 3;
    case 0x44: return 4;
    case 0x40: return 5;
    case 0x43: return 6;
    case 0x07: return 7;
    case 0x15: return 8;
    case 0x09: return 9;
    case 0x19: return 0;
    default: return -1;
  }
}
