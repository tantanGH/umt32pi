# UMT32PI.X

mt32-pi および GS MIDI音源の基本的な制御を行うユーティリティ for X680x0/Human68k

---

## Install

UMT32xxx.ZIP をダウンロードして展開し、UMT32PI.X をパスの通ったところに置きます。

---

## Usage

前提として、RCD.X v3 または ZMUSIC.X v2 のMIDI音源ドライバが常駐していることが必要になります。
常駐しているのがRCD.XかZMUSIC.Xか、及びMIDIボードモードかRS-MIDIモードかは自動的に判別します。

いずれも常駐していない場合はエラーとなり起動できません。


以下の書式で起動します。

    umt32pi <command> [parameter...]

現在サポートしているコマンドは以下です。

    mtvol <vol>    ... [mt32-pi] master volume (0-100)
    synth <type>   ... [mt32-pi] 0:MT-32 1:SoundFont
    sfont <index>  ... [mt32-pi] sound font (0-127)
    reboot         ... [mt32-pi] reboot Raspberry Pi
    mute <mode>    ... [mt32-pi-ex] mute mode(0) or play mode (1) 
    gsvol <vol>    ... [GS] master volume (0-127)
    gsreset        ... [GS] reset
    reverb <type>  ... [GS] reverb type (0-7, default:4)
    chorus <type>  ... [GS] chorus type (0-7, default:2)
    print <str>    ... [GS] print string (max 32chars)

再生中に実行した場合は音源ドライバを制御して再生を一時停止し、システムエクスクルーシブを送った後に自動的に再開します。

実行例: mt32-pi (MT32モードのみ) マスターボリュームの変更(上限100)

    umt32pi mtvol 80

実行例: mt32-pi MT32モード(0)/SoundFontモード(1) 切り替え

    umt32pi synth 0
    umt32pi synth 1

実行例: mt32-pi (SoundFontモード) サウンドフォント切り替え

    umt32pi sfont 1

実行例: mt32-pi (MT32モード/SoundFontモード) ミュート ON/OFF (GIMONS氏による改造mt32-piカーネルのみ対応)

    umt32pi mute 0
    umt32pi mute 1

実行例: GSマスターボリュームの変更(上限127, mt32-pi SoundFontモードは GIMONS氏による改造mt32-piカーネルのみ対応)

    umt32pi gsvol 100

実行例: GSリバーブタイプの変更

    umt32pi reverb 0

実行例: GS文字列表示

    umt32pi print X68030

---

## 注意点

再生中に実行した場合は想定外の音が出る可能性があります。

RCD v2 及び ZMUSIC v3 は環境が無いため未確認・未対応です。RCD v3 と ZMUSIC v2 のみ確認しています。

---

## 謝辞

RCDドライバの制御については、MMDSPのソースを参考にさせて頂きました。この場を借りてお礼申し上げます。

https://github.com/gaolay/MMDSP

---

## 変更履歴

* 0.4.0 (2025/02/15) ... mute 追加 及び GIMONS氏の改造mt32-piカーネルのsysexに対応
* 0.3.2 (2025/02/11) ... gsvol の sysex をGM共通ではなくGS固有のものに変更
* 0.3.1 (2025/02/11) ... RCDタイプ判定部分のデバッグ
* 0.3.0 (2025/02/11) ... mt32-pi のマスターボリューム変更に対応
* 0.2.1 (2023/05/17) ... mt32-pi の対応エクスクルーシブ追加
* 0.2.0 (2023/05/17) ... 初版 (Cで書き直して音源ドライバ制御を追加)
* 0.1.0 (2023/05/16) ... プロトタイプ (MicroPython-x68k版)