# Onslaught!

## Description / 概要
A 2D top view rogue-like game made by Haratake Kentaro. The game includes tower defence aspects by allowing players to place towers. The objective is that the player stays alive while protecting the town from enemies. 

Onslaught!(猛攻撃という意味)は原武健太郎によって制作された、2Dトップビューのアクションローグライクゲームです。建築物（防衛塔など）の設置といったタワーディフェンス要素もあります。ゲームの目的はプレイヤーの生存と、街を敵から守り抜くことです。

Language used / 使用言語: C++

Library used / 使用ライブラリー: Simple Fast Media Library


## How to start / 起動方法　（Windows 10）
The game can be played by opening the Onslaught.exe file, located inside the windows_build folder. The .exe file must be in the same location as the resources folder.

** If windows security does not allow you to start Onslaught.exe, you must open the file directly from Terminal/PowerShell. 
Steps :
1. Open the windows_build folder.
2. Shift+Right click some open space in the folder
3. Click on "Open PowerShell Window here"
4. Type " ./Onslaught.exe " in the PowerShell window and press enter



ゲームは　windows_build　内にある実行ファイル　Onslaught.exe　を開くと起動する。このファイルは　resourcesと同じ場所でないと起動しません。

＊＊WindowsセキュリティーでOnslaught.exeが開かない場合、ファイルを Terminal/PowerShell で直接開けます。
手順　：
1. windows_build フォルダーを開く
2. フォルダー内の空白（どこでも）を　Shift + 右クリック
3. PowerShellウィンドウをここで開く　をクリック
4. PowerShellが開くと、そこに " ./Onslaught.exe " を入力し、Enterキーを押す



## How to start / 起動方法　（macOS）
The game can be played by clicking on the executable file, located inside the macos_build folder. The folder containing the executable must also contain the resources folder.

** If the game does not react to keyboard inputs, you must give permission for input monitoring for the Onslaught executable.
Steps :
1. Open System Settings
2. Navigate to Privacy & Security > Input Monitoring
3. Click + on the bottom left of the list
4. Find the Onslaught executable file, and click open to add the application to the list



ゲームは　macos_build　フォルダー内にある実行ファイルをクリックすると起動します。実行ファイルが入っているフォルダーには、resourcesフォルダーも同じ場所に置く必要があります。

＊＊キーボード入力が反応しない場合、Onslaught実行ファイルに「入力監視」の許可を与える必要があります。
手順　：
1. システム設定を開く
2. 「プライバシーとセキュリティ」＞「入力監視」を開く
3. 左下の「＋」ボタンをクリック
4. Onslaughtの実行ファイルを選択し、「開く」をクリックしてリストに追加する



## Controls
WASD                         -> Movement        /  移動

Spacebar, Left Mouse button  -> Confirm, Attack /  確認、攻撃

Left Shift (Hold)            -> Dash            /  ダッシュ・素早く移動

E                            -> Switch mode (Build mode <-> Attack mode)      /  モード変更（建築 <-> 攻撃）

R (During build mode only)   -> Switch building (Archer tower > Wall > loop)  /  建築物変更（狙撃等 > 壁 > ループ（戻る））

## How to Play / 遊び方
Once the game starts, enemies will start spawning and head towards you and the town (castle like building). You must survive for 3 minutes, while also protecting the town.

The player is able to place defensive towers to help protect the town from enemies. To do so, switch to build mode and click where you want to place building. Creating buildings requires materials, which can be collected by destroying trees and rocks on the field. You can check the materials you have by clicking "Inventory" below the health and stamina bar at the top left of the screen.

2 minutes in the game, a wave of enemies will begin spawning. Prepare your defences by then to protect the town from enemies for 1 minute!


ゲーム開始直後、敵がわきはじめて、プレイヤーと街（城っぽい建物）に向かい、攻撃してきます。街を３分間守り抜きながら生き残れ！

プレイヤーは街の防衛に役立つ、防衛建築物をたてられます。建築をするには、まず建築モードへと切り替えていき、建築物を建てたい場所をクリックします。
建築には素材が必要で、フィールド上の木や岩などを壊すと手に入ります。所持している素材はインベントリーボタンを押すと見れます（画面左上にあるHP・スタミナの下）。

ゲーム開始から２分後、敵が大量にわき始めます（ウェーブ）。それまでに素材を集め、街付近の防衛を強化し、最後の１分間街を敵から守り抜け！

## Credits (Assets used) / クレジット（使用アセット）
Player       / プレイヤー
https://trevor-pupkin.itch.io/minimal-dungeon

Enemy        / 敵
https://snoblin.itch.io/pixel-rpg-skeleton-free

Buildings    / 建築物
https://pixelfrog-assets.itch.io/tiny-swords

Audio files  / オーディオファイル
https://leohpaz.itch.io/minifantasy-dungeon-sfx-pack

Rocks        / 岩・石
https://dustdfg.itch.io/pixel-art-top-down-rocks-pack

Trees        /　木
https://ninjikin.itch.io/trees

Logs (wood)  /　原木
https://frostwindz.itch.io/pixel-art-wood


Music  /　音楽

2:23 AM		written by しゃろう
https://dova-s.jp/bgm/detail/13494

Morning		written by しゃろう
https://dova-s.jp/bgm/detail/2445
