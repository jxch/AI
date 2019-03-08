/*
	��չ���Ż�:
	�����������,��ǿ����
*/
//ĸ����->������
//������ȡ��ֵ�ı���
//�Ż�copy����
//˼�������ƻ����̵Ŀ�����3^225,һά�������

//alpha,beta��֦����������Ϸ
//alpha��ȡ����ֵ,beta��ȡ��Сֵ,alpha>betaֵ��֦(ÿ�����alpha,betaֵ��ͬ,ֻΪ��֦����)
//alpha,beta��Լ���С�����˼�֦

//Ĭ��  ������:2��������:X,0�����������:O,1

//�ܷ�����������
//
/*****************����X������O****************/
//���Բ���������

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
#define maxl 15//���̳���
#define maxnodes 15*15//�ڵ�����
int maxdepth;

#define empty 2//��
#define X_player 0//����
#define O_player 1//AI
const int maxn = 50;//���Ϳⳤ��
int curnode = 0;//��¼��ǰ�Ľڵ�����
int win = 0;//�ж���Ӯ:Ӯ1,��0

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
//X�����½��
set<Node> blackp;
//O�����½��
set<Node> whitep;

int Board[maxl][maxl];//����
//�������
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
//�ж���Ӯ,Ӯ1,ƽ0//heng,zongΪ�����±�
int check_player_win(int player, int heng, int zong)
{
	//�����߿�����������չ
	int curheng1 = heng, curheng2 = heng;
	int curzong1 = zong, curzong2 = zong;
	int ans = 0;
	//��������
	while (Board[heng][curzong1] == player && curzong1 < maxl)
		curzong1++;
	while (Board[heng][curzong2] == player && curzong2 >= 0)
		curzong2--;
	ans = (curzong1 - 1) - (curzong2 + 1) + 1;
	if (ans >= 5)
		return 1;
	//��������
	while (Board[curheng1][zong] == player && curheng1 < maxl)
		curheng1++;
	while (Board[curheng2][zong] == player && curheng2 >= 0)
		curheng2--;
	ans = (curheng1 - 1) - (curheng2 + 1) + 1;
	if (ans >= 5)
		return 1;

	//k=1������(б��)
	//��߻ص���ʼֵ
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
	//k=-1������(б��)
	//��߻ص���ʼֵ
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
	//ƽ��
	return 0;
}
//���ǲ���������
//board2���Ƶ�board1��
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
//��������
int get_humanmove()
{
	int heng, zong;//��ʵ����������±�
	while (true)
	{
		cout << "�������壺���������꣬���������:" << endl;
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
				cout << "!Υ�����壬��������!" << endl;
			}
		}
	}
}




