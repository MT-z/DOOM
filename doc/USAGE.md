# DOOM macOS ポート - 使用方法

## 前提条件

- macOS 11.0 以上
- Apple Silicon (arm64) またはIntel Mac
- Homebrew がインストール済み
- SDL2 ライブラリ

詳細は [MACOS_PORT.md](MACOS_PORT.md) のセットアップセクションを参照してください。

## ビルド

### ビルドの実行

```bash
cd /path/to/DOOM
make -f Makefile.macos
```

### 出力ファイル

```bash
macos/doom  # コンパイル済みアプリケーション（arm64 Mach-O）
```

### ビルド成果物の削除

```bash
make -f Makefile.macos clean
```

## 実行

### 基本的な実行方法

```bash
./macos/doom -iwad /path/to/DOOM.WAD
```

### ゲームデータの入手

DOOM の WAD ファイルはいくつかの方法で入手できます：

1. **Steam** - 最も簡単な方法
   - Steam で DOOM を購入
   - 通常は `~/Library/Application Support/Steam/steamapps/common/DOOM/` に保存

2. **GOG** - DRM フリー版
   - https://www.gog.com/ で DOOM を購入
   - インストーラーからファイルを抽出

3. **公式フリー版** - shareware 版
   - DOOM1.WAD (小サイズ、いくつかのレベルのみ)

### コマンドラインオプション

#### ディスプレイスケーリング

```bash
# 1x スケール（320x200）
./macos/doom -iwad DOOM.WAD

# 2x スケール（640x400）
./macos/doom -iwad DOOM.WAD -2x

# 3x スケール（960x600）
./macos/doom -iwad DOOM.WAD -3x
```

#### フルスクリーンモード

```bash
./macos/doom -iwad DOOM.WAD -fullscreen

# スケーリング込み
./macos/doom -iwad DOOM.WAD -fullscreen -2x
```

#### PWAD ファイルの読み込み

```bash
./macos/doom -iwad DOOM.WAD -file custom_map.wad
```

#### デバッグモード

```bash
./macos/doom -iwad DOOM.WAD -devparm
```

#### 特定レベルの開始

```bash
# E1M1 から開始
./macos/doom -iwad DOOM.WAD -warp 1 1

# E3M4 から開始
./macos/doom -iwad DOOM.WAD -warp 3 4
```

#### スキルレベルの指定

```bash
# 0=IDDQD, 1=Too Young to Die, 2=Hurt Me Plenty, 3=Ultra-Violence, 4=Nightmare
./macos/doom -iwad DOOM.WAD -skill 3
```

#### マルチプレイネットワーク（実験的）

```bash
# ホストサーバー起動
./macos/doom -iwad DOOM.WAD -server 4 10000

# クライアント接続
./macos/doom -iwad DOOM.WAD -connect hostname 10000
```

## ゲーム内キーバインド

### 移動

| キー | 動作 |
|------|------|
| W/↑ | 前進 |
| S/↓ | 後退 |
| A | 左移動（ストレイフ） |
| D | 右移動（ストレイフ） |
| ←/→ | 左右に旋回 |
| ,/. | 左/右移動（ストレイフ） |
| Shift | 走る（ダッシュ） |
| Alt + ←/→ | ストレイフ |
| Space | 開ける/使う（ドア・スイッチ） |

> **注意:** オリジナル DOOM (1993) にはジャンプ・しゃがみ・上下視点はありません。これらは後年のソースポート（ZDoom 等）で追加された機能です。

### 武器

| キー | 動作 |
|------|------|
| 1 | 拳（チェーンソー所持時はチェーンソー） |
| 2 | ピストル |
| 3 | ショットガン |
| 4 | チェインガン |
| 5 | ロケットランチャー |
| 6 | プラズマライフル |
| 7 | BFG9000 |
| Ctrl | 射撃 |

### ゲーム制御

