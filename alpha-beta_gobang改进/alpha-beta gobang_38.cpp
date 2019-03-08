//效率较低对整个棋盘操作,可以修改成对一个节点进行操作
//一个获取最优解的变量
//优化copy函数
//思考二进制画棋盘的可能性3^225,一维标记棋盘

//alpha,beta剪枝的五子棋游戏
//alpha获取极大值,beta获取极小值,alpha>beta值剪枝(每个点的alpha,beta值不同,只为剪枝服务)
//alpha,beta相对极大极小增加了剪枝

//默认  空棋盘:2人类先手:X,0，计算机后手:O,1

//能否引入神经网络
//预定义最大搜索层数 2
/*****************人类X，智能O****************/
//可以不复制棋盘

//#include<bits/stdc++.h>
#include<iostream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<set>
#include<vector>
using namespace std;
#define INF (1<<30)-1
#define maxl 15//棋盘长度
#define maxnodes 15*15//节点总数
int maxdepth;

#define empty 2//空
#define X_player 0//人类
#define O_player 1//AI
const int maxn = 50;//棋型库长度
int curnode = 0;//记录当前的节点总数
int win = 0;//判断输赢:赢1,输0

struct Node {
	int x;
	int y;
	Node(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Node()
	{

	}
	bool friend operator<(Node n1, Node n2)
	{
		if (n1.x != n2.x)
			return n1.x < n2.x;
		else
			return n1.y < n2.y;
	}
};
//X棋所下结点
set<Node> blackp;
//O棋所下结点
set<Node> whitep;

int Board[maxl][maxl];//棋盘
//输出棋盘
void emit_Board()
{
	cout << endl;
	for (int k = 0; k < maxl; k++)
	{
		if (k >= 10)
			cout << k << "  ";
		else
			cout << " " << k << "  ";
	}
	cout << endl;
	for (int k = 0; k < maxl; k++)
	{
		cout << "--- ";
	}
	cout << endl;
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			switch (Board[i][j])
			{
			case 2:cout << "   "; break;
			case 0:cout << " X "; break;
			case 1:cout << " O "; break;
			}
			if (j == maxl - 1)
				cout << " " << i << endl;
			else
				cout << "|";
		}

		for (int k = 0; k < maxl; k++)
		{
			cout << "--- ";
		}
		cout << endl;
	}
}
//判断输赢,赢1,平0//heng,zong为数组下标
int check_player_win(int player, int heng, int zong)
{
	//两组标尺控制向两侧扩展
	int curheng1 = heng, curheng2 = heng;
	int curzong1 = zong, curzong2 = zong;
	int ans = 0;
	//横向搜索
	while (Board[heng][curzong1] == player && curzong1 < maxl)
		curzong1++;
	while (Board[heng][curzong2] == player && curzong2 >= 0)
		curzong2--;
	ans = (curzong1 - 1) - (curzong2 + 1) + 1;
	if (ans >= 5)
		return 1;
	//纵向搜索
	while (Board[curheng1][zong] == player && curheng1 < maxl)
		curheng1++;
	while (Board[curheng2][zong] == player && curheng2 >= 0)
		curheng2--;
	ans = (curheng1 - 1) - (curheng2 + 1) + 1;
	if (ans >= 5)
		return 1;

	//k=1向搜索(斜率)
	//标尺回到初始值
	ans = 0;
	curheng1 = curheng2 = heng;
	curzong1 = curzong2 = zong;
	while (Board[curheng1][curzong1] == player && curheng1 >= 0 && curzong2 < maxl)
	{
		ans++;
		curheng1--;
		curzong1++;
	}
	curheng2 = heng + 1;
	curzong2 = zong - 1;
	while (Board[curheng2][curzong2] == player && curheng2 < maxl && curzong2 >= 0)
	{
		ans++;
		curheng2++;
		curzong2--;
	}
	if (ans >= 5)
		return 1;
	//k=-1向搜索(斜率)
	//标尺回到初始值
	ans = 0;
	curheng1 = curheng2 = heng;
	curzong1 = curzong2 = zong;
	while (Board[curheng1][curzong1] == player && curheng1 >= 0 && curzong1 >= 0)
	{
		ans++;
		curheng1--;
		curzong1--;
	}
	curheng2 = heng + 1;
	curzong2 = zong + 1;
	while (Board[curheng2][curzong2] == player && curheng2 < maxl && curzong2 < maxl)
	{
		ans++;
		curheng2++;
		curzong2++;
	}
	if (ans >= 5)
		return 1;
	//平局
	return 0;
}
//考虑不复制棋盘
//board2复制到board1上
void copy_board(int board1[][maxl], int board2[][maxl])
{
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			board1[i][j] = board2[i][j];
		}
	}
}
//人类下棋
int get_humanmove()
{
	int heng, zong;//其实代表的数组下标
	while (true)
	{
		cout << "人类下棋：输入纵坐标，输入横坐标:" << endl;
		cin >> heng >> zong;

		if (curnode < maxnodes)
		{
			if (Board[heng][zong] == 2 && 0 <= heng && heng < maxl && 0 <= zong && zong < maxl)
			{
				Board[heng][zong] = X_player;
				curnode++;
				return check_player_win(X_player, heng, zong);
			}
			else
			{
				cout << "!违法下棋，重新下棋!" << endl;
			}
		}
	}
}




