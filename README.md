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

Windowsでは、wingetでArduino CLIをインストールできます。

```powershell
winget install --id ArduinoSA.CLI --exact
```

赤外線受信を行うスケッチでは、Arduinoの一般的な`IRremote`ライブラリを使います。mBlockプロジェクトはこのライブラリを使いません。

各スケッチフォルダーの`sketch.yaml`にArduino CLI公式のビルドプロファイルを置き、ボードコアとライブラリのバージョンを管理します。ライブラリ本体はリポジトリに含めず、CLIが不足分を隔離キャッシュに取得します。

```powershell
arduino-cli compile sketches/1-ultrasonic-servo
arduino-cli compile sketches/2-swing
arduino-cli compile sketches/3-car
arduino-cli compile sketches/4-car-c
```

接続したボードとポートは次のコマンドで確認できます。

```powershell
arduino-cli board list
```

Arduino Unoが`COM3`として表示された場合は、次のように各スケッチを転送します。`COM3`は実際に表示されたポート名に置き換えてください。

```powershell
arduino-cli upload --profile default -p COM3 sketches/1-ultrasonic-servo
arduino-cli upload --profile default -p COM3 sketches/2-swing
arduino-cli upload --profile default -p COM3 sketches/3-car
arduino-cli upload --profile default -p COM3 sketches/4-car-c
```

## IRリモコンのキーマップ

同梱リモコンのキーと、IRremoteの`decodedIRData.command`として受信するコマンド値です。値は16進数です。別のリモコンでは値が異なる場合があります。

| リモコンのボタン | コマンド |
| --- | --- |
| `0` | `0x19` |
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
| `#` | `0x0D` |
| `OK` | `0x1C` |
| `←` | `0x08` |
| `↑` | `0x18` |
| `↓` | `0x52` |
| `→` | `0x5A` |

## 参考

[OSOYOO Building Block DIY Programming Kit](https://osoyoo.com/ja/category/building-blocks/osoyoo-building-block-diy-programming-kit/)
