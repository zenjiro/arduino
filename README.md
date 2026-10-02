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

同梱リモコンのキーと、IRremoteの`decodedIRData.command`として受信するコマンド値です。値は16進数です。別のリモコンでは値が異なる場合があります。

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

ロボットカーのキー操作は次のとおりです。数字キーのリピートは無視されます。

| キー | モーター | ステアリング |
| --- | --- | --- |
| `1`、`2`、`3` | 最高速で前進 | 左（0°）、中央（90°）、右（180°） |
| `4`、`5`、`6` | 停止 | 左（0°）、中央（90°）、右（180°） |
| `7`、`8`、`9` | 最高速で後退 | 左（0°）、中央（90°）、右（180°） |

`5`は中央（90°）に戻して停止します。OKボタンは現在の走行状態を反転するトグルです。

障害物回避モードは`0`が標準の距離連動減速（20 cmで減速開始、5 cmで停止）、`*`が左回避（右後方へ1秒後退して左前方へ1秒前進）、`#`が右回避（左後方へ1秒後退して右前方へ1秒前進）です。回避後は元の走行状態に戻り、同じ障害物では20 cm以上離れるまで再度回避しません。モード切替音はありません。

## 参考

[OSOYOO Building Block DIY Programming Kit](https://osoyoo.com/ja/category/building-blocks/osoyoo-building-block-diy-programming-kit/)
