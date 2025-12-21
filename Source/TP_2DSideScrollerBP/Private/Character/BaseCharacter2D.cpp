// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter2D.h"

#include "GAS/Attributes/CharacterAttributeSet.h"
#include "AbilitySystemComponent.h"

ABaseCharacter2D::ABaseCharacter2D()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	MainAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("MainAttributeSet"));
}

UAbilitySystemComponent* ABaseCharacter2D::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter2D::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// 1. Ініціалізуємо зв'язок (Owner і Avatar - це цей персонаж)
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 2. Встановлюємо початкові значення (HP = 100)
		InitializeAttributes();

		// 3. Видаємо здатність стріляти
		GiveDefaultAbilities();
	}
}

void ABaseCharacter2D::UpdateFacingDirection(float InputValue)
{
	const float Tolerance = UE_SMALL_NUMBER;

	// 1. Конвертуємо порівняння в числа (0.0 або 1.0)
	// Якщо Input > 0.01 -> Positive = 1.0, інакше 0.0
	float IsPositive = (float)(InputValue > Tolerance);
	// Якщо Input < -0.01 -> Negative = 1.0, інакше 0.0
	float IsNegative = (float)(InputValue < -Tolerance);

	// 2. Вираховуємо бажаний напрямок (-1, 0 або 1)
	// Якщо вправо: 1 - 0 = 1
	// Якщо вліво:  0 - 1 = -1
	// Якщо стоїмо: 0 - 0 = 0
	float DesiredDir = IsPositive - IsNegative;

	// 3. Створюємо маску "Чи ми стоїмо?"
	// FMath::Abs(DesiredDir) буде 1 (якщо рухаємось) або 0 (якщо стоїмо).
	// Віднімаємо це від 1.0, щоб інвертувати.
	// Result: 0.0 якщо рухаємось, 1.0 якщо стоїмо.
	float IsIdleMask = 1.0f - FMath::Abs(DesiredDir);

	// 4. Фінальна формула змішування
	// Якщо рухаємось: DesiredDir + (Old * 0) = DesiredDir
	// Якщо стоїмо:    0          + (Old * 1) = OldDir
	float FinalX = DesiredDir + (FacingDirection2D.X * IsIdleMask);

	// 5. Записуємо результат
	FacingDirection2D.X = FinalX;

	// (Опціонально) Якщо ви хочете гарантувати, що Y і Z завжди 0:
	// FacingDirection2D = FVector(FinalX, 0.0f, 0.0f);
}

// BaseCharacter2D.cpp

void ABaseCharacter2D::InitializeAttributes()
{
	// Перевіряємо, чи є компонент і чи заповнений масив ефектів в едіторі
	if (!AbilitySystemComponent || DefaultAttributeEffects.IsEmpty())
	{
		return;
	}

	for (auto& SetClass : Attributes)
	{
		if (SetClass)
		{
			// 1. Створюємо новий об'єкт атрибут-сета
			UAttributeSet* NewSet = NewObject<UAttributeSet>(this, SetClass);

			// 2. Важливо: Реєструємо його в системі, щоб GAS його побачив!
			AbilitySystemComponent->AddAttributeSetSubobject(NewSet);

			// 3. Зберігаємо у свій масив, щоб GC не видалив його
			SpawnedAttributeSets.Add(NewSet);
		}
	}

	// Створюємо контекст (хто застосовує ефект? Ми самі)
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Проходимо по кожному класу ефекту з масиву
	for (TSubclassOf<UGameplayEffect> EffectClass : DefaultAttributeEffects)
	{
		if (!EffectClass) continue;

		// Створюємо Spec (специфікацію) - це підготовлений екземпляр ефекту
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);

		if (SpecHandle.IsValid())
		{
			// Застосовуємо ефект до себе
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ABaseCharacter2D::GiveDefaultAbilities()
{
	// Тільки сервер має право видавати абілки
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				// Створюємо "Spec" абілки і видаємо її
				FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, -1, this);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
}
