## ADDED Requirements

### Requirement: 第三人稱角色移動
系統 SHALL 提供一個可由玩家操控的第三人稱角色，支援水平移動與視角旋轉，以 UE5 Third Person 範本為基礎。

#### Scenario: 鍵盤移動
- **WHEN** 玩家按下 W/A/S/D
- **THEN** 角色朝對應方向移動，並播放對應的移動動畫

#### Scenario: 滑鼠轉視角
- **WHEN** 玩家移動滑鼠
- **THEN** 攝影機環繞角色旋轉，角色朝移動方向轉向

### Requirement: 訓練場出生點
系統 SHALL 在啟動 PIE 時，將玩家角色生成於訓練室的固定出生點，面向訓練假人。

#### Scenario: 進入遊戲
- **WHEN** 啟動 Play In Editor
- **THEN** 玩家角色出現在訓練室出生點，訓練假人在其視野前方