int valueBoard[maxl][maxl] = {//落子直接估值
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
	{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
	{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
	{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
	{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};
//棋型(包括对称棋型)
string shapetype[2][maxn] = {
	//进攻棋型
	{
		"11111",//长连
		"211112",//活四
		"211110",//冲四
		"011112","2121112","2111212","2112112",
		"21112",//活三
		"212112","211212",
		"221110",//眠三
		"011122","212110","011212","211210","012112","12211","11221","12121","0211120",
		"22112",//活二
		"21122","21212","212212",
		"222110",//眠二
		"011222","221210","012122","212210","012212","12221","0212120","0211220","0221120",
		"011110",//死四
		"01110",//死三
		"0110",//死二
		"2102"//有效防守
		"2012"
	},
	//防守棋型
	{
		"00000",//长连
		"200002",//活四
		"200001",//冲四
		"100002","2020002","2000202","2002002",
		"20002",//活三
		"202002","200202",
		"220001",//眠三
		"100022","202001","100202","200201","102002","02200","00220","02020","1200021",
		"22002",//活二
		"20022","20202","202202",
		"222001",//眠二
		"100222","220201","102022","202201","102202","02220","1202021","1200221","1220021",
		"100001",//死四
		"10001",//死三
		"1001",//死二
		"2012"//有效防守
		"2102"
	}
};
/**********************************************************修改估值代码***************************************************************************/
/**********棋型设置,返回权值***************/
int get_shapetype_value(int num)
{
	switch (num)
	{
	case 0://长连
		return 10000000; break;
	case 1://活四
		return 10000; break;
	case 2://冲四
	case 3:
	case 4:
	case 5:
	case 6:
		return 500; break;
	case 7://活三
	case 8:
	case 9:
		return 200; break;
	case 10://眠三
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		return 50; break;
	case 20://活二
	case 21:
	case 22:
	case 23:
		return 5; break;
	case 24://眠二
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
		return 3; break;
	case 34://死四
		return -5; break;
	case 35://死三
		return -5; break;
	case 36://死二
		return -5; break;
	case 37:
		return 3; break;
	default:cout << "TYPE ERROR" << endl; break;
	}
}
//
void testout(int board[][maxl], int depth)
{
	cout << "输出当前棋盘" << "当前深度:" << depth << endl;
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			if (board[i][j] == 2)
				cout << ". ";
			else
			{
				if (board[i][j] == O_player)
					cout << "O ";
				else
					cout << "X ";
			}
		}
		cout << endl;
	}
}
//对一个棋子估价
int get_combine_type(int board[][maxl], int i, int j, int order)
{
	int value = 0;
	int st_i, st_j, ed_i, ed_j;
	//横向*********************************
	string level = "";
	st_j = j - 4; ed_j = j + 4;
	for (int w = st_j; w <= ed_j; w++)
	{
		level.append(to_string(board[i][w]));
	}
	//与棋型库匹配
	for (int w = 0; w <= 37; w++)
	{
		if (level.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "获得估值" << w << endl;
			value += curvalue;
		}
	}

	//纵向**********************************
	string vertical = "";
	st_i = i - 4; ed_i = i + 4;
	for (int w = st_i; w <= ed_i; w++)
	{
		vertical.append(to_string(board[w][j]));
	}
	//与棋型库匹配
	for (int w = 0; w <= 37; w++)
	{
		if (vertical.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "获得估值" << w << endl;
			value += curvalue;
		}
	}

	int w_i, w_j;
	//k=1************************************
	string k1 = "";
	st_i = i + 4; st_j = j - 4;
	ed_i = i - 4; ed_j = j + 4;
	w_i = st_i; w_j = st_j;
	while (w_i != ed_i || w_j != ed_j)
	{
		k1.append(to_string(board[w_i][w_j]));
		w_i--; w_j++;
	}
	k1.append(to_string(board[ed_i][ed_j]));
	//与棋型库匹配
	for (int w = 0; w <= 37; w++)
	{
		if (k1.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "获得估值" << w << endl;
			value += curvalue;
		}
	}

	//k=-1***********************************
	string k_1 = "";
	st_i = i - 4; st_j = j - 4;
	ed_i = i + 4; ed_j = j + 4;
	w_i = st_i; w_j = st_j;
	while (w_i != ed_i || w_j != ed_j)
	{
		k_1.append(to_string(board[w_i][w_j]));
		w_i++; w_j++;
	}
	k_1.append(to_string(board[ed_i][ed_j]));
	//与棋型库匹配
	for (int w = 0; w <= 37; w++)
	{
		if (k_1.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "获得估值" << w << endl;
			value += curvalue;
		}
	}
	//返回估值
	return value;
}
/*
void testout(int board[][maxl])
{
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}
*/
//整个棋盘进行一个估价
//X好估分高,X低估分低
//使用get_boardtype()//四个方向//加入组合棋型(棋型有交点)
//string.find()

int evaluate_board(int player, int board[][maxl])
{
	int value = 0;
	//对防守分计算做预处理
	int checkboard[maxl][maxl];
	fill(checkboard[0], checkboard[0] + maxl*maxl, 2);
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			if (Board[i][j] == 2 && board[i][j] != 2)
			{
				checkboard[i][j] = X_player;
			}
			else if (Board[i][j] != 2)
				checkboard[i][j] = Board[i][j];
		}
	}
	//cout << "当前棋盘" << endl;
	//testout(board);
	//cout << "检查棋盘" << endl;
	//testout(checkboard);
	//cout << endl;
	//计算棋盘评估
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			//对新加入棋盘上的点估值
			if (Board[i][j] == 2)
			{
				//进攻分**************************************
				if (board[i][j] == O_player)
				{
					value += get_combine_type(board, i, j, 0);
					//加上位置分
					value += valueBoard[i][j];
				}
				//防守分**************************************防守分计算失败
				if (board[i][j] == O_player && checkboard[i][j] == X_player)
				{
					value += get_combine_type(checkboard, i, j, 1);
					//加上位置分
					value += valueBoard[i][j];
				}

			}
		}
	}
	return value;
}

