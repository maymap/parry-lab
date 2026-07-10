## ADDED Requirements

### Requirement: 假人生命值
系統 SHALL 賦予訓練假人一組 GAS 生命屬性（Health / MaxHealth），初始為滿血，受火球傷害時下降。

#### Scenario: 受擊扣血
- **WHEN** 假人被火球命中並套用傷害 Effect
- **THEN** 假人 Health 依傷害量下降，且不低於 0

### Requirement: 受擊反饋
系統 SHALL 在假人受擊當下播放至少一種可見反饋（閃白材質或擊退位移擇一）。

#### Scenario: 命中反饋
- **WHEN** 假人 Health 因受擊而下降
- **THEN** 假人播放受擊反饋，讓玩家明確看出「打中了」

### Requirement: 死亡與重置
系統 SHALL 在假人 Health 歸零時使其短暫失能，隨後於原位重置為滿血，使 demo 可重複試玩。

#### Scenario: 假人死亡重置
- **WHEN** 假人 Health 降到 0
- **THEN** 假人短暫失能（例如倒下或消失數秒），隨後在原位置重置為 MaxHealth

##### Example: 連續擊殺可重複
- **GIVEN** 假人 MaxHealth=100、火球傷害=40
- **WHEN** 玩家連續命中 3 次
- **THEN** 第 3 次命中使 Health 歸零、假人重置，可再次被擊殺
