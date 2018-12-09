// 数独游戏.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <windows.h>
#include <process.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <ctime>

using namespace std;


/*生成数独终局所需变量*/
int full_permutation_visit[40320]; //标记访问过的全排列
int row1[9]; //存储数独第一行数字1-9的排列
int row2to3_translation_sequence[2][3] = { { 0,3,6 },{ 0,6,3 } }; //2到3行的平移序列排序。row2to3_translation_sequence[][0]标记该中平移序列是否已访问
int row4to6_translation_sequence[6][4] = { { 0,2,5,8 },{ 0,2,8,5 },{ 0,5,2,8 },{ 0,5,8,2 },{ 0,8,2,5 },{ 0,8,5,2 } }; //4到6行的平移序列排序。row4to6_translation_sequence[][0]标记该中平移序列是否已访问
int row7to9_translation_sequence[6][4] = { { 0,1,4,7 },{ 0,1,7,4 },{ 0,4,1,7 },{ 0,4,7,1 },{ 0,7,1,4 },{ 0,7,4,1 } }; //7到9行的平移序列排序。row7to9_translation_sequence[][0]标记该中平移序列是否已访问


																													  /*求解数独所需变量*/
int sudoku[9][9]; //数独总体盘面
int locate[9][9]; //记录空白单元格的行、列位置
int blank[9];//记录每行空白单元格的数量
int sum; //总的空白单元格数
int visit[80][10]; //记录空白单元访问了哪些数字；1为不在访问范围内，0为未访问，2为已访问
ofstream out;

void GenerateSudokuEndings(int N); //生成数独终局
void SudokuSolver(int su[][9]); //求解数独
int Check(int x, int row, int col); //返回0表示不满足条件；返回1表示满足条件
void ProduceOneSudokuByTranslation(int seq1, int seq2, int seq3); //通过平移生成一个数独终局
void PrintOneSudoku(int is_first_sudoku);  //输出一个数独终局到sudoku.txt文件
 // int CheckResult(); 检查某个数独是否正确

/*
int CheckResult()
{
	int v[10], i, j;
	for (i = 0; i < 9; i++) //检查每行有无重复数字
	{
		memset(v, 0, sizeof(v));
		for (j = 0; j < 9; j++)
		{
			if (v[sudoku[i][j]] == 1) return 0;
			v[sudoku[i][j]] = 1;
		}
	}
	for (j = 0; j < 9; j++) //检查每列有无重复数字
	{
		memset(v, 0, sizeof(v));
		for (i = 0; i < 9; i++)
		{
			if (v[sudoku[i][j]] == 1) return 0;
			v[sudoku[i][j]] = 1;
		}
	}
	int ii, jj;
	for (i = 0; i < 7; i += 3) //检查每宫有无重复数字
	{
		for (j = 0; j < 7; j += 3)
		{
			memset(v, 0, sizeof(v));
			for (ii = i; ii < i + 3; ii++)
			{
				for (jj = j; jj < j + 3; jj++)
				{
					if (v[sudoku[ii][jj]] == 1) return 0;
					v[sudoku[ii][jj]] = 1;
				}
			}
		}
	}
	return 1;
}
*/
										   
void PrintOneSudoku(int is_first_sudoku)
{
	int ii, jj;
	if (is_first_sudoku != 1) out << endl << endl;
	for (ii = 0; ii < 9; ii++)
	{
		for (jj = 0; jj < 9; jj++)
		{
			if (jj < 8)
			{
				out << sudoku[ii][jj] << ' ';
			}
			else
			{
				out << sudoku[ii][jj];
			}
		}
		if (ii < 8) out << endl;
	}
}

