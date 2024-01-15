#pragma once
#include <wchar.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <basetsd.h>
#include "SrcVmm.h"

/// @brief 队友等级
enum EPartnerLevel : uint8_t {
    None,               // 无等级
    Normal,             // 普通等级
    Lead,               // 领导等级
    EPartnerLevel_MAX,  // 伙伴等级最大值
};

/// @brief 角色状态
enum ECharacterState : uint8_t {
    Unk,      // 未知
    Fire,     // 开枪
    Wounded,  // 掉血
    BeHit,    // 倒地
    Offline,  // 掉线
};

/// @brief 可视性
enum class ESlateVisibility : uint8_t {
    Visible = 0,               // 可见
    Collapsed = 1,             // 折叠
    Hidden = 2,                // 隐藏
    HitTestInvisible = 3,      // 不可见
    SelfHitTestInvisible = 4,  // 测试不可见
    ESlateVisibility_MAX = 5   // 最大可见度
};

template <class T>
class TArray {
   private:
    T* Data;
    int32_t Count;
    int32_t Max;

    friend struct FString;

   public:
    size_t size(size_t MaxSize = 0x2000) const { return Count < MaxSize ? Count : MaxSize; }

    bool GetValue(int i, T& value) const {
        if (i < 0 || i >= size(0x2000)) return false;
        // return memcpy(&value, (PVOID)(Data + sizeof(T) * i), sizeof(T));
        return VmmCore::ReadByte(reinterpret_cast<uintptr_t>((Data + sizeof(T) * i)), &value, sizeof(T));
    }

    bool GetValues(T& value, size_t MaxSize = 0x2000) const {
        return VmmCore::ReadByte(reinterpret_cast<uintptr_t>(Data), &value, sizeof(T) * size(MaxSize));
        // return memcpy(&value, (PVOID)Data, sizeof(T) * size(MaxSize));
    }

    std::vector<T> GetVector(size_t MaxSize = 0x2000) const {
        std::vector<T> v(size(MaxSize));
        if (!GetValues(v[0], MaxSize)) v.clear();

        return v;
    }

    operator std::vector<T>() const { return GetVector(); }

    T operator[](int i) const {
        T Value;
        if (!GetValue(i, Value)) ZeroMemory(&Value, sizeof(Value));
        return Value;
    }
};

template <class T>
struct TSetElement {
    T Value;         // 0
    int HashNextId;  // 9
    int HashIndex;   // A
};

template <class T>
class TSet : public TArray<TSetElement<T>> {};

template <class KeyType, class ValueType>
struct TPair {
    KeyType Key;
    ValueType Value;
};

template <class KeyType, class ValueType>
class TMap : public TSet<TPair<KeyType, ValueType>> {
   public:
    bool GetValue(const KeyType& Key, ValueType& Value) const {
        for (const auto& Elem : this->GetVector()) {
            if (Elem.Value.Key == Key) {
                Value = Elem.Value.Value;
                return true;
            }
        }
        return false;
    }
};

struct FString : public TArray<wchar_t> {
    FString() = default;

    explicit FString(const wchar_t* other) {
        Max = Count = (int32_t)(*other ? std::wcslen(other) + 1 : 0);

        if (Count) {
            Data = const_cast<wchar_t*>(other);
        }
    };

    inline bool IsValid() const { return Data != nullptr; }

    inline const wchar_t* cw_str() const { return Data; }

    inline const char* c_str() const { return (const char*)Data; }

    std::string ToString() const {
        size_t length = std::wcslen(Data);
        std::string str(length, '\0');
        std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

        return str;
    }

    std::wstring ToWString() const {
        std::wstring str(Data);
        return str;
    }
};

