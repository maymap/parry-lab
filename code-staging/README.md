# code-staging — 預先寫好的 C++，等專案建好後放進去

這裡放的是**還沒放進 UE 專案的程式碼**。因為 UE 專案要先在編輯器裡建立（task 1.1），
這些檔案先寫好放這，等專案存在後再搬進去。

## 放置步驟（做完 task 1.1、有了 UE 專案之後）

假設你建的專案名是 **ParryLab**，UE 會產生 `ParryLab/Source/ParryLab/` 資料夾。

1. 把這兩個檔案搬到 `ParryLab/Source/ParryLab/`：
   - `ParryLabAttributeSet.h`
   - `ParryLabAttributeSet.cpp`

2. 打開 `ParryLab/Source/ParryLab/ParryLab.Build.cs`，把 GAS 相關模組加進相依：

   ```csharp
   PublicDependencyModuleNames.AddRange(new string[]
   {
       "Core", "CoreUObject", "Engine", "InputCore",
       "GameplayAbilities",   // GAS 本體
       "GameplayTags",        // GAS 依賴
       "GameplayTasks"        // GAS 依賴
   });
   ```

3. 在 UE 編輯器選單 **Tools → Refresh Xcode Project**（或右鍵 .uproject → Generate Project Files），
   然後在 Xcode（或 VS Code / Rider）**Build** 一次。

4. 編譯成功後，回編輯器就能在 Blueprint 裡看到 `ParryLabAttributeSet` 與 Health/MaxHealth/Mana/MaxMana 這四個屬性。

## ⚠️ 若你的專案不叫 ParryLab

把 `.h`/`.cpp` 裡的 `PARRYLAB_API`（大寫模組名 + `_API`）改成你的模組名，
例如專案叫 `MyGame` → 改成 `MYGAME_API`。類別名 `UParryLabAttributeSet` 可保留或一併改名。

## 這段 C++ 做了什麼（對應 spec / design）

- 定義 4 個 GAS 屬性：`Health`/`MaxHealth`（假人）、`Mana`/`MaxMana`（玩家）。
- `PreAttributeChange` + `PostGameplayEffectExecute` 把 Health/Mana 夾在 `[0, Max]`，
  對應 spec「假人生命值下限為 0」「魔力不足不觸發」的邊界。
- 設好 replication 樣板（本 demo 單機也能跑，日後要連線不用重寫）。

其餘（火球 Ability、Cost/Cooldown/Damage Effect、假人反饋、HUD）都在**編輯器用 Blueprint**做，
不需要再寫 C++。
