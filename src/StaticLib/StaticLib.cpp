#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// 2分木の初期化
void initialize(tree* t)
{
	if (!t) return;

	t->root = NULL;
}

static void release_recursive(node* n)
{
	if (!n) return;

	if (n->right) {
		release_recursive(n->right);
		n->right = NULL;
	}

	if (n->left) {
		release_recursive(n->left);
		n->left = NULL;
	}

	free(n);
}

// 使用メモリの全解放
void finalize(tree* t)
{
	if (!t) return;

	release_recursive(t->root);
	t->root = NULL;
}


static node* generate(int key, const char* value)
{
	node* p = (node*)malloc(sizeof(node));

	if (!p) return NULL;

	p->key = key;
	int n = (int)strlen(value);
	memcpy(p->value, value, strlen(value)+1);

	p->left = p->right = NULL;

	return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
	if (!t) return false;

	node* p = generate(key, value);
	if (!p) return false;// メモリ確保できなかった。

	if (!t->root) {
		t->root = p;
		return true;
	}

	// Todo: t->rootの下にkeyの値の大小でleftかrightを切り替えながらpを追加する処理を実装する
	node *data;
	data = t->root;
	while (true) {
		if (data->key > key) {
			if (!data->left) {
				data->left = p;
				break;
			}
			data = data->left;
		}
		else if(data->key < key) {
			if (!data->right) {
				data->right = p;
				break;
			}
			data = data->right;
		}
		else {
			memcpy(data, p, sizeof(node));
			free(p);
			break;
		}

	}
	return true;
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	// ToDo: 実装する
	if (!t || !t->root)return NULL;
	node* data;
	data = t->root;
	if (data->key == key) {
		return data->value;
	}
	while (data) {
		
		if (data->key > key) {
			if (!data->left)return NULL;
			data = data->left;
		}
		else {
			if (!data->right)return NULL;
			data = data->right;
		}
		if (data->key == key) {
			return data->value;
		}

	}
	return NULL;
}

// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p))
{
	// ToDo: 実装する
	if (!t || !t->root)return;
	
	if (t->root->left) {
		small_order(t->root->left, func);
	}
	func(t->root);

	if (t->root->right) {
		small_order(t->root->right, func);
	}
}

void small_order(const node* data, void (*func)(const node* p)) {
	if (!data->left && !data->right) {
		func(data);
		return;
	}
	if (data->left) {
		small_order(data->left, func);
	}
	func(data);

	if (data->right) {
		small_order(data->right, func);
	}
}