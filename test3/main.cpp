#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#define IDT_TIMER1 1

#define MAXLENGTH 375 //=15*25 - size of the field

void DrawRectangles( HDC );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

struct point
{
public:
	int x;
	int y;
};

class Member : public point
{
public:
	Member() { this->x = -1; this->y = -1; } //default constructor
	Member(int x, int y) { this->x = x; this->y = y; } //constructor
	void set_pos ( int x, int y ) { this->x = x; this->y = y; } //it's obvious, isn't it?
	int get_x( ) { return x; }
	int get_y( ) { return y; }
	friend int isSnake( struct point ); //friend function
};

class Snake
{
public:
	Snake( ); //constructor
	int length; //Length of snake = number of members
	int direction; //current direction of snake 1 - up, 2 - down, 3 - left, 4 - right
	int prev_direction;
	Member body[ MAXLENGTH ]; //snake's body
	void Move( ); //Moves the snake
}SNAKE;

class Field
{
public:
	Field();
	struct point size; //Unecpectable, Height and Width of field
	struct point food; //Coordinates of food
	friend int isSnake( struct point ); //When randomly places food check is this field is empty(No member of snake there) 0 - no snake, 1 - snake
	int level; //level of the game = speed of moving = time to sleep
	int isEnd(); //check if the game ended in this turn(Snake eat itself, crashed into border and e.t.c.)
	int End(); //Ends the game
	void key_pressed( int ); //move the snake according to key pressed
	void show( HDC, int, int); //draw everything
	int score; //result
	//friend void start();
	friend point gen_food();
}FIELD;

Field::Field()
{
	this->size.x = 25;
	this->size.x = 15;
}

Snake::Snake( )
{
	SNAKE.direction = 4;
	SNAKE.prev_direction = 4;
	SNAKE.length = 4;
	for(int i = 0; i<4; i++)
	{
		SNAKE.body[i] = Member(14- i, 7 );
	}
	for( int i = 4; i<MAXLENGTH; i++ )
	{
		SNAKE.body[i] = Member();
	}
}

void Snake::Move( )		//direction of snake 1 - up, 2 - down, 3 - left, 4 - right
{
	int i;

	switch (SNAKE.direction)
	{
	case 1:
		for( i = length; i > 0; i--)
		{
			SNAKE.body[i].y = SNAKE.body[i-1].y;
			SNAKE.body[i].x = SNAKE.body[i-1].x;
		}
		SNAKE.body[0].y--;
		break;
	case 2:
		for( i = length; i > 0; i--)
		{
			SNAKE.body[i].y = SNAKE.body[i-1].y;
			SNAKE.body[i].x = SNAKE.body[i-1].x;
		}
		SNAKE.body[0].y++;
		break;
	case 3:
		for( i = length; i > 0; i--)
		{
			SNAKE.body[i].y = SNAKE.body[i-1].y;
			SNAKE.body[i].x = SNAKE.body[i-1].x;
		}
		SNAKE.body[0].x--;
		break;
	case 4:
		for( i = length; i > 0; i--)
		{
			SNAKE.body[i].y = SNAKE.body[i-1].y;
			SNAKE.body[i].x = SNAKE.body[i-1].x;
		}
		SNAKE.body[0].x++;
		break;
	}
}

void Field::show( HDC hdc, int spaceWidth, int spaceHeight)
{
	HPEN gridPen = CreatePen(PS_SOLID, 1, 0x00000000);
	HBRUSH gridBrush = CreateSolidBrush(0x00000000);
	HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, gridBrush);
	int i, j, s;
	
	for (s = 0; s < SNAKE.length; s++)
	{
		i = SNAKE.body[s].x;
		j = SNAKE.body[s].y;
		POINT ptPoints1[] =
		{
			{(i - 1) * spaceWidth, (j - 1) * spaceHeight}, {i * spaceWidth, (j - 1) * spaceHeight},
			{i * spaceWidth, j * spaceHeight}, {(i - 1) * spaceWidth, j * spaceHeight}
		};
		Polygon(hdc, ptPoints1, sizeof(ptPoints1)/sizeof(ptPoints1[0]));
	}
}

