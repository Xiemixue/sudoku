#pragma once
#include <fstream>
#include <windows.h>
#include <process.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

/*生成数独终局所需变量*/
int row2to3_translation_sequence[2][2] = { { 3,6 },{ 6,3 } }; //2到3行的平移序列排序。
int row4to6_translation_sequence[6][3] = { { 2,5,8 },{ 2,8,5 },{ 5,2,8 },{ 5,8,2 },{ 8,2,5 },{ 8,5,2 } }; //4到6行的平移序列排序。
int row7to9_translation_sequence[6][3] = { { 1,4,7 },{ 1,7,4 },{ 4,1,7 },{ 4,7,1 },{ 7,1,4 },{ 7,4,1 } }; //7到9行的平移序列排序。
																										  /*求解数独所需变量*/																											  /*求解数独所需变量*/
char sudoku[9][10]; //数独总体盘面
int blank[81][2];//记录空白单元格的位置
int sum; //总的空白单元格数
int visit[81][10]; //记录空白单元访问了哪些数字；1为不在访问范围内，0为未访问，2为已访问
ofstream out;
string filepath; //sudoku.txt的文件路径
char output_buffer[165000000]; //把将要输出的数独缓存到output_buffer字符串中
int length_of_buffer; //缓存字符串实际长度

int IdentifyOrder(int argcount, char* argv[]); //返回-1表示是求解数独命令，0表示命令行有错误,大于0小于1000000表示属于生成数独，且返回值即为N
void GenerateSudokuEndings(int N);  //生成数独终局
void SudokuSolver(char su[][10]);  //求解数独
int Check(int x, int row, int col); //返回0表示不满足条件；返回1表示满足条件
void ProduceOneSudokuByTranslation(int seq1, int seq2, int seq3, int sudoku_order); //通过平移生成一个数独终局
void PrintSudoku();   //输出所有的数独到sudoku.txt文件
void PutIntoBuffer(int sudoku_order);  //把某个数独存入字符串缓存output_buffer中
 int CheckResult(char *file); //检查某个数独是否正确


int CheckResult(char *file)
{
	ifstream ifile;
	ifile.open(file);
	string line;
	char x;
	char su[9][10];
	int i = 0, j, ii, jj;
	while (getline(ifile, line))
	{
		if (int len = line.length() > 0)
		{
			for (j = 0; j < 9; j++)
			{
				x = line[j * 2];
				su[i][j] = x;
			}
			su[i][9] = '\0';
			i += 1;
			if (i == 9)
			{
				int v[10];
				for (ii = 0; ii < 9; ii++) //检查每行有无重复数字
				{
					memset(v, 0, sizeof(v));
					for (jj = 0; jj < 9; jj++)
					{
						if (v[su[ii][jj] - '0'] == 1) return 0;
						v[su[ii][jj] - '0'] = 1;
					}
				}
				for (jj = 0; jj < 9; jj++) //检查每列有无重复数字
				{
					memset(v, 0, sizeof(v));
					for (ii = 0; ii < 9; ii++)
					{
						if (v[su[ii][jj] - '0'] == 1) return 0;
						v[su[ii][jj] - '0'] = 1;
					}
				}
				int iii, jjj;
				for (ii = 0; ii < 7; ii += 3) //检查每宫有无重复数字
				{
					for (jj = 0; jj < 7; jj += 3)
					{
						memset(v, 0, sizeof(v));
						for (iii = ii; iii < ii + 3; iii++)
						{
							for (jjj = jj; jjj < jj + 3; jjj++)
							{
								if (v[su[iii][jjj] - '0'] == 1) return 0;
								v[su[iii][jjj] - '0'] = 1;
							}
						}
					}
				}		
				i = 0;		
			}
		}
	}
	ifile.close();
	return 1;
}


int IdentifyOrder(int argcount, char* arg[]) //返回-1表示是求解数独命令，0表示命令行有错误,大于0小于1000000表示属于生成数独，且返回值即为N
{
	int N = 0;
	if (argcount != 3)
	{
		return 0;
	}
	if (strcmp(arg[1], "-c") == 0)
	{
		string str = *(arg + 2); //取命令行第二个参数——数独终局个数
		int len = str.length();
		for (int i = 0; i < len; i++)
		{
			if (str[i] < '0' || str[i] > '9')
			{
				return 0;
			}
			N = N * 10 + str[i] - '0';
		}
		if( 1 <= N && N <=1000000 ) return N;
		else return 0;
	}
	else if (strcmp(arg[1], "-s") == 0)
	{
		ifstream ff;
		ff.open(arg[2]);
		if (!ff.is_open())
		{
			return 0;
		}
		else return -1;
	}
	return 0;
}

void PutIntoBuffer(int sudoku_order)
{
	int i, j;
	if (sudoku_order != 1)
	{
		output_buffer[length_of_buffer] = '\n';
		length_of_buffer++;
		output_buffer[length_of_buffer] = '\n';
		length_of_buffer++;
	}
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			output_buffer[length_of_buffer] = sudoku[i][j];
			length_of_buffer++;
			output_buffer[length_of_buffer] = ' ';
			length_of_buffer++;
		}
		output_buffer[length_of_buffer] = sudoku[i][8];
		length_of_buffer++;
		output_buffer[length_of_buffer] = '\n';
		length_of_buffer++;
	}
	for (j = 0; j < 8; j++)
	{
		output_buffer[length_of_buffer] = sudoku[8][j];
		length_of_buffer++;
		output_buffer[length_of_buffer] = ' ';
		length_of_buffer++;
	}
	output_buffer[length_of_buffer] = sudoku[8][8];
	length_of_buffer++;
}

