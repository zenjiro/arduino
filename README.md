# Arduino / OSOYOO

OSOYOOのArduino用Building Block DIY Programming Kitを使った、センサーやサーボモーターの個別動作から、赤外線リモコン操作やモーター制御を組み合わせたロボットカーまでを扱うArduinoサンプルプロジェクトです。C++スケッチとmBlockの作例を収録しています。

## 内容

- 超音波センサーとサーボモーターを使った制御
- 赤外線リモコンによる操作
- モーターの速度・方向制御
- Arduino向けのC++スケッチ
- mBlockで作成した動作例（`.mblock`）

## サンプル

### mBlockプロジェクト

- `1-ultrasonic-servo.mblock` — 超音波センサーとサーボモーターの例
- `2-swing.mblock` — スイング動作の例
- `3-car.mblock` — ロボットカーの制御例
- `4-car-c.mblock` — ロボットカーの制御例

### C++スケッチ

- `sketches/1-ultrasonic-servo/1-ultrasonic-servo.ino` — 超音波センサーとサーボモーターのC++スケッチ
- `sketches/2-swing/2-swing.ino` — 標準IRremoteライブラリを使うスイング動作のC++スケッチ
- `sketches/3-car/3-car.ino` — 標準IRremoteライブラリを使うロボットカーのC++スケッチ
- `sketches/4-car-c/4-car-c.ino` — 標準IRremoteライブラリを使うロボットカーのC++スケッチ

## Arduino CLI

赤外線受信を行うC++スケッチ（`2-swing`、`3-car`、`4-car-c`）では、Arduinoの一般的な`IRremote`ライブラリを使います。mBlockプロジェクトはこのライブラリを使いません。

各スケッチフォルダーの`sketch.yaml`にArduino CLI公式のビルドプロファイルを置き、ボードコアとライブラリのバージョンを管理します。ライブラリ本体はリポジトリに含めず、CLIが不足分を隔離キャッシュに取得します。

```powershell
arduino-cli compile sketches/1-ultrasonic-servo
arduino-cli compile sketches/2-swing
arduino-cli compile sketches/3-car
arduino-cli compile sketches/4-car-c
```

`2-swing`は受信ピン7、`3-car`と`4-car-c`は受信ピン10を使います。

## 参考

[OSOYOO Building Block DIY Programming Kit](https://osoyoo.com/ja/category/building-blocks/osoyoo-building-block-diy-programming-kit/)