struct FTslWidgetState {
    unsigned char Pad[0x30];
    ULONG64 WidgetClass;                // 0x0030(0x0008) (CPF_ZeroConstructor, CPF_Transient, CPF_IsPlainOldData)
    unsigned char UnknownData00[0x8];   // 0x0038(0x0008) MISSED OFFSET
    ULONG64 Widget;                     // 0x0040(0x0008) (CPF_ExportObject, CPF_ZeroConstructor, CPF_Transient, CPF_InstancedReference,
                                        // CPF_IsPlainOldData)
    unsigned char UnknownData01[0x18];  // 0x0048(0x0018) MISSED OFFSET
};

struct FMargin {
    float Left;    // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
    float Top;     // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
    float Right;   // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
    float Bottom;  // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
};

/// @brief 武器弹道配置3
struct FWeaponTrajectoryConfig3 {
    float InitialSpeed;                                 // 0x0000(0x0004) (CPF_Edit, CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float HitDamage;                                    // 0x0004(0x0004) (CPF_Edit, CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float LowerClampDamage;                             // 0x0008(0x0004) (CPF_Edit, CPF_ZeroConstructor, CPF_DisableEditOnInstance,
                                                        // CPF_IsPlainOldData)
    unsigned long long BallisticCurve;                  // 0x0010(0x0008) (CPF_Edit, CPF_ZeroConstructor, CPF_DisableEditOnInstance,
                                                        // CPF_IsPlainOldData)
    float RangeModifier;                                // 0x0018(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly, CPF_ZeroConstructor,
                                                        // CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float ReferenceDistance;                            // 0x001C(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float TravelDistanceMax;                            // 0x0020(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    unsigned char bUseMaxDamageDistance : 1;            // 0x0024(0x0001) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float SimulationSubstepTime;                        // 0x0028(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float VDragCoefficient;                             // 0x002C(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    float BDS;                                          // 0x0030(0x0004) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly, CPF_ZeroConstructor,
                                                        // CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    unsigned char bUseAdvancedBallistics : 1;           // 0x0034(0x0001) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    unsigned char bCanProduceCrackSound : 1;            // 0x0035(0x0001) (CPF_Edit, CPF_BlueprintVisible, CPF_BlueprintReadOnly,
                                                        // CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    unsigned char IsPenetrable : 1;                     // 0x0036(0x0001) (CPF_Edit, CPF_ZeroConstructor, CPF_DisableEditOnInstance,
                                                        // CPF_IsPlainOldData)
    unsigned long long DamageType;                      // 0x0038(0x0008) (CPF_Edit, CPF_ZeroConstructor, CPF_DisableEditOnInstance, CPF_IsPlainOldData)
    unsigned char bIsTrajectoryReplicationEnabled : 1;  // 0x0040(0x0001) (CPF_Edit, CPF_DisableEditOnInstance)
};

struct FWeaponTrajectoryConfig {
    float InitialSpeed;
    float HitDamage;
    float HitDamage2;
    char UnknownData00[0x4];
    DWORD_PTR UnknownData000;
    float VehicleDamageScalar;
    float LowerClampDamage;
    DWORD_PTR BallisticCurve;
    float RangeModifier;
    float ReferenceDistance;
    float TravelDistanceMax;
    bool bUseMaxDamageDistance;
    char UnknownData01[0x3];
    float SimulationSubstepTime;
    float VDragCoefficient;
    float BDS;
    bool bUseAdvancedBallistics;
    bool bCanProduceCrackSound;
    bool IsPenetrable;
    char UnknownData02[0x1];
    DWORD_PTR DamageType;
    char bIsTrajectoryReplicationEnabled : 1;
    char UnknownData03[0x3];
    float WeaponSpread;
};

