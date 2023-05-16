# UMT32PI.X

mt32-pi および GS MIDI音源の基本的な制御を行うユーティリティ for X680x0/Human68k

---

## Install

UMT320x0.ZIP をダウンロードして展開し、UMT32PI.X をパスの通ったところに置きます。

---

## Usage

前提として、RCD.X v3 または ZMUSIC.X v2 のMIDI音源ドライバが常駐していることが必要になります。
いずれも常駐していない場合はエラーとなり起動できません。

常駐しているのがRCD.XかZMUSIC.Xか、及びMIDIボードモードかRS-MIDIモードかは自動的に判別します。

以下の書式で起動します。

    umt32pi <command> [parameter...]

現在サポートしているコマンドは以下です。

    volume <vol>   ... [GS] master volume (0-127)
    gsreset        ... [GS] reset
    reverb <type>  ... [GS] reverb type (0-7, default:4)
    chorus <type>  ... [GS] chorus type (0-7, default:2)
    print <str>    ... [GS] print string (max 32chars)
    sfont <index>  ... [mt32-pi] sound font (0-127)
    reboot         ... [mt32-pi] reboot Raspberry Pi

再生中に実行した場合は音源ドライバを一時停止し、システムエクスクルーシブを送った後に自動的に再開します。

実行例: マスターボリュームの変更

    umt32pi volume 100

実行例: リバーブタイプの変更

    umt32pi reverb 0

実行例: 文字列表示

    umt32pi print X68030

---

## 注意点

再生中に実行した場合は想定外の音が出る可能性があります。

RCD v2 及び ZMUSIC v3 は環境が無いため未確認・未対応です。RCD v3 と ZMUSIC v2 のみ確認しています。

当方X680x0実機用のMIDIボードを所有していないので、MIDIボードモードでの動作については未確認です。
動作した場合はお知らせ頂けると助かります。なお、MIDIボードは1枚目のみを想定しています。

---

## 変更履歴

* 0.2.0 (2023/05/17) ... 初版 (Cで書き直して音源ドライバ制御を追加)
* 0.1.0 (2023/05/16) ... プロトタイプ (MicroPython-x68k版)