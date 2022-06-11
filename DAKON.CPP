#include <iostream>
#include <Windows.h>
#include <time.h>
#include <conio.h>
using namespace std;

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}
void printMap(int x);

class node
{
private:
	node* next;
	node* prev;
	int value;
	int isPos;
	int ownership;

public:
	node(int val = 7)
	{
		value = val;
		next = NULL;
		prev = NULL;
		isPos = 5;
		ownership = 5;
	}
	void setVal(int val)
	{
		value = val;
	}
	void setNext(node* x)
	{
		next = x;
	}
	void setPrev(node* x)
	{
		prev = x;
	}
	void addVal(int x = 1)
	{
		value += x;
	}
	void setStat(int x)
	{
		isPos = x;
	}
	void setOwner(int x)
	{
		ownership = x;
	}
	int getOwner()
	{
		return ownership;
	}
	int getVal()
	{
		return value;
	}
	int getStat()
	{
		return isPos;
	}
	node* getNext()
	{
		return next;
	}
	node* getPrev()
	{
		return prev;
	}
};

class mapAttributes //CircularDoubleLinkedlist
{
private:
	node* head;
	node* tail;
	int size;
	int len;

public:
	mapAttributes(int sz = 7)
	{
		len = 0;
		head = NULL;
		tail = NULL;
		size = sz;
	}
	int getLen()
	{
		return len;
	}
	void setSize(int sz)
	{
		size = sz;

		if (size < 7)
			deleteBoard();
		else
		{
			for (int i = 0;i < 4 * (size - 7);i++)
			{
				addboard();
			}
		}
	}

	node* getNodeat(int x)
	{
		node* iterator = head;
		for (int i = 0;i < x;i++)
		{
			iterator = iterator->getNext();
		}
		return iterator;
	}

	bool checkFinish()
	{
		bool finish = true;
		node* iterator = head;
		for (int i = 0;i < len;i++)
		{
			if (iterator->getStat() == 5 && iterator->getVal() != 0)
			{
				finish = false;
				break;
			}
			iterator = iterator->getNext();
		}
		return finish;
	}

	node* getNodePos(int user)
	{
		node* iterator = head;
		int firstPost = size;
		for (int i = 0;i <= user;i++)
		{
			iterator = getNodeat(firstPost);
			firstPost += size + 1;
		}
		return iterator;
	}
	int getSize()
	{
		return size;
	}

	void deleteBoard()
	{
		int deleteLen = (7 - size) * 4;

		node* iterator = tail;
		for (int i = 1;i < deleteLen;i++)
		{
			iterator = iterator->getPrev();
		}

		iterator->getPrev()->setNext(head);
		head->setPrev(iterator->getPrev());
		tail = iterator->getPrev();

		len -= deleteLen;
	}

	void makePost()
	{
		int firstPost = size;
		for (int i = 0;i < 4;i++)
		{
			getNodeat(firstPost)->setVal(0);
			getNodeat(firstPost)->setStat(i);
			firstPost += size + 1;
		}
	}

	void giveOwner()
	{
		int firstPost = size;
		for (int i = 0;i < 4 * (size + 1);i++)
		{
			if (i < firstPost)
			{
				getNodeat(i)->setOwner(0);
			}
			else if (i < 2 * firstPost + 1)
			{
				getNodeat(i)->setOwner(1);
			}
			else if (i < 3 * firstPost + 2)
			{
				getNodeat(i)->setOwner(2);
			}
			else if (i < 4 * firstPost + 3)
			{
				getNodeat(i)->setOwner(3);
			}
		}
	}
	void addboard()
	{
		node* temp = new node;
		temp->setVal(7);
		if (head == NULL)
		{
			head = temp;
			tail = head;
			len++;
		}
		else
		{
			tail->setNext(temp);
			temp->setPrev(tail);
			temp->setNext(head);
			head->setPrev(temp);
			tail = temp;
			len++;
		}
	}

	void initialize()
	{
		int temp = 0;
		for (int i = 0;i < 4 * (size + 1);i++)
		{
			addboard();
		}
	}

};

class cgame//kurang buat mode 2,3,4
{
private:
	mapAttributes board;
	int mode;
	int turn;
	int user;
	bool win;
	bool firstStrike;
	int timer;
public:
	cgame()
	{
		firstStrike = false;
		timer = 0;
		win = false;
		user = 0;
		board.initialize();
		mode = 0;
		turn = 0;
	}
	int getTime()
	{
		return GetTickCount() - timer;
	}

