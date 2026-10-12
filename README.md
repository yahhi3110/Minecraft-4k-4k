# Minecraft 4K — フルHD・4KBソース版

Notch の Minecraft 4k（2010年 Java 4k Contest）を、GPU のフラグメントシェーダーによる
ボクセル・レイマーチングで再現した Windows 向けリマスターです。
**C++ とシェーダーの合計ソースが 4096 バイト以下**（`mc4k.cpp` 1ファイル）という縛りを守っています。

## 特徴

- 解像度は **1920x1080 固定**（ボーダーレスの 1920x1080 ウィンドウ）
- 地形は GPU 側の数式で生成します（無限に続く地形、ワールドデータ・テクスチャ画像は不要）
- 草・砂・土・石、ブロックごとのドット模様
- ライティング: 面ごとの陰影、太陽方向への影、フォグ、空、太陽の昼夜サイクル（約125秒で1周）
- 依存は Windows 標準の `opengl32` / `gdi32` / `user32` のみ（SDL・glad 不要）

## 操作方法

| 操作 | キー |
|---|---|
| 前後左右 | W / A / S / D |
| 上昇 / 下降 | Space / Ctrl |
| 視点 | マウス |
| 終了 | Esc |

4KB に収めるため、元作品にあった重力・衝突判定・ブロックの破壊と設置は省いた**飛行カメラ**です。

## ビルド済み exe

`bin/mc4k.exe`（x64、約19KB）を同梱しています。MinGW-w64 でクラウド上からクロスビルドしたもので、クラウド上では Wine + Mesa（ソフトウェア GL）で起動して地形が表示されることまで確認しましたが、**Windows 実機での起動は未確認**です。
実機で動かない場合は、下記の手順で自分のPCでビルドし直してください。
再ビルドのコマンドは `x86_64-w64-mingw32-g++ -Os -s -static -mwindows mc4k.cpp -o bin/mc4k.exe -lopengl32 -lgdi32 -luser32` です。

## ビルド（Windows）

OpenGL 4.3 以上に対応した GPU とドライバーが必要です（RTX 2050 は対応）。

### Visual Studio（CMake）

1. Visual Studio の「C++ によるデスクトップ開発」と CMake を入れる
2. 「x64 Native Tools Command Prompt for VS」で以下を実行
   - 構成: `cmake -S . -B build -A x64`
   - ビルド: `cmake --build build --config Release`
3. `build\Release\mc4k.exe` を実行

### MinGW-w64（MSYS2 など）

- 直接ビルド: `g++ -O2 -mwindows mc4k.cpp -o mc4k.exe -lopengl32 -lgdi32 -luser32`

## 4KB の確認

- Linux / WSL / Git Bash: `tools/check_size.sh`（`mc4k.cpp` のバイト数を表示）
- PowerShell: `(Get-Item mc4k.cpp).Length`

現在は 2917 バイトです。

## 実機で確認する項目（やっひーさんのWindows PC）

クラウド環境には GPU も Windows もないため、以下は実機での確認が必要です。

1. `mc4k.exe` を起動して、画面いっぱいに地形と空が表示される
2. マウスで視点が回り、WASD / Space / Ctrl で動く（Esc で終了できる）
3. フレームレート（目安: RTX 2050 で 1920x1080 が 30fps 以上）。重い場合は `mc4k.cpp` のシェーダー内
   `L<110.`（描画距離）と `i<30`（影の長さ）を小さくする
4. OpenGL 4.3 未対応やシェーダーのコンパイルエラーのときは、原因を示すメッセージボックスが出て終了します。
   内容（GPU 名やエラー文）を報告してください。ハイブリッド GPU の PC 向けに、NVIDIA / AMD の高性能 GPU を要求する設定を入れています
5. 画面解像度が 1920x1080 未満のモニターだと、ウィンドウがはみ出します

## クラウドでの検証内容

- MinGW-w64 でのクロスコンパイルとリンクが成功
- Wine + Xvfb + Mesa で `bin/mc4k.exe` を起動し、ウィンドウに地形が描画されることを確認
- `glslangValidator` による GLSL の文法検証が成功
- Mesa（ソフトウェア OpenGL 4.5）で `tools/shader_preview.cpp` により 1 フレーム描画し、
  昼・夕・夜の画像が正しく出ることを目視確認

検証用ツール（Linux 向け、SDL2 と Mesa が必要）:

- シェーダー抽出: `python3 tools/extract_shader.py mc4k.cpp > s.frag`
- プレビューのビルド: `g++ tools/shader_preview.cpp -o prev -lSDL2 -lGL`
- 描画（引数は x y z yaw pitch time 出力名）: `xvfb-run -a ./prev s.frag 256 30 256 0.6 -0.25 0 out.ppm`

## 出典とライセンス

- 本プロジェクトは **GNU GPL v3.0** で公開します（`LICENSE` を参照）。
- 土台: [RealTheSunCat/Minecraft4k-CPP](https://github.com/RealTheSunCat/Minecraft4k-CPP)（GPL-3.0、
  GPU レイマーチングの構成とボクセル走査アルゴリズムを参考にしました）
- 参考: [RealTheSunCat/Minecraft4k-Reversed](https://github.com/RealTheSunCat/Minecraft4k-Reversed)
  （元 jar の逆コンパイルと解説）
- 元の作品: Notch『Minecraft 4k』（2010年 Java 4k Contest）。元の作品に明示的なライセンスはありません
- ボクセル走査: [A Fast Voxel Traversal Algorithm for Ray Tracing](http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf)
  （Amanatides & Woo）
