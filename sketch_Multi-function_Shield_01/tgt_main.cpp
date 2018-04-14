/*
 TGTソースファイル
 * File:   tgt_main.cpp
 * Author: H.Seiyama
 */

/* インクルード宣言 */
#include "sys_type.h"
#include "tgt_type.h"

/* 定数定義 */
#define TGT_TIME_1000MS				( 1000 )									// 1000ms

#define TGT_PIN_IN_SW1				( A1 )										// SW1
#define TGT_PIN_IN_SW2				( A2 )										// SW2
#define TGT_PIN_IN_SW3				( A3 )										// SW3

#define TGT_PIN_OUT_LED1			( 13 )										// LED1
#define TGT_PIN_OUT_LED2			( 12 )										// LED2
#define TGT_PIN_OUT_LED3			( 11 )										// LED3
#define TGT_PIN_OUT_LED4			( 10 )										// LED4

#define TGT_PIN_OUT_BUZZER			( 3 )										// BUZZER

#define TGT_PIN_ADC_VR1				( 0 )										// VR1

#define TGT_PIN_OUT_7SEG_LATCH		( 4 )										// 7SEG_LATCH
#define TGT_PIN_OUT_7SEG_CLOCK		( 7 )										// 7SEG_CLOCK
#define TGT_PIN_OUT_7SEG_DATA		( 8 )										// 7SEG_DATA

/* スタティック変数定義 */
static ST_SYS_TIME stS_Dtime_10ms;												// 10ms周期タスク用タイマ
static ST_SYS_TIME stS_Dtime_100ms;												// 100ms周期タスク用タイマ

static const u_char aubS_DdisplayTable[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90, 0x00 };
																				// 7SEG表示用テーブル
static const u_char aubS_DdisplayBuffer[] = { 0xF1, 0xF2, 0xF4, 0xF8 };			// 7SEG表示用バッファ
static u_char ubS_Ctiming7SEG;													// 7SEG表示用カウンタ

/* プロトタイプ宣言 */

/*
 * TGT初期化関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_init( void ){
	/* デジタル出力関数 */
	digitalWrite( TGT_PIN_OUT_LED1, HIGH );										// LED1
	digitalWrite( TGT_PIN_OUT_LED2, HIGH );										// LED2
	digitalWrite( TGT_PIN_OUT_LED3, HIGH );										// LED3
	digitalWrite( TGT_PIN_OUT_LED4, HIGH );										// LED4
	digitalWrite( TGT_PIN_OUT_BUZZER, HIGH );									// BUZZER

	/* デジタル入出力の設定 */
	pinMode( TGT_PIN_IN_SW1, INPUT );											// SW1
	pinMode( TGT_PIN_IN_SW2, INPUT );											// SW2
	pinMode( TGT_PIN_IN_SW3, INPUT );											// SW3
	pinMode( TGT_PIN_OUT_LED1, OUTPUT );										// LED1
	pinMode( TGT_PIN_OUT_LED2, OUTPUT );										// LED2
	pinMode( TGT_PIN_OUT_LED3, OUTPUT );										// LED3
	pinMode( TGT_PIN_OUT_LED4, OUTPUT );										// LED4
	pinMode( TGT_PIN_OUT_BUZZER, OUTPUT );										// BUZZER

	pinMode( TGT_PIN_OUT_7SEG_LATCH, OUTPUT );									// 7SEG_LATCH
	pinMode( TGT_PIN_OUT_7SEG_CLOCK, OUTPUT );									// 7SEG_CLOCK
	pinMode( TGT_PIN_OUT_7SEG_DATA, OUTPUT );									// 7SEG_DATA
	ubS_Ctiming7SEG = 0;														// 7SEG表示用カウンタ

	/* シリアル通信の設定 */
	Serial.begin( 9600 );

	/* タイマ開始処理 */
	sys_lib_StartTimer( &stS_Dtime_10ms );
	sys_lib_StartTimer( &stS_Dtime_100ms );
}

