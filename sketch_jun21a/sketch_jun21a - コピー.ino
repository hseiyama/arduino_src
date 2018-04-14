#include <MsTimer2.h>

#define LED1	13
#define LED2	12
#define SW1		7
#define VR1		0
#define PWM1	9

#define CYCLE_TIME			1000
#define SYS_TIMER_UNABLE	0xFFFFFFFF

#define SYS_TIMER_STOP		0
#define SYS_TIMER_BEFORE	1
#define SYS_TIMER_JUST		2
#define SYS_TIMER_AFTER		3

typedef unsigned char	u_char;
//typedef unsigned short	u_short;
typedef unsigned long	u_long;

static u_long ulS_Ctimer_1ms;
static u_long ulS_Ctimer_sys;

static u_char ubS_Dstate;
static u_long ulS_Ctimer_sts;

#if 0
void sys_StartTimer( u_long* pulW_Dtimer ) {
	pulW_Dtimer = ulS_Ctimer_sys;
}

void sys_StopTimer( u_long* pulW_Dtimer ) {
	pulW_Dtimer = SYS_TIMER_UNABLE;
}

u_char sys_CheckTimer( u_long* pulW_Dtimer, u_long ulW_Dtimer_max ) {
	u_char ubW_Dcheck_result;

	if( *pulW_Dtimer == SYS_TIMER_UNABLE ){
		ubW_Dcheck_result = SYS_TIMER_STOP;
	}
	else if( *pulW_Dtimer < ulW_Dtimer_max ){
		ubW_Dcheck_result = SYS_TIMER_BEFORE;
	}
	else if( *pulW_Dtimer == ulW_Dtimer_max ){
		ubW_Dcheck_result = SYS_TIMER_JUST;
	}
	else{
		ubW_Dcheck_result = SYS_TIMER_AFTER;
	}
	
	return ubW_Dcheck_result;
}
#endif

void sys_main_1ms() {
	return;
}

void sys_main_5ms() {
	u_char	ubW_Dvalue_sw1;
	u_short	usW_Dvalue_vr1;
	u_char	ubW_Dvalue_pwm1;

	usW_Dvalue_vr1 = analogRead( VR1 );
	ubW_Dvalue_pwm1 = usW_Dvalue_vr1 >> 2;
	analogWrite( PWM1, ubW_Dvalue_pwm1 );

	digitalWrite( LED1, HIGH );

	switch( ubS_Dstate ){
	case HIGH:
//		sys_StartTimer( &ulS_Ctimer_sts );
		ubS_Dstate = LOW;
		break;
	case LOW:
	default:
		ubS_Dstate = HIGH;
		break;
	}
	digitalWrite( LED1, ubS_Dstate );

	ubW_Dvalue_sw1 = digitalRead( SW1 );
	if( ubW_Dvalue_sw1 == LOW ){
		Serial.print( "VR1 = 0x" );
		Serial.println( usW_Dvalue_vr1 );
		Serial.print( "ulS_Ctimer_1ms = " );
		Serial.println( ulS_Ctimer_1ms );
	}
	else{
		/* ‰½‚à‚µ‚È‚¢ */
	}

	return;
}

void interrupt_timer() {
	ulS_Ctimer_1ms++;
	ulS_Ctimer_sys++;
	sys_main_1ms();
}

void setup() {
	// put your setup code here, to run once:
	pinMode( LED1, OUTPUT );
	pinMode( LED2, OUTPUT );
	pinMode( SW1, INPUT );
	Serial.begin( 9600 );

	ulS_Ctimer_1ms = 0;
	ulS_Ctimer_sys = 0;
	MsTimer2::set( 1, interrupt_timer );
	MsTimer2::start();

	ubS_Dstate = HIGH;
}

void loop() {
	// put your main code here, to run repeatedly:

	if( ulS_Ctimer_1ms >= CYCLE_TIME ){
		ulS_Ctimer_1ms = 0;
		sys_main_5ms();
	}
	else{
		/* ‰½‚à‚µ‚È‚¢ */
	}
#if 0
	u_char	ubW_Dvalue_sw1;
	u_short	usW_Dvalue_vr1;
	u_char	ubW_Dvalue_pwm1;

	usW_Dvalue_vr1 = analogRead( VR1 );
	Serial.print( "VR1 = 0x" );
	Serial.println( usW_Dvalue_vr1, HEX );
	Serial.print( "ulS_Ctimer_1ms = " );
	Serial.println( ulS_Ctimer_1ms );

	ubW_Dvalue_sw1 = digitalRead( SW1 );
	if( ubW_Dvalue_sw1 == LOW ){
		Serial.println( usW_Dvalue_vr1 );
		ubW_Dvalue_pwm1 = usW_Dvalue_vr1 >> 2;
		Serial.println( ubW_Dvalue_pwm1 );
		analogWrite( PWM1, ubW_Dvalue_pwm1 );
	}
	else{
		/* ‰½‚à‚µ‚È‚¢ */
	}
//	digitalWrite( LED2, ubW_Dvalue_sw1 );

	digitalWrite( LED1, HIGH );
	delay( 1000 );
	digitalWrite( LED1, LOW );
	delay( 1000 );
#endif
}
