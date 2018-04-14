/*
 TGTヘッダファイル
 * File:   tgt_type.h
 * Author: H.Seiyama
 */

#ifndef __TGT_TYPE_H__
#define __TGT_TYPE_H__

/* プロトタイプ宣言 */
extern void tgt_init( void );													// TGT初期化関数
extern void tgt_intr_main_1ms( void );											// 1ms割込みTGTメイン関数
extern void tgt_task_main_10ms( void );											// 10ms周期タスクTGTメイン関数
extern void tgt_task_main_100ms( void );										// 100ms周期タスクTGTメイン関数
extern void tgt_task_main_1000ms( void );										// 1000ms周期タスクTGTメイン関数
extern void tgt_task_main_idle( void );											// IDLEタスクTGTメイン関数

#endif /* __TGT_TYPE_H__ */

