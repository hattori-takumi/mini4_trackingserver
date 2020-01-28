// Copyright (C) 2019 Sony PCL Inc.
// All Rights Reserved.
#pragma once
#include "M4Types.h"

#ifdef M4TRACKING_EXPORTS
#define M4TRACKING_API extern "C"
#else
#define M4TRACKING_API extern "C" __declspec(dllimport)
#endif


//! @file M4Tracking.h
//! @brief �~�j�l��SDK 
//! 
//! @section �r���h��
//! �~�j�l��SDK�̃A�v���P�[�V�����v���O�����́A�ȉ��̊J�������g�p���č쐬���܂��B
//! - Microsoft Windows 10 64bit
//! - Microsoft Visual Studio 2017
//! .
//! �~�j�l��SDK���g�p����A�v���P�[�V�����v���O�����́A�w�b�_�[�t�@�C��M4Tracking.h���C���N���[�h���Ă��������B
//! �܂��C���|�[�g���C�u����M4Tracking.lib�������N���܂��B
//! @code
//! #include <Windows.h>
//! #include "M4Tracking.h"
//! #pragma comment(lib, "M4Tracking.lib")
//! @endcode
//! ���t�@�C���ւ̃p�X��Visual Studio�̃v���W�F�N�g�ɐݒ肵�Ă��������B
//! 
//! @section ���s��
//! �~�j�l��SDK�͈ȉ��̃I�y���[�e�B���O�V�X�e���œ��삵�܂��B
//! - Microsoft Windows 10 64bit
//! .
//! �ȉ��̃f�o�C�X�h���C�o���C���X�g�[�����Ă��������B
//! - OMRON Sentech�Ђ̃J�����h���C�o
//! .
//! �쐬�����A�v���P�[�V�����v���O�����Ƌ��ɁA�ȉ���dll��z�z����K�v������܂��B
//! - M4Tracking.dll
//! - AHTracking.dll
//! - AHLog.dll
//! - filter-libs-dll.dll
//! - HSVCL_200.dll
//! - MC1IF.dll
//! - PortMC1.dll
//! .
//!
//! @section �g�p�菇
//! @subsection sub1 �P�DAPI�̎g�p���J�n����
//! �A�v���P�[�V�����v���O�����̋N�����Ɉ��Ăяo���܂��B
//! - M4SDK_Open()
//! .
//! 
//! @subsection sub2 �Q�D�Q�[�����J�n����
//! �~�j�l��̌��o�ƒǐՂ��J�n���܂��B
//! - M4SDK_StartTracking()
//! .
//! 
//! @subsection sub3 �R�D�}���b�g/�p�b�N�̈ʒu���擾����
//! �A�v���P�[�V�����͒���I�ɉ��L�֐����Ăяo���ă~�j�l��̍ŐV���W���擾���܂��B
//! �ǐՒ��̃~�j�l������X�g�����ꍇ�́A�����I�ɍČ��o�����݂܂��B
//! - M4SDK_GetTargetStatus()
//! .
//! 
//! @subsection sub4 �S�D�Q�[�����I������
//! �~�j�l��̒ǐՂ��I�����܂��B
//! �ēx�Q�[�����J�n����ꍇ�́A(�Q)�ɖ߂�܂��B
//! - M4SDK_StopTracking()
//! .
//! 
//! @subsection sub5 �T�DAPI�̎g�p���I������
//! �A�v���P�[�V�����v���O�����̏I�����Ɉ��Ăяo���܂��B
//! - M4SDK_Close()
//! .
//!


//! 
//! @brief SDK�̎g�p���J�n���܂�
//! 
//! �A�v���P�[�V�����v���O�����̊J�n���ɌĂяo���Ă��������B
//! @a pszIniFile �ɂ͐ݒ�t�@�C���̃t�@�C���p�X��^���܂��B
//! @a pszIniFile �����΃p�X�ŗ^����ꂽ�ꍇ�́A�J�����g�t�H���_����_�ƂȂ�܂��B
//! @a pszIniFile ������0�̕�����܂��� NULL �ŗ^����ꂽ�ꍇ�́A CameraSettings.ini ���g�p����܂��B
//! @param [in] pszIniFile �ݒ�t�@�C��
//! @return �I���R�[�h
//! @sa M4SDK_Close()
//! 
M4TRACKING_API M4RESULT M4SDK_Open(LPCWSTR pszInifile);

