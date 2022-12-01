#pragma once // インクルードガード

#include <stdbool.h>

// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct node_
	{
		node_* left;
		node_* right;
		int key;
		char value[256];
	}node;

	typedef struct
	{
		node* root;
	}tree;

	// 2分木の初期化
	void initialize(tree* t);

	// 使用メモリの全解放
	void finalize(tree* t);

	// keyの値を見てノードを追加する(同じキーの物が合ったら上書き)
	bool add(tree* t, int key, const char* value);

	// keyの値を見てノードを検索して、値を取得する
	const char* find(const tree* t, int key);

	// keyの小さな順にコールバック関数funcを呼び出す
	void search(const tree* t, void (*func)(const node* p));

	// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
} // extern "C"
#endif
