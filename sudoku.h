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

/*���������վ��������*/
int row2to3_translation_sequence[2][2] = { { 3,6 },{ 6,3 } }; //2��3�е�ƽ����������
int row4to6_translation_sequence[6][3] = { { 2,5,8 },{ 2,8,5 },{ 5,2,8 },{ 5,8,2 },{ 8,2,5 },{ 8,5,2 } }; //4��6�е�ƽ����������
int row7to9_translation_sequence[6][3] = { { 1,4,7 },{ 1,7,4 },{ 4,1,7 },{ 4,7,1 },{ 7,1,4 },{ 7,4,1 } }; //7��9�е�ƽ����������
																										  /*��������������*/																											  /*��������������*/
char sudoku[9][10]; //������������
int blank[81][2];//��¼�հ׵�Ԫ���λ��
int sum; //�ܵĿհ׵�Ԫ����
int visit[81][10]; //��¼�հ׵�Ԫ��������Щ���֣�1Ϊ���ڷ��ʷ�Χ�ڣ�0Ϊδ���ʣ�2Ϊ�ѷ���
ofstream out;
string filepath; //sudoku.txt���ļ�·��
char output_buffer[165000000]; //�ѽ�Ҫ������������浽output_buffer�ַ�����
int length_of_buffer; //�����ַ���ʵ�ʳ���

int IdentifyOrder(int argcount, char* argv[]); //����-1��ʾ������������0��ʾ�������д���,����0С��1000000��ʾ���������������ҷ���ֵ��ΪN
void GenerateSudokuEndings(int N);  //���������վ�
void SudokuSolver(char su[][10]);  //�������
int Check(int x, int row, int col); //����0��ʾ����������������1��ʾ��������
void ProduceOneSudokuByTranslation(int seq1, int seq2, int seq3, int sudoku_order); //ͨ��ƽ������һ�������վ�
void PrintSudoku();   //������е�������sudoku.txt�ļ�
void PutIntoBuffer(int sudoku_order);  //��ĳ�����������ַ�������output_buffer��
 int CheckResult(char *file); //���ĳ�������Ƿ���ȷ


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
				for (ii = 0; ii < 9; ii++) //���ÿ�������ظ�����
				{
					memset(v, 0, sizeof(v));
					for (jj = 0; jj < 9; jj++)
					{
						if (v[su[ii][jj] - '0'] == 1) return 0;
						v[su[ii][jj] - '0'] = 1;
					}
				}
				for (jj = 0; jj < 9; jj++) //���ÿ�������ظ�����
				{
					memset(v, 0, sizeof(v));
					for (ii = 0; ii < 9; ii++)
					{
						if (v[su[ii][jj] - '0'] == 1) return 0;
						v[su[ii][jj] - '0'] = 1;
					}
				}
				int iii, jjj;
				for (ii = 0; ii < 7; ii += 3) //���ÿ�������ظ�����
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


int IdentifyOrder(int argcount, char* arg[]) //����-1��ʾ������������0��ʾ�������д���,����0С��1000000��ʾ���������������ҷ���ֵ��ΪN
{
	int N = 0;
	if (argcount != 3)
	{
		return 0;
	}
	if (strcmp(arg[1], "-c") == 0)
	{
		string str = *(arg + 2); //ȡ�����еڶ����������������վָ���
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
	out.open(filepath, ios::trunc); //���sudoku.txt��ԭ������
	out << output_buffer;
	//cout << "�����ַ���output_buffer���ȣ�"<< length_of_buffer << endl;
	out.close();
}

void ProduceOneSudokuByTranslation(int seq1, int seq2, int seq3, int sudoku_order) //ƽ������һ�������վ�
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
	for (i = 0; i < 2; i++) //ƽ��2��3��
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
	for (i = 0; i < 3; i++)  //ƽ��4��5��6��
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
	for (i = 0; i < 2; i++)  //ƽ��7��8
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
	sudoku[0][0] = '8';//����ĿҪ��ѧ�ź���λ����������Ϊ���Ͻ���������֣���6+1��%9+1 =8
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
	//cout << "������������:" << count << endl;
}


int Check(int x, int row, int col) //������������Ƿ���������
{
	int i, j;
	char figure = x + '0';
	for (j = 0; j < 9; j++) //��������û���ظ�����
	{
		if (sudoku[row][j] == figure && j != col) return 0;
	}
	for (i = 0; i < 9; i++)
	{
		if (sudoku[i][col] == figure && i != row) return 0;//��������û���ظ�����
	}
	int row1, col1;
	row1 = row / 3;
	col1 = col / 3;
	for (i = row1 * 3; i < 3 + row1 * 3; i++)
	{
		for (j = col1 * 3; j < 3 + col1 * 3; j++)
		{
			if (i == row && j == col) continue;
			if (sudoku[i][j] == figure) return 0;//���ù���û���ظ�����
		}
	}
	return 1;
}

void SudokuSolver(char su[][10])
{
	int m, x, flag = 0, s = 0; //flag=0��ʾû�к��ʵ����ֿ���
	while (s < sum)
	{
		flag = 0;
		for (x = 1; x < 10; x++)
		{
			if (visit[s][x] != 0) continue; //�������Ѿ����ڻ����ѱ�������
			if (Check(x, blank[s][0], blank[s][1]) == 1)
			{
				sudoku[blank[s][0]][blank[s][1]] = x + '0';
				visit[s][x] = 2; //���Ϊ�ѷ���
				flag = 1;
				s += 1; //������չ��һ���հ׵�Ԫ��
				break;
			}
		}
		if (flag == 0) //��ǰ�ڵ�û�к��ʵ����ֿ��������һ����㣬�����޸�
		{
			for (m = 1; m < 10; m++) //�����ǰ�ѷ��ʹ�������
			{
				if (visit[s][m] == 2) visit[s][m] = 0;
			}
			sudoku[blank[s][0]][blank[s][1]] = '0';
			s -= 1; //���ص���һ���հ׵�Ԫ��
			if (s < 0) s = 0;
		}
	}
}