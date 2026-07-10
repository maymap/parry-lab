# Tasks — core-combat-loop

> 標記說明：`[Editor]` = UE5 編輯器內操作（spectra/AI 無法代跑，需你手動）；`[C++]` = 可寫的程式碼；`[Doc]` = 文件。

## 1. 專案骨架

- [x] 1.1 [Editor] **以 UE5 Third Person 範本為基礎** 建立 5.x 專案（C++ 型別），啟用 GameplayAbilities 外掛。驗證：專案能開啟並 PIE，角色可 WASD 移動（達成「第三人稱角色移動」需求的基礎）。
- [x] 1.2 [Editor] 搭建訓練室關卡（內建地板 + 牆 + 一個出生點），設定 GameMode 使玩家生成於出生點面向假人位置。驗證：PIE 時滿足「訓練場出生點」需求 — 角色出生於固定點、假人在視野前方。

## 2. GAS 屬性基礎

- [x] 2.1 [C++] 依「**AttributeSet 用最小 C++，其餘 Blueprint**」的決策，實作一個 AttributeSet 定義 `Health`/`MaxHealth`/`Mana`/`MaxMana`，含 replication 樣板。驗證：編譯通過（命令列 Build.sh 成功，libUnrealEditor-ParryLab.dylib 連結完成）。屬性可在 ASC 讀到於 2.2 掛上 ASC 後確認。
- [x] 2.2 [C++] 為玩家角色掛上 AbilitySystemComponent 並初始化屬性（AttributeSet 建構子設定 Health/Mana=100）。驗證：PIE 用 `showdebug abilitysystem` 可見玩家 Health/Mana=100。（假人的 ASC 於 task 4 建立假人 actor 時一併掛上。）

## 3. 火球技能（GAS）

- [x] 3.1 [Editor] 依「**用 GAS 實作火球，而非單純 Blueprint 計時器**」，建立 Fireball GameplayAbility（BP），綁定滑鼠左鍵觸發，達成「**火球主動技能**」需求。驗證：按左鍵能生成火球投射物並朝面向飛行。
- [x] 3.2 [Editor] 依「**火球為「投射物命中」而非「即時射線」**」，實作火球投射物（ProjectileMovement + 碰撞）。驗證：火球飛行、命中環境時銷毀（「火球命中傷害」的命中牆面情境）。
- [x] 3.3 [Editor] 加上 Cost GameplayEffect 實現「**魔力消耗**」需求：魔力足夠才施放、扣固定量、不足不觸發。驗證：依 spec 魔力邊界表（100→75、25→0、20 不觸發）手動確認。
- [x] 3.4 [Editor] 加上 Cooldown GameplayEffect 實現「**冷卻時間**」需求。驗證：冷卻中再按不觸發、不扣魔力；冷卻結束可再放。
- [x] 3.5 [Editor] 火球命中假人時套用 Damage GameplayEffect，完成「**火球命中傷害**」需求。驗證：命中假人使其 Health 下降且火球銷毀。

## 4. 訓練假人

- [x] 4.1 [Editor] 設定假人「**假人生命值**」：以 GAS Health 屬性承受傷害、下限為 0。驗證：連續命中使 Health 遞減不為負。
- [ ] 4.2 [Editor] 實作「**受擊反饋**」（閃白材質或擊退擇一）。驗證：每次扣血當下有明顯可見反饋。
- [x] 4.3 [Editor] 實作「**死亡與重置**」：Health 歸零 → 短暫失能 → 原位重置滿血。驗證：依 spec 範例（MaxHealth=100、傷害=40，命中 3 次歸零後可重複擊殺）。

## 5. 戰鬥 HUD（UMG）

- [ ] 5.1 [Editor] 建立 UMG HUD，實作「**玩家魔力條**」，填充比例綁定 Mana/MaxMana。驗證：施放火球後魔力條即時下降。
- [ ] 5.2 [Editor] 實作「**假人血條**」，綁定假人 Health/MaxHealth。驗證：假人受擊血條下降、重置後回滿。
- [ ] 5.3 [Editor] 確保「**HUD 與屬性一致**」：屬性變動時 HUD 同畫面更新。驗證：連續施放與受擊時，HUD 數值不與實際 Attribute 脫節。

## 6. 文件與交付

- [x] 6.1 [Doc] 撰寫 `docs/GDD.md`：一句話概念、核心玩法迴圈、設計決策、技術範圍、學到/卡住之處。驗證：內容涵蓋 design.md 的決策摘要，可獨立閱讀。
- [x] 6.2 [Doc] 依「**彈反只寫文件、不實作（本次）**」，在 GDD 的「下一步」章節寫清楚彈反機制設計（時機窗、成功/失敗反饋、與火球投射物系統的關係）。驗證：讀者能看懂彈反設計且明白本次未實作。
- [x] 6.3 [Doc] 撰寫 repo `README`：如何開專案、操作說明、功能與結構。驗證：他人照 README 能開起專案並試玩。（demo 錄影連結待補。）
