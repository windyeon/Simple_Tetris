#pragma once
#define BX 4		// 블럭의 크기
#define BY 4		// 블럭의 크기
#define R  4		// 블럭의 회전가능 수
#define BT 7		// 블럭 종류
#define szCell 15	// 블럭의 셀크기	
#define CX 10		// 보드 크기
#define CY 20		// 보드 크기
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

	int t, nt;	//	현블럭, 다음블럭
	int r;		//	현블럭 회전값
	int x,y;	//	현블럭 위치
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