void ProduceOneSudokuByTranslation(int seq1, int seq2, int seq3) //平移生成一个数独终局
{
	int i, j, t;
	for (i = 1; i < 3; i++) //平移2、3行
	{
		t = row2to3_translation_sequence[seq1][i];
		for (j = 0; j < 9; j++)
		{
			sudoku[i][j] = sudoku[0][(j + t) % 9];
		}
	}
	for (i = 3; i < 6; i++)  //平移4、5、6行
	{
		t = row4to6_translation_sequence[seq2][i - 2];
		for (j = 0; j < 9; j++)
		{
			sudoku[i][j] = sudoku[0][(j + t) % 9];
		}
	}
	for (i = 6; i < 9; i++)  //平移7、8、9行
	{
		t = row7to9_translation_sequence[seq3][i - 5];
		for (j = 0; j < 9; j++)
		{
			sudoku[i][j] = sudoku[0][(j + t) % 9];
		}
	}
}

void GenerateSudokuEndings(int N)
{
	int i, j, k, count = 0;
	int visit[10];
	memset(visit, 0, sizeof(visit));
	sudoku[0][0] = 8;//按题目要求将学号后两位的运算结果作为左上角填入的数字：（6+1）%9+1 =8
	visit[8] = 1;
	for (i = 0; i < 2 && count < N; i++)
	{
		for (j = 0; j < 6 && count < N; j++)
		{
			for (k = 0; k < 6 && count < N; k++)
			{
				for (int h2 = 1; h2 < 10 && count < N; h2++)
				{
					if (visit[h2] == 1) continue;
					visit[h2] = 1;
					sudoku[0][1] = h2;
					for (int h3 = 1; h3 < 10 && count < N; h3++)
					{
						if (visit[h3] == 1) continue;
						visit[h3] = 1;
						sudoku[0][2] = h3;
						for (int h4 = 1; h4 < 10 && count < N; h4++)
						{
							if (visit[h4] == 1) continue;
							visit[h4] = 1;
							sudoku[0][3] = h4;
							for (int h5 = 1; h5 < 10 && count < N; h5++)
							{
								if (visit[h5] == 1) continue;
								visit[h5] = 1;
								sudoku[0][4] = h5;
								for (int h6 = 1; h6 < 10 && count < N; h6++)
								{
									if (visit[h6] == 1) continue;
									visit[h6] = 1;
									sudoku[0][5] = h6;
									for (int h7 = 1; h7 < 10 && count < N; h7++)
									{
										if (visit[h7] == 1) continue;
										visit[h7] = 1;
										sudoku[0][6] = h7;
										for (int h8 = 1; h8 < 10 && count < N; h8++)
										{
											if (visit[h8] == 1) continue;
											visit[h8] = 1;
											sudoku[0][7] = h8;
											for (int h9 = 1; h9 < 10 && count < N; h9++)
											{
												if (visit[h9] == 1) continue;
												sudoku[0][8] = h9;
												ProduceOneSudokuByTranslation(i, j, k);
												//if (CheckResult() == 0) cout << "数独不正确" << endl << "数独序号："<< count<< endl;
												count += 1;
												PrintOneSudoku(count);
											}
											visit[h8] = 0;
										}
										visit[h7] = 0;
									}
									visit[h6] = 0;
								}
								visit[h5] = 0;
							}
							visit[h4] = 0;
						}
						visit[h3] = 0;
					}
					visit[h2] = 0;
				}
			}
		}
	}
	out.close();
	cout << "生成数独个数" << count << endl;
}


int Check(int x, int row, int col) //检查所填数字是否满足条件
{
	int i, j;
	for (j = 0; j < 9; j++) //检查该行有没有重复数字
	{
		if (sudoku[row][j] == x && j != col) return 0;
	}
	for (i = 0; i < 9; i++)
	{
		if (sudoku[i][col] == x && i != row) return 0;//检查该列有没有重复数字
	}
	int row1, col1;
	row1 = row / 3;
	col1 = col / 3;
	for (i = row1 * 3; i < 3 + row1 * 3; i++)
	{
		for (j = col1 * 3; j < 3 + col1 * 3; j++)
		{
			if (i == row && j == col) continue;
			if (sudoku[i][j] == x) return 0;//检查该宫有没有重复数字
		}
	}
	return 1;
}