	void drawGame()
	{
		printMap(board.getSize());

		int boardSize = board.getSize();
		char player = char(65 + user);

		gotoxy(0, 0);
		cout << "PLAYER " << player << " TURN!";
		gotoxy(25, 0);
		cout << "Turn Left: " << board.getSize() * 2 - board.getSize() / 2 - turn;

		int x = 18 + ((boardSize - 1) * 7);
		int y = 9 + (boardSize * 3);

		int xPos = 30 + ((boardSize - 1) * 7);
		int yPos = 10 + (boardSize * 3);

		gotoxy(6, yPos);cout << player; player++;if (player == 'E')player = 'A';
		gotoxy(6, 3);cout << player; player++;if (player == 'E')player = 'A';
		gotoxy(xPos, 3);cout << player; player++;if (player == 'E')player = 'A';
		gotoxy(xPos, yPos);cout << player;

		node* iterator = board.getNodeat(user * (boardSize + 1));
		gotoxy(x, y);

		int temp = 0;

		for (int i = 0;i < board.getLen();i++)//cetak isi board
		{
			if (i == boardSize || i == 2 * boardSize + 1 || i == 3 * boardSize + 2 || i == 4 * boardSize + 3)
			{
				if (iterator->getVal() != 0)
					cout << iterator->getVal();
			}
			else
				cout << iterator->getVal();

			iterator = iterator->getNext();

			if (i < boardSize)
			{
				if (i == boardSize - 1)
				{
					gotoxy(x, y + 2);
					cout << temp++;

					x -= 9;
					gotoxy(x, y);
				}
				else
				{
					gotoxy(x, y + 2);
					cout << temp++;

					x -= 7;
					gotoxy(x, y);
				}
			}
			else if (i < 2 * boardSize + 1)
			{
				if (i == 2 * boardSize)
				{
					y -= 4;
					gotoxy(x, y);
				}
				else if (i == boardSize)
				{
					y -= 4;
					gotoxy(x, y);
				}
				else
				{
					y -= 3;
					gotoxy(x, y);
				}

			}
			else if (i < 3 * boardSize + 2)
			{
				if (i == 3 * boardSize + 1)
				{
					x += 9;
					gotoxy(x, y);
				}
				else if (i == 2 * boardSize + 1)
				{
					x += 9;
					gotoxy(x, y);
				}
				else
				{
					x += 7;
					gotoxy(x, y);
				}
			}
			else if (i < 4 * boardSize + 3)
			{
				if (i == 4 * boardSize + 2)
				{
					y += 4;
					gotoxy(x, y);
				}
				else if (i == 3 * boardSize + 2)
				{
					y += 4;
					gotoxy(x, y);
				}
				else
				{
					y += 3;
					gotoxy(x, y);
				}
			}
		}
	}

	void initialize()
	{
		int chc;
		char chc1;
		do
		{
			cout << "Pilih Mode: " << endl;
			cout << "1. 1 v 1 v 1 v 1 (Local Multiplayer)\n";
			cout << "2. 1 v 1 v 1 v 1 (Lawan bot)\n";
			cout << "3. 2 v 2 (Team dengan teman)\n";
			cout << "4. 2 v 2 (Team melawan bot)\n";
			cout << "Choice: ";
			cin >> chc;

			if (chc < 1 || chc >4)
				cout << "Input tidak sesuai" << endl;
		} while (chc < 1 || chc>4);

		mode = chc;

		cout << "Lakukan kustomasi board? [y/n]";
		cin >> chc1;

		if (chc1 == 'y' || chc1=='Y')
		{
			do
			{
				cout << "Masukan banyak board [2-9]: ";
				cin >> chc;
				if (chc <2  || chc>9)
					cout << "Input tidak sesuai " << endl;
			} while (chc < 2 || chc>9);
			board.setSize(chc);
		}
		board.giveOwner();
		board.makePost();
		system("CLS");
	}

