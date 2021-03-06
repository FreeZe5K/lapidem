//////////////////////////////////////////////////////////////////////////
// If you have to use math functions, use ones out of here. 
// This will do all the optimizations for you.
//
// It's still in progress... Don't hate!
//                                                          - Mike
//////////////////////////////////////////////////////////////////////////
#pragma once 

#include <ctime>
#include <cmath>
#include <cfloat>
#include <cstdlib>

#define ARGB( a, r, g, b )       D3DCOLOR_ARGB( a, r, g, b ) 

#define RAND_INT(min,max)        (((rand()/(int)RAND_MAX)*((max)-(min)))+(min))
#define RAND_FLOAT(min,max)      (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

#define MAX_RAND_RANGE           0x7FFFFFFFUL
#define EPSILON                  0.00001f
#define PI                       3.141592653589732
#define IA                       16807
#define IM                       2147483647
#define IQ                       127773
#define IR                       2836
#define NTAB                     32
#define NDIV                     (1+(IM-1)/NTAB)

#define AM                       (1.0/IM)
#define EPS                      1.2e-7
#define RNMX                     (1.0-EPS)

typedef int             s32;
typedef unsigned int    u32;
typedef bool            s1;
typedef unsigned char   u1;
typedef float           f32;
typedef unsigned long   ul32;

static long idum = 0;

struct tVector2D
{
	f32 _x;
	f32 _y;

	tVector2D operator+( const tVector2D& _v )
	{
		tVector2D _add = { _x + _v._x, _y + _v._y };
		return _add;
	}

	tVector2D operator-( const tVector2D& _v )
	{
		tVector2D _sub = { _x - _v._x, _y - _v._y };
		return _sub;
	}

	tVector2D operator*( const f32 _f )
	{
		tVector2D _mult = { _x * _f, _y * _f };
		return _mult;
	}

	tVector2D operator/( const f32 _f )
	{
		tVector2D _div = { _x / _f, _y / _f };
		return _div;
	}
};

typedef union TVECTOR
{
	float e[4];
	struct { float x, y, z, w; };
}*LPTVECTOR;

class Lapidem_Math
{
private:
	Lapidem_Math( )   { /* DO NOTHING */ }
	~Lapidem_Math( )  { /* DO NOTHING */ }
	Lapidem_Math( const Lapidem_Math& );
	Lapidem_Math& operator=( const Lapidem_Math& );

public:
	static Lapidem_Math* GetInstance( );

	/////////////////////////
	// Integer math.
	/////////////////////////
	int Add2Ints( int, int );
	int Sub2Ints( int, int );
	int Mult2Ints( int, int );
	int Div2Ints( int, int );

	/////////////////////////
	// Unsigned int math.
	/////////////////////////
	u32 Add2UInts( u32, u32 );
	u32 Sub2UInts( u32, u32 );
	u32 Mult2UInts( u32, u32 );
	u32 Div2UInts( u32, u32 );

	/////////////////////////
	// Float math.
	/////////////////////////
	float Add2Floats( f32, f32 );
	float Sub2Floats( f32, f32 );
	float Mult2Floats( f32, f32 );
	float Div2Floats( f32, f32 );

	bool IsEqual( float, float );
	bool IsZero( float );
	float Max( float, float );
	float Min( float, float );
	float Degrees_To_Radians( float );
	float Radians_To_Degrees( float );

	/////////////////////////
	// Physics.
	/////////////////////////
	// - Gravity.

	/////////////////////////
	// Vector math.
	/////////////////////////
	float VectorLengthSq( TVECTOR );
	float VectorLength( TVECTOR );
	float DotProduct( tVector2D, tVector2D );
	float AngleBetweenVectors( tVector2D, tVector2D );
	tVector2D Vector2DRotate( tVector2D, float );
	tVector2D Vector2DNormalize( tVector2D );

	/////////////////////////
	// Bit manipulation.
	/////////////////////////

	/////////////////////////
	// Logical manipulation.
	/////////////////////////

	/////////////////////////
	// Random numbers.
	/////////////////////////
	bool RandomBoolean();
	void SeedRandomNumberGenerator( long lSeed = 0 );
	long RandomLong( long, long );
	float RandomFloat( float, float );
	long ran1( );
	float fran1( );

	/////////////////////////
	// Misc math.
	/////////////////////////
	int IncInt( int );
	u32 IncUInt( u32 );
	int DecInt( int );
	u32 DecUInt( u32 );
};