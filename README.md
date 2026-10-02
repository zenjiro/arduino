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

## IRリモコンのキーマップ

`3-car`と`4-car-c`で使用するリモコンを実測したコマンド表です。値はIRremoteの`decodedIRData.command`を16進数で表しています。別のリモコンでは値が異なることがあるため、反応しない場合は受信コードを測定してスケッチのキーマップを更新してください。

| リモコンのボタン | コマンド |
| --- | --- |
| `1` | `0x45` |
| `2` | `0x46` |
| `3` | `0x47` |
| `4` | `0x44` |
| `5` | `0x40` |
| `6` | `0x43` |
| `7` | `0x07` |
| `8` | `0x15` |
| `9` | `0x09` |
| `*` | `0x16` |
| `0` | `0x19` |
| `#` | `0x0D` |
| `↑` | `0x18` |
| `←` | `0x08` |
| `OK` | `0x1C` |
| `→` | `0x5A` |
| `↓` | `0x52` |

## 参考

[OSOYOO Building Block DIY Programming Kit](https://osoyoo.com/ja/category/building-blocks/osoyoo-building-block-diy-programming-kit/)