	void input(int idx)
	{
		int yHeader = 14 + (board.getSize() * 3);
		int xHeader = 18 + ((board.getSize() / 2 - 2) * 7);

		int teman = user - 2;
		if (teman < 0)
			teman += 4;
		int boardSize = board.getSize();
		node* iterator = board.getNodeat(user * (boardSize + 1) + idx);

		int poin = iterator->getVal();
		iterator->setVal(0);

		while (poin > 0)
		{
			iterator = iterator->getNext();
			if (iterator->getStat() == 5 || iterator->getStat() == user)
			{
				iterator->addVal();
				poin--;
			}

			if (mode == 1)
			{
				if (poin == 0)
				{
					system("CLS");
					drawGame();
					if (iterator->getStat() == 5 && iterator->getVal() > 1)//ngambil biasa	
					{
						poin = iterator->getVal();
						iterator->setVal(0);
					}
					else if (iterator->getStat() == 5 && iterator->getOwner() == user && iterator->getVal() == 1)//nembak
					{
						char temp;
						gotoxy(xHeader, yHeader);
						cout << "(MENEMBAK) Masukkan Kode musuh [A/B/C/D]: ";
						gotoxy(xHeader, yHeader + 1);
						cout << " \n";
						gotoxy(xHeader, yHeader + 1);
						cin >> temp;

						int musuh = (int)temp - 65;
						int ctr;
						if (musuh != user)
						{
							if (musuh < user)
								ctr = musuh + 4 - user;
							else
								ctr = musuh - user;

							node* musuh = iterator;

							for (int i = 0;i < ctr * (boardSize + 1);i++)
							{
								musuh = musuh->getNext();
							}

							board.getNodePos(user)->addVal(musuh->getVal() + 1);
							musuh->setVal(0);
						}
						iterator->setVal(0);
					}
					else if (iterator->getStat() == 5 && iterator->getOwner() != user && iterator->getVal() == 1)//mikul
					{
						if (iterator->getNext()->getStat() == 5)
						{
							board.getNodePos(user)->addVal(iterator->getNext()->getVal());
							iterator->getNext()->setVal(0);
						}

						if (iterator->getPrev()->getStat() == 5)
						{
							board.getNodePos(user)->addVal(iterator->getPrev()->getVal());
							iterator->getPrev()->setVal(0);
						}
						board.getNodePos(user)->addVal(iterator->getVal());
						iterator->setVal(0);

					}
					else if (iterator->getStat() == user)//Ace
					{
						int rng = rand() % board.getSize();

						node* musuh = board.getNodeat(user * (boardSize + 1) + rng);

						for (int j = 0;j < 3;j++)
						{
							for (int i = 0;i < boardSize + 1;i++)
							{
								musuh = musuh->getNext();
							}

							board.getNodePos(user)->addVal(musuh->getVal());
							musuh->setVal(0);
						}
					}
					gotoxy(0, yHeader + 3);
				}
			}
			else if (mode == 2)
			{
				if (user == 0)
				{
					if (poin == 0)
					{
						system("CLS");
						drawGame();
						if (iterator->getStat() == 5 && iterator->getVal() > 1) //ngambil biasa	
						{
							poin = iterator->getVal();
							iterator->setVal(0);
						}
						else if (iterator->getOwner() == user && iterator->getVal() == 1 && iterator->getStat() == 5) // nembak mode 2
						{
							char temp;
							gotoxy(xHeader, yHeader);
							cout << "(MENEMBAK) Masukkan Kode musuh [A/B/C/D]: ";
							gotoxy(xHeader, yHeader + 1);
							cout << " \n";
							gotoxy(xHeader, yHeader + 1);
							cin >> temp;

							int musuh = (int)temp - 65;
							int ctr;
							if (musuh != user)
							{
								if (musuh < user)
									ctr = musuh + 4 - user;
								else
									ctr = musuh - user;

								node* musuh = iterator;

								for (int i = 0; i < ctr * (boardSize + 1); i++)
								{
									musuh = musuh->getNext();
								}

								board.getNodePos(user)->addVal(musuh->getVal() + 1);
								musuh->setVal(0);
							}
							iterator->setVal(0);
						}
						else if (iterator->getOwner() != user && iterator->getStat() == 5 && iterator->getVal() == 1)//mikul
						{
							if (iterator->getPrev()->getStat() == 5)
							{
								board.getNodePos(user)->addVal(iterator->getPrev()->getVal());
								iterator->getPrev()->setVal(0);
							}
							if (iterator->getNext()->getStat() == 5)
							{
								board.getNodePos(user)->addVal(iterator->getNext()->getVal());
								iterator->getNext()->setVal(0);
							}
							board.getNodePos(user)->addVal(iterator->getVal());
							iterator->setVal(0);
						}
						else if (iterator->getStat() == user)
						{
							int random = rand() % board.getSize();
							node* musuh = board.getNodeat(user * (boardSize + 1) + random);

							for (int j = 0; j < 3; j++)
							{
								for (int i = 0; i < boardSize + 1; i++)
								{
									musuh = musuh->getNext();
								}

								board.getNodePos(user)->addVal(musuh->getVal());
								musuh->setVal(0);
							}
						}
						gotoxy(0, yHeader + 3);
					}
				}
				else //bot
				{
					if (poin == 0)
					{
						system("CLS");
						drawGame();
						if (iterator->getStat() == 5 && iterator->getVal() > 1) //ngambil biasa	
						{
							poin = iterator->getVal();
							iterator->setVal(0);
						}
						else if (iterator->getOwner() == user && iterator->getVal() == 1 && iterator->getStat() == 5) // nembak mode 2
						{

							int musuh = -1;
							while (musuh == -1)
							{
								musuh = rand() % 4;
								if (musuh == user)
								{
									musuh = -1;
								}
							}
							gotoxy(xHeader, yHeader + 1);
							cout << "Pilihan user: " << (char)(musuh + 65) << endl;
							int ctr;
							if (musuh != user)
							{
								if (musuh < user)
									ctr = musuh + 4 - user;
								else
									ctr = musuh - user;

								node* musuh = iterator;

								for (int i = 0; i < ctr * (boardSize + 1); i++)
								{
									musuh = musuh->getNext();
								}

								board.getNodePos(user)->addVal(musuh->getVal() + 1);
								musuh->setVal(0);
							}
							iterator->setVal(0);
						}
						else if (iterator->getOwner() != user && iterator->getStat() == 5 && iterator->getVal() == 1)//mikul bot
						{
							if (iterator->getPrev()->getStat() == 5)
							{
								board.getNodePos(user)->addVal(iterator->getPrev()->getVal());
								iterator->getPrev()->setVal(0);
							}
							if (iterator->getNext()->getStat() == 5)
							{
								board.getNodePos(user)->addVal(iterator->getNext()->getVal());
								iterator->getNext()->setVal(0);
							}
							board.getNodePos(user)->addVal(iterator->getVal());
							iterator->setVal(0);
						}
						else if (iterator->getStat() == user)
						{
							int random = rand() % board.getSize();
							node* musuh = board.getNodeat(user * (boardSize + 1) + random);

							for (int j = 0; j < 3; j++)
							{
								for (int i = 0; i < boardSize + 1; i++)
								{
									musuh = musuh->getNext();
								}

								board.getNodePos(user)->addVal(musuh->getVal());
								musuh->setVal(0);
							}
						}
						gotoxy(0, yHeader + 3);
					}
				}
			}
			else if (mode == 3)
			{
				if (poin == 0)
				{
					system("CLS");
					drawGame();
					if (iterator->getStat() == 5 && iterator->getVal() > 1)//ngambil biasa	
					{
						poin = iterator->getVal();
						iterator->setVal(0);
					}
					else if (iterator->getStat() == 5 && iterator->getOwner() == user && iterator->getVal() == 1)//nembak
					{
						if (user == 0 || user == 2) {
							char temp;
							gotoxy(xHeader, yHeader);
							cout << "(MENEMBAK) Masukkan Kode musuh [B/D]: ";
							gotoxy(xHeader, yHeader + 1);
							cout << " \n";
							gotoxy(xHeader, yHeader + 1);
							cin >> temp;
							int musuh = (int)temp - 65;
							int ctr;
							if (musuh != user && musuh != 0 && musuh != 2)
							{
								if (musuh < user)
									ctr = musuh + 4 - user;
								else
									ctr = musuh - user;

								node* musuh = iterator;

								for (int i = 0; i < ctr * (boardSize + 1); i++)
								{
									musuh = musuh->getNext();
								}
								board.getNodePos(user)->addVal(musuh->getVal() + 1);
								musuh->setVal(0);
							}
							iterator->setVal(0);
						}
						else if (user == 1 || user == 3) {
							char temp;
							gotoxy(xHeader, yHeader);
							cout << "(MENEMBAK) Masukkan Kode musuh [A/C]: ";
							gotoxy(xHeader, yHeader + 1);
							cout << " \n";
							gotoxy(xHeader, yHeader + 1);
							cin >> temp;
							int musuh = (int)temp - 65;
							int ctr;
							if (musuh != user && musuh == 0 || musuh != user && musuh == 2)
							{
								if (musuh < user)
									ctr = musuh + 4 - user;
								else
									ctr = musuh - user;

								node* musuh = iterator;

								for (int i = 0; i < ctr * (boardSize + 1); i++)
								{
									musuh = musuh->getNext();
								}

								board.getNodePos(user)->addVal(musuh->getVal() + 1);
								musuh->setVal(0);
							}
							iterator->setVal(0);
						}

					}
					else if (iterator->getStat() == 5 && iterator->getOwner() != user && iterator->getVal() == 1 && iterator->getOwner() != teman)//mikul
					{
						if (iterator->getNext()->getStat() == 5)
						{
							board.getNodePos(user)->addVal(iterator->getNext()->getVal());
							iterator->getNext()->setVal(0);
						}

						if (iterator->getPrev()->getStat() == 5)
						{
							board.getNodePos(user)->addVal(iterator->getPrev()->getVal());
							iterator->getPrev()->setVal(0);
						}
						board.getNodePos(user)->addVal(iterator->getVal());
						iterator->setVal(0);
					}
					else if (iterator->getStat() == user)//Ace
					{
						int rng = rand() % (board.getSize() + 1);
						node* musuh = board.getNodeat(user * (boardSize + 1) + idx);

						for (int j = 0; j < 3; j++)
						{
							for (int i = 0; i < boardSize + 1; i++)
							{
								musuh = musuh->getNext();
							}
							if (j != 1)
							{
								board.getNodePos(user)->addVal(musuh->getVal());
								musuh->setVal(0);
							}
						}
					}
					gotoxy(0, yHeader + 3);
				}
			}
			else if (mode == 4)
			{
				if (poin == 0)
				{
					system("CLS");
					drawGame();
					if (iterator->getStat() == 5 && iterator->getVal() > 1)//ngambil biasa	
					{
						poin = iterator->getVal();
						iterator->setVal(0);
					}
					else if (iterator->getStat() == 5 && iterator->getOwner() == user && iterator->getVal() == 1)//nembak
					{
						char temp;
						gotoxy(xHeader, yHeader);
						cout << "(MENEMBAK) Masukkan Kode musuh [A/B/C/D]: ";
						gotoxy(xHeader, yHeader + 1);
						cout << " \n";
						gotoxy(xHeader, yHeader + 1);
						if (user == 1 || user == 3)
						{
							int rng = (rand() % 2) * 2 - 1;
							rng = 65 + user + rng;
							if (rng > 68)rng = 65;
							temp = (char)(rng);
							cout << temp;
						}
						else
							cin >> temp;

						int musuh = (int)temp - 65;
						int ctr;
						if (musuh != user && musuh != teman)
						{
							if (musuh < user)
								ctr = musuh + 4 - user;
							else
								ctr = musuh - user;

							node* musuh = iterator;

							for (int i = 0;i < ctr * (boardSize + 1);i++)
							{
								musuh = musuh->getNext();
							}

							board.getNodePos(user)->addVal(musuh->getVal() + 1);
							musuh->setVal(0);
						}
						iterator->setVal(0);
					}
					else if (iterator->getStat() == 5 && iterator->getOwner() != user && iterator->getVal() == 1 && iterator->getOwner() != teman)//mikul
					{
						if (iterator->getNext()->getStat() == 5)
						{
							board.getNodePos(user)->addVal(iterator->getNext()->getVal());
							iterator->getNext()->setVal(0);
						}

						if (iterator->getPrev()->getStat() == 5)
						{
							board.getNodePos(user)->addVal(iterator->getPrev()->getVal());
							iterator->getPrev()->setVal(0);
						}

					}
					else if (iterator->getStat() == user)//Ace
					{
						int rng = rand() % board.getSize();

						node* musuh = board.getNodeat(user * (boardSize + 1) + rng);

						for (int j = 0;j < 3;j++)
						{
							for (int i = 0;i < boardSize + 1;i++)
							{
								musuh = musuh->getNext();
							}

							if (j != 1)
							{
								board.getNodePos(user)->addVal(musuh->getVal());
								musuh->setVal(0);
							}
						}
					}
					gotoxy(0, yHeader + 3);
				}
			}

		}

		if (!firstStrike)
		{
			if (mode == 1)
			{
				if (board.getNodePos(user)->getVal() >= 30)//firstrike
				{
					int index1;
					char usr;
					firstStrike = true;
					cout << "Pilih index anda yang ingin ditukar: ";
					cin >> index1;
					cout << "(First Strike) Masukkan Kode musuh [A/B/C/D]: ";
					cin >> usr;

					iterator = board.getNodeat(user * (boardSize + 1) + index1);

					int musuh = (int)usr - 65;
					int ctr;
					if (musuh != user)
					{
						if (musuh < user)
							ctr = musuh + 4 - user;
						else
							ctr = musuh - user;

						node* musuh = iterator;

						for (int i = 0;i < ctr * (boardSize + 1);i++)
						{
							musuh = musuh->getNext();
						}

						node* dummy = new node;
						iterator->getNext()->setPrev(musuh);
						iterator->getPrev()->setNext(musuh);

						musuh->getNext()->setPrev(iterator);
						musuh->getPrev()->setNext(iterator);

						dummy->setNext(musuh->getNext());
						dummy->setOwner(musuh->getOwner());
						dummy->setStat(musuh->getStat());
						dummy->setPrev(musuh->getPrev());

						musuh->setNext(iterator->getNext());
						musuh->setOwner(iterator->getOwner());
						musuh->setStat(iterator->getStat());
						musuh->setPrev(iterator->getPrev());
						iterator->setNext(dummy->getNext());
						iterator->setOwner(dummy->getOwner());
						iterator->setStat(dummy->getStat());
						iterator->setPrev(dummy->getPrev());

						delete dummy;
					}
				}
			}
			else if (mode == 2)
			{
				if (user == 0 && board.getNodePos(user)->getVal() >= 30)
				{
					int idxx;
					char player;
					firstStrike = true;
					cout << "Pilih index anda yang ingin ditukar: ";
					cin >> idxx;
					cout << "(First Strike) Masukkan Kode musuh [A/B/C/D]: ";
					cin >> player;
					iterator = board.getNodeat(user * (boardSize + 1) + idxx);
					int enemy = (int)player - 65;
					int control;

					if (enemy != user)
					{
						if (enemy < user)
						{
							control = enemy + 4 - user;
						}
						else
							control = enemy - user;
						node* musuh = iterator;

						for (int i = 0; i < control * (boardSize + 1); i++)
						{
							musuh = musuh->getNext();
						}
						node* temp = new node;
						iterator->getNext()->setPrev(musuh);
						iterator->getPrev()->setNext(musuh);

						musuh->getNext()->setPrev(iterator);
						musuh->getPrev()->setNext(iterator);

						temp->setNext(musuh->getNext());
						temp->setOwner(musuh->getOwner());
						temp->setStat(musuh->getStat());
						temp->setPrev(musuh->getPrev());

						musuh->setNext(iterator->getNext());
						musuh->setOwner(iterator->getOwner());
						musuh->setStat(iterator->getStat());
						musuh->setPrev(iterator->getPrev());

						iterator->setNext(temp->getNext());
						iterator->setOwner(temp->getOwner());
						iterator->setStat(temp->getStat());
						iterator->setPrev(temp->getPrev());

						delete temp;
					}

				}
				else if (user != 0 && board.getNodePos(user)->getVal() >= 30)
				{
					int idxx = rand() % board.getSize();
					int enemy = rand() % 4;
					firstStrike = true;

					while (enemy == user)
					{
						enemy = rand() % 4;
					}
					int control;

					iterator = board.getNodeat(user * (boardSize + 1) + idxx);

					if (enemy != user)
					{
						if (enemy < user)
						{
							control = enemy + 4 - user;
						}
						else
							control = enemy - user;
						node* musuh = iterator;

						for (int i = 0; i < control * (boardSize + 1); i++)
						{
							musuh = musuh->getNext();
						}
						node* temp = new node;
						iterator->getNext()->setPrev(musuh);
						iterator->getPrev()->setNext(musuh);

						musuh->getNext()->setPrev(iterator);
						musuh->getPrev()->setNext(iterator);

						temp->setNext(musuh->getNext());
						temp->setOwner(musuh->getOwner());
						temp->setStat(musuh->getStat());
						temp->setPrev(musuh->getPrev());

						musuh->setNext(iterator->getNext());
						musuh->setOwner(iterator->getOwner());
						musuh->setStat(iterator->getStat());
						musuh->setPrev(iterator->getPrev());

						iterator->setNext(temp->getNext());
						iterator->setOwner(temp->getOwner());
						iterator->setStat(temp->getStat());
						iterator->setPrev(temp->getPrev());

						delete temp;
					}
				}
			}
			else if (mode == 3)
			{
				if (board.getNodePos(user)->getVal() >= 30)//firstrike
				{
					int index1;
					char usr;
					firstStrike = true;
					cout << "Pilih index anda yang ingin ditukar: ";
					cin >> index1;
					cout << "(First Strike) Masukkan Kode musuh [A/B/C/D]: ";
					cin >> usr;

					iterator = board.getNodeat(user * (boardSize + 1) + index1);

					int musuh = (int)usr - 65;
					int ctr;
					if (musuh != user)
					{
						if (musuh < user)
							ctr = musuh + 4 - user;
						else
							ctr = musuh - user;

						node* musuh = iterator;

						for (int i = 0; i < ctr * (boardSize + 1); i++)
						{
							musuh = musuh->getNext();
						}

						node* dummy = new node;
						iterator->getNext()->setPrev(musuh);
						iterator->getPrev()->setNext(musuh);

						musuh->getNext()->setPrev(iterator);
						musuh->getPrev()->setNext(iterator);

						dummy->setNext(musuh->getNext());
						dummy->setOwner(musuh->getOwner());
						dummy->setStat(musuh->getStat());
						dummy->setPrev(musuh->getPrev());

						musuh->setNext(iterator->getNext());
						musuh->setOwner(iterator->getOwner());
						musuh->setStat(iterator->getStat());
						musuh->setPrev(iterator->getPrev());
						iterator->setNext(dummy->getNext());
						iterator->setOwner(dummy->getOwner());
						iterator->setStat(dummy->getStat());
						iterator->setPrev(dummy->getPrev());

						delete dummy;
					}
				}
			}
			else if (mode == 4)
			{
				if (board.getNodePos(user)->getVal() >= 30)//firstrike
				{
					int index1;
					char usr;
					firstStrike = true;
					if (user == 1 || user == 3)
					{
						index1 = rand() % board.getSize();
						cout << "Pilih index anda yang ingin ditukar: ";
						cout << index1;
					}
					else
					{
						cout << "Pilih index anda yang ingin ditukar: ";
						cin >> index1;
					}

					if (user == 1 || user == 3)
					{
						int rng = (rand() % 3) + 1;
						rng = user + rng;
						if (rng > 68)rng -= 4;
						usr = (char)(rng);
						cout << "(First Strike) Masukkan Kode musuh [A/B/C/D]: ";
						cout << usr;
					}
					else
					{
						cout << "(First Strike) Masukkan Kode musuh [A/B/C/D]: ";
						cin >> usr;
					}

					iterator = board.getNodeat(user * (boardSize + 1) + index1);

					int musuh = (int)usr - 65;
					int ctr;
					if (musuh != user)
					{
						if (musuh < user)
							ctr = musuh + 4 - user;
						else
							ctr = musuh - user;

						node* musuh = iterator;

						for (int i = 0;i < ctr * (boardSize + 1);i++)
						{
							musuh = musuh->getNext();
						}

						node* dummy = new node;
						iterator->getNext()->setPrev(musuh);
						iterator->getPrev()->setNext(musuh);

						musuh->getNext()->setPrev(iterator);
						musuh->getPrev()->setNext(iterator);

						dummy->setNext(musuh->getNext());
						dummy->setOwner(musuh->getOwner());
						dummy->setStat(musuh->getStat());
						dummy->setPrev(musuh->getPrev());

						musuh->setNext(iterator->getNext());
						musuh->setOwner(iterator->getOwner());
						musuh->setStat(iterator->getStat());
						musuh->setPrev(iterator->getPrev());
						iterator->setNext(dummy->getNext());
						iterator->setOwner(dummy->getOwner());
						iterator->setStat(dummy->getStat());
						iterator->setPrev(dummy->getPrev());

						delete dummy;
					}
				}
			}
		}

		if (board.checkFinish() || (turn == board.getSize() * 2 - board.getSize() / 2))
		{
			int max, index = 0;
			max = board.getNodePos(0)->getVal();

			if (max < board.getNodePos(1)->getVal())
			{
				max = board.getNodePos(1)->getVal();
				index = 1;
			}
			if (max < board.getNodePos(2)->getVal())
			{
				max = board.getNodePos(2)->getVal();
				index = 2;
			}
			if (max < board.getNodePos(3)->getVal())
			{
				max = board.getNodePos(3)->getVal();
				index = 3;
			}

			if (mode == 3 || mode == 4)
			{
				int temp;
				if (index + 67 > 68)temp = index + 65;
				else
					temp = user + 67;
				cout << "Player " << (char)(index + 65) << " dan " << (char)(temp) << " Menang" << endl;
			}
			else
				cout << "Player " << (char)(index + 65) << " Menang" << endl;
			win = true;
		}
	}

