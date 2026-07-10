## ADDED Requirements

### Requirement: 玩家魔力條
系統 SHALL 透過 UMG 在 HUD 常駐顯示玩家魔力條，其填充比例 MUST 反映當前 Mana / MaxMana。

#### Scenario: 施放後魔力條下降
- **WHEN** 玩家施放火球消耗魔力
- **THEN** 魔力條填充比例即時下降至對應數值

### Requirement: 假人血條
系統 SHALL 透過 UMG 顯示訓練假人血條，其填充比例 MUST 反映假人當前 Health / MaxHealth。

#### Scenario: 假人受擊血條下降
- **WHEN** 假人受火球傷害
- **THEN** 假人血條填充比例即時下降

#### Scenario: 假人重置血條回滿
- **WHEN** 假人死亡後重置為滿血
- **THEN** 假人血條回到滿格

### Requirement: HUD 與屬性一致
系統 SHALL 確保 HUD 顯示值與 GAS Attribute 實際值一致，不得出現顯示與實際脫節。

#### Scenario: 數值同步
- **WHEN** 任一被追蹤的 Attribute（Mana 或 Health）改變
- **THEN** 對應 HUD 元件於同一畫面更新反映新值
