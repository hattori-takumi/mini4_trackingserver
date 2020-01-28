// Copyright (C) 2019 Sony PCL Inc.
// All Rights Reserved.
#pragma once


#define M4SDK_TARGET_COUNT	(2)		//!< 追跡対象の数 
#define M4SDK_CAMERA_COUNT	(2)		//!< 追跡対象ごとのカメラ数 


//! @struct M4POINT64F
typedef struct
{
	double	x;						//!< ホモグラフィー原点からの水平距離[mm] 
	double	y;						//!< ホモグラフィー原点からの垂直距離[mm] 
} M4POINT64F;


//! @enum M4TARGETSTATE
typedef enum
{
	M4TargetState_Detecting,		//!< 検出中 
	M4TargetState_Tracking,			//!< 追跡中 
} M4TARGETSTATE;


//! @struct M4TARGETSTATUS
typedef struct
{
	ULONGLONG		TimeStamp;		//!< 更新時刻[us]
	M4TARGETSTATE	State;			//!< 追跡対象の状態 
	M4POINT64F		Position;		//!< 追跡対象の位置 
	M4POINT64F		RawPosition;	//!< 追跡対象の位置（補正なし） 
} M4TARGETSTATUS, *LPM4TARGETSTATUS;


//! @enum M4RESULT
typedef enum
{
	M4Result_OK = 0,				//!< 成功 
	M4Result_Fail,					//!< 失敗 
	M4Result_InvalidSequence,		//!< 不正な呼び出し手順 
	M4Result_InvalidParameter,		//!< 不正なパラメータ 
	M4Result_NoMemory,				//!< メモリー不足 
	M4Result_NoResource,			//!< リソース不足 
	M4Result_TimeOut,				//!< 時間切れ 
} M4RESULT;


typedef void (CALLBACK *M4FILTERCALLBACK)(ULONG TargetIndex, LPM4TARGETSTATUS pFilterIn, LPM4TARGETSTATUS pFilterOut, LPVOID pArg);
