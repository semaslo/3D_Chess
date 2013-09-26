#include <windows.h>
#include <gl/gl.h>
#include <WindowsX.h>
#include <gl/glu.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include "Mathematical.h"
#include "Piece.h"
#include "Coordinate.h"
#include "DrawPawn.h"
#include "DrawRook.h"
#include "DrawKnight.h"
#include "DrawBishop.h"
#include "DrawQueen.h"
#include "DrawKing.h"
#include "DrawSquares.h"

//Included 3 libraries in the Linker: opengl32, glu32, and gdi32
using namespace std;

string Message1 = "beginning";
ofstream out;
Piece Board[8][8];
//Creating these as external so I don't have to search Board every time I want to check if there is check or checkmate
extern unsigned int texture1;
int kingx0 = 3;
int kingy0 = 0;
int kingy1 = 7;
int kingx1 = 3;
int enpassant = 100;
int enpassanttime = 100;
int castle0 = 3;
int castle1 = 3;
float clientx, clienty;
int mousex = 10000;
int mousey = 10000;
bool player = 1;
int selection[2] = {100, 100};

bool Checkmate(Coordinate, bool);
bool Blockerabiltity(int, int, bool);
GLuint generatetexture(FILE *);
void rescalenormals(float*);
void subtract3f(float*, float*, float*);
void crossproduct3f(float*, float*, float*);
int CheckCheck(bool, int, int, int, int, int);
Coordinate Check(int, int, int);
int PlayerTurn(int);
int DrawPawn(int, int);
int DrawKnight(int, int);
int DrawRook(int, int);
int DrawQueen(int, int);
int DrawKing(int, int);
int DrawBishop(int, int);
void DrawSquares(int);
void ChangeBoardSize(HWND);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
int SetUpBoard(void);
int MovePawn(int, int, int, int, bool);
int MoveRook(int, int, int, int, bool);
int MoveKnight(int, int, int, int, bool);
int MoveBishop(int, int, int, int, bool);
int MoveQueen(int, int, int, int, bool);
int MoveKing(int, int, int, int, bool);
int UpdateBoard();
void OnLButtonDown(int, int);


DWORD WINAPI ThreadFunc( LPVOID lpParam )
{

    out.open("moves.txt");
    SetUpBoard();
    //color is used to keep track of which player's turn it is; white (1) starts
    //Enter a while loop of normal game play
    //Need to check for checkmate somewhere, perhaps at the end of the loop
    while(player < 100)
    {
        PlayerTurn(player);

        //Check for Checkmate
        Coordinate attacker;
        //If black just moved, check for Checkmate of the white king
        if (player == 0)
        {
            attacker = Check(kingx1, kingy1, 1);
            if (attacker.x != 50)
            {
                if (Checkmate(attacker, 1) == 1)
                {
                    cout << "Black won!" << endl;
                    return 0;
                }
            }
        }
        else
        {
            attacker = Check(kingx0, kingy0, 0);
            if (attacker.x != 50)
            {
                if (Checkmate(attacker, 0) == 1)
                {
                    cout << "White won!" << endl;
                    return 0;
                }
            }
        }
        //Switch color to show that it is the other person's turn
        if (player == 1)
            player = 0;
        else if (player == 0)
            player = 1;
    }
    out.close();
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    //Creating the second thread
    DWORD dwThreadId, dwThrdParam = 1;
    HANDLE hThread;
    hThread = CreateThread(
        NULL,                        // no security attributes
        0,                           // use default stack size
        ThreadFunc,                  // thread function

        &dwThrdParam,                // argument to thread function
        0,                           // use default creation flags
        &dwThreadId);
    if (hThread == NULL)
        cout << "NULL" << endl;

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          656,
                          678,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    MoveWindow(hwnd, 10, 10, 660, 680, true);
    FILE *fp = fopen("Picture 007.bmp", "rb");
    texture1 = generatetexture(fp);
    fclose(fp);



    /* program main loop */
    while (!bQuit)
    {
        while(!bQuit)
        {

            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            }
            else
            {
                if (Message1 == "Piece moving" || Message1 == "selection" || Message1 == "beginning")
                {

                glClearColor(1.0, 1.0, 1.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
                ChangeBoardSize(hwnd);  /*Changes Board Size with the window*/
                /*May want to check to see if the size of the window has changed, rather than rerunnning the function*/
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                //gluPerspective(0, 1, 4, 8);


                glPushMatrix();

                UpdateBoard();
                glPopMatrix();
                SwapBuffers(hDC);
                Sleep(1);
                }
            }

        }
    }
    glDisable(GL_COLOR_MATERIAL);
    CloseHandle( hThread );
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);



    /* destroy the window explicitly */
    DestroyWindow(hwnd);



    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        case WM_LBUTTONDOWN:
        {
            OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }

        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}



void ChangeBoardSize(HWND hwnd)
{
    RECT ClientArea;
    GetClientRect(hwnd, &ClientArea);
    if (ClientArea.right<ClientArea.bottom)
        glViewport(0, 0, ClientArea.right, ClientArea.right);
    else
        glViewport(0, 0, ClientArea.bottom, ClientArea.bottom);

    clientx = ClientArea.right;
    clienty = ClientArea.bottom;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

}