void PrintSudoku()
{
	out.open(filepath, ios::trunc); //清空sudoku.txt的原有内容
	out << output_buffer;
	//cout << "缓存字符串output_buffer长度："<< length_of_buffer << endl;
	out.close();
}

void ProduceOneSudokuByTranslation(int seq1, int seq2, int seq3, int sudoku_order) //平移生成一个数独终局
{
	int i, j, t;
	if (sudoku_order != 1)
	{
		output_buffer[length_of_buffer++] = '\n';
		output_buffer[length_of_buffer++] = '\n';
	}
	for (j = 0; j < 8; j++)
	{
		output_buffer[length_of_buffer++] = sudoku[0][j];
		output_buffer[length_of_buffer++] = ' ';
	}
	output_buffer[length_of_buffer++] = sudoku[0][8];
	output_buffer[length_of_buffer++] = '\n';
	for (i = 0; i < 2; i++) //平移2、3行
	{
		t = row2to3_translation_sequence[seq1][i];
		for (j = 0; j < 8; j++)
		{
			output_buffer[length_of_buffer++] = sudoku[0][(j + t) % 9];
			output_buffer[length_of_buffer++] = ' ';
		}
		output_buffer[length_of_buffer++] = sudoku[0][(8 + t) % 9];
		output_buffer[length_of_buffer++] = '\n';
	}
	for (i = 0; i < 3; i++)  //平移4、5、6行
	{
		t = row4to6_translation_sequence[seq2][i];
		for (j = 0; j < 8; j++)
		{
			output_buffer[length_of_buffer++] = sudoku[0][(j + t) % 9];
			output_buffer[length_of_buffer++] = ' ';
		}
		output_buffer[length_of_buffer++] = sudoku[0][(8 + t) % 9];
		output_buffer[length_of_buffer++] = '\n';
	}
	for (i = 0; i < 2; i++)  //平移7、8
	{
		t = row7to9_translation_sequence[seq3][i];
		for (j = 0; j < 8; j++)
		{
			output_buffer[length_of_buffer++] = sudoku[0][(j + t) % 9];
			output_buffer[length_of_buffer++] = ' ';
		}
		output_buffer[length_of_buffer++] = sudoku[0][(8 + t) % 9];
		output_buffer[length_of_buffer++] = '\n';
	}
	t = row7to9_translation_sequence[seq3][2];
	for (j = 0; j < 8; j++)
	{
		output_buffer[length_of_buffer++] = sudoku[0][(j + t) % 9];
		output_buffer[length_of_buffer++] = ' ';
	}
	output_buffer[length_of_buffer++] = sudoku[0][(8 + t) % 9];
}

void GenerateSudokuEndings(int N)
{
	int i, j, k, count = 0;
	sudoku[0][0] = '8';//按题目要求将学号后两位的运算结果作为左上角填入的数字：（6+1）%9+1 =8
	sudoku[0][1] = '1';
	sudoku[0][2] = '2';
	sudoku[0][3] = '3';
	sudoku[0][4] = '4';
	sudoku[0][5] = '5';
	sudoku[0][6] = '6';
	sudoku[0][7] = '7';
	sudoku[0][8] = '9';
	sudoku[0][9] = '\0';
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 6; j++)
		{
			for (k = 0; k < 6; k++)
			{
				do
				{
					count += 1;
					ProduceOneSudokuByTranslation(i, j, k, count);
					if (count >= N) return;
				} while (next_permutation(&(sudoku[0][1]), &(sudoku[0][9])));
			}
		}
	}
	//cout << "生成数独个数:" << count << endl;
}


int Check(int x, int row, int col) //检查所填数字是否满足条件
{
	int i, j;
	char figure = x + '0';
	for (j = 0; j < 9; j++) //检查该行有没有重复数字
	{
		if (sudoku[row][j] == figure && j != col) return 0;
	}
	for (i = 0; i < 9; i++)
	{
		if (sudoku[i][col] == figure && i != row) return 0;//检查该列有没有重复数字
	}
	int row1, col1;
	row1 = row / 3;
	col1 = col / 3;
	for (i = row1 * 3; i < 3 + row1 * 3; i++)
	{
		for (j = col1 * 3; j < 3 + col1 * 3; j++)
		{
			if (i == row && j == col) continue;
			if (sudoku[i][j] == figure) return 0;//检查该宫有没有重复数字
		}
	}
	return 1;
}

void SudokuSolver(char su[][10])
{
	int m, x, flag = 0, s = 0; //flag=0表示没有合适的数字可填
	while (s < sum)
	{
		flag = 0;
		for (x = 1; x < 10; x++)
		{
			if (visit[s][x] != 0) continue; //该数字已经存在或者已被访问了
			if (Check(x, blank[s][0], blank[s][1]) == 1)
			{
				sudoku[blank[s][0]][blank[s][1]] = x + '0';
				visit[s][x] = 2; //标记为已访问
				flag = 1;
				s += 1; //继续扩展下一个空白单元格
				break;
			}
		}
		if (flag == 0) //当前节点没有合适的数字可填，返回上一个结点，进行修改
		{
			for (m = 1; m < 10; m++) //清除当前已访问过的数字
			{
				if (visit[s][m] == 2) visit[s][m] = 0;
			}
			sudoku[blank[s][0]][blank[s][1]] = '0';
			s -= 1; //返回到上一个空白单元格
			if (s < 0) s = 0;
		}
	}
}