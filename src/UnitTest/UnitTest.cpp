#include "pch.h"
#include "CppUnitTest.h"

#include "../include/lib_func.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	static int s_table_num = 0;
	static char** s_table = NULL;

	void check_values(const node* p)
	{
		Assert::AreEqual(s_table[s_table_num++], p->value);
	}


	TEST_CLASS(基本チェック)
	{
	public:
		_CrtMemState sStartMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(一要素)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 0, "Hello world!"));
			Assert::AreEqual("Hello world!", find(&t, 0), "0のキーの値はHello world!");

			s_table_num = 0;
			s_table = new char* [1];
			s_table[0] = "Hello world!";
			search(&t, check_values);
			Assert::AreEqual(1, s_table_num, (const wchar_t*)("1要素のチェック"));
			delete[] s_table;

			finalize(&t);
		}

		TEST_METHOD(二要素)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 4, "world!"));
			Assert::IsTrue(add(&t, 3, "Hello"));
			Assert::AreEqual("Hello", find(&t, 3), "3のキーの値はHello");
			Assert::AreEqual("world!", find(&t, 4), "4のキーの値はworld!");

			s_table_num = 0;
			s_table = new char* [2];
			s_table[0] = "Hello";
			s_table[1] = "world!";
			search(&t, check_values);
			Assert::AreEqual(2, s_table_num, (const wchar_t*)("2要素のチェック"));
			delete[] s_table;
			 
			finalize(&t);
		}

		TEST_METHOD(同じ値は上書き)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 10, "world!"));
			Assert::IsTrue(add(&t, 10, "Hello"));
			Assert::AreEqual("Hello", find(&t, 10), "10のキーの値はHello");

			s_table_num = 0;
			s_table = new char* [1];
			s_table[0] = "Hello";
			search(&t, check_values);
			Assert::AreEqual(1, s_table_num, (const wchar_t*)("上書きしたので１要素しかない"));
			delete[] s_table;

			finalize(&t);
		}

		TEST_METHOD(３要素)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 6, "Hello"));
			Assert::IsTrue(add(&t, 7, "world"));
			Assert::IsTrue(add(&t, 8, "!"));
			Assert::AreEqual("Hello", find(&t, 6), "6のキーの値はHello");
			Assert::AreEqual("world", find(&t, 7), "7のキーの値はworld");
			Assert::AreEqual("!", find(&t, 8), "8のキーの値は!");

			s_table_num = 0;
			s_table = new char* [3];
			s_table[0] = "Hello";
			s_table[1] = "world";
			s_table[2] = "!";
			search(&t, check_values);
			Assert::AreEqual(3, s_table_num, (const wchar_t*)("3要素のチェック"));
			delete[] s_table;

			finalize(&t);
		}

		TEST_METHOD(追加順を入れ替えても同じ結果)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 8, "!"));
			Assert::IsTrue(add(&t, 7, "world"));
			Assert::IsTrue(add(&t, 6, "Hello"));
			Assert::AreEqual("Hello", find(&t, 6), "6のキーの値はHello");
			Assert::AreEqual("world", find(&t, 7), "7のキーの値はworld");
			Assert::AreEqual("!", find(&t, 8), "8のキーの値は!");

			s_table_num = 0;
			s_table = new char* [3];
			s_table[0] = "Hello";
			s_table[1] = "world";
			s_table[2] = "!";
			search(&t, check_values);
			Assert::AreEqual(3, s_table_num, (const wchar_t*)("3要素のチェック"));
			delete[] s_table;

			finalize(&t);
		}
	};


	TEST_CLASS(品質チェック)
	{
	public:
	public:
		_CrtMemState sStartMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(NULLチェック)
		{
			initialize(NULL);
			finalize(NULL);

			tree t;
			initialize(&t);
			Assert::IsNull(find(&t, -1000), (const wchar_t*)("何も登録していなければ検索しても何も得られない"));
			finalize(&t);
		}

		TEST_METHOD(異なる要素)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 1, "This is a pen"));
			Assert::IsNull(find(&t, 0), (const wchar_t*)("存在しないキーを検索しても何も得られない"));

			finalize(&t);
		}

		TEST_METHOD(大量の要素)
		{
			tree t;

			initialize(&t);
			const int NUM = 10000;
			for (int i = 0; i < NUM; i++) {
				char c[2] = { 'a' + (i%26) };
				add(&t, i, c);
			}

			for (int i = 0; i < NUM; i++) {
				Assert::AreEqual((char)('a'+(i % 26)), *find(&t, i), (const wchar_t*)("周期的にアルファベットが入出てくるはず"));
			}

			finalize(&t);
		}

		TEST_METHOD(２度の解放)
		{
			tree t;

			initialize(&t);
			Assert::IsTrue(add(&t, 2, "Release twice"));
			finalize(&t);
			finalize(&t);
		}
	};

}
