/*
 TGTソースファイル
 * File:   tgt_main.cpp
 * Author: H.Seiyama
 */

/* インクルード宣言 */
#include "sys_type.h"
#include "tgt_type.h"
#include "EEPROM.h"

/* 定数定義 */
#define TGT_PIN_SW1			( 2 )												// SW1
#define TGT_PIN_SW2			( 3 )												// SW2
#define TGT_PIN_LED1		( 12 )												// LED1
#define TGT_PIN_PWM1		( 5 )												// PWM1
#define TGT_PIN_ADC1		( 0 )												// ADC1
#define TGT_E2P_PWM1		( 0 )												// E2P_PWM1

#define TGT_TIME_1000MS		( 1000 )											// 1000ms
#define TGT_TIME_BLINK		( 500 )												// 500ms
#define TGT_FILTER_COUNT	( 3 )												// フィルタ確定回数

enum tag_ENUM_TGT_STS{
	TGT_STS_NORMAL = 0,															// 状態:NORMAL
	TGT_STS_EDIT_ADC,															// 状態:EDIT_ADC
	TGT_STS_EDIT_SCM															// 状態:EDIT_SCM
};
enum tag_ENUM_TGT_EVT{
	TGT_EVT_NONE = 0,															// EVENT:NONE
	TGT_EVT_SW1_PUSH,															// EVENT:SW1_PUSH
	TGT_EVT_SW2_PUSH															// EVENT:SW2_PUSH
};

/* 型宣言 */
typedef struct tag_ST_TGT_FILTER{
	u_char ub_level;															// フィルタ値
	u_char ub_edge;																// エッジ判定値
	u_char ub_count;															// フィルタ確定用カウンタ
	u_char ub_direct;															// ポート値
} ST_TGT_FILTER;																// ST_TGT_FILTER

/* スタティック変数定義 */
static ST_TGT_FILTER	stS_Dfilter_sw1;										// SW1フィルタ情報
static ST_TGT_FILTER	stS_Dfilter_sw2;										// SW2フィルタ情報

static ST_SYS_TIME		stS_Dtime_blink;										// LED点滅用タイマ

static u_char			ubS_Dvalue_led1;										// LED1出力情報
static u_char			ubS_Dvalue_pwm1;										// PWM1出力情報
static u_char			ubS_Dstate_PwmControl;									// PWM制御状態
static u_char			ubS_Devent_PwmControl;									// PWM制御用EVENT

/* プロトタイプ宣言 */
static void		tgt_lib_InitFilter( ST_TGT_FILTER*, u_char );					// フィルタ情報初期化処理
static void		tgt_lib_GetFilter( u_char, ST_TGT_FILTER*, u_char );			// フィルタ情報取得処理

static void		tgt_sts_init_PwmControl( void );								// PWM制御初期化処理
static void		tgt_evt_event_PwmControl( void );								// PWM制御用EVENT判定処理
static void		tgt_sts_update_PwmControl( void );								// PWM制御更新処理
static void		tgt_sts_PwmControl_Normal( void );								// PWM制御状態:NORMAL
static void		tgt_sts_PwmControl_EditAdc( void );								// PWM制御状態:EDIT_ADC
static void		tgt_sts_PwmControl_EditScm( void );								// PWM制御状態:EDIT_SCM

static u_char	tgt_scm_GetNumber( u_long* );									// SCM数値取得処理

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
	/* デジタル入出力の設定 */
	pinMode( TGT_PIN_SW1, INPUT );												// SW1
	pinMode( TGT_PIN_SW2, INPUT );												// SW2
	pinMode( TGT_PIN_LED1, OUTPUT );											// LED1

	/* フィルタ情報初期化処理 */
	tgt_lib_InitFilter( &stS_Dfilter_sw1, HIGH );								// SW1フィルタ情報
	tgt_lib_InitFilter( &stS_Dfilter_sw2, HIGH );								// SW2フィルタ情報

	/* PWM制御初期化処理 */
	tgt_sts_init_PwmControl();

	/* シリアル通信の設定 */
	Serial.begin( 9600 );
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
	/* フィルタ情報取得処理 */
	tgt_lib_GetFilter( TGT_PIN_SW1, &stS_Dfilter_sw1, TGT_FILTER_COUNT );		// SW1フィルタ情報
	tgt_lib_GetFilter( TGT_PIN_SW2, &stS_Dfilter_sw2, TGT_FILTER_COUNT );		// SW2フィルタ情報

	/* PWM制御用EVENT判定処理 */
	tgt_evt_event_PwmControl();

	/* PWM制御更新処理 */
	tgt_sts_update_PwmControl();

	/* デジタル出力関数 */
	digitalWrite( TGT_PIN_LED1, ubS_Dvalue_led1 );								// LED1出力情報
	/* アナログ出力関数 */
	analogWrite( TGT_PIN_PWM1, ubS_Dvalue_pwm1 );								// PWM1出力情報
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
	Serial.print( "ubS_Dvalue_pwm1 = " );
	Serial.println( ubS_Dvalue_pwm1 );

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
 * フィルタ情報初期化処理
 *
 * 引数:
 * ST_TGT_FILTER *stW_Dfilter フィルタ情報のポインタ
 * u_char ubW_Dvalue          ポートの初期値
 *
 * 復帰値:
 * なし
 */