//限制下棋位置,获取每一层可下棋位置

void judge(int board[][maxl], int i, int j, set<Node> &ans)
{
	for (int w = i - 2; w <= i + 2; w++)
	{
		for (int k = j - 2; k <= j + 2; k++)
		{
			if (0 <= w && w < maxl && 0 <= k && k < maxl)
			{
				if (w != i || k != j && board[w][k] == 2)
					ans.insert(Node(w, k));
			}
		}
	}
}

/*
void judge(int board[][maxl], int i, int j, set<Node> &ans)
{
	if(board[i][j-1] == 2)
		ans.insert(Node(i, j - 1));

	//if(board[i-1][j] == 2)
	//	ans.insert(Node(i - 1, j));
	//if(board[i][j+1]==2)
	//	ans.insert(Node(i, j + 1));
	//if(board[i+1][j]==2)
	//	ans.insert(Node(i + 1, j));

	if (board[i+1][j - 1] == 2)
		ans.insert(Node(i + 1, j - 1));
	if (board[i+1][j+1] == 2)
		ans.insert(Node(i + 1, j + 1));
	if (board[i-1][j+1] == 2)
		ans.insert(Node(i - 1, j + 1));
	if (board[i-1][j-1] == 2)
		ans.insert(Node(i - 1, j - 1));

}
*/
set<Node> get_emptyplace(int board[][maxl])
{
	set<Node> ans;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (board[i][j] != 2)
			{
				judge(board, i, j, ans);
			}
		}
	}
	return ans;
}