int SetUpBoard()
{
    //black pieces
    //black pawns
    for(int i=0; i < 8; i++)
    {
        Board[i][1].piecetype=1; //1 for pawn
        //change this side to black while we're at it
        Board[i][1].color = 0;
        Board[i][0].color = 0;
    }
    Board[0][0].piecetype = 2; //2 for rook
    Board[7][0].piecetype = 2;
    Board[1][0].piecetype = 3; //3 for knight
    Board[6][0].piecetype = 3;
    Board[5][0].piecetype = 4; //4 for bishop
    Board[2][0].piecetype = 4;
    Board[4][0].piecetype = 5; //5 for queen
    Board[3][0].piecetype = 6; //6 for king

    //white pieces
    //white pawns
    for(int i = 0; i < 8; i++)
    {
        Board[i][6].piecetype = 1;
        Board[i][7].color = 1;
        Board[i][6].color = 1;
    }
    Board[0][7].piecetype = 2;
    Board[7][7].piecetype = 2;
    Board[1][7].piecetype = 3;
    Board[6][7].piecetype = 3;
    Board[5][7].piecetype = 4;
    Board[2][7].piecetype = 4;
    Board[4][7].piecetype = 5;
    Board[3][7].piecetype = 6;

    //sets all other squares to 0 (empty)
    for(int i=0; i <8; i++)
    {
        for(int j=2; j<6; j++)
        {
            Board[i][j].piecetype = 0;
        }
    }
    return 0;
}

int PlayerTurn(int color)
{
    int x, y;
    int endy;
    int endx;
    int error = 1;
    do
    {
        do
        {
            //mousex is larger than 7 if a square has not been clicked yet
            if(mousex > 7)
                error = 1;
            else
            {
                x = mousex;
                y = mousey;
                if ((Board[x][y].piecetype == 0) || (Board[x][y].color != color))
                {
                    //cout << "The square you clicked on was invalid.  Please choose another square." << endl;
                    error = 1;
                }
                else
                {
                error = 0;
                selection[0] = x;
                selection[1] = y;
                }
            }
        }
        while(error == 1);

        Message1 = "selection";

        while(mousey == y && x == mousex)
        {
        }
        while (Message1 != "done calculating")
        {
        }
        //cout << mousex << " $ " << mousey << endl;

        endy = mousey;
        endx = mousex;

        //Checks to make sure it is not hitting a piece of the same color
        //Also would check to make sure that the end square is different from the beginning square
        if (Board[endx][endy].piecetype != 0 && Board[endx][endy].color == color)
        {
            error = 1;
        }
        else
        {
        switch (Board[x][y].piecetype)
        {
            case 0:
            {
                error = 1;
            }
            case 1:
            {
                error = MovePawn(x, y, endx, endy, color);
                break;
            }
            case 2:
            {
                error = MoveRook(x, y, endx, endy, color);
                break;
            }
            case 3:
            {
                error = MoveKnight(x, y, endx, endy, color);
                break;
            }
            case 4:
            {
                error = MoveBishop(x, y, endx, endy, color);
                break;
            }
            case 5:
            {
                error = MoveQueen(x, y, endx, endy, color);
                break;
            }
            case 6:
            {
                error = MoveKing(x, y, endx, endy, color);
                break;
            }
        }
        }
        mousex = 100;
        mousey = 100;
        selection[0] = 100;
        selection[1] = 100;
    }
    while (error == 1);
    if (enpassanttime < 2)
        enpassanttime++;
    //Save a record to a text file
    Message1 = "Piece moving";
    out << x << " " << y << " " << endx << " " << endy << endl;

    return error;
}


int MovePawn(int x, int y, int endx, int endy, bool color)
{
    //splitting it because numbers for white (red) will be negative
    if (color == 0)
    {
        if(y == 1 && endy-y == 2 && Board[x][y+1].piecetype == 0 && Board[x][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                enpassant = endx;
                enpassanttime = 0;
                return 0;
            }
            else
                return 1;
        }
        else if (endy-y == 1 && Board[endx][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }
        //Checking to see if it can take another piece out en passant
        else if (enpassanttime == 1 && enpassant == endx && endy == 5)
        {
            cout << enpassant << endl;
            if (abs(endx-x) == 1 && endy-y == 1)
            {
                if (CheckCheck(color, 1, x, y, endx, endy) == 50)
                {
                    Board[endx][y].piecetype = 0;
                    Board[endx][endy].piecetype = 1;
                    Board[endx][endy].color = color;
                    Board[x][y].piecetype = 0;
                    return 0;
                }
                else
                    return 1;
            }
            else
            {
                return 1;
            }
        }
        //Checking to see it is moving diagonally 1 and hitting another piece
        else if (abs(endx-x) == 1 && endy-y == 1 && Board[endx][endy].piecetype != 0 && Board[endx][endy].color != color)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }

        else
        {
            return 1;
        }
    }
    else if (color == 1)
    {
        if (y == 6 && y-endy == 2 && Board[x][y-1].piecetype == 0 && Board[x][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                enpassant = endx;
                enpassanttime = 0;
                return 0;
            }
            else
                return 1;
        }
        else if (endy - y == -1 && Board[endx][endy].piecetype == 0 && endx == x)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }
        else if (abs(endx-x) == 1 && y-endy == 1 && Board[endx][endy].piecetype != 0 && Board[endx][endy].color != color)
        {
            if (CheckCheck(color, 1, x, y, endx, endy) == 50)
            {
                Board[x][y].piecetype = 0;
                Board[endx][endy].piecetype = 1;
                Board[endx][endy].color = color;
                return 0;
            }
            else
                return 1;
        }
        //pawn capture en passant
        else if (enpassanttime == 1 && abs(endx-x) == 1 && endy - y == -1)
        {
            cout << enpassant << endl;
            if (enpassant == endx && endy == 2)
            {
                if (CheckCheck(color, 1, x, y, endx, endy) == 50)
                {
                    Board[endx][y].piecetype = 0;
                    Board[endx][endy].piecetype = 1;
                    Board[endx][endy].color = color;
                    Board[x][y].piecetype = 0;
                    return 0;
                }
                else
                    return 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }


    }
    //Pawn Exchange
    if ((color == 1 && endy == 0) || ((color == 0 && endy == 7)))
    {
        cout << "Which piece would you like to exchange your pawn for?"  << endl;
        cout << "2: rook" << endl << "3: knight" << endl << "4: bishop" << endl << "5: queen" << endl;
        int choice;
        cin >> choice;
        Board[endx][endy].piecetype = choice;
    }

    return 0;
}


