/*
 TGTソースファイル
 * File:   tgt_main.cpp
 * Author: H.Seiyama
 */

/* インクルード宣言 */
#include "sys_type.h"
#include "tgt_type.h"

/* 定数定義 */
#define TGT_PIN_LED1		( 12 )												// LED1
#define TGT_PIN_LED2		( 13 )												// LED2
#define TGT_PIN_TEST1		( 7 )												// TEST1
#define TGT_PIN_TEST2		( 8 )												// TEST2
#define TGT_INTR0			( 0 )												// INTR0
#define TGT_INTR1			( 1 )												// INTR1

#define TGT_TIME_1000MS		( 1000 )											// 1000ms

/* スタティック変数定義 */
ST_SYS_TIME	stS_Dtime_led1;														// LED1点灯用タイマ
ST_SYS_TIME	stS_Dtime_led2;														// LED2点灯用タイマ

volatile u_short	usS_Cinterrupt_ext0;										// 外部割込み回数(ext0)
volatile u_char		ubS_Fevent_ext0;											// 外部割込みイベント
volatile u_char		ubS_Fevent_ext1;											// 外部割込みイベント

/* プロトタイプ宣言 */
static void tgt_intr_external0( void );											// 外部割込み0関数(ext0)
static void tgt_intr_external1( void );											// 外部割込み1関数(ext1)

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
	usS_Cinterrupt_ext0 = 0;													// 外部割込み回数(ext0)
	ubS_Fevent_ext0 = OFF;														// 外部割込みイベント(ext0)
	ubS_Fevent_ext1 = OFF;														// 外部割込みイベント(ext1)

	/* デジタル入出力の設定 */
	pinMode( TGT_PIN_LED1, OUTPUT );
	pinMode( TGT_PIN_LED2, OUTPUT );
	pinMode( TGT_PIN_TEST1, OUTPUT );
	pinMode( TGT_PIN_TEST2, OUTPUT );

	digitalWrite( TGT_PIN_LED1, HIGH );											// LED1消灯
	digitalWrite( TGT_PIN_LED2, HIGH );											// LED2消灯
	digitalWrite( TGT_PIN_TEST1, LOW );											// TEST1 LOW
	digitalWrite( TGT_PIN_TEST2, LOW );											// TEST2 LOW

	/* 外部割込みの設定 */
	attachInterrupt( TGT_INTR0, tgt_intr_external0, FALLING );
	attachInterrupt( TGT_INTR1, tgt_intr_external1, FALLING );

	/* シリアル通信の設定 */
	Serial.begin( 9600 );

	/* タイマ停止処理 */
	sys_lib_StopTimer( &stS_Dtime_led1 );
	sys_lib_StopTimer( &stS_Dtime_led2 );
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
	u_char ubW_Dcheck_timer;

	/* 外部割込みイベント(ext0)がONの場合 */
	if( ubS_Fevent_ext0 == ON ){
		ubS_Fevent_ext0 = OFF;													// 外部割込みイベント(ext0)
		
		/* タイマ開始処理 */
		sys_lib_StartTimer( &stS_Dtime_led1 );
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}

	/* タイマー満了確認処理 */
	ubW_Dcheck_timer = sys_lib_CheckTimer( &stS_Dtime_led1, TGT_TIME_1000MS );
	/* タイマが満了している場合 */
	if( ubW_Dcheck_timer == ON ){
		digitalWrite( TGT_PIN_LED1, HIGH );										// LED1消灯
		
		/* タイマ停止処理 */
		sys_lib_StopTimer( &stS_Dtime_led1 );
	}
	/* 上記以外 */
	else{
		digitalWrite( TGT_PIN_LED1, LOW );										// LED1点灯
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

	/* 外部割込みイベント(ext1)がONの場合 */
	if( ubS_Fevent_ext1 == ON ){
		ubS_Fevent_ext1 = OFF;													// 外部割込みイベント(ext1)
		
		/* タイマ開始処理 */
		sys_lib_StartTimer( &stS_Dtime_led2 );
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}

	/* タイマー満了確認処理 */
	ubW_Dcheck_timer = sys_lib_CheckTimer( &stS_Dtime_led2, TGT_TIME_1000MS );
	/* タイマが満了している場合 */
	if( ubW_Dcheck_timer == ON ){
		digitalWrite( TGT_PIN_LED2, HIGH );										// LED2消灯
		
		/* タイマ停止処理 */
		sys_lib_StopTimer( &stS_Dtime_led2 );
	}
	/* 上記以外 */
	else{
		digitalWrite( TGT_PIN_LED2, LOW );										// LED2点灯
	}

	/* ピン出力の遅延を確認 */
	digitalWrite( TGT_PIN_TEST1, HIGH );										// TEST1 HIGH
	delayMicroseconds( 100 );													// Delay 100us
	digitalWrite( TGT_PIN_TEST1, LOW );											// TEST1 LOW
	digitalWrite( TGT_PIN_TEST1, HIGH );										// TEST1 HIGH
	digitalWrite( TGT_PIN_TEST1, LOW );											// TEST1 LOW
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
	digitalWrite( TGT_PIN_TEST2, HIGH );										// TEST2 HIGH
	delayMicroseconds( 100 );													// Delay 100us
	digitalWrite( TGT_PIN_TEST2, LOW );											// TEST2 LOW
	Serial.print( "usS_Cinterrupt_ext0 = " );
	digitalWrite( TGT_PIN_TEST2, HIGH );										// TEST2 HIGH
	Serial.println( usS_Cinterrupt_ext0 );
	digitalWrite( TGT_PIN_TEST2, LOW );											// TEST2 LOW
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
 * 外部割込み0関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_intr_external0( void ){
	usS_Cinterrupt_ext0++;														// 外部割込み回数(ext0)
	ubS_Fevent_ext0 = ON;														// 外部割込みイベント(ext0)
}

/*
 * 外部割込み1関数
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_intr_external1( void ){
	usS_Cinterrupt_ext0 = 0;													// 外部割込み回数(ext0)
	ubS_Fevent_ext1 = ON;														// 外部割込みイベント(ext1)
}

