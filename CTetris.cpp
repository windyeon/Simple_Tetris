#include "pch.h"
#include "CTetris.h"
#include "resource.h"

void CBlock::Init(int c, char* Pattern)
{
	Color = c;
	strcpy_s(Dummy, Pattern);
}

void CBlock::Display(CDC* pDC, int bx, int by, int nRot)
{
	for (int y = 0; y < BY; y++)
		for (int x = 0; x < BX; x++)
			if (Img[nRot][y][x] == '1')
				pDC->FillSolidRect(bx+x*szCell, by+y*szCell, szCell, szCell, Color);
}

void CTetris::Init()
{
	//bmpBack.CreateCompatibleBitmap(pDC, )
	bmpBack.LoadBitmapA(IDB_BITMAP_BACK);

	Score = 0;
	Stage = 0;
	//srand(time(NULL));
	nt = rand() % BT;
	InitNextBlock();
	memset(Board, -1, sizeof Board);
	Board[19][3] = Board[19][4] =
	Board[19][5] = Board[18][3] = 2;
	Board[19][6] = Board[18][6] =
	Board[17][6] = Board[18][5] = 4;

	//			             0               1               2               3
	Block[0].Init(0xff0000, "0000000011000110000001001100100000001100011000000000001001100100");//Z
	Block[1].Init(0x00ff00, "0100010001000100000011110000000000100010001000100000000011110000");//I
	Block[2].Init(0x0000ff, "0000100011100000000001100100010000000000111000100000010001001100");//ㄴ
	Block[3].Init(0xffff00, "0000001011100000000001000100011000000000111010000000110001000100");
	Block[4].Init(0x00ffff, "0000010011100000000001000110010000000000111001000000010011000100");
	Block[5].Init(0xff00ff, "0000000001101100000010001100010000000000011011000000100011000100");
	Block[6].Init(0xffffff, "0000000011001100000000001100110000000000110011000000000011001100");
	bRun = true;
}

void CTetris::InitNextBlock()
{
	t = nt;
	nt = rand() % BT;
	r = 0;
	x = CX / 2 - 2;
	y = 0;
	Move(0, -1);
	Move(0, -1);
	if (!IsPossible(r, x, y))
		bRun = false;
}

void CTetris::Display(CDC* pDC, int bdx, int bdy)
{
	//pDC->FillSolidRect(bdx, bdy, CX * szCell, CY * szCell, 0x000000);
	CDC tdc, bdc;
	bdc.CreateCompatibleDC(pDC);
	CBitmap bmpBuf;
	bmpBuf.CreateCompatibleBitmap(pDC, (CX+5) * szCell,CY * szCell);
	CBitmap* pOldBufBmp = bdc.SelectObject(&bmpBuf);

	tdc.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = tdc.SelectObject(&bmpBack);
	//bdc.BitBlt(0, 0, CX * szCell, CY * szCell, &tdc, 0, 0, SRCCOPY);
	bdc.SetStretchBltMode(STRETCH_HALFTONE);
	bdc.StretchBlt(0, 0, CX * szCell, CY * szCell, &tdc, 0, 0, 420, 540, SRCCOPY);
	tdc.SelectObject(pOldBmp);

	for (int iy = 0; iy < CY; iy++)
		for (int ix = 0; ix < CX; ix++)
			if (Board[iy][ix] != EMPTY)
				bdc.FillSolidRect(
					ix * szCell, iy * szCell,
					szCell, szCell, Block[Board[iy][ix]].GetColor());

	//for (int b = 0; b < BT; b++)// 블럭리스트 출력
	//	for (int r = 0; r < R; r++)
	//		Block[b].Display(pDC, bdx + r*szCell*5, bdy + b*szCell* 5, r);

	Block[t].Display(&bdc, x * szCell, y * szCell, r);
	Block[nt].Display(&bdc, CX * szCell, 0 * szCell, 0);
	CString s;
	s.Format("%d", Score);
	bdc.SetBkMode(TRANSPARENT);
	bdc.TextOut(0, 0, s);
	pDC->BitBlt(bdx, bdy, (CX+5) * szCell, CY * szCell, &bdc, 0, 0, SRCCOPY);
	bdc.SelectObject(pOldBufBmp);
}

bool CTetris::Drop()
{
	bool bResult = Move(0, 1);
	if (bResult == false) {
		Block2Cell();
		CheckBoard();
		InitNextBlock();
	}
	return bResult;
}

bool CTetris::IsPossible(int dr, int dx, int dy)
{
	for (int iy = 0; iy < BY; iy++)
		for (int ix = 0; ix < BX; ix++)
			if (B(dx + ix, dy + iy) != EMPTY &&
				Block[t].C(dr, ix, iy) != '0')
				return false;
	return true;
}

void CTetris::Block2Cell()
{
	for (int iy = 0; iy < BY; iy++)
		for (int ix = 0; ix < BX; ix++)
			if (Block[t].C(r, ix, iy) != '0')
				Board[y + iy][x + ix] = t;
}

void CTetris::CheckBoard()
{
	for (int iy = 1; iy < CY; iy++) {
		bool bFullLine = true;
		for (int ix = 0; ix < CX; ix++)
			if (B(ix, iy) == EMPTY)
				bFullLine = false;
		if (bFullLine) {
			memmove(&Board[1][0], &Board[0][0], iy * CX);
			Score++;
			if (Score % 2 == 0) {
				Stage++;

			}
		}
	}
}

bool CTetris::Rotate()
{
	if (IsPossible((r + 1) % R, x, y) == false)
		return false;
	r = (r+1) % R;
	return true;
}

bool CTetris::Move(int dx, int dy)
{
	if (IsPossible(r, x + dx, y + dy) == false)
		return false;
	x += dx;
	y += dy;
	return true;
}