/*
 * 1ms割込みTGTメイン関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_intr_main_1ms( void ){
}

/*
 * 10ms周期タスクTGTメイン関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_task_main_10ms( void ){
	u_char ubW_Dvalue_sw1;														// SW1
	u_char ubW_Dvalue_sw2;														// SW2
	u_char ubW_Dvalue_sw3;														// SW3
	u_short	usW_Dvalue_vr1;														// VR1
	u_char ubW_Dcheck_timer;

	/* デジタル入力関数 */
	ubW_Dvalue_sw1 = digitalRead( TGT_PIN_IN_SW1 );								// SW1
	ubW_Dvalue_sw2 = digitalRead( TGT_PIN_IN_SW2 );								// SW2
	ubW_Dvalue_sw3 = digitalRead( TGT_PIN_IN_SW3 );								// SW3

	/* アナログ入力関数 */
	usW_Dvalue_vr1 = analogRead( TGT_PIN_ADC_VR1 );								// VR1

	/* デジタル出力関数 */
	digitalWrite( TGT_PIN_OUT_LED1, ubW_Dvalue_sw1 );							// LED1
	digitalWrite( TGT_PIN_OUT_LED2, ubW_Dvalue_sw2 );							// LED2
	digitalWrite( TGT_PIN_OUT_LED3, ubW_Dvalue_sw3 );							// LED3
	digitalWrite( TGT_PIN_OUT_BUZZER, ubW_Dvalue_sw3 );							// BUZZER

	/* タイマー満了確認処理 */
	ubW_Dcheck_timer = sys_lib_CheckTimer( &stS_Dtime_10ms, TGT_TIME_1000MS );
	/* タイマが満了している場合 */
	if( ubW_Dcheck_timer == ON ){
		/* シリアル通信の出力 */
		Serial.println( "tgt_task_main_10ms" );
		
		/* シリアル通信の出力 */
		Serial.print( "    VR1 = " );
		Serial.println( usW_Dvalue_vr1, DEC );
		
		/* タイマ開始処理 */
		sys_lib_StartTimer( &stS_Dtime_10ms );
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}
}

/*
 * 100ms周期タスクTGTメイン関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_task_main_100ms( void ){
	u_char ubW_Dcheck_timer;

	/* タイマー満了確認処理 */
	ubW_Dcheck_timer = sys_lib_CheckTimer( &stS_Dtime_100ms, TGT_TIME_1000MS );
	/* タイマが満了している場合 */
	if( ubW_Dcheck_timer == ON ){
		/* シリアル通信の出力 */
//		Serial.println( "tgt_task_main_100ms" );
		
		/* タイマ開始処理 */
		sys_lib_StartTimer( &stS_Dtime_100ms );
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}
}

/*
 * 1000ms周期タスクTGTメイン関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_task_main_1000ms( void ){
	/* シリアル通信の出力 */
//	Serial.println( "tgt_task_main_1000ms" );
//	Serial.println( "" );
}

/*
 * IDLEタスクTGTメイン関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_task_main_idle( void ){
}

/*
 * HardTime周期タスクTGTメイン関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
void tgt_task_main_hard_tm( void ){
	/* デジタル出力関数 */
	digitalWrite( TGT_PIN_OUT_LED4, LOW );										// LED4
	switch( ubS_Ctiming7SEG ){
	case 0:
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, LOW );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayTable[ 9 ] );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayBuffer[ 0 ] );
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, HIGH );
		ubS_Ctiming7SEG = 1;													// 7SEG表示用カウンタ
		break;
	case 1:
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, LOW );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayTable[ 1 ] );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayBuffer[ 1 ] );
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, HIGH );
		ubS_Ctiming7SEG = 2;													// 7SEG表示用カウンタ
		break;
	case 2:
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, LOW );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayTable[ 7 ] );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayBuffer[ 2 ] );
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, HIGH );
		ubS_Ctiming7SEG = 3;													// 7SEG表示用カウンタ
		break;
	case 3:
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, LOW );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayTable[ 3 ] );
		shiftOut( TGT_PIN_OUT_7SEG_DATA, TGT_PIN_OUT_7SEG_CLOCK, MSBFIRST, aubS_DdisplayBuffer[ 3 ] );
		digitalWrite( TGT_PIN_OUT_7SEG_LATCH, HIGH );
		ubS_Ctiming7SEG = 0;													// 7SEG表示用カウンタ
		break;
	default:
		ubS_Ctiming7SEG = 0;													// 7SEG表示用カウンタ
		break;
	}
	/* デジタル出力関数 */
	digitalWrite( TGT_PIN_OUT_LED4, HIGH );										// LED4
}
