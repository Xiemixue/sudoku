//命令行格式:sudoku.exe -c 123或 sudoku.exe -s absolute_path of_puzzlefile

// 数独游戏.cpp: 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "sudoku.h"

/*
string filepath; //sudoku.txt的文件路径
char output_buffer[165000000]; //把将要输出的数独缓存到output_buffer字符串中
int length_of_buffer; //缓存字符串实际长度
char sudoku[9][10]; //数独总体盘面
int blank[81][2];//记录空白单元格的位置
int sum; //总的空白单元格数
int visit[81][10]; //记录空白单元访问了哪些数字；1为不在访问范围内，0为未访问，2为已访问
*/

int main(int argc, char* argv[])
{
	int l, num, N;
	double totaltime;
	clock_t start, finish;
	start = clock();

	//生成相对路径
	filepath = argv[0];
	l = filepath.length() - 10;
	filepath.erase(l);
	filepath += "sudoku.txt";

	memset(output_buffer, '\0', sizeof(output_buffer));
	length_of_buffer = 0;
	num = 0; //num记录待求解的数独个数
	N = 0;

	if(IdentifyOrder(argc,argv) == 0) //命令行参数不正确
	{
		cout << "正确格式如sudoku.exe -c 123或 sudoku.exe -s absolute_path of_puzzlefile\n";
	}
	else
	{
		N = IdentifyOrder(argc, argv);
		if (N > 0)
		{
			GenerateSudokuEndings(N);
			//if (CheckResult(filepath) == 0) cout << "数独不正确" << endl;
			PrintSudoku();
			finish = clock();
			totaltime = ((double)(finish)-(double)start) / CLOCKS_PER_SEC;
			cout << "此程序的运行时间为" << totaltime << "秒！" << endl;
		}
		else if(IdentifyOrder(argc, argv) == -1)
		{
			ifstream f;
			f.open(argv[2]);
			string line;
			int i = 0, j = 0, ii, jj, kk, row1, col1;
			char x;
			memset(visit, 0, sizeof(visit));
			memset(blank, 0, sizeof(blank));
			while (getline(f, line))
			{
				if (int len = line.length() > 0)
				{
					for (j = 0; j < 9; j++)
					{
						x = line[j * 2];
						sudoku[i][j] = x;
						if (x == '0')
						{
							blank[sum][0] = i;
							blank[sum][1] = j;
							sum += 1;
						}
					}
					sudoku[i][9] = '\0';
					i += 1;
					if (i == 9)
					{
						for (ii = 0; ii < sum; ii++)
						{
							for (jj = 0; jj < 9; jj++) //把空白单元格所在行中已存在的数字在visit[][]中标记为1
							{
								if (sudoku[blank[ii][0]][jj] == '0') continue;
								visit[ii][sudoku[blank[ii][0]][jj] - '0'] = 1;
							}
							for (jj = 0; jj < 9; jj++) //把空白单元格所在列中已存在的数字在visit[][]中标记为1
							{
								if (sudoku[jj][blank[ii][1]] == '0') continue;
								visit[ii][sudoku[jj][blank[ii][1]] - '0'] = 1;
							}
							row1 = blank[ii][0] / 3;
							col1 = blank[ii][1] / 3;
							for (jj = row1 * 3; jj < 3 + row1 * 3; jj++) //把空白单元格所在宫中已存在的数字在visit[][]中标记为1
							{
								for (kk = col1 * 3; kk < 3 + col1 * 3; kk++)
								{
									if (sudoku[jj][kk] == '0') continue;
									visit[ii][sudoku[jj][kk] - '0'] = 1;
								}
							}
						}
						num += 1;
						SudokuSolver(sudoku); //求解数独
						PutIntoBuffer(num);
						sum = 0;
						i = 0;
						memset(visit, 0, sizeof(visit));
						memset(blank, 0, sizeof(blank));
					}
				}
			}
			f.close();
			//if (CheckResult(filepath) == 0) cout << "数独不正确" << endl;
			PrintSudoku();
			//cout << "求解数独个数：" << num << endl;
			finish = clock();
			totaltime = ((double)(finish)-(double)start) / CLOCKS_PER_SEC;
			cout << "此程序的运行时间为" << totaltime << "秒！" << endl;
		}
	}
	system("pause");
	return 0;
}