//! 
//! @brief SDK�̎g�p���I�����܂�
//! 
//! �A�v���P�[�V�����v���O�����̏I�����ɌĂяo���Ă��������B
//! SDK���g�p�������\�[�X��������܂��B
//! @return �I���R�[�h
//! 
M4TRACKING_API M4RESULT M4SDK_Close(void);


M4TRACKING_API M4RESULT M4SDK_GetMarkerPitch(double *pWidth, double *pHeight);


M4TRACKING_API M4RESULT M4SDK_SetPredictionFilter(M4FILTERCALLBACK pFilter, LPVOID pArg);


//! 
//! @brief �~�j�l��̌��o�E�ǐՂ��J�n���܂�
//! 
//! �{�֐��̌Ăяo���ȍ~ M4SDK_StopTracking() ���Ăяo���܂ł̊ԁA
//! �A�v���P�[�V�����v���O������ M4SDK_GetTargetStatus() �����I�ɌĂяo�����ƂŁA
//! �~�j�l��̍ŐV�ʒu���擾���邱�Ƃ��ł��܂��B
//! @return �I���R�[�h
//! 
M4TRACKING_API M4RESULT M4SDK_StartTracking(void);

//! 
//! @brief �~�j�l��̌��o�E�ǐՂ��~���܂�
//! 
//! M4SDK_GetTargetStatus() ����M�҂���Ԃ������ꍇ�A���Y�֐��͒����ɒ��f���� M4Result_TimeOut ��Ԃ��܂��B
//! @return �I���R�[�h
//! @sa M4SDK_StartTracking()
//! 
M4TRACKING_API M4RESULT M4SDK_StopTracking(void);

//! 
//! @brief �~�j�l��̏�Ԃ��擾���܂�
//!
//! @a Timeout �ɂ̓^�C���A�E�g�l���~���b�P�ʂŎw�肵�܂��B
//! �w�肵�����Ԃ܂łɃJ��������f�[�^����M���Ȃ������ꍇ�A�{�֐��� M4Result_TimeOut ��Ԃ��܂��B
//! ������ @a Timeout �� INFINITE(0xFFFFFFFF) ���w�肵���ꍇ�́A�f�[�^��������܂Ń^�C���A�E�g�����ɑ҂������܂��B
//! �܂��A��M�҂��̊Ԃ� M4SDK_StopTracking() ���Ăяo�����ꍇ�A M4Result_TimeOut �ŏI�����܂��B 
//! @attention
//! �ȉ��̗��R�ɂ��A��Ԃ��擾�ł��鎞�ԊԊu�͈��ɂ͂Ȃ�܂���B
//! - �����~�j�l��𕡐��̃J�����ŒǐՂ��Ă��� 
//! - �����J�����Ԃ̃t���[���͓������Ă��Ȃ� 
//! - �J��������f�[�^����M����ƁA�{�֐��͌Ăяo�����Ɍ��ʂ�Ԃ� 
//! .
//! �܂��A�{�֐��̕�����̌Ăяo���ɑ΂��������ʂ��擾�����\�������邽�߁A�K�v�ɉ����� 
//! @a pStatus �Ɋ܂܂��^�C���X�^���v�̒l���m�F���Ă��������B 
//! @param [in] TargetIndex �Q�̃~�j�l��̂����A�ǂ���̏�����邩�w�肵�܂� 
//! @param [out] pStatus �ǐՑΏۂ̍ŐV��񂪊i�[����܂�
//! @param [in] Timeout �ő�҂����Ԃ��~���b�P�ʂŎw�肵�܂�
//! @return �I���R�[�h
//! @sa M4TARGETSTATUS
//! 
M4TRACKING_API M4RESULT M4SDK_GetTargetStatus(ULONG TargetIndex, LPM4TARGETSTATUS pStatus, ULONG Timeout);


M4TRACKING_API M4RESULT M4SDK_GetCourseFile(ULONG TargetIndex, LPWSTR pszFilePath, int cchFilePath);