/****************************层次问题修改********************************/
//AI构建解空间树,返回对其最优的棋盘
//返回叶棋盘估价值
//alpha-beta只简化搜索(根据叶棋盘选择下一层棋盘)
int a_b_dfs(int player, int depth, int alpha, int beta, int curboard[][maxl], int &heng, int &zong)
{
	//剪枝
	if (alpha > beta)
	{
		//cout << "剪枝" << endl;
		/*
		if (player == O_player)//上一层是X_player
		{
			return alpha;
		}
		else if (player == X_player)//上一层是O_player
		{
			return beta;
		}
		*/
		return alpha;
	}
	//递归边界(返回棋盘估值)
	if (depth > maxdepth)
	{
		//testout(curboard,depth);
		int value = evaluate_board(player, curboard);
		//cout << "当前下棋位置:" << heng << " " << zong << " "<< "当前棋盘估值:" << value << endl;
		return value;
	}

	//构建子棋盘
	set<Node> cur = get_emptyplace(curboard);
	for (set<Node>::iterator it = cur.begin(); it != cur.end(); it++)
	{
		int curi = it->x;
		int curj = it->y;
		//cout << curi << " " << curj << endl;
		if (curboard[curi][curj] == 2)
			curboard[curi][curj] = player;
		else
			continue;

		int value = a_b_dfs(abs(player - 1), depth + 1, alpha, beta, curboard, heng, zong);
		//回溯:从下一层更新当前层的值
		//alpha获取极大值
		if (player == O_player)
		{
			/********获取最优解*********/
			if (value >= alpha)
			{
				alpha = value;
				if (depth == 0)//获取下棋坐标
				{
					heng = curi; zong = curj;
				}
			}
		}
		//beta获取极小值
		else if (player == X_player)
		{
			beta = min(value, beta);
		}
		//回溯置空
		curboard[curi][curj] = 2;
		//testout(curboard, depth);
	}
	//***************测试*****************
	if (depth == 0)
	{
		cout << "获得棋盘返回值" << alpha << endl;
	}
	//向上一层返回alpha-beta的值
	if (player == X_player)
		return beta;
	else
		return alpha;
}
//AI下棋
int get_aimove()
{
	int heng, zong;
	int alpha = -INF, beta = INF;
	int depth = 0;

	//获取当前棋盘
	int curboard[maxl][maxl];
	copy_board(curboard, Board);

	//heng,zong表示AI下棋的点
	heng = -1; zong = -1;
	a_b_dfs(O_player, depth, alpha, beta, curboard, heng, zong);
	Board[heng][zong] = O_player;
	cout << "AI下棋坐标:" << heng << " " << zong << endl;
	curnode++;

	return check_player_win(O_player, heng, zong);
}
int main()
{
	//初始化棋盘数据
	fill(Board[0], Board[0] + maxl * maxl, 2);
	curnode = 0;

	/******输入搜索层数******/
	cout << "输入搜索层数(<=4):" << endl;
	cin >> maxdepth;
	//maxdepth = 1;

	//默认人类先手
	//开始下棋
	while (!win)
	{
		emit_Board();
		//人类下棋
		win = get_humanmove();
		if (win == 1)
		{
			emit_Board();
			cout << endl << "人类获胜!" << endl;
			break;
		}
		//平局的最后一个棋子总是人类下的
		else if (win == 0 && curnode == maxnodes)
		{
			cout << "平局!" << endl;
			break;
		}

		//ai下棋
		if (win == 0)
		{
			/***********加入近层绝杀,防守*************/
			win = get_aimove();
			if (win == 1)
			{
				emit_Board();
				cout << endl << "AI获胜!" << endl;
				break;
			}
		}

	}

	return 0;
}

