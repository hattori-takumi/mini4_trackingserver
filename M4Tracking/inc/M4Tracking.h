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
//! @brief ミニ四駆SDK 
//! 
//! @section ビルド環境
//! ミニ四駆SDKのアプリケーションプログラムは、以下の開発環境を使用して作成します。
//! - Microsoft Windows 10 64bit
//! - Microsoft Visual Studio 2017
//! .
//! ミニ四駆SDKを使用するアプリケーションプログラムは、ヘッダーファイルM4Tracking.hをインクルードしてください。
//! またインポートライブラリM4Tracking.libをリンクします。
//! @code
//! #include <Windows.h>
//! #include "M4Tracking.h"
//! #pragma comment(lib, "M4Tracking.lib")
//! @endcode
//! 両ファイルへのパスをVisual Studioのプロジェクトに設定してください。
//! 
//! @section 実行環境
//! ミニ四駆SDKは以下のオペレーティングシステムで動作します。
//! - Microsoft Windows 10 64bit
//! .
//! 以下のデバイスドライバをインストールしてください。
//! - OMRON Sentech社のカメラドライバ
//! .
//! 作成したアプリケーションプログラムと共に、以下のdllを配布する必要があります。
//! - M4Tracking.dll
//! - AHTracking.dll
//! - AHLog.dll
//! - filter-libs-dll.dll
//! - HSVCL_200.dll
//! - MC1IF.dll
//! - PortMC1.dll
//! .
//!
//! @section 使用手順
//! @subsection sub1 １．APIの使用を開始する
//! アプリケーションプログラムの起動時に一回呼び出します。
//! - M4SDK_Open()
//! .
//! 
//! @subsection sub2 ２．ゲームを開始する
//! ミニ四駆の検出と追跡を開始します。
//! - M4SDK_StartTracking()
//! .
//! 
//! @subsection sub3 ３．マレット/パックの位置を取得する
//! アプリケーションは定期的に下記関数を呼び出してミニ四駆の最新座標を取得します。
//! 追跡中のミニ四駆をロストした場合は、自動的に再検出を試みます。
//! - M4SDK_GetTargetStatus()
//! .
//! 
//! @subsection sub4 ４．ゲームを終了する
//! ミニ四駆の追跡を終了します。
//! 再度ゲームを開始する場合は、(２)に戻ります。
//! - M4SDK_StopTracking()
//! .
//! 
//! @subsection sub5 ５．APIの使用を終了する
//! アプリケーションプログラムの終了時に一回呼び出します。
//! - M4SDK_Close()
//! .
//!


//! 
//! @brief SDKの使用を開始します
//! 
//! アプリケーションプログラムの開始時に呼び出してください。
//! @a pszIniFile には設定ファイルのファイルパスを与えます。
//! @a pszIniFile が相対パスで与えられた場合は、カレントフォルダが基点となります。
//! @a pszIniFile が長さ0の文字列または NULL で与えられた場合は、 CameraSettings.ini が使用されます。
//! @param [in] pszIniFile 設定ファイル
//! @return 終了コード
//! @sa M4SDK_Close()
//! 
M4TRACKING_API M4RESULT M4SDK_Open(LPCWSTR pszInifile);

//! 
//! @brief SDKの使用を終了します
//! 
//! アプリケーションプログラムの終了時に呼び出してください。
//! SDKが使用したリソースを解放します。
//! @return 終了コード
//! 
M4TRACKING_API M4RESULT M4SDK_Close(void);


M4TRACKING_API M4RESULT M4SDK_GetMarkerPitch(double *pWidth, double *pHeight);


M4TRACKING_API M4RESULT M4SDK_SetPredictionFilter(M4FILTERCALLBACK pFilter, LPVOID pArg);


//! 
//! @brief ミニ四駆の検出・追跡を開始します
//! 
//! 本関数の呼び出し以降 M4SDK_StopTracking() を呼び出すまでの間、
//! アプリケーションプログラムは M4SDK_GetTargetStatus() を定期的に呼び出すことで、
//! ミニ四駆の最新位置を取得することができます。
//! @return 終了コード
//! 
M4TRACKING_API M4RESULT M4SDK_StartTracking(void);

//! 
//! @brief ミニ四駆の検出・追跡を停止します
//! 
//! M4SDK_GetTargetStatus() が受信待ち状態だった場合、当該関数は直ちに中断して M4Result_TimeOut を返します。
//! @return 終了コード
//! @sa M4SDK_StartTracking()
//! 
M4TRACKING_API M4RESULT M4SDK_StopTracking(void);

//! 
//! @brief ミニ四駆の状態を取得します
//!
//! @a Timeout にはタイムアウト値をミリ秒単位で指定します。
//! 指定した時間までにカメラからデータを受信しなかった場合、本関数は M4Result_TimeOut を返します。
//! ただし @a Timeout に INFINITE(0xFFFFFFFF) を指定した場合は、データが得られるまでタイムアウトせずに待ち続けます。
//! また、受信待ちの間に M4SDK_StopTracking() を呼び出した場合、 M4Result_TimeOut で終了します。 
//! @attention
//! 以下の理由により、状態を取得できる時間間隔は一定にはなりません。
//! - 同じミニ四駆を複数のカメラで追跡している 
//! - 複数カメラ間のフレームは同期していない 
//! - カメラからデータを受信すると、本関数は呼び出し元に結果を返す 
//! .
//! また、本関数の複数回の呼び出しに対し同じ結果が取得される可能性があるため、必要に応じて 
//! @a pStatus に含まれるタイムスタンプの値を確認してください。 
//! @param [in] TargetIndex ２つのミニ四駆のうち、どちらの情報を取るか指定します 
//! @param [out] pStatus 追跡対象の最新情報が格納されます
//! @param [in] Timeout 最大待ち時間をミリ秒単位で指定します
//! @return 終了コード
//! @sa M4TARGETSTATUS
//! 
M4TRACKING_API M4RESULT M4SDK_GetTargetStatus(ULONG TargetIndex, LPM4TARGETSTATUS pStatus, ULONG Timeout);


M4TRACKING_API M4RESULT M4SDK_GetCourseFile(ULONG TargetIndex, LPWSTR pszFilePath, int cchFilePath);
