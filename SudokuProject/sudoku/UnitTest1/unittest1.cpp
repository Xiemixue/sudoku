#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku/sudoku.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(order_request1) //测试正确的命令
		{
			using namespace std;
			int argc = 3;
			char* argv[3];
			argv[1] = "-c";
			argv[2] = "100000";
			int realvalue = IdentifyOrder(argc, argv);
			int expectvalue = 100000;
			Assert::AreEqual(expectvalue, realvalue);
		}
		TEST_METHOD(order_request2) //测试命令参数个数错误的命令
		{
			using namespace std;
			int argc = 4;
			char* argv[4];
			argv[1] = "-c";
			argv[2] = "100000";
			argv[3] = "asd";
			int realvalue = IdentifyOrder(argc, argv);
			int expectvalue = 0;
			Assert::AreEqual(expectvalue, realvalue);
		}
		TEST_METHOD(order_request3) //测试生成数独个数超过范围的错误命令
		{
			using namespace std;
			int argc = 3;
			char* argv[3];
			argv[1] = "-c";
			argv[2] = "1000000000";
			int realvalue = IdentifyOrder(argc, argv);
			int expectvalue = 0;
			Assert::AreEqual(expectvalue, realvalue);
		}
		TEST_METHOD(order_request4) //测试包含非法字符的错误命令
		{
			using namespace std;
			int argc = 3;
			char* argv[3];
			argv[1] = "-c";
			argv[2] = "100we$#%0";
			int realvalue = IdentifyOrder(argc, argv);
			int expectvalue = 0;
			Assert::AreEqual(expectvalue, realvalue);
		}
		TEST_METHOD(order_request5) //测试文件路径无效的错误命令
		{
			using namespace std;
			int argc = 3;
			char* argv[3];
			argv[1] = "-s";
			argv[2] = "D:\sudoku_puzzle.txt";
			int realvalue = IdentifyOrder(argc, argv);
			int expectvalue = 0;
			Assert::AreEqual(expectvalue, realvalue);
		}
		
		TEST_METHOD(check) //检查生成和求解的数独是否满足条件
		{
			char file[100] = "E:\sudoku.txt";
			int realvalue = CheckResult(file);
			int expectvalue = 1;
			Assert::AreEqual(expectvalue, realvalue);

		}
	};

	
}