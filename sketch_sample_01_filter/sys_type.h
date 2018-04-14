/*
 SYSヘッダファイル
 * File:   sys_type.h
 * Author: H.Seiyama
 */

#ifndef __SYS_TYPE_H__
#define __SYS_TYPE_H__

/* インクルード宣言 */
#include "Arduino.h"

/* 定数定義 */
#define ON						( 1 )											// ON
#define OFF						( 0 )											// OFF

#define TRUE					true											// TRUE
#define FALSE					false											// FALSE

#define SYS_TASK_CYCLE_1MS		( 1 )											// 1ms
#define SYS_TASK_CYCLE_10MS		( 10 )											// 10ms
#define SYS_TASK_CYCLE_100MS	( 100 )											// 100ms
#define SYS_TASK_CYCLE_1000MS	( 1000 )										// 1000ms

/* 型宣言 */
typedef unsigned char	u_char;													// u_char
typedef unsigned short	u_short;												// u_short
typedef unsigned long	u_long;													// u_long
typedef signed char		s_char;													// s_char
typedef signed short	s_short;												// s_short
typedef signed long		s_long;													// s_long

typedef struct tag_ST_SYS_TIME{
	u_long ul_time;																// タイマ計測時間
} ST_SYS_TIME;																	// ST_SYS_TIME

/* プロトタイプ宣言 */
extern void		setup( void );													// セットアップ関数
extern void		loop( void );													// ループ関数
extern void		sys_lib_StartTimer( ST_SYS_TIME* );								// タイマ開始処理
extern void		sys_lib_StopTimer( ST_SYS_TIME* );								// タイマ停止処理
extern u_char	sys_lib_CheckTimer( ST_SYS_TIME*, u_long );						// タイマ満了確認処理

#endif /* __SYS_TYPE_H__ */

