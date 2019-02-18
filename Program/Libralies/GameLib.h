//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameLib.h
//!
//! @brief  ゲーム制作用ライブラリのヘッダファイル
//!
//! @date   2015/6/10
//!
//! @author 高木 晋
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once




// ヘッダファイルの読み込み ================================================
#include <DxLib.h>

#include "DxLibHelper\GameGraphics.h"
#include "DxLibHelper\GameInput.h"
#include "DxLibHelper\GameMath.h"
#include "DxLibHelper\GameSystem.h"
#include "DxLibHelper\GameTime.h"

#include "Utilities\MemoryLeakDetector.h"




// 型の定義 ================================================================
typedef int HGRP;    // グラフィックハンドル     
typedef int HSND;    // サウンドハンドル
typedef int HFNT;    // フォントハンドル
typedef int HNET;    // ネットワークハンドル 
