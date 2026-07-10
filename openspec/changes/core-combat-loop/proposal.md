## Why

這是一份求職作品集用的微型遊戲 demo，目標職位是一家做「3D 第三人稱 · 動漫風 · Action-Roguelite · 技能/彈反戰鬥」遊戲的公司。開發者為 4+ 年後端/全端工程師，UE5 與遊戲開發為新領域。

面試方需要看到兩件事：(1) 真的動手做出可玩的東西、碰過 GAS；(2) 看得懂他們遊戲的設計語言（技能戰鬥手感、彈反）。因此本 demo 只做「一個能玩 30 秒的核心戰鬥迴圈」，用最小範圍證明動手能力，並在文件層面展示對彈反機制的設計理解。

## What Changes

- 建立可玩的第三人稱場景：一個訓練室、一個可操控角色（沿用 UE5 Third Person 範本）。
- 角色具備一個 GAS 火球主動技能：按左鍵發射投射物，**消耗魔力**、進入**冷卻**。
- 場景中有一個訓練假人：被火球擊中會**扣血、播放受擊反饋**（閃白/擊退），血歸零後短暫失能再重置。
- 螢幕 HUD 顯示玩家魔力條與假人血條（UMG）。
- 交付一份人類可讀的 `GDD.md` 作為門面文件，內含核心玩法、設計決策，以及**規劃中的彈反機制**（本次不實作）。

## Capabilities

### New Capabilities

- `player-locomotion`: 第三人稱角色移動與攝影機控制（以 UE5 範本為基礎，最小客製）。
- `fireball-ability`: 以 GAS 實作的火球主動技能，含魔力消耗與冷卻的 Gameplay Ability / Attribute / Effect。
- `training-dummy`: 可受擊的訓練假人，含生命值、受擊反饋與死亡後重置。
- `combat-hud`: 顯示玩家魔力與假人生命的 UMG 介面。

### Modified Capabilities

（無，全新專案）

## Impact

- 新建 UE5 5.x 專案，啟用 GameplayAbilities 外掛。
- 主要以 Blueprint 實作；GAS 的 AttributeSet 需少量 C++。
- 交付物：可執行的 UE5 專案（`ue5/`）+ `docs/GDD.md`。
- 明確不影響/不做：連線多人、程序化關卡、存檔、敵人 AI（見 design.md 的 Non-Goals）。
