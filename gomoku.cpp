// level4
#include <ctime> 
#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
using namespace std;

char board[15][15]={ '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+',
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', 
					 '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+', '+' };
int value[15][15];
char piece='O';
int row;
char col;
int x, y, px=0, py=0;

void return_value()
{
	int t[15][15]={ 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
					16, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 16,
					16, 44, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 44, 16,
					16, 44, 65, 80, 80, 80, 80, 80, 80, 80, 80, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 90, 90, 90, 90, 90, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 96, 96, 96, 96, 96, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 96, 99, 99, 99, 96, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 96, 99,100, 99, 96, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 96, 99, 99, 99, 96, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 96, 96, 96, 96, 96, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 90, 90, 90, 90, 90, 90, 90, 80, 65, 44, 16,
					16, 44, 65, 80, 80, 80, 80, 80, 80, 80, 80, 80, 65, 44, 16,
					16, 44, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 44, 16,
					16, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 16,
					16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 };
	for (int i=0 ; i<15 ; i++)
		for (int j=0 ; j<15 ; j++)
			value[j][i]=t[j][i];
}

void place_piece(int x, int y)
{
	board[x][y]=piece;
}

bool is_valid_move(int x, int y, char b[15][15])
{
	if (x>14 or x<0 or y>14 or y<0) return false;
	if (b[x][y]!='+') return false;
	return true;
}

void switch_piece()
{
	if (piece=='X') piece='O';
	else piece='X';
}

void print_board()
{
	for (int i=0 ; i<15 ; i++)
	{
		cout << 15-i << " ";
		if (15-i<10) cout << " ";
		for (int j=0 ; j<15 ; j++)
		{
			if (board[i][j]=='O') cout << "��";
			else if (board[i][j]=='X') cout << "��";
			else cout << "��"; 
		}
		cout << "\n";
	}
	cout << "   �ϢТѢҢӢԢբ֢עآ٢ڢۢܢ�" << endl;
}

void print_value()
{
	for (int i=0 ; i<15 ; i++)
	{
		cout << 15-i << " ";
		if (15-i<10) cout << " ";
		for (int j=0 ; j<15 ; j++)
		{
			cout << value[i][j];
			if (j!=14) cout << "-"; 
		}
		cout << endl;
	}
	cout << "   A B C D E F G H I J K L M N O\n";
}

bool is_winx(int x, int y, char b[15][15]) // v o o o o o v 
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-6 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<7 ; k++)
			{
				if (k==0 or k==6) // !=piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]!=piece) check++;
					else break;
				}
				else if (k==1 or k==2 or k==3 or k==4 or k==5) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==7) return true;
		}
	}
	return false;
}

bool is_win(int x, int y, char b[15][15]) // v o o o o o v 
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-4 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<5 ; k++)
			{
				if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
				else break;
			}
			if (check==5) return true;
		}
	}
	return false;
}

bool is_tied(char b[15][15])
{
	for (int i=0 ; i<225 ; i++) 
		if (b[i/15][i%15]=='+') return false;
	
	return true;
}

int live_four(int x, int y, char b[15][15]) // v _ o o o o _ v
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-7 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<8 ; k++)
			{
				if (k==0 or k==7) // !=piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]!=piece) check++;
					else break;
				}
				else if (k==1 or k==6) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==2 or k==3 or k==4 or k==5) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==8)
			{
				count++;
				break;
			}
		}
	}
	return count;
}

int dead_four(int x, int y, char b[15][15]) // v _ o o o o x
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-6 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<6 ; k++)
			{
				if (k==0) // !=piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]!=piece) check++;
					else break;
				}
				else if (k==1) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==2 or k==3 or k==4 or k==5) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
				else if (k==6) // opp-piece
				{
					switch_piece();
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else
					{
						switch_piece();
						break;
					}
					switch_piece();
				}
				
			}
			if (check==7)
			{
				count++;
				break;
			}
			
			check=0;
			for (int k=0 ; k<6 ; k++)
			{				
				check=0;
				if (k==6) // !=piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]!=piece) check++;
					else break;
				}
				else if (k==5) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==4 or k==3 or k==2 or k==1) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
				else if (k==0) // opp-piece
				{
					switch_piece();
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else
					{
						switch_piece();
						break;
					}
					switch_piece();
				}
			}
			if (check==7)
			{
				count++;
				break;
			}
		}
	}
	return count;
}

