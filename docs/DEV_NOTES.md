# 開發筆記 (Dev Notes)

給接手者（或未來的自己）的實作流程與踩坑紀錄。

---

## 環境

- **機器**：Apple Silicon Mac（M1 / 16GB）
- **引擎**：Unreal Engine 5.8
- **編譯工具**：Xcode 16+（**需含 Metal 工具鏈**）
- 專案為 **C++ 型別**，Source 在 `ParryLab/Source/ParryLab/`

---

## 編譯：命令列 build（推薦）

編輯器**完全關閉**時，用命令列編譯最可靠（能直接看到錯誤）：

```bash
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  ParryLabEditor Mac Development \
  -Project="/Users/may/ai/parry-lab/ParryLab/ParryLab.uproject" \
  -WaitMutex
```

看到 `Result: Succeeded` 即成功；接著重開專案。

## 何時要關編輯器、何時能熱編（Live Coding）

| 改動類型 | 做法 |
|---|---|
| **改函式邏輯、調數值**（既有程式） | **Live Coding**：編輯器右下角 Live Coding 按鈕 → Compile，不用關 |
| **新增類別、改 header 結構、加成員變數** | **必須關閉編輯器**，用上面的命令列重編 |

> Mac 上 `Ctrl+Alt+F11` 常被系統攔截（F11 需 Fn），用右下角按鈕比較保險。
> 攝影機等在建構子設定的值，Live Coding 後需 **Stop → 再 Play** 才會套用（角色重生才跑新建構子）。

---

## 踩過的坑

### 1. Xcode 太新 → UE 5.8 缺 Metal 工具鏈
- macOS 15 的 App Store 可能給到 **Xcode 26**，對 UE 5.8 偏新。
- 首次啟動報 `missing Metal Toolchain`。**解法**：
  ```bash
  xcodebuild -downloadComponent MetalToolchain
  ```
  （Xcode 16.3+ 把 Metal 編譯器改成按需下載的獨立元件。）
- **不要**為了裝新 Xcode 去升級 macOS 26——論壇回報 macOS 26 + Xcode 26 反而編不動 UE 5.8。留在 macOS 15。

### 2. GameplayEffect 建構子不能加元件（會 crash）
- 在 `UGameplayEffect` 建構子呼叫 `FindOrAddComponent<UTargetTagsGameplayEffectComponent>()`
  會在 CDO 建立階段因 `NewObject` 空名而 **fatal**（編輯器一開就 crash）。
- **解法**：冷卻改用 **loose gameplay tag + 計時器**（見 `GA_Fireball::ActivateAbility`），
  並覆寫 `GetCooldownTags()`。若真要用 GE 的授予標籤，改成在**編輯器建 Blueprint GE**（元件 UI 正常）。

### 3. UE 把警告當錯誤
- 例：`for` 迴圈內立即 `break` 觸發 `-Wunreachable-code-loop-increment` → 編譯失敗。
- 寫 UE C++ 時，警告等同錯誤，避免無用迴圈、未使用變數等。

---

## 調手感（多數不用改程式）

以下都是**編輯器可調參數**，改完不必重編：

- `GA_Fireball`：`NumProjectiles`（一次幾顆）、`SpreadAngleDeg`（散射）、`CooldownDuration`（冷卻）、`SpawnForwardOffset`/`SpawnHeightOffset`
- `AParryLabCharacter`：`ManaRegenRate`（回魔速度）
- `AParryLabGameMode`：`NumDummies`、`ForwardDistance`、`SideSpacing`（假人數量與排列）

攝影機角度/距離目前寫死在 `ParryLabCharacter` 建構子（`SetWorldRotation` 的 pitch、`TargetArmLength`），改這兩個數字要重編（或 Live Coding + Stop/Play）。

---

## 流程：spec-driven（Spectra）

- 進度與待辦：`openspec/changes/core-combat-loop/tasks.md`
- 設計決策：`openspec/changes/core-combat-loop/design.md`
- 要開新功能（例如彈反）：開一個新的 spectra change（`spectra new change ...`）
