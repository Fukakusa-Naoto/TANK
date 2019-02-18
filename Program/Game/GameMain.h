//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  ゲーム関連のヘッダファイル
//!
//! @date   2017.7.31
//!
//! @author GF1.21.深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once




// 定数の定義 ==============================================================

// <システム> ----------------------------------------------------------
#define GAME_TITLE "ShootingGame"        // ゲームタイトル


// <画面> --------------------------------------------------------------
#define SCREEN_WIDTH    (640)                  // 画面の幅[pixel]
#define SCREEN_HEIGHT   (480)                  // 画面の高さ[pixel]

#define SCREEN_TOP      (0)                    // 画面の上端
#define SCREEN_BOTTOM   (SCREEN_HEIGHT)        // 画面の下端
#define SCREEN_LEFT     (0)                    // 画面の左端
#define SCREEN_RIGHT    (SCREEN_WIDTH)         // 画面の右端

#define SCREEN_CENTER_X (SCREEN_WIDTH  / 2)    // 画面の中央(X座標)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)    // 画面の中央(Y座標)

//ゲームに必要なシーンのIDを登録
enum tag_SceneId
{
	SCENE_NONE,		//なし
	SCENE_LOGO,		//ロゴ
	SCENE_TITLE,	//タイトル
	SCENE_READY,	//プレイ前準備
	SCENE_PLAYING,		//プレイ
	SCENE_RESULT	//結果
};
typedef enum tag_SceneId SceneId;



// 関数の宣言 ==============================================================

// ゲームの初期化処理
void InitializeGame(void);

// ゲームの更新処理
void UpdateGame(void);

// ゲームの描画処理
void RenderGame(void);

// ゲームの終了処理
void FinalizeGame(void);

//シーンの切り替え
void ChangeScene(SceneId nextSceneId);



//グローバル変数の宣言======================================================
//キーの入力状態
extern int g_currentInputState;	//現在

extern int g_lastInputState;	//前回