static void tgt_lib_InitFilter( ST_TGT_FILTER *stW_Dfilter, u_char ubW_Dvalue ){
	stW_Dfilter->ub_level = ubW_Dvalue;											// フィルタ値
	stW_Dfilter->ub_edge = OFF;													// エッジ判定値
	stW_Dfilter->ub_count = 0;													// フィルタ確定用カウンタ
	stW_Dfilter->ub_direct = ubW_Dvalue;										// ポート値
}

/*
 * フィルタ情報取得処理
 *
 * 引数:
 * u_char ubW_Dport           ポート番号
 * ST_TGT_FILTER *stW_Dfilter フィルタ情報のポインタ
 * u_char ubW_Dcount          フィルタ確定回数
 *
 * 復帰値:
 * なし
 */
static void tgt_lib_GetFilter( u_char ubW_Dport, ST_TGT_FILTER *stW_Dfilter, u_char ubW_Dcount ){
	stW_Dfilter->ub_edge = OFF;													// エッジ判定値
	stW_Dfilter->ub_direct = digitalRead( ubW_Dport );							// ポート値

	/* ポート値がフィルタ値と同じ場合 */
	if( stW_Dfilter->ub_direct == stW_Dfilter->ub_level ){
		stW_Dfilter->ub_count = 0;												// フィルタ確定用カウンタ
	}
	/* 上記以外 */
	else{
		stW_Dfilter->ub_count++;												// フィルタ確定用カウンタ
		
		/* フィルタ確定用カウンタがフィルタ確定回数以上である場合 */
		if( stW_Dfilter->ub_count >= ubW_Dcount ){
			stW_Dfilter->ub_level = stW_Dfilter->ub_direct;						// フィルタ値
			stW_Dfilter->ub_edge = ON;											// エッジ判定値
			stW_Dfilter->ub_count = 0;											// フィルタ確定用カウンタ
		}
		/* 上記以外 */
		else{
			/* 処理なし */
		}
	}
}

/*
 * PWM制御初期化処理
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_sts_init_PwmControl( void ){
	ubS_Dvalue_led1 = HIGH;														// LED1出力情報(消灯)
	/* EEPROM読込み関数 */
	ubS_Dvalue_pwm1 = EEPROM.read( TGT_E2P_PWM1 );								// PWM1出力情報
	ubS_Dstate_PwmControl = TGT_STS_NORMAL;										// PWM制御状態
	ubS_Devent_PwmControl = TGT_EVT_NONE;										// PWM制御用EVENT

	/* タイマ停止処理 */
	sys_lib_StopTimer( &stS_Dtime_blink );										// LED点滅用タイマ
}

/*
 * PWM制御用EVENT判定処理
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void	 tgt_evt_event_PwmControl( void ){
	ubS_Devent_PwmControl = TGT_EVT_NONE;										// EVENT:NONE

	/* SW1フィルタ情報のエッジ判定値がONで、			*/
	/* かつ、SW1フィルタ情報のフィルタ値がLOWである場合	*/
	if(( stS_Dfilter_sw1.ub_edge == ON   )
	&& ( stS_Dfilter_sw1.ub_level == LOW )){
		ubS_Devent_PwmControl = TGT_EVT_SW1_PUSH;								// EVENT:SW1_PUSH
	}
	/* 上記以外で										*/
	/* かつ、SW1フィルタ情報のエッジ判定値がONで、		*/
	/* かつ、SW1フィルタ情報のフィルタ値がLOWである場合	*/
	else if(( stS_Dfilter_sw2.ub_edge == ON   )
	&&      ( stS_Dfilter_sw2.ub_level == LOW )){
		ubS_Devent_PwmControl = TGT_EVT_SW2_PUSH;								// EVENT:SW2_PUSH
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}
}