int MoveKnight(int x, int y, int endx, int endy, bool color)
{
    //check to see if move is valid
    if(abs(endx - x) > 2 || abs(endy - y) > 2 || endx == x || endy == y || abs(endx-x) == abs(endy-y))
    {
        return 1;
    }
    else if (CheckCheck(color, 3, x, y, endx, endy) == 50)
    {
        Board[x][y].piecetype = 0;
        Board[endx][endy].piecetype = 3;
        Board[endx][endy].color = color;
    }
    else
        return 1;
    return 0;
}

int MoveRook(int x, int y, int endx, int endy, bool color)
{
    //No need to check if endy-y != 0 because this has already been disproven
    if (x == endx)
    {
        //Checks to see if there are pieces between starting and ending squares
        if (endy - y > 0)
        {
            for(int n = y+1; n < endy; n++)
            {
                if(Board[x][n].piecetype != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            for(int n = y-1; n > endy; n--)
            {
                if(Board[x][n].piecetype != 0)
                {
                    return 1;
                }
            }
        }
    }
    else if (y == endy)
    {
        if (endx - x > 0)
        {
            for(int n = x+1; n < endx; n++)
            {
                if(Board[n][y].piecetype != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            for (int n = x-1; n > endx; n--)
            {
                if (Board[n][y].piecetype != 0)
                {
                    return 1;
                }
            }
        }
    }
    //If it fails these two conditions, the rook cannot move there.
    else
    {
        return 1;
    }

    if (CheckCheck(color, 2, x, y, endx, endy) != 50)
        return 1;
    //If everthing is correct, move piece; if anything is incorrect, the function will return 1

    Board[x][y].piecetype = 0;
    Board[endx][endy].piecetype = 2;
    Board[endx][endy].color = color;
    //changing castle0 and castle1 which tell if a king can castle 0 = cannot castle, 1 = can castle of the left only, 2 can castle on the right only
    if (color == 0)
    {
        if (x == 0 && y == 0)
        {
            if (castle0 == 3)
                castle0 = 2;
            else if (castle0 == 1)
                castle0 = 0;
        }
        else if (x == 7 && y == 0)
        {
            if (castle0 == 3)
                castle0 = 1;
            else if (castle0 == 2)
                castle0 = 0;
        }
    }
    else if (color == 1)
    {
        if (x == 0 && y == 7)
        {
            if (castle1 == 3)
                castle1 = 2;
            else if (castle1 == 1)
                castle1 = 0;
        }
        else if (x == 7 && y == 7)
        {
            if (castle1 == 3)
                castle1= 1;
            else if (castle1 == 2)
                castle1 = 0;
        }
    }

    return 0;
}


int MoveQueen(int x, int y, int endx, int endy, bool color)
{
    //Same as for Bishop
    if(abs(endx-x) == abs(endy-y))
    {
        int deltax, deltay;
        if (endx > x)
            deltax = 1;
        else
            deltax = -1;

        if (endy > y)
            deltay = 1;
        else
            deltay = -1;

        int i = x + deltax;
        int j = y + deltay;
        while (i != endx && j != endy)
        {
            if(Board[i][j].piecetype != 0)
            {
                return 1;
            }
            i += deltax;
            j += deltay;
        }

    }
    //Same as for Rook
    else if (x == endx)
    {
        //Checks to see if there are pieces between starting and ending squares
        if (endy - y > 0)
        {
            for(int n = y+1; n < endy; n++)
            {
                if(Board[x][n].piecetype != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            for(int n = y-1; n > endy; n--)
            {
                if(Board[x][n].piecetype != 0)
                {
                    return 1;
                }
            }
        }
    }
    else if (y == endy)
    {
        if (endx - x > 0)
        {
            for(int n = x+1; n < endx; n++)
            {
                if(Board[n][y].piecetype != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            for (int n = x-1; n > endx; n--)
            {
                if (Board[n][y].piecetype != 0)
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        return 1;
    }
    if (CheckCheck(color, 5, x, y, endx, endy) != 50)
        return 1;

    Board[x][y].piecetype = 0;
    Board[endx][endy].piecetype = 5;
    Board[endx][endy].color = color;
    return 0;
}

int MoveKing(int x, int y, int endx, int endy, bool color)
{
    if ((abs(endx - x) == 1 || endx -x == 0) && (abs(endy - y) == 1 || endy - y == 0) && CheckCheck(color, 6, x, y, endx, endy) == 50)
    {
        Board[endx][endy].piecetype = 6;
        Board[endx][endy].color = color;
        Board[x][y].piecetype = 0;
    }
    //Castling
    else if ((endx - x == 2 || endx - x == -2) && endy == y)
    {
        if (color == 0)
        {
            if (endx == 1 && (castle0 == 1 || castle0 == 3))
            {
                if (Board[2][0].piecetype == 0 && CheckCheck(color, 6, x, y, 2, 0) == 50 && CheckCheck(color, 6, x, y, 1, 0) == 50)
                {
                    castle0 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[2][0].piecetype = 2;
                    Board[0][0].piecetype = 0;
                    Board[endx][endy].color = 0;
                    Board[2][0].color = 0;
                }
                else
                {
                    return 1;
                }
            }
            else if (endx == 5 && (castle0 == 2 || castle0 == 3))
            {
                if (Board[4][0].piecetype == 0 && CheckCheck(color, 6, x, y, 4,0) == 50 && CheckCheck(color, 6, x, y, 5, 0) == 50)
                {
                    castle0 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[endx][endy].color = 0;
                    Board[4][0].piecetype = 2;
                    Board[4][0].color = 0;
                    Board[7][0].piecetype = 1;
                }
                else
                {
                    return 1;
                }
            }
        }
        else if (color == 1)
        {
            if (endx == 1 && (castle1 == 1 || castle1 == 3))
            {
                if (Board[2][7].piecetype == 0 && CheckCheck(color, 6, x, y, 2, endy) == 50 && CheckCheck(color, 6, x, y, 1, endy) == 50)
                {
                    castle1 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[2][7].piecetype = 2;
                    Board[0][7].piecetype = 0;
                    Board[endx][endy].color = 1;
                    Board[2][7].color = 1;
                }
                else
                {
                    return 1;
                }
            }
            else if (endx == 5 && (castle1 == 2 || castle1 == 3))
            {
                if (Board[4][7].piecetype == 0 && CheckCheck(color, 6, x, y, 4,7) == 50 && CheckCheck(color, 6, x, y, 5, 7) == 50)
                {
                    castle1 = 0;
                    Board[endx][endy].piecetype = 6;
                    Board[x][y].piecetype = 0;
                    Board[endx][endy].color = 1;
                    Board[4][7].piecetype = 2;
                    Board[4][7].color = 1;
                    Board[7][7].piecetype = 0;
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        return 1;
    }
    if (color == 0)
    {
        castle0 = 0;
        kingx0 = endx;
        kingy0 = endy;
    }
    else if (color == 1)
    {
        castle1 = 0;
        kingx1 = endx;
        kingy1 = endy;
    }
    return 0;
}

int UpdateBoard()
{

    float white_specular[] = {1.0, 1.0, 1.0, 1.0};
    float white_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    float white_ambient[] = {7, 7, 7, 1.0};
    float mat_shininess[] = {60};
    float black_specular[] = {2.0, 2.0, 2.0, 1.0};
    float black_diffuse[] = {0.2, 0.2, 0.2, 1.0};
    float black_ambient[] = {0, 0.0, 0., 1.0};
    float black_shininess[] = {50};
    float light_position[] = {20.0, 1.0, -100.0};
    if (player == 1)
    {
        light_position[2] = 40;
        light_position[0] = -20;
    }
    float white_light[] = {1.0, 1.0, 1.0, 1.0};
    float lmodel_ambient[] = {.1, .1, .1, 1.0};
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glFrustum(.5, -.5, -1, 0, 1, 10);
    gluLookAt(0, .5, 0, 0, 0, -1, 0, 1, 0);
    glTranslatef(0, -2, -2);
    if (player == 0)
    {
        glRotatef(180, 0, 1, 0);
    }

    glTranslatef(-1, 0, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DrawSquares(8);
    //Draw Square that is selected
    //Lighting attributes
    float selected_ambient[] = {0.0, 0.0, 5.0, 1.0};
    float selected_diffuse[] = {0.0, 0.0, 5.0, 1.0};
    float selected_specular[] = {0.0, 0.0, 5.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, selected_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, selected_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, selected_specular);
    glBegin(GL_QUADS);
        glVertex3f(selection[0]/4.0, 0.0001, selection[1]/4.0);
        glVertex3f((selection[0]+1)/4.0, 0.0001, selection[1]/4.0);
        glVertex3f((selection[0]+1)/4.0, 0.0001, (selection[1]+1)/4.0);
        glVertex3f(selection[0]/4.0, 0.0001, (selection[1]+1)/4.0);
    glEnd();
    //Draw the pieces
    glBindTexture(GL_TEXTURE_2D, texture1);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(Board[i][j].piecetype != 0)
            {
                //set color
                if(Board[i][j].color == 0)
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
                    glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);
                }
                else
                {
                    glColor3f(1.0, 1.0, 1.0);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white_specular);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, white_diffuse);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
                    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
                }
                //Choose correct drawing function
                switch (Board[i][j].piecetype)
                {
                    case 1:
                    {
                        DrawPawn(i, j);
                        break;
                    }
                    case 2:
                    {
                        DrawRook(i, j);
                        break;
                    }
                    case 3:
                    {
                        DrawKnight(i, j);
                        break;
                    }
                    case 4:
                    {
                        DrawBishop(i, j);
                        break;
                    }
                    case 5:
                    {
                        DrawQueen(i, j);
                        break;
                    }
                    case 6:
                    {
                        DrawKing(i, j);
                        break;
                    }
                }
            }
        }
    }

    glPopMatrix();
    return 0;
}


Coordinate Check(int kingx, int kingy, int color)
{
    //Checking 4 cardinal directions from king; stops if it hits a piece; If piece is of the same color as the king, moves to the next direction
    Coordinate toreturn;
    //Try replacing this with a while loop - while (Board[i][kingy].piecetype != 0 && i < 8) ... - this would get rid of the multiple break statements
    for(int i = kingx + 1; i < 8; i++)
    {
        if (Board[i][kingy].piecetype != 0)
        {
            if (Board[i][kingy].color == color)
                break;
            else
            {
                if (Board[i][kingy].piecetype == 2 || Board[i][kingy].piecetype == 5)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
                else if (Board[i][kingy].piecetype == 6 && (i - kingx) == 1)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
            }
            break;
        }
    }
    for (int i = kingx - 1; i >= 0; i--)
    {
        if (Board[i][kingy].piecetype != 0)
        {
            if (Board[i][kingy].color == color)
                break;
            else
            {
                if (Board[i][kingy].piecetype == 2 || Board[i][kingy].piecetype == 5)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
                else if (Board[i][kingy].piecetype == 6 && (kingx - i) == 1)
                {
                    toreturn.x = i;
                    toreturn.y = kingy;
                    return toreturn;
                }
            }
            break;
        }
    }
    for (int i = kingy + 1; i < 8; i++)
    {
        if (Board[kingx][i].piecetype != 0)
        {
            if (Board[kingx][i].color == color)
                break;
            else
            {
                if (Board[kingx][i].piecetype == 2 || Board[kingx][i].piecetype == 5)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
                else if (Board[kingx][i].piecetype == 6 && (i - kingy) == 1)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
            }
            break;
        }
    }
    for (int i = kingy - 1; i >= 00; i--)
    {
        if (Board[kingx][i].piecetype != 0)
        {
            if (Board[kingx][i].color == color)
                break;
            else
            {
                if (Board[kingx][i].piecetype == 2 || Board[kingx][i].piecetype == 5)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
                else if (Board[kingx][i].piecetype == 6 && (kingy - i) == 1)
                {
                    toreturn.x = kingx;
                    toreturn.y = i;
                    return toreturn;
                }
            }
            break;
        }
    }
    //Checking the 4 diagonals from king
    //Upper right diagonal
    //for some reason it gave me an error message when I declared i and j within the for loop parentheses and I needed to put and between the two arguments in the middle section
    int i, j;
    for ( i = (kingx + 1), j = (kingy + 1); j < 8 && i < 8; i++, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (j - kingy == 1)
                {
                    //If the color is black, then white pawns one diagonal away can take out the king
                    if (color == 0 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }
    //Lower right diagonal
    for (i = (kingx + 1), j = (kingy - 1); j >= 0 && i < 8; i++, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (kingy - j == 1)
                {
                    //If the color is white, then white pawns one diagonal away can take out the king
                    if (color == 1 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }
    //Lower left diagonal
    for (i = kingx - 1, j = kingy - 1; j >= 0 && i >= 0; i--, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (kingy - j == 1)
                {
                    //If the color is white, then white pawns one diagonal away can take out the king
                    if (color == 1 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }
    //Upper left diagonal
    for (i = kingx - 1, j = kingy + 1; j < 8 && i >= 0; i--, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == color)
                break;
            else
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                {
                    toreturn.x = i;
                    toreturn.y = j;
                    return toreturn;
                }
                else if (kingy - j == 1)
                {
                    //If the color is black, then white pawns one diagonal away can take out the king
                    if (color == 0 && Board[i][j].piecetype == 1)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                    else if (Board[i][j].piecetype == 6)
                    {
                        toreturn.x = i;
                        toreturn.y = j;
                        return toreturn;
                    }
                }
            }
            break;
        }
    }

    //Checking the spots where a knight could be and hit the king
    if(kingx+1 < 8 && kingy+2 < 8)
    {
        if (Board[kingx + 1][kingy + 2].piecetype == 3 && Board[kingx + 1][kingy + 2].color != color)
        {
            toreturn.x = kingx + 1;
            toreturn.y = kingy + 2;
            return toreturn;
        }
    }
    if(kingx+1 < 8 && kingy -2 >= 0)
    {
        if (Board[kingx + 1][kingy - 2].piecetype == 3 && Board[kingx + 1][kingy - 2].color != color)
        {
            toreturn.x = kingx + 1;
            toreturn.y = kingy - 2;
            return toreturn;
        }
    }
    if(kingx- 1 >=0 && kingy - 2 >= 0)
    {
        if (Board[kingx - 1][kingy - 2].piecetype == 3 && Board[kingx - 1][kingy - 2].color != color)
        {
            toreturn.x = kingx - 1;
            toreturn.y = kingy - 2;
            return toreturn;
        }
    }
    if(kingx - 1 >= 0 && kingy + 2 < 8)
    {
        if (Board[kingx - 1][kingy + 2].piecetype == 3 && Board[kingx - 1][kingy + 2].color != color)
        {
            toreturn.x = kingx - 1;
            toreturn.y = kingy + 2;
            return toreturn;
        }
    }
    if(kingx - 2 >= 0 && kingy + 1 < 8)
    {
        if (Board[kingx - 2][kingy + 1].piecetype == 3 && Board[kingx - 2][kingy + 1].color != color)
        {
            toreturn.x = kingx - 2;
            toreturn.y = kingy + 1;
            return toreturn;
        }
    }
    if(kingx + 2 < 8 && kingy + 1 < 8)
    {
        if (Board[kingx + 2][kingy + 1].piecetype == 3 && Board[kingx + 2][kingy + 1].color != color)
        {
            toreturn.x = kingx + 2;
            toreturn.y = kingy + 1;
            return toreturn;
        }
    }
    if(kingx + 2 < 8 && kingy - 1 >= 0)
    {
        if (Board[kingx + 2][kingy - 1].piecetype == 3 && Board[kingx + 2][kingy - 1].color != color)
        {
            toreturn.x = kingx + 2;
            toreturn.y = kingy - 1;
            return toreturn;
        }
    }
    if(kingx - 2 >= 0 && kingy - 1 >= 0)
    {
        if (Board[kingx - 2][kingy - 1].piecetype == 3 && Board[kingx - 2][kingy - 1].color != color)
        {
            toreturn.x = kingx - 2;
            toreturn.y = kingy - 1;
            return toreturn;
        }
    }

    //If the function doesn't return by the time it gets here, the king is not in check
    toreturn.x = 50;
    toreturn.y = 50;
    return toreturn;
}

int CheckCheck(bool color, int piecetype, int x, int y, int endx, int endy)
{
    //Changes kingx_ if the pioece moving is a king
    //int tempkx, int tempky;
    if (piecetype == 6)
    {
        if (color == 0)
        {
            kingx0 = endx;
            kingy0 = endy;
        }
        else
        {
            kingx1 = endx;
            kingy1 = endy;
        }
    }
    //Change Board temporarily so that it agrees with move
    int temp = Board[endx][endy].piecetype;
    int tempcolor = Board[endx][endy].color;
    Board[endx][endy].piecetype = piecetype;
    Board[endx][endy].color = color;
    Board[x][y].piecetype = 0;
    int n;
    if (color == 0)
        n = Check(kingx0, kingy0, color).x;
    else if (color == 1)
        n = Check(kingx1, kingy1, color).x;
    //Undo temporary changes
    Board[endx][endy].piecetype = temp;
    Board[endx][endy].color = tempcolor;
    Board[x][y].piecetype = piecetype;
    // Undoing changes to kingx0
    if (piecetype == 6)
    {
        if (color == 0)
        {
            kingx0 = x;
            kingy0 = y;
        }
        else
        {
            kingx1 = x;
            kingy1 = y;
        }
    }
    return n;
}


void OnLButtonDown(int x, int y)
{
    Message1 = "calculating";
    float windowsize;
    if (clientx > clienty)
        windowsize = clienty;
    else
        windowsize = clientx;
    float temp;
    temp = y/windowsize*642.0f/129.6;
    temp = floor(log(temp)/.1644);
    if (player == 0)
        temp = 8.0-temp;
    else if (player == 1)
        temp = temp - 1;
    if (temp >= 0 && temp < 8)
    {
        mousey = temp;
    }
    float squarewidth;
    squarewidth = (windowsize - 2 * (-.3619 * y + 207.02))/8.0f*644.0f/windowsize;

    //The problem is with temp in the temp in the next line
    temp = x/windowsize*644.0f - (-.3619f*y + 207.02f)*644.0f/windowsize;
    temp = floor(temp/squarewidth);
    if (player == 1)
        temp = 7 - temp;
    if (temp >= 0 && temp < 8)
        mousex = temp;
    Message1 = "done calculating";
}

GLuint generatetexture(FILE* fp)
{
    if (fp == NULL)
    {
        printf("File did not open.");
    }
    BITMAPFILEHEADER bmfh;
    fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);

    BITMAPINFOHEADER bmih;
    fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp);

    unsigned int size = bmih.biSizeImage;
    BYTE PixelArray[size];
    fread(&PixelArray, 1, size, fp);
    //Switching order of Array;
    BYTE* Pixelpointer = &(PixelArray[2]);
    unsigned int i = 2;
    while(i < size)
    {
        BYTE temporary;
        temporary = *Pixelpointer;
        *(Pixelpointer) = *(Pixelpointer - 2);
        *(Pixelpointer - 2) = temporary;
        Pixelpointer += 3;
        i += 3;
    }
    static GLuint texname;

    //glShadeModel(GL_FLAT);


    glGenTextures(1, &texname);

    glBindTexture(GL_TEXTURE_2D, texname);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bmih.biWidth, bmih.biHeight, 0,
        GL_RGB, GL_UNSIGNED_BYTE, &PixelArray);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texname);
    return texname;
    }

//Checks to see if a piece can move inbetween the king and the attacking piece; a modified version of Check
//returns 0 if no piece can block, 1 if a piece can block
bool Blockerability(int squarex, int squarey, bool kingcolor)
{
    //Checking 4 cardinal directions from square
    for(int i = squarex + 1; i < 8; i++)
    {
        if (Board[i][squarey].piecetype != 0)
        {
            if (Board[i][squarey].color == kingcolor)
            {
                if (Board[i][squarey].piecetype == 2 || Board[i][squarey].piecetype == 5)
                    return 1;
                break;
            }
            break;
        }
    }
    for (int i = squarex - 1; i >= 0; i--)
    {
        if (Board[i][squarey].piecetype != 0)
        {
            if (Board[i][squarey].color == kingcolor)
            {
                if (Board[i][squarey].piecetype == 2 || Board[i][squarey].piecetype == 5)
                    return 1;
                break;
            }
            break;
        }
    }
    for (int i = squarey + 1; i < 8; i++)
    {
        if (Board[squarex][i].piecetype != 0)
        {
            if (Board[squarex][i].piecetype == kingcolor)
            {
                if (Board[squarex][i].piecetype == 2 || Board[squarex][i].piecetype == 5)
                    return 1;
                //white pawns would be able to move
                else if (Board[squarex][i].piecetype == 1 && (i - squarey) == 1 && kingcolor == 1)
                    return 1;
                //White pawns may also be able to move two from the starting row
                else if (Board[squarex][i].piecetype == 1 && i - squarey == 2 && kingcolor == 1)
                    return 1;
                break;
            }
            break;
        }
    }
    for (int i = squarey - 1; i >= 00; i--)
    {
        if (Board[squarex][i].piecetype != 0)
        {
            if (Board[squarex][i].color == kingcolor)
            {
                if (Board[squarex][i].piecetype == 2 || Board[squarex][i].piecetype == 5)
                    return 1;
                //Black pawns would be able to move
                else if (Board[squarex][i].piecetype == 1 && (squarey - i) == 1 && kingcolor == 0)
                    return 1;
                else if (Board[squarex][i].piecetype == 1 && (squarey - i) == 2 && kingcolor == 0)
                    return 1;
                break;
            }
            break;
        }
    }
    //Checking the 4 diagonals from king
    //Upper right diagonal
    //for some reason it gave me an error message when I declared i and j within the for loop parentheses and I needed to put and between the two arguments in the middle section
    int i, j;
    for ( i = (squarex + 1), j = (squarey + 1); j < 8 && i < 8; i++, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }
    //Lower right diagonal
    for (i = (squarex + 1), j = (squarey - 1); j >= 0 && i < 8; i++, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }
    //Lower left diagonal
    for (i = squarex - 1, j = squarey - 1; j >= 0 && i >= 0; i--, j--)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }
    //Upper left diagonal
    for (i = squarex - 1, j = squarey + 1; j < 8 && i >= 0; i--, j++)
    {
        if (Board[i][j].piecetype != 0)
        {
            if (Board[i][j].color == kingcolor)
            {
                if (Board[i][j].piecetype == 5 || Board[i][j].piecetype == 4)
                    return 1;
                break;
            }
            break;
        }
    }

    //Checking the spots where a knight could be and move to the square
    if(squarex+1 < 8)
    {
        if (squarey + 2 < 8)
        {
            if (Board[squarex + 1][squarey + 2].piecetype == 3 && Board[squarex + 1][squarey + 2].color == kingcolor)
                return 1;
        }
        if (squarey - 2 >= 0)
        {
            if (Board[squarex + 1][squarey - 2].piecetype == 3 && Board[squarex + 1][squarey - 2].color == kingcolor)
            return 1;
        }
    }
    if(squarex - 1 >=0)
    {
        if (squarey - 2 >= 0)
        {
            if (Board[squarex - 1][squarey - 2].piecetype == 3 && Board[squarex - 1][squarey - 2].color == kingcolor)
                return 1;
        }
        if (squarey + 2 < 8)
        {
            if (Board[squarex - 1][squarey + 2].piecetype == 3 && Board[squarex - 1][squarey + 2].color == kingcolor)
                return 1;
        }
    }
    if(squarex - 2 >= 0)
    {
        if (squarey + 1 < 8)
        {
            if (Board[squarex - 2][squarey + 1].piecetype == 3 && Board[squarex - 2][squarey + 1].color == kingcolor)
                return 1;
        }
        if (squarey - 1 >= 0)
        {
            if (Board[squarex - 2][squarey - 1].piecetype == 3 && Board[squarex - 2][squarey - 1].color == kingcolor)
                return 1;
        }
    }
    if(squarex + 2 < 8)
    {
        if (squarey + 1)
        {
            if (Board[squarex + 2][squarey + 1].piecetype == 3 && Board[squarex + 2][squarey + 1].color == kingcolor)
                return 1;
        }
        if (squarey - 1 >= 0)
        {
            if (Board[squarex + 2][squarey - 1].piecetype == 3 && Board[squarex + 2][squarey - 1].color == kingcolor)
                return 1;
        }
    }

    //If the function doesn't return by the time it gets here, a piece cannot more to the square
    return 0;
}

//Checks to see if the king is in Checkmate; return 0 if not in checkmate, 1 if in checkmate
bool Checkmate(Coordinate attacker, bool kingcolor)
{
    //Determining what the other color is
    bool attackercolor;
    if (kingcolor == 0)
        attackercolor = 1;
    else
        attackercolor = 0;

    //Determining where the king is
    Coordinate kingcoords;
    if (kingcolor == 0)
    {
        kingcoords.x = kingx0;
        kingcoords.y = kingy0;
    }
    else
    {
        kingcoords.x = kingy1;
        kingcoords.y = kingy1;
    }

    //Checking to see if the attacking piece can be taken out
    //Also checks to make sure that piece that can take out the attacker is not putting the king into check
    Coordinate takerouter;
    takerouter = Check(attacker.x, attacker.y, attackercolor);

    if (takerouter.x != 50)
    {
        if (CheckCheck(kingcolor, Board[takerouter.x][takerouter.y].piecetype, takerouter.x, takerouter.y, attacker.x, attacker.y) == 50)
            return 0;
    }

    //Checking to see if a piece can block the king from the attacking piece
    //Must make sure that the attacking piece is not a knight
    if (Board[attacker.x][attacker.y].piecetype != 3)
    {
        //Determines what direction the attacking piece is from the king for the for-loop
        int a, b;
        if (attacker.x > kingcoords.x)
            a = 1;
        else if (attacker.x == kingcoords.x)
            a = 0;
        else
            a = -1;

        if (attacker.y > kingcoords.y)
            b = 1;
        else if (attacker.y == kingcoords.y)
            b = 0;
        else
            b = -1;
        cout << endl << a << " " << b << endl;
        cout << attacker.x << " " << attacker.y << endl;
        for (int i = kingcoords.x + a, j = kingcoords.y + b; i != attacker.x && j != attacker.y; i += a, j += b)
        {
            cout << i << " " << j << endl;
            int a = Blockerability(i, j, kingcolor);
            cout << "Blockerability " << a << endl;
            if (a == 1)
                return 0;
            /*if (Blockerability(i, j, kingcolor) == 1)
                return 0;*/
        }
    }

    //Checking to see if the king can move to escape
    if (kingcoords.x + 1 < 8)
    {
        if (Board[kingcoords.x + 1][kingcoords.y].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y) == 50)
            return 0;

        if (Board[kingcoords.x + 1][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y+1) == 50)
        if (kingcoords.y + 1 < 8)
        {
            if (Board[kingcoords.x + 1][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y+1) == 50)
                return 0;

            if (Board[kingcoords.x][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x, kingcoords.y+1) == 50)
                return 0;
        }
        if (kingcoords.y  - 1 >= 0)
        {
            if (Board[kingcoords.x + 1][kingcoords.y - 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x + 1, kingcoords.y-1) == 50)
                return 0;
            if (Board[kingcoords.x][kingcoords.y - 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x, kingcoords.y-1) == 50)
                return 0;
        }
    }
    if (kingcoords.x - 1 >= 0)
    {
        if (Board[kingcoords.x - 1][kingcoords.y].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x - 1, kingcoords.y) == 50)
            return 0;
        if (kingcoords.y + 1 < 8)
        {
            if (Board[kingcoords.x - 1][kingcoords.y + 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x - 1, kingcoords.y+1) == 50)
                return 0;
        }
        if (kingcoords.y - 1 >= 0)
        {
            if (Board[kingcoords.x - 1][kingcoords.y - 1].piecetype == 0 && CheckCheck(kingcolor, 6, kingcoords.x, kingcoords.y, kingcoords.x - 1, kingcoords.y-1) == 50)
                return 0;
        }
    }
    cout << "Checkmate!" << endl;
    return 1;
}

int MoveBishop(int x, int y, int endx, int endy, bool color)
{
    if(abs(endx-x) == abs(endy-y))
    {
        int deltax, deltay;
        if (endx > x)
            deltax = 1;
        else
            deltax = -1;

        if (endy > y)
            deltay = 1;
        else
            deltay = -1;

        int i = x + deltax;
        int j = y + deltay;
        while (i != endx && j != endy)
        {
            if(Board[i][j].piecetype != 0)
            {
                return 1;
            }
            i += deltax;
            j += deltay;
        }

    }
    else
    {
        return 1;
    }
    if (CheckCheck(color, 4, x, y, endx, endy) != 50)
        return 1;

    Board[x][y].piecetype = 0;
    Board[endx][endy].piecetype = 4;
    Board[endx][endy].color = color;
    return 0;
}
