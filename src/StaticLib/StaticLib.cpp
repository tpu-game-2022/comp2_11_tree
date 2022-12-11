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
	if (t==NULL) return;

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
	if (!t) return false;

	node* p = generate(key, value);
	if (!p) return false;// メモリ確保できなかった。

	if (!t->root) 
	{
		t->root = p;
		return true;
	}

	node* root_data = t->root;

	while (true)
	{
		if (key < root_data->key)
		{
			if (!root_data->left)
			{
				root_data->left = p;
				break;
			}
			root_data = root_data->left;
		}
		else if (key >root_data->key)
		{
			if (!root_data->right)
			{
				root_data->right =p;
				break;
			}
			root_data = root_data->right;
		}
		else
		{
			memcpy_s(root_data->value, sizeof(root_data->value),value, sizeof(root_data->value));
			free(p);
			break;
		}
			
	}
	return true;
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	if (!t->root || !t)return NULL;
	node* root_data;
	root_data = t->root;
	if (root_data->key == key)
	{
		return root_data->value;
	}

	while (root_data!=NULL )
	{
		if (key < root_data->key)
		{
			if (!root_data->left)return NULL;
			root_data = root_data->left;
		}

		else if (key > root_data->key)
		{
			if (!root_data->right)return NULL;
			root_data = root_data->right;
		}

		else if (key == root_data->key)
			return root_data->value;
	}

	return NULL;

}

// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p))
{
	if (!t || !t->root)return ;

	if (t->root->left)
	{
		exe_small(t->root->left, func);
	}
	func(t->root);

	if (t->root->right)
	{
		exe_small(t->root->right, func);
	}
}

void exe_small(const node* root_data, void (func)(const node* p))
{
	if (!root_data->left && !root_data->right)
	{
		func(root_data);
		return;

	}
	if (root_data->left)
	{
		exe_small(root_data->left, func);
	}
	func(root_data);

	if (root_data->right)
	{
		exe_small(root_data->right, func);
	}
}