// Copyright (C) 2019 Sony PCL Inc.
// All Rights Reserved.
#pragma once


#define M4SDK_TARGET_COUNT	(2)		//!< �ǐՑΏۂ̐� 
#define M4SDK_CAMERA_COUNT	(2)		//!< �ǐՑΏۂ��Ƃ̃J������ 


//! @struct M4POINT64F
typedef struct
{
	double	x;						//!< �z���O���t�B�[���_����̐�������[mm] 
	double	y;						//!< �z���O���t�B�[���_����̐�������[mm] 
} M4POINT64F;


//! @enum M4TARGETSTATE
typedef enum
{
	M4TargetState_Detecting,		//!< ���o�� 
	M4TargetState_Tracking,			//!< �ǐՒ� 
} M4TARGETSTATE;


//! @struct M4TARGETSTATUS
typedef struct
{
	ULONGLONG		TimeStamp;		//!< �X�V����[us]
	M4TARGETSTATE	State;			//!< �ǐՑΏۂ̏�� 
	M4POINT64F		Position;		//!< �ǐՑΏۂ̈ʒu 
	M4POINT64F		RawPosition;	//!< �ǐՑΏۂ̈ʒu�i�␳�Ȃ��j 
} M4TARGETSTATUS, *LPM4TARGETSTATUS;


//! @enum M4RESULT
typedef enum
{
	M4Result_OK = 0,				//!< ���� 
	M4Result_Fail,					//!< ���s 
	M4Result_InvalidSequence,		//!< �s���ȌĂяo���菇 
	M4Result_InvalidParameter,		//!< �s���ȃp�����[�^ 
	M4Result_NoMemory,				//!< �������[�s�� 
	M4Result_NoResource,			//!< ���\�[�X�s�� 
	M4Result_TimeOut,				//!< ���Ԑ؂� 
} M4RESULT;


typedef void (CALLBACK *M4FILTERCALLBACK)(ULONG TargetIndex, LPM4TARGETSTATUS pFilterIn, LPM4TARGETSTATUS pFilterOut, LPVOID pArg);
