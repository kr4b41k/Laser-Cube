// Copyright (C) Developed by Pask, Published by Dark Tower Interactive SRL 2023. All Rights Reserved.

#include "ACFAssaultPoint.h"
#include "ACFUnitTypes.h"
#include "Components/ACFAIWavesMasterComponent.h"
#include "Net/UnrealNetwork.h"

void AACFAssaultPoint::SetConqueringState_Implementation(EConqueredState newState)
{
    conqueringState = newState;
    OnConquerStateChanged.Broadcast(conqueringState);
}

void AACFAssaultPoint::StartConquering_Implementation()
{
    SetConqueringState(EConqueredState::EConquerInProgress);
    OnConquestStarted();
}

void AACFAssaultPoint::CompleteConquering_Implementation()
{
    SetConqueringState(EConqueredState::EConquered);
    OnConquestCompleted();
}

void AACFAssaultPoint::InterruptConquering_Implementation()
{
    SetConqueringState(EConqueredState::ENotConquered);
    OnConquestInterrupted();
}

void AACFAssaultPoint::BeginPlay()
{
    Super::BeginPlay();
}

void AACFAssaultPoint::OnConquestStarted_Implementation()
{
}

void AACFAssaultPoint::OnConquestCompleted_Implementation()
{
}

void AACFAssaultPoint::OnConquestInterrupted_Implementation()
{
}

void AACFAssaultPoint::OnRep_ConqueringState()
{
    OnConquerStateChanged.Broadcast(conqueringState);
}

void AACFAssaultPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AACFAssaultPoint, conqueringState);
}
