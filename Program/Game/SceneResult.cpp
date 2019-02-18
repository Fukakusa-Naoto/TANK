//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   SceneResult.cpp
//!
//! @brief  SceneResultのソースファイル
//!
//! @date   
//!
//! @author 深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "SceneResult.h"
#include "GameMain.h"


//定数の定義================================================================





//データ型の定義============================================================
struct Tag_Score
{
	int type1;
	int type2;
	int type3;
	int type4;
	int type5;
	int type6;
	int type7;
};
typedef struct Tag_Score Score;


//関数のプロトタイプ宣言====================================================




// グローバル変数の定義 ====================================================
extern Score g_score;

//画像
HGRP g_resultImage[10];

//数字
HGRP g_resultNumber[16];

//表示タイミングを計る時間
int g_resultTimer;

// 関数の定義 ==============================================================
//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeResult(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------

	//画像の読み込み
	g_resultImage[0] = LoadGraph("Resources/Images/Result.png");

	//数字画像の読み込み
	LoadDivGraph("Resources/Images/resultNumber.png", 16, 4, 4, 45, 45, g_resultNumber);

	//時間
	g_resultTimer = 0;
}



//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateResult(void)
{
	//ローカル変数の宣言------------------------------------------------
	


	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------

	//カウント
	g_resultTimer++;

	//リザルト画面の操作
	if (((g_lastInputState&g_currentInputState)&PAD_INPUT_10) != 0)	//スペースキーが押される
	{
		if ((g_resultTimer>0)&&(g_resultTimer < 60))
		{
			g_resultTimer = 60;
		}

		if ((g_resultTimer>60) && (g_resultTimer < 120))
		{
			g_resultTimer = 120;
		}

		if ((g_resultTimer>120) && (g_resultTimer < 180))
		{
			g_resultTimer = 180;
		}

		if ((g_resultTimer>180) && (g_resultTimer < 240))
		{
			g_resultTimer = 240;
		}

		if ((g_resultTimer>240) && (g_resultTimer < 300))
		{
			g_resultTimer = 300;
		}

		if ((g_resultTimer>300) && (g_resultTimer < 360))
		{
			g_resultTimer = 360;
		}

		if ((g_resultTimer>360) && (g_resultTimer < 420))
		{
			g_resultTimer = 420;
		}

		if (g_resultTimer>420)
		{
			//シーンの切り替え
			ChangeScene(SCENE_TITLE);	//タイトルへ
		}

	}

}



//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderResult(void)
{
	//ローカル変数の宣言------------------------------------------------
	int total;



	//ローカル変数の初期化----------------------------------------------
	total = g_score.type1 + g_score.type2 + g_score.type3 + g_score.type4 + g_score.type5 + g_score.type6 + g_score.type7;



	//処理の内容--------------------------------------------------------

	//背景の描画
	DrawGraph(0, 0, g_resultImage[0], TRUE);

	//スコアの描画
	//type1
	if (g_resultTimer >= 60)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 100, g_resultNumber[g_score.type1 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 100, g_resultNumber[g_score.type1 % 10], TRUE);
	}
	//type2
	if (g_resultTimer >= 120)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 140, g_resultNumber[g_score.type2 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 140, g_resultNumber[g_score.type2 % 10], TRUE);
	}
	//type3
	if (g_resultTimer >= 180)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 180, g_resultNumber[g_score.type3 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 180, g_resultNumber[g_score.type3 % 10], TRUE);
	}
	
	//type4
	if (g_resultTimer >= 240)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 220, g_resultNumber[g_score.type4 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 220, g_resultNumber[g_score.type4 % 10], TRUE);
	}

	//type5
	if (g_resultTimer >= 300)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 260, g_resultNumber[g_score.type5 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 260, g_resultNumber[g_score.type5 % 10], TRUE);
	}

	//type6
	if (g_resultTimer >= 360)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 300, g_resultNumber[g_score.type6 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 300, g_resultNumber[g_score.type6 % 10], TRUE);
	}

	//type7
	if (g_resultTimer >= 420)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X, 340, g_resultNumber[g_score.type7 / 10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 15, 340, g_resultNumber[g_score.type7 % 10], TRUE);
	}

	//合計
	if (g_resultTimer >= 480)
	{
		//十の桁
		DrawGraph(SCREEN_CENTER_X + 30, 420, g_resultNumber[total/10], TRUE);
		//一の桁
		DrawGraph(SCREEN_CENTER_X + 45, 420, g_resultNumber[total % 10], TRUE);
	}

}



//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeResult(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//画像データの削除
	InitGraph();


}
