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

static bool insert(node *parent, node *p)
{
	if (p->key < parent->key) {
		if (parent->left == NULL) {
			parent->left = p;
			return (parent->left != NULL);
		}
		return insert(parent->left, p);
	}
	else if (parent->key < p->key) {
		if (parent->right == NULL) {
			parent->right = p;
			return (parent->right != NULL);
		}
		return insert(parent->right, p);
	}

	// 値が同じ時は、Valueを上書きする
	strcpy_s(parent->value, 256, p->value);
	free(p);
	return true;
}


// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
	if (t == NULL) return false;

	node* p = generate(key, value);
	if (p == NULL) return false;// メモリ確保できなかった。

	if (t->root == NULL) {
		t->root = p;
		return true;
	}

	return insert(t->root, p);
}

static const char* find(const node* p, int key)
{
	if (p == NULL) return NULL;

	if (p->key == key) return p->value;

	return find((key < p->key) ? p->left : p->right, key);
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	if (t == NULL) return NULL;

	return find(t->root, key);
}

static void search(const node* p, void (*func)(const node* p))
{
	if (p == NULL) return;

	search(p->left, func);  // 左側を実効(p->leftのNULLチェックした方が高速)
	func(p);                // 自分自身を実効
	search(p->right, func);	// 右側を実効

}
// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p))
{
	if (t == NULL) return;

	search(t->root, func);
}