void SudokuSolver(int su[][9])
{
	int i, j, x, flag = 0, s = 0; //flag=0表示没有合适的数字可填
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < blank[i]; j++)
		{
			flag = 0;
			for (x = 1; x < 10; x++)
			{
				if (visit[s][x] == 1 || visit[s][x] == 2) continue; //该数字已经存在或者已被访问了
				if (Check(x, i, locate[i][j]) == 1)
				{
					sudoku[i][locate[i][j]] = x;
					visit[s][x] = 2; //标记为已访问
					flag = 1;
					s += 1;
					break;
				}
			}
			if (flag == 0) //当前节点没有合适的数字可填，返回上一个结点，进行修改
			{
				for (int m = 1; m < 10; m++) //清除当前已访问过的数字
				{
					if (visit[s][m] == 2) visit[s][m] = 0;
				}
				sudoku[i][locate[i][j]] = 0;
				s -= 1;
				j -= 2;
				if (j < -1)
				{
					i -= 1;
					if (i < 0) i = 0;
					while (blank[i] <= 0)
					{
						i -= 1;
					}
					if (i < 0) i = 0;
					j = blank[i] - 2;
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//生成相对路径
	string filepath;
	filepath = argv[0];
	int l = filepath.length() - 10;
	filepath.erase(l);
	filepath += "sudoku.txt";
	
	int num = 0;
	clock_t start, finish;
	double totaltime;
	start = clock();
	int N = 0;
	if (argc != 3)
	{
		perror("命令参数个数出错,正确格式sudoku.exe -c 123或 sudoku.exe -s absolute_path of_puzzlefile\n");
		exit(0);
	}
	if (strcmp(argv[1], "-c") == 0)
	{
		string str = *(argv + 2); //取命令行第二个参数——数独终局个数
		int len = str.length();
		for (int i = 0; i < len; i++)
		{
			if (str[i] < '0' || str[i] > '9')
			{
				perror("第二个命令参数包含非数字字\n");
				exit(0);
			}
			N = N * 10 + str[i] - '0';
		}
		if (N > 1000000 || N < 1)
		{
			perror("数独终局个数超出范围\n");
			exit(0);
		}
		out.open(filepath, ios::trunc); //清空sudoku.txt的原有内容
		GenerateSudokuEndings(N);
	}
	else if (strcmp(argv[1], "-s") == 0)
	{
		ifstream f;
		f.open(argv[2]);
		out.open(filepath, ios::trunc);
		if (!f.is_open())
		{
			perror("文件路径名无效\n");
			exit(0);
		}
		string line;
		int i = 0, j = 0, k = 0, t = 0, x, ii, jj, kk, z;
		memset(locate, -1, sizeof(locate));
		memset(visit, 0, sizeof(visit));
		memset(blank, 0, sizeof(blank));
		while (getline(f, line))
		{
			if (int len = line.length() > 0)
			{
				for (j = 0, k = 0; j < 9; j++)
				{
					x = line[j * 2] - '0';
					sudoku[i][j] = x;
					if (x == 0)
					{
						locate[i][k] = j;
						k += 1;
					}
				}
				blank[t] = k;
				t += 1;
				i += 1;
				if (i == 9)
				{
					sum = 0;
					for (ii = 0; ii < 9; ii++)
					{
						for (jj = 0; jj < blank[ii]; jj++)
						{
							for (kk = 0; kk < 9; kk++)
							{
								z = sudoku[ii][kk];
								if (z != 0) visit[sum][z] = 1;
							}
							sum += 1;
						}
					}
					num += 1;
					SudokuSolver(sudoku); //求解数独
										  //if (CheckResult() == 0) cout << "数独不正确" << endl << "数独序号："<< count<< endl;
					PrintOneSudoku(num);
					i = 0;
					t = 0;
					memset(locate, -1, sizeof(locate));
					memset(visit, 0, sizeof(visit));
					memset(blank, 0, sizeof(blank));
				}
			}
		}
		f.close();
		out.close();
		cout << "求解数独个数：" << num << endl;
	}
	else
	{
		perror("该命令不存在，正确格式sudoku.exe -c 123或 sudoku.exe -s absolute_path of_puzzlefile\n");
		exit(0);
	}
	finish = clock();
	totaltime = ((double)(finish)-(double)start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
	return 0;
}