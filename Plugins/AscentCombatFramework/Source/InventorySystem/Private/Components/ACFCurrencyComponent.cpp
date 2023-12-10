// Copyright (C) Developed by Pask, Published by Dark Tower Interactive SRL 2021. All Rights Reserved.

#include "Components/ACFCurrencyComponent.h"
#include "ACFItemSystemFunctionLibrary.h"
#include "ARSStatisticsComponent.h"
#include "ARSFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UACFCurrencyComponent::UACFCurrencyComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    CurrencyAmount = 0.f;
}

void UACFCurrencyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UACFCurrencyComponent, CurrencyAmount);
}

void UACFCurrencyComponent::RemoveCurrency_Implementation(float amount)
{
    CurrencyAmount -= amount;
    CurrencyAmount = FMath::Clamp(CurrencyAmount, 0.f, BIG_NUMBER);
    OnCurrencyChanged.Broadcast(CurrencyAmount, -amount);
}

bool UACFCurrencyComponent::RemoveCurrency_Validate(float amount)
{
    return true;
}

void UACFCurrencyComponent::SetCurrency_Implementation(float amount)
{
    CurrencyAmount = amount;
    OnCurrencyChanged.Broadcast(CurrencyAmount, 0.f);
}

void UACFCurrencyComponent::AddCurrency_Implementation(float amount)
{
    CurrencyAmount += amount;
    OnCurrencyChanged.Broadcast(CurrencyAmount, amount);
}

bool UACFCurrencyComponent::AddCurrency_Validate(float amount)
{
    return true;
}

// Called when the game starts
void UACFCurrencyComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bDropCurrencyOnOwnerDeath && UKismetSystemLibrary::IsServer(this)) {
        UARSStatisticsComponent* statComp = GetOwner()->FindComponentByClass<UARSStatisticsComponent>();

        if (statComp) {
            statComp->OnStatisiticReachesZero.AddDynamic(this, &UACFCurrencyComponent::HandleStatReachedZero);
        }
    }
}

void UACFCurrencyComponent::HandleStatReachedZero(FGameplayTag stat)
{
    if (UKismetSystemLibrary::IsServer(this) && UARSFunctionLibrary::GetHealthTag() == stat) {
        const float randomVariation = FMath::FRandRange(-CurrencyDropVariation, CurrencyDropVariation);
        const float finalDrop = CurrencyAmount + randomVariation;
        if (finalDrop > 0.f) {
            UACFItemSystemFunctionLibrary::SpawnCurrencyItemNearLocation(this,finalDrop ,GetOwner()->GetActorLocation(), 100.f);
            RemoveCurrency(finalDrop);
        }
    }
}

void UACFCurrencyComponent::OnRep_Currency()
{
    OnCurrencyChanged.Broadcast(CurrencyAmount, 0);
}
