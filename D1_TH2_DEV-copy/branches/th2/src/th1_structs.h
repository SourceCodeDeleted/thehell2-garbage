// sizeof 12
struct BaseEffect
{
	begin_( BaseEffect );
	AFFIX_EFFECT	id;		_( id );	// 0
	int				minLow;	_( minLow );// 4
	int				maxLow;	_( maxLow );// 8
};
end_( BaseEffect ); reflect( BaseEffect ); check_size( BaseEffect, 12 );

// sizeof 48 
struct Affix
{
	begin_( Affix );
	char*		Name;					_(Name);					// 0
	union{ int	EndMarker;				
	AFFIX_EFFECT Effects; };			_(Effects);					// 4
	int			MinEffectValue;			_(MinEffectValue);			// 8
	int			MaxEffectValue;			_(MaxEffectValue);			// 12
	int			QualityLevel;			_(QualityLevel);			// 16
	ITEM_AFFIX	item;					_(item);					// 20
	int			ExcludedCombinations;	_(ExcludedCombinations);	// 24 если в префиксе 16, а в постфиксе 1 (или наоборот), то не комбинируется
	int			DoubleChance;			_(DoubleChance);			// 28
	int			NotCursed;				_(NotCursed);				// 32
	int			MinGoldValue;			_(MinGoldValue);			// 36
	int			MaxGoldValue;			_(MaxGoldValue);			// 40
	int			Multiplier;				_(Multiplier);				// 44
};
end_( Affix ); reflect( Affix ); check_size( Affix , 48 );