/*
 * PWM制御更新処理
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_sts_update_PwmControl( void ){
	switch( ubS_Dstate_PwmControl ){											// PWM制御状態
	case TGT_STS_NORMAL:														// 状態:NORMAL
		/* PWM制御状態:NORMAL */
		tgt_sts_PwmControl_Normal();
		break;
	case TGT_STS_EDIT_ADC:														// 状態:EDIT_ADC
		/* PWM制御状態:EDIT_ADC */
		tgt_sts_PwmControl_EditAdc();
		break;
	case TGT_STS_EDIT_SCM:														// 状態:EDIT_SCM
		/* PWM制御状態:EDIT_SCM */
		tgt_sts_PwmControl_EditScm();
		break;
	default:
		/* TGT初期化関数 */
		tgt_init();
		break;
	}
}

/*
 * PWM制御状態:NORMAL
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_sts_PwmControl_Normal( void ){
	switch( ubS_Devent_PwmControl ){											// PWM制御用EVENT
	case TGT_EVT_SW1_PUSH:														// EVENT:SW1_PUSH
		/* シリアル通信の出力 */
		Serial.println( "[ STS_EDIT_ADC ]" );
		
		ubS_Dvalue_led1 = LOW;													// LED1出力情報(点灯)
		/* PWM制御状態の遷移 */
		ubS_Dstate_PwmControl = TGT_STS_EDIT_ADC;								// 状態:EDIT_ADC
		break;
	case TGT_EVT_SW2_PUSH:														// EVENT:SW2_PUSH
		/* シリアル通信の出力 */
		Serial.println( "[ STS_EDIT_SCM ]" );
		
		ubS_Dvalue_led1 = LOW;													// LED1出力情報(点灯)
		/* タイマ開始処理 */
		sys_lib_StartTimer( &stS_Dtime_blink );									// LED点滅用タイマ
		/* PWM制御状態の遷移 */
		ubS_Dstate_PwmControl = TGT_STS_EDIT_SCM;								// 状態:EDIT_SCM
		break;
	case TGT_EVT_NONE:															// EVENT:NONE
		/* 処理なし */
		break;
	default:
		/* TGT初期化関数 */
		tgt_init();
		break;
	}
}

/*
 * PWM制御状態:EDIT_ADC
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_sts_PwmControl_EditAdc( void ){
	u_short	usW_Dvalue_adc1;													// ADC1

	/* アナログ入力関数 */
	usW_Dvalue_adc1 = analogRead( TGT_PIN_ADC1 );								// ADC1
	ubS_Dvalue_pwm1 = ( u_char )(( usW_Dvalue_adc1 >> 2 ) & 0xFF );				// PWM1出力情報

	switch( ubS_Devent_PwmControl ){											// PWM制御用EVENT
	case TGT_EVT_SW1_PUSH:														// EVENT:SW1_PUSH
		/* シリアル通信の出力 */
		Serial.println( "[ STS_NORMAL ]" );
		
		ubS_Dvalue_led1 = HIGH;													// LED1出力情報(消灯)
		/* EEPROM読込み関数 */
		ubS_Dvalue_pwm1 = EEPROM.read( TGT_E2P_PWM1 );							// PWM1出力情報
		/* PWM制御状態の遷移 */
		ubS_Dstate_PwmControl = TGT_STS_NORMAL;									// 状態:NORMAL
		break;
	case TGT_EVT_SW2_PUSH:														// EVENT:SW2_PUSH
		/* シリアル通信の出力 */
		Serial.println( "EEPROM.write -> [ STS_NORMAL ]" );
		
		ubS_Dvalue_led1 = HIGH;													// LED1出力情報(消灯)
		/* EEPROM書込み関数 */
		EEPROM.write( TGT_E2P_PWM1, ubS_Dvalue_pwm1 );							// PWM1出力情報
		/* PWM制御状態の遷移 */
		ubS_Dstate_PwmControl = TGT_STS_NORMAL;									// 状態:NORMAL
		break;
	case TGT_EVT_NONE:															// EVENT:NONE
		/* 処理なし */
		break;
	default:
		/* TGT初期化関数 */
		tgt_init();
		break;
	}
}

/*
 * PWM制御状態:EDIT_SCM
 *
 * 引数:
 * なし
 *
 * 復帰値:
 * なし
 */
