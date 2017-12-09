/* 
 * delay utilite for STM8 family
 * COSMIC and SDCC
 * Terentiev Oleg
 * t.oleg@ymail.com
 */

#define F_CPU 16000000UL

#ifndef _UTIL_DELAY_H_
#define _UTIL_DELAY_H_ 1

#ifndef F_CPU
#warning F_CPU is not defined!
#endif

/* 
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 */

@inline static void _delay_cycl( unsigned short __ticks )
{
#if defined(__CSMC__)
/* COSMIC */
  #define T_COUNT(x) (( 16UL * x )-3)/3)
	// ldw X, __ticks ; insert automaticaly
	_asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", __ticks);
#else
 #error Unsupported Compiler!          /* Compiler defines not found */
#endif
}

@inline static void _delay_us( const unsigned short __us )
{
	_delay_cycl( (unsigned short)( T_COUNT(__us) );
}

@inline static void _delay_ms( unsigned short __ms )
{
	while ( __ms-- )
	{
		_delay_us( 250 );
		_delay_us( 250 );
		_delay_us( 250 );
		_delay_us( 250 );
	}
}

#endif