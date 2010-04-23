#include "Lapidem_Math.h"

Lapidem_Math* Lapidem_Math::GetInstance( )
{
	static Lapidem_Math instance;
	return &instance;
}

//////////////////////////////////////////////
// Integer math.
//////////////////////////////////////////////
int Lapidem_Math::Add2Ints( int i1, int i2 )
{
	_asm {
		mov eax, i1
			add eax, i2
	}
}

int Lapidem_Math::Sub2Ints( int i1, int i2 )
{
	_asm {
		mov eax, i1
			sub eax, i2
	}
}

int Lapidem_Math::Mult2Ints( int i1, int i2 )
{
	// - - - - - - - - 
	// TODO:: Fix.
	// - - - - - - - - 

	//_asm {
	//	mov eax, i1
	//	mul eax, i2
	//}
	return 0;
}

int Lapidem_Math::Div2Ints( int i1, int i2 )
{
	// - - - - - - - - 
	// TODO:: Fix.
	// - - - - - - - - 

	//_asm {
	//	mov eax, i1
	//	div eax, i2
	//}
	return 0;
}

//////////////////////////////////////////////
// Unsigned int math.
//////////////////////////////////////////////
u32 Lapidem_Math::Add2UInts( u32 u1, u32 u2 )
{
	_asm {
		mov eax, u1
			add eax, u2
	}
}

u32 Lapidem_Math::Sub2UInts( u32 u1, u32 u2 )
{
	_asm {
		mov eax, u1
			add eax, u2
	}
}

u32 Lapidem_Math::Mult2UInts( u32 u1, u32 u2 )
{
	// - - - - - - - - 
	// TODO:: Fix.
	// - - - - - - - - 

	//_asm {
	//	mov eax, u1
	//	add eax, u2
	//}
	return 0;
}

u32 Lapidem_Math::Div2UInts( u32 u1, u32 u2 )
{
	_asm {
		mov eax, u1
			add eax, u2
	}
}

//////////////////////////////////////////////
// Float math.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Physics.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Vector math.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Random numbers.
//////////////////////////////////////////////
void Lapidem_Math::SeedRandomNumberGenerator( long lSeed )
{
	if( lSeed ) idum = lSeed;
	else idum = -time_t( NULL );

	if( 1000 < idum ) idum = -idum;
	else if( -1000 < idum ) idum = idum - 22261048;
}

long Lapidem_Math::ran1( )
{
	int j;
	long k;
	static long iy = 0;
	static long iv[NTAB];

	if( idum <= 0 || !iy )
	{
		if( -( idum ) < 1 ) idum = 1;
		else idum = -( idum );

		for( j = NTAB + 7; j >= 0; j-- )
		{
			k      = ( idum ) / IQ;
			idum   = IA * ( idum - k * IQ ) - IR * k;

			if( idum < 0 ) idum = idum + IM;
			if( j < NTAB ) iv[j] = idum;
		}

		iy = iv[0];
	}

	k = ( idum ) / IQ;
	idum = IA * ( idum - k * IQ ) - IR * k;
	if( idum < 0 ) idum = idum + IM;

	j       = iy / NDIV;
	iy      = iv[j];
	iv[j]   = idum;

	return iy;
}

float Lapidem_Math::fran1( )
{
	float temp = float( AM * ran1( ) );

	if( temp > RNMX ) return float( RNMX );
	else return temp;
}

float Lapidem_Math::RandomFloat( float flLow, float flHigh )
{
	if( idum == 0 )
		SeedRandomNumberGenerator( 0 );

	float fl = fran1( );
	return ( fl * ( flHigh - flLow ) ) + flLow;
}

long Lapidem_Math::RandomLong( long lLow, long lHigh )
{
	if( idum == 0 )
		SeedRandomNumberGenerator( 0 );

	ul32 maxAcceptable;
	ul32 x = lHigh - lLow + 1;
	ul32 n;

	if( x <= 0 || MAX_RAND_RANGE < x - 1 )
		return lLow;

	maxAcceptable = MAX_RAND_RANGE - ( ( MAX_RAND_RANGE + 1 ) % x );

	do { n = ran1( ); }
	while ( n > maxAcceptable );

	return lLow + ( n% x );
}

//////////////////////////////////////////////
// Misc math.
//////////////////////////////////////////////
int Lapidem_Math::IncInt( int i )
{
	_asm {
		mov eax, i
			inc eax
	}
}

int Lapidem_Math::DecInt( int i )
{
	_asm { 
		mov eax, i
			dec eax
	}
}