# Arduino / OSOYOO

OSOYOOのArduino用Building Block DIY Programming Kitを使った、センサーやサーボモーターの個別動作から、赤外線リモコン操作やモーター制御を組み合わせたロボットカーまでを扱うArduinoサンプルプロジェクトです。C++スケッチとmBlockの作例を収録しています。

## 内容

- 超音波センサーとサーボモーターを使った制御
- 赤外線リモコンによる操作
- モーターの速度・方向制御
- Arduino向けのC++スケッチ
- mBlockで作成した動作例（`.mblock`）

## サンプル

各例のフォルダーに、その例の説明（`README.md`）、C++スケッチ、mBlockプロジェクト（ある場合）、Arduino CLIの設定をまとめています。

- [1-ultrasonic-servo](sketches/1-ultrasonic-servo/) — 超音波センサーとサーボモーター
- [2-swing](sketches/2-swing/) — 赤外線リモコンでサーボをスイング
- [3-car](sketches/3-car/) — mBlock生成コードを元にしたロボットカー
- [4-car-c](sketches/4-car-c/) — C++で整理したロボットカー制御

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