int live_three(int x, int y, char b[15][15]) // _ o o o _
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-4 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<5 ; k++)
			{
				if (k==0 or k==4)  // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==1 or k==2 or k==3) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==5)
			{
				count++;
				break;
			}
		}
	}
	return count;
}

int dead_three(int x, int y, char b[15][15]) // v _ _ o o o x
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-6 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<6 ; k++)
			{
				if (k==0) // !=piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]!=piece) check++;
					else break;
				}
				else if (k==1 or k==2) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==3 or k==4 or k==5) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
				else if (k==6) // opp-piece
				{
					switch_piece();
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else
					{
						switch_piece();
						break;
					}
					switch_piece();
				}
			}
			if (check==7)
			{
				count++;
				break;
			}
			
			check=0;
			for (int k=0 ; k<6 ; k++)
			{
				if (k==6) // !=piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]!=piece) check++;
					else break;
				}
				else if (k==5 or k==4) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==3 or k==2 or k==1) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
				else if (k==0) // opp-piece
				{
					switch_piece();
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else
					{
						switch_piece();
						break;
					}
					switch_piece();
				}
			}
			if (check==7)
			{
				count++;
				break;
			}
		}
	}
	return count;
}

int live_two(int x, int y, char b[15][15]) // _ _ o o _ _
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-5 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<10 ; k++)
			{
				if (x+(j+k)*dx>=15 or x+(j+k)*dx<0) continue;
				if (y+(j+k)*dy>=15 or y+(j+k)*dy<0) continue;
				if (k==0 or k==1 or k==4 or k==5) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==2 or k==3) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==6)
			{
				count++;
				break;
			}
		}
	}
	return count;
}

int one_one(int x, int y, char b[15][15]) // _ o _ o _
{
	int check=0, count=0;
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-4 ; j<=0 ; j++)
		{
			check=0;
			for (int k=0 ; k<9 ; k++)
			{
				if (x+(j+k)*dx>=15 or x+(j+k)*dx<0) continue;
				if (y+(j+k)*dy>=15 or y+(j+k)*dy<0) continue;
				if (k==0 or k==2 or k==4) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==1 or k==3) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==5)
			{
				count++;
				break;
			}
		}
	}
	return count;
}

int one_two(int x, int y, char b[15][15]) // v _ o _ o o _ v
{
	int check=0, count=0; 
	int delta_x[4]={1, 0, 1, 1}, delta_y[4]={0, 1, 1, -1};
	
	for (int i=0 ; i<4 ; i++)
	{
		int dx=delta_x[i], dy=delta_y[i];
		for (int j=-5 ; j<=0 ; j++)
		{
			bool flag=false;
			check=0;
			for (int k=0 ; k<6 ; k++)
			{
				if (k==0 or k==2 or k==5) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==1 or k==3 or k==4) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==8)
			{
				count++;
				break;
			}
	
			check=0;
			for (int k=0 ; k<6 ; k++)
			{
				if (k==0 or k==3 or k==5) // '+'
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]=='+') check++;
					else break;
				}
				else if (k==1 or k==2 or k==4) // piece
				{
					if (b[x+(j+k)*dx][y+(j+k)*dy]==piece) check++;
					else break;
				}
			}
			if (check==8)
			{
				count++;
				break;
			}
		}
	}
	
	return count;
}
 
