# Arduino / OSOYOO

OSOYOOのArduino用Building Block DIY Programming Kitを使って、センサーやモーターを組み合わせたロボットカーをC++で制御するためのプロジェクトです。

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

- `1-ultrasonic-servo.cpp` — 超音波センサーとサーボモーターの例
- `2-swing.cpp` — 標準IRremoteライブラリを使うスイング動作例
- `3-car.cpp` — 標準IRremoteライブラリを使うロボットカー制御例
- `4-car-c.cpp` — 標準IRremoteライブラリを使うロボットカー制御例

## Arduino CLI

赤外線受信を行うC++スケッチ（`2-swing.cpp`、`3-car.cpp`、`4-car-c.cpp`）では、Arduinoの一般的な`IRremote`ライブラリを使います。mBlockプロジェクトはこのライブラリを使いません。C++スケッチの依存ライブラリは次のコマンドでインストールできます。

```powershell
Get-Content arduino-libraries.txt | ForEach-Object { arduino-cli lib install $_ }
```

依存ライブラリ名とバージョンは[`arduino-libraries.txt`](arduino-libraries.txt)に記録しています。ライブラリ本体はリポジトリに含めず、Arduino CLIがユーザー領域にインストールします。

`2-swing.cpp`は受信ピン7、`3-car.cpp`と`4-car-c.cpp`は受信ピン10を使います。リモコンのボタンコードは`getKey()` / `getIrKey()`内で割り当てています。

## 参考

[OSOYOO Building Block DIY Programming Kit](https://osoyoo.com/ja/category/building-blocks/osoyoo-building-block-diy-programming-kit/)
