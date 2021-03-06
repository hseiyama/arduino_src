/*
 TGTソースファイル
 * File:   tgt_main.cpp
 * Author: H.Seiyama
 */

/* インクルード宣言 */
#include "sys_type.h"
#include "tgt_type.h"

/* 定数定義 */
#define TGT_PIN_SW1			( 2 )												// SW1

#define TGT_TIME_1000MS		( 1000 )											// 1000ms
#define TGT_FILTER_COUNT	( 3 )												// フィルタ確定回数

/* 型宣言 */
typedef struct tag_ST_TGT_FILTER{
	u_char ub_level;															// フィルタ値
	u_char ub_edge;																// エッジ判定値
	u_char ub_count;															// フィルタ確定用カウンタ
	u_char ub_direct;															// ポート値
} ST_TGT_FILTER;																// ST_TGT_FILTER

/* スタティック変数定義 */
ST_SYS_TIME		stS_Dtime_10ms;													// 10ms周期タスク用タイマ
ST_SYS_TIME		stS_Dtime_100ms;												// 100ms周期タスク用タイマ
ST_TGT_FILTER	stS_Dfilter_sw1;												// SW1フィルタ情報

/* プロトタイプ宣言 */
static void tgt_lib_InitFilter( ST_TGT_FILTER*, u_char );						// フィルタ情報初期化処理
static void tgt_lib_GetFilter( u_char, ST_TGT_FILTER*, u_char );				// フィルタ情報取得処理

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
	pinMode( TGT_PIN_SW1, INPUT );

	/* フィルタ情報初期化処理 */
	tgt_lib_InitFilter( &stS_Dfilter_sw1, HIGH );

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
	u_char ubW_Dcheck_timer;

	/* フィルタ情報取得処理 */
	tgt_lib_GetFilter( TGT_PIN_SW1, &stS_Dfilter_sw1, TGT_FILTER_COUNT );

	/* フィルタ情報のエッジ判定値がONである場合 */
	if( stS_Dfilter_sw1.ub_edge == ON ){
		/* シリアル通信の出力 */
		Serial.print( "stS_Dfilter_sw1.ub_level = " );
		Serial.println( stS_Dfilter_sw1.ub_level );
	}
	/* 上記以外 */
	else{
		/* 処理なし */
	}

	/* タイマー満了確認処理 */
	ubW_Dcheck_timer = sys_lib_CheckTimer( &stS_Dtime_10ms, TGT_TIME_1000MS );
	/* タイマが満了している場合 */
	if( ubW_Dcheck_timer == ON ){
		/* シリアル通信の出力 */
		Serial.println( "tgt_task_main_10ms" );
		
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
		Serial.println( "tgt_task_main_100ms" );
		
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
	Serial.println( "tgt_task_main_1000ms" );
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

