# Parry Lab

一個用 **Unreal Engine 5.8 + Gameplay Ability System（GAS）** 打造的微型戰鬥原型。

玩家操控第三人稱角色，用消耗魔力、有冷卻的**火球技能**攻擊會受擊、會重生的**訓練假人**，聚焦在「一個技能做到位」的動作戰鬥手感。

> 這是一個原型（prototype），不是完整遊戲。
> 設計文件見 [`docs/GDD.md`](docs/GDD.md)；編譯流程與踩坑紀錄見 [`docs/DEV_NOTES.md`](docs/DEV_NOTES.md)。

---

## 玩法

- **WASD**：移動（角色自動轉向移動方向）
- **滑鼠左鍵**：朝角色面向連發火球（一次多顆、散射）
- 開場自動在前方生成數個訓練假人；打空血後會原位重生
- 螢幕左上角顯示 Mana / Health 數值
- 攝影機為固定斜俯視（3/4 視角），不隨滑鼠轉動

## 功能

- ✅ 以 **GAS** 實作的火球技能：Ability + AttributeSet + GameplayEffect
- ✅ 魔力消耗（Cost Effect）與魔力自動回復
- ✅ 冷卻（loose gameplay tag + 計時器）
- ✅ 命中傷害（Damage Effect）
- ✅ 訓練假人：生命值、死亡失能、原位重生、開場自動生成
- ✅ 固定斜俯視攝影機
- ⏳ 規劃中：彈反機制、正式血條 UI、受擊視覺回饋（見 GDD）

## 技術棧

Unreal Engine 5.8 · Gameplay Ability System · C++（屬性 / 技能 / 效果 / 假人 / GameMode）+ Blueprint（角色、輸入）· 第三人稱範本

---

## 開發環境需求

- Unreal Engine **5.8**
- macOS：Xcode 16+（含 Metal 工具鏈）；或 Windows：Visual Studio 2022（C++ 遊戲開發工作負載）
- 專案為 **C++ 型別**，首次開啟需編譯

## 如何執行

1. 用 UE 5.8 開啟 `ParryLab/ParryLab.uproject`
   - 若提示模組需重建，選「Yes」讓它編譯
2. 開啟後直接按 **Play**（無需手動擺放任何物件——假人會自動生成）

命令列編譯（macOS，可選）：

```bash
"/path/to/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  ParryLabEditor Mac Development \
  -Project="/absolute/path/ParryLab/ParryLab.uproject" -WaitMutex
```

---

## 專案結構

```
parry-lab/
├── ParryLab/                       # UE5 專案
│   ├── ParryLab.uproject
│   ├── Source/ParryLab/            # C++ 原始碼
│   │   ├── ParryLabAttributeSet.*  # GAS 屬性：Health / Mana
│   │   ├── ParryLabCharacter.*     # 玩家角色：ASC、火球輸入、回魔、俯視鏡頭
│   │   ├── GA_Fireball.*           # 火球 GameplayAbility（多發、冷卻）
│   │   ├── FireballProjectile.*    # 火球投射物（飛行、命中套用傷害）
│   │   ├── GE_FireballCost.*       # 魔力消耗 Effect
│   │   ├── GE_FireballDamage.*     # 命中傷害 Effect
│   │   ├── TrainingDummy.*         # 訓練假人：生命值、死亡重生
│   │   └── ParryLabGameMode.*      # 開場自動生成假人
│   └── Content/                    # 場景、角色、輸入等資產
├── docs/GDD.md                     # 遊戲設計文件
└── openspec/                       # spec-driven 開發的規格與任務
```

## 可調參數

多數手感數值可在編輯器直接調整（`GA_Fireball` 的火球數量 / 散射 / 冷卻、角色的回魔速度、`ParryLabGameMode` 的假人數量與間距等），無需改動程式碼。
