#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// 2分木の初期化
void initialize(tree* t)
{
	if (t == NULL) return;

	t->root = NULL;
}

static void release_recursive(node* n)
{
	if (n == NULL) return;

	if (n->right) 
	{
		release_recursive(n->right);
		n->right = NULL;
	}

	if (n->left)
	{
		release_recursive(n->left);
		n->left = NULL;
	}

	free(n);
}

// 使用メモリの全解放
void finalize(tree* t)
{
	if (t == NULL) return;

	release_recursive(t->root);
	t->root = NULL;
}

static node* generate(int key, const char* value)
{
	node* p = (node*)malloc(sizeof(node));

	if (p == NULL) return NULL;

	p->key = key;
	int n = (int)strlen(value);
	memcpy(p->value, value, strlen(value)+1);

	p->left = p->right = NULL;

	return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
	if (t == NULL) return false;

	node* p = generate(key, value);
	if (p == NULL) return false;// メモリ確保できなかった。

	if (t->root == NULL) 
	{
		t->root = p;
		return true;
	}

	node* t_data = t->root;
	
	while (true) 
	{
		if (t_data->key > key)
		{
			if (!t_data->left)
			{
				t_data->left = p;
				break;
			}

			t_data = t_data->left;
		}

		else if (t_data->key < key)
		{
			if (!t_data->right)
			{
				t_data->right = p;
				break;
			}

			t_data = t_data->right;
		}

		else
		{
			memcpy(t_data, p, sizeof(node));
			free(p);
			break;
		}
	}
	// Todo: t->rootの下にkeyの値の大小でleftかrightを切り替えながらpを追加する処理を実装する
	return true;
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	if (!t || !t->root)return NULL;

	node* t_data = t->root;

	while (t_data)
	{
		if (t_data->key > key) t_data = t_data->left;
	
		else if(key > t_data->key) t_data = t_data->right;
		
		else return t_data->value;
	}
	// ToDo: 実装する
	return NULL;
}

// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p))
{
	// ToDo: 実装する
	if (!t || !t->root)return;

	asc_call(t->root, func);
}

void asc_call(const node* t_data, void (*func)(const node* p)) 
{
	if (!t_data->left && !t_data->right)
	{
		func(t_data);
		return;
	}

	if (t_data->left) asc_call(t_data->left, func);
	
	func(t_data);

	if (t_data->right) asc_call(t_data->right, func);
}