void analyze(char b[15][15])
{
	vector<int> best_x, best_y;
	int max=0;
	for (int i=0 ; i<15 ; i++)
	{
		for (int j=0 ; j<15 ; j++)
		{
			if (!is_valid_move(i, j, b))
			{
				value[i][j]=0;
				continue;
			}
			
			//defending
			switch_piece();
			b[i][j]=piece;
			if (is_win(i, j, b)) value[i][j]+=500000;
			if (live_four(i, j, b)) value[i][j]+=60000;
			if (dead_four(i, j, b)+live_three(i, j, b)+one_two(i, j, b)>=2 and dead_four(i, j, b)>0) value[i][j]+=20000;
			if (dead_four(i, j, b)+live_three(i, j, b)+one_two(i, j, b)>=2 and dead_four(i, j, b)==0) value[i][j]+=10000;
			value[i][j]+=550*dead_four(i, j, b);
			value[i][j]+=500*(live_three(i, j, b)+one_two(i, j, b));
			value[i][j]+=100*(dead_three(i, j, b)+live_two(i, j, b)+one_one(i, j, b));
			b[i][j]='+';
			switch_piece();
			
			//attacking 
			b[i][j]=piece;
			if (is_win(i, j, b))
			{
				px=x;
				py=y;
				b[i][j]='+';
				x=i;
				y=j;
				return;
			}
			
			if (live_four(i, j, b)>=1) value[i][j]+=100000;
			if (dead_four(i, j, b)+live_three(i, j, b)+one_two(i, j, b)>=2 and dead_four(i, j, b)>0) value[i][j]+=80000;
			if (dead_four(i, j, b)+live_three(i, j, b)+one_two(i, j, b)>=2 and dead_four(i, j, b)==0) value[i][j]+=50000;
			value[i][j]+=1000*(dead_three(i, j, b)+live_two(i, j, b)+one_one(i, j, b));
			
			b[i][j]='+';
			
		}
	}
	
	for (int i=0 ; i<15 ; i++)
	{
		for (int j=0 ; j<15 ; j++)
		{
			if (max>value[i][j]) continue;
			else if (max<value[i][j])
			{
				max=value[i][j];
				best_x.clear();
				best_y.clear();
				best_x.push_back(i);
				best_y.push_back(j);
			}
			else
			{
				best_x.push_back(i);
				best_y.push_back(j);
			}
		}
	}
	
	time_t T=0;
	T = time(&T);
	int N=T%best_x.size();
	px=x;
	py=y;
	x=best_x[N];
	y=best_y[N];
}

int main()
{
	cout << "================GOMOKU=================" << endl;
	cout << "Double four and double three are LEGAL." << endl;
	print_board();
	
	int opt;
	cin >> opt;
	while(opt==1)
	{
		/*
			if (1) human vs human
			if (piece=='O') human first
			if (piece=='X') computer first
			if (0) computer vs computer
		*/
		if (1) //X computer first O human first
		{
			print_board();
			cout << piece << "'s turn: ";
			cin >> col;
			if (col=='q') return 0;
			if (col=='b')
			{
				board[px][py]='+';
				board[x][y]='+';
				continue;
			}
			cin >> row;
			px=x;
			py=y;
			x=15-row;
			y=col-'A';
			if (is_valid_move(x, y, board))
			{ 
				place_piece(x, y);
				print_board();
				if (is_win(x, y, board))
				{
					cout << "You won.";
					return 0;
				}
				if (is_tied(board))
				{
					cout << "It's a TIED" << endl;
					return 0;
				}
				switch_piece();
			}
			else
			{
				cout << "Invalid move." << endl;
				continue;
			}
		}
		
		else
		{
			return_value();
			analyze(board);
			place_piece(x, y);
			print_value();
			print_board();
			for (char c='A' ; c<='O' ; c++)
				if (y==c-'A')
				{
					cout << "Computer's move:" << c << 15-x << endl;
					break;
				}

			if (is_win(x, y, board))
			{
				cout << "Computer won.";
				return 0;
			}
			if (is_tied(board))
			{
				cout << "It's a TIED" << endl;
				return 0;
			}
			switch_piece();
		}
	}
	while(opt==2)
	{
		/*
			if (1) human vs human
			if (piece=='O') human first
			if (piece=='X') computer first
			if (0) computer vs computer
		*/
		if (piece=='O') //X computer first O human first
		{
			print_board();
			cin >> col;
			col = positionA[intt];
			if (col=='q') return 0;
			if (col=='b')
			{
				board[px][py]='+';
				board[x][y]='+';
				continue;
			}
			cin >> row
			px=x;
			py=y;
			x=15-row;
			y=col-'A';
			if (is_valid_move(x, y, board))
			{ 
				place_piece(x, y);
				print_board();
				if (is_win(x, y, board))
				{
					cout << "You won.";
					return 0;
				}
				if (is_tied(board))
				{
					cout << "It's a TIED" << endl;
					return 0;
				}
				switch_piece();
			}
			else
			{
				cout << "Invalid move." << endl;
				cin >> col >> row;
				continue;
			}
		}
		
		else
		{
			return_value();
			analyze(board);
			place_piece(x, y);
			print_value();
			print_board();
			for (char c='A' ; c<='O' ; c++)
				if (y==c-'A')
				{
					cout << "Computer's move:" << c << 15-x << endl;
					break;
				}

			if (is_win(x, y, board))
			{
				cout << "Computer won.";
				return 0;
			}
			if (is_tied(board))
			{
				cout << "It's a TIED" << endl;
				return 0;
			}
			switch_piece();
		}
	}
}