int valueBoard[maxl][maxl] = {//����ֱ�ӹ�ֵ
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
//����(�����Գ�����)
string shapetype[2][maxn] = {
	//��������
	{
		"11111",//����
		"211112",//����
		"211110",//����
		"011112","2121112","2111212","2112112",
		"21112",//����
		"212112","211212",
		"221110",//����
		"011122","212110","011212","211210","012112","12211","11221","12121","0211120",
		"22112",//���
		"21122","21212","212212",
		"222110",//�߶�
		"011222","221210","012122","212210","012212","12221","0212120","0211220","0221120",
		"011110",//����
		"01110",//����
		"0110",//����
		"2102"//��Ч����
		"2012"
	},
	//��������
	{
		"00000",//����
		"200002",//����
		"200001",//����
		"100002","2020002","2000202","2002002",
		"20002",//����
		"202002","200202",
		"220001",//����
		"100022","202001","100202","200201","102002","02200","00220","02020","1200021",
		"22002",//���
		"20022","20202","202202",
		"222001",//�߶�
		"100222","220201","102022","202201","102202","02220","1202021","1200221","1220021",
		"100001",//����
		"10001",//����
		"1001",//����
		"2012"//��Ч����
		"2102"
	}
};
/**********************************************************�޸Ĺ�ֵ����***************************************************************************/
/**********��������,����Ȩֵ***************/
int get_shapetype_value(int num)
{
	switch (num)
	{
	case 0://����
		return 10000000; break;
	case 1://����
		return 10000; break;
	case 2://����
	case 3:
	case 4:
	case 5:
	case 6:
		return 500; break;
	case 7://����
	case 8:
	case 9:
		return 200; break;
	case 10://����
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
	case 20://���
	case 21:
	case 22:
	case 23:
		return 5; break;
	case 24://�߶�
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
	case 34://����
		return -5; break;
	case 35://����
		return -5; break;
	case 36://����
		return -5; break;
	case 37:
		return 3; break;
	default:cout << "TYPE ERROR" << endl; break;
	}
}
//
void testout(int board[][maxl], int depth)
{
	cout << "�����ǰ����" << "��ǰ���:" << depth << endl;
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
//��һ�����ӹ���
int get_combine_type(int board[][maxl], int i, int j, int order)
{
	int value = 0;
	int st_i, st_j, ed_i, ed_j;
	//����*********************************
	string level = "";
	st_j = j - 4; ed_j = j + 4;
	for (int w = st_j; w <= ed_j; w++)
	{
		level.append(to_string(board[i][w]));
	}
	//�����Ϳ�ƥ��
	for (int w = 0; w <= 37; w++)
	{
		if (level.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "��ù�ֵ" << w << endl;
			value += curvalue;
		}
	}

	//����**********************************
	string vertical = "";
	st_i = i - 4; ed_i = i + 4;
	for (int w = st_i; w <= ed_i; w++)
	{
		vertical.append(to_string(board[w][j]));
	}
	//�����Ϳ�ƥ��
	for (int w = 0; w <= 37; w++)
	{
		if (vertical.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "��ù�ֵ" << w << endl;
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
	//�����Ϳ�ƥ��
	for (int w = 0; w <= 37; w++)
	{
		if (k1.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "��ù�ֵ" << w << endl;
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
	//�����Ϳ�ƥ��
	for (int w = 0; w <= 37; w++)
	{
		if (k_1.find(shapetype[order][w]) != string::npos)
		{
			int curvalue = get_shapetype_value(w);
			//cout << "��ù�ֵ" << w << endl;
			value += curvalue;
		}
	}
	//���ع�ֵ
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
//�������̽���һ������
//X�ù��ָ�,X�͹��ֵ�
//ʹ��get_boardtype()//�ĸ�����//�����������(�����н���)
//string.find()

int evaluate_board(int player, int board[][maxl])
{
	int value = 0;
	//�Է��طּ�����Ԥ����
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
	//cout << "��ǰ����" << endl;
	//testout(board);
	//cout << "�������" << endl;
	//testout(checkboard);
	//cout << endl;
	//������������
	for (int i = 0; i < maxl; i++)
	{
		for (int j = 0; j < maxl; j++)
		{
			//���¼��������ϵĵ��ֵ
			if (Board[i][j] == 2)
			{
				//������**************************************
				if (board[i][j] == O_player)
				{
					value += get_combine_type(board, i, j, 0);
					//����λ�÷�
					value += valueBoard[i][j];
				}
				//���ط�**************************************���طּ���ʧ��
				if (board[i][j] == O_player && checkboard[i][j] == X_player)
				{
					value += get_combine_type(checkboard, i, j, 1);
					//����λ�÷�
					value += valueBoard[i][j];
				}

			}
		}
	}
	return value;
}

//��������λ��,��ȡÿһ�������λ��

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

/****************************��������޸�********************************/
//AI������ռ���,���ض������ŵ�����
//����Ҷ���̹���ֵ
//alpha-betaֻ������(����Ҷ����ѡ����һ������)
int a_b_dfs(int player, int depth, int alpha, int beta, int curboard[][maxl], int &heng, int &zong)
{
	//��֦
	if (alpha > beta)
	{
		return alpha;
	}
	//�ݹ�߽�(�������̹�ֵ)
	if (depth > maxdepth)
	{
		return evaluate_board(player, curboard);
	}

	//����������
	int BMinvalue = INF;
	int BMaxvalue = -INF;
	set<Node> cur = get_emptyplace(curboard);
	for (set<Node>::iterator it = cur.begin(); it != cur.end(); it++)
	{
		int curi = it->x;
		int curj = it->y;
		if(curboard[curi][curj]==2)
            curboard[curi][curj] = player;
        else
            continue;

		int value = a_b_dfs(abs(player - 1), depth + 1, alpha, beta, curboard, heng, zong);
		//����:����һ����µ�ǰ���ֵ
		//alpha��ȡ����ֵ
		if (player == O_player)
		{
			BMaxvalue = max(BMaxvalue, value);
			if (value > alpha)
			{
				alpha = value;
				//��ȡ��������
				if (depth == 0)
				{
					heng = curi; zong = curj;
				}
			}
		}
		//beta��ȡ��Сֵ
		else if (player == X_player)
		{
			BMinvalue = min(BMinvalue, value);
			beta = min(value, beta);
		}
		//�����ÿ�
		curboard[curi][curj] = 2;
	}
	//***************����*****************
	if (depth == 0)
	{
		cout << "������̷���ֵ" << alpha << endl;
	}
	//����һ�㷵��alpha-beta��ֵ
	if (player == X_player)
		return BMinvalue;
	else
		return BMaxvalue;
}
//AI����
int get_aimove()
{
	int heng, zong;
	int alpha = -INF, beta = INF;
	int depth = 0;

	//��ȡ��ǰ����
	int curboard[maxl][maxl];
	copy_board(curboard, Board);

	//heng,zong��ʾAI����ĵ�
	heng = -1; zong = -1;
	a_b_dfs(O_player, depth, alpha, beta, curboard, heng, zong);
	Board[heng][zong] = O_player;
	cout << "AI��������:" << heng << " " << zong << endl;
	curnode++;

	return check_player_win(O_player, heng, zong);
}
int main()
{
	//��ʼ����������
	fill(Board[0], Board[0] + maxl * maxl, 2);
	curnode = 0;

	/******������������******/
	cout << "������������(<=4):" << endl;
	cin >> maxdepth;
	//maxdepth = 1;

	//Ĭ����������
	//��ʼ����
	while (!win)
	{
		emit_Board();
		//��������
		win = get_humanmove();
		if (win == 1)
		{
			emit_Board();
			cout << endl << "�����ʤ!" << endl;
			break;
		}
		//ƽ�ֵ����һ���������������µ�
		else if (win == 0 && curnode == maxnodes)
		{
			cout << "ƽ��!" << endl;
			break;
		}

		//ai����
		if (win == 0)
		{
			/***********��������ɱ,����*************/
			win = get_aimove();
			if (win == 1)
			{
				emit_Board();
				cout << endl << "AI��ʤ!" << endl;
				break;
			}
		}

	}

	return 0;
}