	void run()
	{
		int yHeader = 14 + (board.getSize() * 3);
		int xHeader = 18 + ((board.getSize() / 2 - 1) * 7);
		int idx;
		bool pass;

		while (!win)// yang bisa bot kalo team, C N D, Kalo freefor all bot B-D
		{
			system("CLS");
			pass = true;
			drawGame();
			gotoxy(xHeader, yHeader);
			timer = GetTickCount();

			if (mode == 1)
			{
				cout << "Masukkan Index: ";
				gotoxy(xHeader, yHeader + 1);
				while (getTime() < 60000 && pass)
				{
					if (_kbhit())
					{
						do
						{
							cin >> idx;
							if (idx<0 || idx>board.getSize() - 1)
							{
								gotoxy(xHeader, yHeader + 1);
								cout << " \n";
								cout << "Index tidak benar";
								gotoxy(xHeader, yHeader + 1);
							}
						} while (idx<0 || idx>board.getSize() - 1);
						pass = false;
					}
				}
				if (pass)
				{
					gotoxy(xHeader, yHeader + 2);
					cout << "Maaf waktu anda habis\n";
				}
				else
				{
					input(idx);
				}
			}
			else if (mode == 2)
			{
				//jangan lupa input, lek bot g usa kbhit
				if (user == 0)
				{
					cout << "Masukkan Index: ";
					gotoxy(xHeader, yHeader + 1);
					while (getTime() < 60000 && pass)
					{
						if (_kbhit())
						{
							do
							{
								cin >> idx;
								if (idx<0 || idx>board.getSize() - 1)
								{
									gotoxy(xHeader, yHeader + 1);
									cout << " \n";
									cout << "Index tidak benar";
									gotoxy(xHeader, yHeader + 1);
								}
							} while (idx<0 || idx>board.getSize() - 1);
							pass = false;
						}
					}
					if (pass)
					{
						gotoxy(xHeader, yHeader + 2);
						cout << "Maaf waktu anda habis\n";
					}
					else
					{
						input(idx);
					}
				}
				else
				{
					int getsize = board.getSize();
					idx = rand() % getsize;


					gotoxy(xHeader, yHeader + 2);
					cout << "User " << (char)(user + 65) << " : " << idx << "\n";

					system("pause");
					input(idx);
				}

			}
			else if (mode == 3)
			{
				//jangan lupa input
				cout << "Masukkan Index: ";
				gotoxy(xHeader, yHeader + 1);
				while (getTime() < 60000 && pass)
				{
					if (_kbhit())
					{
						do
						{
							cin >> idx;
							if (idx<0 || idx>board.getSize() - 1)
							{
								gotoxy(xHeader, yHeader + 1);
								cout << " \n";
								cout << "Index tidak benar";
								gotoxy(xHeader, yHeader + 1);
							}
						} while (idx<0 || idx>board.getSize() - 1);
						pass = false;
					}
				}
				if (pass)
				{
					gotoxy(xHeader, yHeader + 2);
					cout << "Maaf waktu anda habis\n";
				}
				else
				{
					input(idx);
				}
			}
			else if (mode == 4)// 2v2 team lawan bot
			{
				cout << "Masukkan Index: ";
				gotoxy(xHeader, yHeader + 1);

				if (user == 0 || user == 2)
				{
					while (getTime() < 60000 && pass)
					{
						if (_kbhit())
						{
							do
							{
								cin >> idx;
								if (idx<0 || idx>board.getSize() - 1)
								{
									gotoxy(xHeader, yHeader + 1);
									cout << " \n";
									cout << "Index tidak benar";
									gotoxy(xHeader, yHeader + 1);
								}
							} while (idx<0 || idx>board.getSize() - 1);
							pass = false;
						}
					}
					if (pass)
					{
						gotoxy(xHeader, yHeader + 2);
						cout << "Maaf waktu anda habis\n";
					}
					else
					{
						input(idx);
					}
				}
				else
				{
					int rng = rand() % board.getSize();
					idx = rng;

					gotoxy(xHeader, yHeader + 2);
					cout << "User " << (char)(user + 65) << " : " << idx << "\n";
					input(idx);
				}
			}

			user++;
			if (user == 4)
			{
				user = 0;
				turn++;
			}
			system("pause");
		}
		gotoxy(0, yHeader + 5);
	}
};