static void tgt_sts_PwmControl_EditScm( void ){
	u_char ubW_Dcheck_timer;													// タイマ満了確認結果
	u_char ubW_Drcode;															// SCM数値取得結果
	u_long ulW_Dvalue_scm;														// SCM取得数値

	/* SCM数値取得処理 */
	ubW_Drcode = tgt_scm_GetNumber( &ulW_Dvalue_scm );							// SCM取得数値
	/* SCM取得数値がある場合 */
	if( ubW_Drcode == ON ){
		ubS_Dvalue_pwm1 = ( u_char )( ulW_Dvalue_scm & 0xFF );					// PWM1出力情報
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}

	switch( ubS_Devent_PwmControl ){											// PWM制御用EVENT
	case TGT_EVT_SW1_PUSH:														// EVENT:SW1_PUSH
		/* シリアル通信の出力 */
		Serial.println( "[ STS_NORMAL ]" );
		
		ubS_Dvalue_led1 = HIGH;													// LED1出力情報(消灯)
		/* EEPROM読込み関数 */
		ubS_Dvalue_pwm1 = EEPROM.read( TGT_E2P_PWM1 );							// PWM1出力情報
		/* タイマ停止処理 */
		sys_lib_StopTimer( &stS_Dtime_blink );									// LED点滅用タイマ
		/* PWM制御状態の遷移 */
		ubS_Dstate_PwmControl = TGT_STS_NORMAL;									// 状態:NORMAL
		break;
	case TGT_EVT_SW2_PUSH:														// EVENT:SW2_PUSH
		/* シリアル通信の出力 */
		Serial.println( "EEPROM.write -> [ STS_NORMAL ]" );
		
		ubS_Dvalue_led1 = HIGH;													// LED1出力情報(消灯)
		/* EEPROM書込み関数 */
		EEPROM.write( TGT_E2P_PWM1, ubS_Dvalue_pwm1 );							// PWM1出力情報
		/* タイマ停止処理 */
		sys_lib_StopTimer( &stS_Dtime_blink );									// LED点滅用タイマ
		/* PWM制御状態の遷移 */
		ubS_Dstate_PwmControl = TGT_STS_NORMAL;									// 状態:NORMAL
		break;
	case TGT_EVT_NONE:															// EVENT:NONE
		/* タイマー満了確認処理 */
		ubW_Dcheck_timer = sys_lib_CheckTimer( &stS_Dtime_blink, TGT_TIME_BLINK );
																				// LED点滅用タイマ
		/* タイマが満了している場合 */
		if( ubW_Dcheck_timer == ON ){
			ubS_Dvalue_led1 = !ubS_Dvalue_led1;									// LED1出力情報(反転)
			
			/* タイマ開始処理 */
			sys_lib_StartTimer( &stS_Dtime_blink );								// LED点滅用タイマ
		}
		/* 上記以外 */
		else{
			/* 処理なし */
		}
		break;
	default:
		/* TGT初期化関数 */
		tgt_init();
		break;
	}
}

/*
 * SCM数値取得処理
 *
 * 引数:
 * u_long *ulW_Dvalue_scm SCM取得数値変数のポインタ
 *
 * 復帰値:
 * ON  取得数値あり
 * OFF 取得数値なし
 */
static u_char tgt_scm_GetNumber( u_long *ulW_Dvalue_scm ){
	u_char ubW_Drcode;															// 戻り値
	u_char ubW_Dlength_scm;														// 取得データ長
	u_char ubW_Ddata_1byte;														// 1byteデータ
	u_char ubW_Cindex;															// インデックス

	ubW_Drcode = OFF;															// 取得数値なし
	*ulW_Dvalue_scm = 0;														// SCM取得数値

	ubW_Dlength_scm = Serial.available();										// 取得データ長
	for( ubW_Cindex = 0; ubW_Cindex < ubW_Dlength_scm; ubW_Cindex++ ){
		ubW_Ddata_1byte = Serial.read();										// 1byteデータ
		
		/* 1byteデータが数字である場合 */
		if(( ubW_Ddata_1byte >= '0' )
		&& ( ubW_Ddata_1byte <= '9' )){
			*ulW_Dvalue_scm = ( ubW_Ddata_1byte - '0' ) + ( *ulW_Dvalue_scm * 10 );
																				// SCM取得数値
		}
		/* 上記以外 */
		else{
			*ulW_Dvalue_scm = *ulW_Dvalue_scm * 10;								// SCM取得数値
		}
		
		ubW_Drcode = ON;														// 取得数値あり
	}

	return ubW_Drcode;
}

