# UMT32PI.X

mt32-pi および GS MIDI音源の基本的な制御を行うユーティリティ for X680x0/Human68k

---

## Install

UMT320x0.ZI をダウンロードして、UMT32PI.X をパスの通ったところに置きます。

---

## Usage

前提として、RCD.X または ZMUSIC.X のMIDI音源ドライバが常駐していることが必要になります。
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

---

## 注意点

再生中に実行した場合は想定外の音が出る可能性があります。

当方X680x0実機用のMIDIボードを所有していないので、MIDIボードモードでの動作については未確認です。
動作した場合はお知らせ頂けると助かります。