void main()
{
	srand(time(NULL));
	cgame dakon;
	dakon.initialize();
	dakon.run();
}




//<----------------------------------------------------------------MAP-------------------------------------------------------------->

void printMap(int x)
{
	gotoxy(5, 2);
	int ctr = 3;
	if (x == 2)
	{
		cout << " -------                  -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                  -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                      ---";gotoxy(5, ctr++);
			cout << "  |   |                    |   |";gotoxy(5, ctr++);
			cout << "   ---                      ---";gotoxy(5, ctr++);
		}
		cout << " -------                  -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                  -------";gotoxy(5, ctr++);
	}
	else if (x == 3)
	{
		cout << " -------                         -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                         -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                             ---";gotoxy(5, ctr++);
			cout << "  |   |                           |   |";gotoxy(5, ctr++);
			cout << "   ---                             ---";gotoxy(5, ctr++);
		}
		cout << " -------                         -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                         -------";gotoxy(5, ctr++);
	}
	else if (x == 4)
	{
		cout << " -------                                -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                                    ---";gotoxy(5, ctr++);
			cout << "  |   |                                  |   |";gotoxy(5, ctr++);
			cout << "   ---                                    ---";gotoxy(5, ctr++);
		}
		cout << " -------                                -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                -------";gotoxy(5, ctr++);
	}
	else if (x == 5)
	{
		cout << " -------                                       -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                       -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                                           ---";gotoxy(5, ctr++);
			cout << "  |   |                                         |   |";gotoxy(5, ctr++);
			cout << "   ---                                           ---";gotoxy(5, ctr++);
		}

		cout << " -------                                       -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                       -------";

	}
	else if (x == 6)
	{
		cout << " -------                                              -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                              -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                                                  ---";gotoxy(5, ctr++);
			cout << "  |   |                                                |   |";gotoxy(5, ctr++);
			cout << "   ---                                                  ---";gotoxy(5, ctr++);
		}

		cout << " -------                                              -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                              -------";
	}
	else if (x == 7)
	{
		cout << " -------                                                     -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                                     -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                                                         ---";gotoxy(5, ctr++);
			cout << "  |   |                                                       |   |";gotoxy(5, ctr++);
			cout << "   ---                                                         ---";gotoxy(5, ctr++);
		}

		cout << " -------                                                     -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                                     -------";
	}
	else if (x == 8)
	{
		cout << " -------                                                            -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                                            -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                                                                ---";gotoxy(5, ctr++);
			cout << "  |   |                                                              |   |";gotoxy(5, ctr++);
			cout << "   ---                                                                ---";gotoxy(5, ctr++);
		}

		cout << " -------                                                            -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                                            -------";
	}
	else if (x == 9)
	{
		cout << " -------                                                                   -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                                                   -------";gotoxy(5, ctr++);

		for (int i = 0;i < x;i++)
		{
			cout << "   ---                                                                       ---";gotoxy(5, ctr++);
			cout << "  |   |                                                                     |   |";gotoxy(5, ctr++);
			cout << "   ---                                                                       ---";gotoxy(5, ctr++);
		}

		cout << " -------                                                                   -------";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << "|       |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |   |  |       |";gotoxy(5, ctr++);
		cout << "|       |   ---    ---    ---    ---    ---    ---    ---    ---    ---   |       |";gotoxy(5, ctr++);
		cout << " -------                                                                   -------";
	}
	cout << "\n\n";
}