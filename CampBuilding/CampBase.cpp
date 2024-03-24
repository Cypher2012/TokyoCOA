// Copyright © 2024, Alan Aldred / Ginger Beard Gaming Studios.


#include "CampBase.h"

// Engine classes
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Custom classes
#include "../Characters/Playable/AdventureCharacterBase.h"

ACampBase::ACampBase()
{
	RecruitmentBeacon = CreateDefaultSubobject<UStaticMeshComponent>("RecruitmentBeacon");
	RootComponent = RecruitmentBeacon;

	CampBounds = CreateDefaultSubobject<USphereComponent>("SettlementBounds");
	CampBounds->SetSphereRadius(1500.0f);
	CampBounds->bHiddenInGame = false;
	CampBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CampBounds->SetupAttachment(RootComponent);
}

void ACampBase::BeginPlay()
{
	Super::BeginPlay();
	
	CampBounds->OnComponentBeginOverlap.AddDynamic(this, &ACampBase::OverlapBegin);
	CampBounds->OnComponentEndOverlap.AddDynamic(this, &ACampBase::OverlapEnd);

	NotifyPlayerBeginPlay();
}

void ACampBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerCharacter.IsValid() && PlayerCharacter == OtherActor)
	{
		PlayerCharacter->OnCampBeginOverlap(this);
	}
}

void ACampBase::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerCharacter.IsValid() && PlayerCharacter == OtherActor)
	{
		PlayerCharacter->OnCampEndOverlap(this);
	}
}

TWeakObjectPtr<AAdventureCharacterBase> ACampBase::GetPlayerCharacter()
{
	if (PlayerCharacter.IsValid())
	{
		return PlayerCharacter;
	}

	PlayerCharacter = Cast<AAdventureCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	return PlayerCharacter;
}

void ACampBase::NotifyPlayerBeginPlay()
{
	// Get the player character
	if (!GetPlayerCharacter().IsValid())
	{
		return;
	}

	GetPlayerCharacter()->OnCampCreated(this);
}

void ACampBase::SetPlayerCharacter(TWeakObjectPtr<AAdventureCharacterBase> InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

bool ACampBase::ValidateActorOverlap(const AActor* Other)
{
	return CampBounds->IsOverlappingActor(Other);
}

float ACampBase::GetCampBoundsRadius() const
{
	return CampBounds->GetScaledSphereRadius();
}