void DrawGrid(HDC hdc, int spaceWidth, int spaceHeight)
{
	static HPEN gridPen = CreatePen(PS_SOLID, 1, 0x00000000);
	static HBRUSH gridBrush = CreateSolidBrush(0x00FFFFFF);

	HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, gridBrush);

	Rectangle(hdc, 0, 0, spaceWidth * 25, spaceHeight * 15);

	for (int index = 1; index <= 25; index++)
	{
		MoveToEx(hdc, spaceWidth * index, 0, NULL);
		LineTo(hdc, spaceWidth * index, spaceHeight * 15);
	}
	for (int index = 1; index <= 15; index++)
	{
		MoveToEx(hdc, 0, spaceHeight * index, NULL);
		LineTo(hdc, spaceWidth * 25, spaceHeight * index);
	}

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	PAINTSTRUCT ps;
	HDC hdc;
	SetTimer( hWnd, IDT_TIMER1, 1000, NULL);
	switch (message)
	{
	case WM_KEYDOWN:				//direction of snake 1 - up, 2 - down, 3 - left, 4 - right
		wmId    = LOWORD( wParam );
		switch( wmId )
		{
			case VK_LEFT:
				SNAKE.prev_direction = 3;
				break;
			case VK_RIGHT:
				SNAKE.prev_direction = 4;
				break;
			case VK_UP:
				SNAKE.prev_direction = 1;
				break;
			case VK_DOWN:
				SNAKE.prev_direction = 2;
				break;
		}
		break;

	case WM_KEYUP:
		if( LOWORD( wParam ) == VK_SPACE )
		{

		}
		break;

	case WM_TIMER:
		switch( wParam )
		{
			case IDT_TIMER1:
				SNAKE.direction = SNAKE.prev_direction;
				SNAKE.Move();
				InvalidateRect(hWnd, NULL, true);
				break;
		}
		break;

	case WM_PAINT:
		{
		// Get window dimensions
		RECT clientRectangle;
		GetClientRect(hWnd, &clientRectangle);
		LONG spaceWidth = clientRectangle.right / 25;
		LONG spaceHeight = clientRectangle.bottom / 15;
		if (spaceWidth == 0 || spaceHeight == 0)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		hdc = BeginPaint(hWnd, &ps);
            
		DrawGrid(hdc, spaceWidth, spaceHeight);
		FIELD.show( hdc, spaceWidth, spaceHeight );

		EndPaint(hWnd, &ps);
		return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASS  wc;
	MSG Messages;
	HWND hWndMain;
    
	wc.style            = 0;
	wc.lpfnWndProc      = (WNDPROC)WndProc;
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = hInstance;
	wc.hIcon            = NULL;
	wc.hCursor          = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground    = (HBRUSH)(COLOR_BTNFACE+1);
	wc.lpszMenuName     = L"LISTMENU";
	wc.lpszClassName    = L"Snake";
	RegisterClass( &wc );

	hWndMain = CreateWindow( L"Snake", 
                            L"Snake 1.9.87", 
							WS_OVERLAPPEDWINDOW,
                            10, 10, 
                            500, 300,
                            NULL, NULL, hInstance, NULL );

	if ( !hWndMain )
		return NULL;

	ShowWindow( hWndMain, nCmdShow );
	UpdateWindow( hWndMain ); 
    
	while (GetMessage (&Messages, NULL, 0, 0))
	{
		if (Messages.message == WM_TIMER) 
        { 
            Messages.hwnd = hWndMain; 
        } 

		TranslateMessage(&Messages);
		DispatchMessage(&Messages);
	}

	return Messages.wParam;
}