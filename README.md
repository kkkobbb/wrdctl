# wrdctl

WRD-130-UIを制御する

## 依存パッケージ
* `sudo apt install libusb-1.0 pkg-config`

## ビルド
* `make`

## 使い方
* `sudo ./wrdctl`
    * 接続、切断のみ行う。エラーがあればメッセージを表示する

## 備考
* usbにアクセスするため、実行には管理者権限が必要

