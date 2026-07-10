## ADDED Requirements

### Requirement: 火球主動技能
系統 SHALL 透過 GAS（GameplayAbility）提供一個火球主動技能，由滑鼠左鍵觸發，發射一顆朝角色面向方向飛行的投射物。

#### Scenario: 成功施放
- **WHEN** 玩家按下滑鼠左鍵，且魔力足夠且技能不在冷卻中
- **THEN** 角色前方生成一顆火球投射物並朝面向方向飛行

### Requirement: 魔力消耗
系統 SHALL 以 GameplayEffect（Cost）在施放火球時扣除固定魔力，魔力不足時 MUST NOT 施放。

#### Scenario: 魔力足夠
- **WHEN** 玩家施放火球且當前魔力 ≥ 火球魔力消耗
- **THEN** 施放成功，魔力依消耗量下降

#### Scenario: 魔力不足
- **WHEN** 玩家嘗試施放火球且當前魔力 < 火球魔力消耗
- **THEN** 技能不觸發，魔力維持不變

##### Example: 魔力邊界
| 當前魔力 | 火球消耗 | 結果 |
|---|---|---|
| 100 | 25 | 施放成功，魔力變 75 |
| 25 | 25 | 施放成功，魔力變 0 |
| 20 | 25 | 不觸發，魔力維持 20 |

### Requirement: 冷卻時間
系統 SHALL 以 GameplayEffect（Cooldown）在施放後令火球進入固定冷卻，冷卻期間 MUST NOT 再次施放。

#### Scenario: 冷卻中再按
- **WHEN** 玩家在火球冷卻期間再次按下滑鼠左鍵
- **THEN** 技能不觸發，不額外消耗魔力

#### Scenario: 冷卻結束
- **WHEN** 冷卻時間結束後玩家再次施放
- **THEN** 技能可正常觸發

### Requirement: 火球命中傷害
系統 SHALL 在火球投射物命中訓練假人時，透過 GameplayEffect 對其套用固定傷害並銷毀投射物。

#### Scenario: 命中假人
- **WHEN** 火球投射物與訓練假人碰撞
- **THEN** 假人 Health 依傷害量下降，火球投射物被銷毀

#### Scenario: 命中牆面
- **WHEN** 火球投射物與非假人的環境碰撞
- **THEN** 火球投射物被銷毀，不造成傷害