struct FWeaponTrajectoryConfig2 {
    float InitialSpeed;
    float HitDamage;
    float UnknownFloat;
    char pad_47[0x10];
    ULONG64 UnknownData2;
    float VehicleDamageScalar;
    float LowerClampDamage;
    ULONG64 BallisticCurve;
    float RangeModifier;          // 0x30(0x04)
    float ReferenceDistance;      // 0x34(0x04)
    float TravelDistanceMax;      // 0x38(0x04)
    bool bUseMaxDamageDistance;   // 0x3c(0x01)
    char pad_3D[0x3];             // 0x3d(0x03)
    float SimulationSubstepTime;  // 0x40(0x04)
    float VDragCoefficient;       // 0x44(0x04)
    float BDS;                    // 0x48(0x04)
    bool bUseAdvancedBallistics;  // 0x4c(0x01)
    bool bCanProduceCrackSound;   // 0x4d(0x01)
    bool UnknownBool01;           // 0x4e(0x01)
    char pad_4F[0x1];             // 0x4f(0x01)
    ULONG64 DamageType;
    char pad_58_0 : 1;                         // 0x58(0x01)
    char bIsTrajectoryReplicationEnabled : 1;  // 0x58(0x01)
    char pad_58_2 : 6;                         // 0x58(0x01)
    char pad_59[0x3];                          // 0x59(0x03)
    float WeaponSpread;                        // 0x5c(0x04)
};

struct FKeyHandleMap {
    unsigned char UnknownData00[0x50];  // 0x0000(0x0050) MISSED OFFSET
};
struct FIndexedCurve {
    unsigned char UnknownData00[0x8];          // 0x0000(0x0008) MISSED OFFSET
    struct FKeyHandleMap KeyHandlesToIndices;  // 0x0008(0x0050) (Transient)
};
struct FRichCurve : public FIndexedCurve {
    BYTE PreInfinityExtrap;   // 0x0058(0x0001) (ZeroConstructor, IsPlainOldData)
    BYTE PostInfinityExtrap;  // 0x0059(0x0001) (ZeroConstructor, IsPlainOldData)
    BYTE UnknownData00[0x2];  // 0x005A(0x0002) MISSED OFFSET
    float DefaultValue;       // 0x005C(0x0004) (Edit, ZeroConstructor, IsPlainOldData)
    ULONG64 Keys;             // 0x0060(0x0010) (Edit, EditFixedSize, ZeroConstructor)
};
struct FRichCurveKey {
    BYTE InterpMode;            // 0x0000(0x0001) (ZeroConstructor, IsPlainOldData)
    BYTE TangentMode;           // 0x0001(0x0001) (ZeroConstructor, IsPlainOldData)
    BYTE TangentWeightMode;     // 0x0002(0x0001) (ZeroConstructor, IsPlainOldData)
    BYTE UnknownData00[0x1];    // 0x0003(0x0001) MISSED OFFSET
    float Time;                 // 0x0004(0x0004) (Edit, ZeroConstructor, IsPlainOldData)
    float Value;                // 0x0008(0x0004) (Edit, ZeroConstructor, IsPlainOldData)
    float ArriveTangent;        // 0x000C(0x0004) (ZeroConstructor, IsPlainOldData)
    float ArriveTangentWeight;  // 0x0010(0x0004) (ZeroConstructor, IsPlainOldData)
    float LeaveTangent;         // 0x0014(0x0004) (ZeroConstructor, IsPlainOldData)
    float LeaveTangentWeight;   // 0x0018(0x0004) (ZeroConstructor, IsPlainOldData)
};

struct FWuPubgIdData {
    int SurvivalTier;   // 0x0000(0x0004) (ZeroConstructor, IsPlainOldData)
    int SurvivalLevel;  // 0x0004(0x0004) (ZeroConstructor, IsPlainOldData)
};

struct FWuCharacterClanInfo {
    FString ClanId;    // 0x0000(0x0010) (ZeroConstructor)
    FString ClanName;  // 0x0010(0x0010) (ZeroConstructor)
    FString ClanTag;   // 0x0020(0x0010) (ZeroConstructor)
    int ClanLevel;     // 0x0030(0x0004) (ZeroConstructor, IsPlainOldData)
};