| キー | 動作 |
|------|------|
| Esc | メニュー表示 |
| Enter | メニュー選択 |
| Pause | ゲーム一時停止 |
| F1 | ヘルプ |
| F2 | セーブ |
| F3 | ロード |
| F4 | サウンド音量 |
| F5 | ディテール切り替え |
| F6 | クイックセーブ |
| F7 | ゲーム終了 |
| F8 | メッセージ表示切り替え |
| F9 | クイックロード |
| F10 | 終了 |
| F11 | ガンマ補正 |
| +/- | 画面サイズ変更 |

### オートマップ（Tab で切り替え）

| キー | 動作 |
|------|------|
| Tab | マップ表示/終了 |
| ←↑↓→ | マップスクロール |
| =/- | ズームイン/アウト |
| 0 | 全体表示切り替え |
| F | フォローモード切り替え |
| G | グリッド表示切り替え |
| M | マーク追加 |
| C | マーク消去 |

### チート

ゲームプレイ中にそのままタイプします：

| チート | 効果 |
|-------|------|
| IDDQD | 神モード（無敵） |
| IDKFA | 全武器・全弾薬・全キー |
| IDFA | 全武器・全弾薬（キーなし） |
| IDSPISPOPD | 壁抜け（noclip） |
| IDCLEV##  | レベル移動（例: IDCLEV14 = E1M4） |
| IDMUS## | BGM 変更 |
| IDBEHOLDx | パワーアップ（x = V/S/I/R/A/L） |
| IDCHOPPERS | チェーンソー入手 |
| IDMYPOS | 現在座標表示 |

## マウス入力

- **左クリック** - 射撃
- **右クリック** - ストレイフ修飾（押しながら移動で平行移動）
- **中クリック** - 前進
- **マウス左右移動** - 旋回
- **マウス前後移動** - 前進/後退

> ボタン割り当ては `~/.doomrc` の `mouseb_fire` / `mouseb_strafe` / `mouseb_forward` で変更できます。

## トラブルシューティング

### ビルド失敗

**エラー:** `clang: error: -lSDL2: linker command failed`

**解決:** SDL2 をインストール

```bash
brew install sdl2
```

### ウィンドウが表示されない

**エラー:** 黒いウィンドウ、クラッシュ

**原因:** SDL2 の初期化エラー

**解決:** ターミナルで実行し、エラーメッセージを確認

```bash
./macos/doom -iwad DOOM.WAD 2>&1 | head -20
```

### WAD ファイルが見つからない

**エラー:** `Can't find DOOM.WAD`

**解決:** WAD ファイルの完全パスを指定

```bash
./macos/doom -iwad ~/Games/DOOM/DOOM.WAD
```

### サウンド出力がない

**確認:** オーディオデバイスの接続状態を確認

```bash
# システムボリューム確認
osascript -e "output volume of (get volume settings)"
```

### オーディオ デバイスの選択

オーディオ出力デバイスは macOS システム環境設定から変更できます：

1. **システム設定** → **サウンド**
2. **出力**タブで希望するデバイスを選択

## パフォーマンス最適化

### フレームレート制限

標準的な実行で 35 FPS 制限が有効です（DOOM オリジナルの仕様）。

### 高解像度でのパフォーマンス

Apple Silicon Mac は高スケーリングで最適なパフォーマンスを発揮：

```bash
# Apple Silicon では推奨
./macos/doom -iwad DOOM.WAD -3x
```

Intel Mac：

```bash
# Intel では -2x 推奨
./macos/doom -iwad DOOM.WAD -2x
```

## 今後の拡張機能

- 🎵 MIDI/デジタルミュージックサポート
- 🎮 ゲームパッド/コントローラーサポート
- 🖼️ OpenGL/Metal レンダリング
- 📱 ネットワークマルチプレイの完全実装
- 💾 savegame の改善

## ライセンス

このmacOS ポートは GNU General Public License v2 の下で公開されています。

詳細は [MACOS_PORT.md](MACOS_PORT.md) を参照してください。
