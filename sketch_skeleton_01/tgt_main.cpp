/*
 TGTソースファイル
 * File:   tgt_main.cpp
 * Author: H.Seiyama
 */

/* インクルード宣言 */
#include "sys_type.h"
#include "tgt_type.h"

/* 定数定義 */
#define TGT_TIME_1000MS		( 1000 )											// 1000ms

/* スタティック変数定義 */
ST_SYS_TIME stS_Dtime_10ms;														// 10ms周期タスク用タイマ
ST_SYS_TIME stS_Dtime_100ms;													// 100ms周期タスク用タイマ

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

