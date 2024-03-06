UENUM(BlueprintType)
enum class EIAState : uint8 
{
     RANDOM_MOVE = 0    UMETA(DisplayName = "RANDOM_MOVE"),
     MOVE_TO = 1        UMETA(DisplayName = "MOVE_TO"),
     BALL = 2           UMETA(DisplayName = "BALL"),
     SHIELD = 3         UMETA(DisplayName = "SHIELD"),
     SPEAR = 4         UMETA(DisplayName = "SPEAR")
};