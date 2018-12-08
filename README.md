# sudoku
实现一个能够生成数独终局并且求解数独问题的Windows控制台程序。
程序要求：
(1) 程序能够生成不重复的数独终局至一个文本文件;
(2) 读取文件内的数独问题，求解并将结果输出到一个文本文件;

生成终局：在命令行中使用-c参数加数字N（1≤N≤1000000）控制生成数独终局的数量，例如sudoku.exe -c 20
求解数独：在命令行中使用-s参数加文件名的形式求解数独，并将结果输出至文件，例如sudoku.exe -s absolute_path_of_puzzle
