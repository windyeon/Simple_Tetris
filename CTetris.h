#pragma once
#define BX 4		// ���� ũ��
#define BY 4		// ���� ũ��
#define R  4		// ���� ȸ������ ��
#define BT 7		// �� ����
#define szCell 15	// ���� ��ũ��	
#define CX 10		// ���� ũ��
#define CY 20		// ���� ũ��
#define EMPTY -1
#define OUTBD -2

class CBlock {
	union {
		char Img[R][BY][BX];
		char Dummy[R* BX* BY + 1];
	};
	int Color = 0x000000;
public:
	void Init(int c, char *Pattern);
	void Display(CDC *pDC, int x, int y, int nRot=0);
	int GetColor() { return Color; }
	char C(int r, int x, int y) {
		return Img[r][y][x];
	}
};

class CTetris
{
	CBlock Block[BT];
	char Board[CY][CX];

	int t, nt;	//	����, ������
	int r;		//	���� ȸ����
	int x,y;	//	���� ��ġ
	bool bRun;
	int Score;
	int Stage;

	char B(int x, int y) {
		if (x < 0 || x >= CX || y < 0 || y >= CY) return OUTBD;
		return Board[y][x];
	}

	CBitmap bmpBack;
public:
	void Init();
	void InitNextBlock();
	void Display(CDC* pDC, int bdx, int bdy);
	bool Drop();
	bool Rotate();
	bool Move(int dx, int dy);
	bool IsPossible(int dr, int dx, int dy);
	void Block2Cell();
	void CheckBoard();
	bool IsGameOver() { return !bRun; }
	int GetDelay() { return 500 - Stage * 100; }
};

