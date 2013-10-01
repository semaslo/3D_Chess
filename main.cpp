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
//Universal variable Piece Board[8][8] is first declared in SetUpBoard.h
#include "SetUpBoard.h"
#include "Check.h"
#include "CheckCheck.h"
#include "MovePieces.h"
#include "Blockerability.h"
#include "Checkmate.h"
#include "UpdateBoard.h"
#include "GenerateTexture.h"
#include "EnableDisableOpenGL.h"

//Included 3 libraries in the Linker: opengl32, glu32, and gdi32
using namespace std;

string Message1 = "beginning";
ofstream out;
extern Piece Board[8][8];
extern unsigned int texture1;
//Creating these as external so I don't have to search Board every time I want to check if there is check or checkmate
extern int enpassant;
extern int enpassanttime;
//Store if castling is possible and if so, on what sides; castle0 stores for black, castle1 for white; 3 symbolizes that castling is possible on both
//queen and king sides
extern int castle0;
extern int castle1;
//Store window size
float clientx, clienty;
//Store position of mouse
int mousex = 10000;
int mousey = 10000;
//Stores which player's turn it is
extern bool player;
//Stores which square is selected
extern int selection[2];

int PlayerTurn(int);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void OnLButtonDown(int, int);
void ChangeBoardSize(HWND);

/*
This thread runs the player's actions while the other thread, Winapi Winmain runs the message operations and redraws the board
FIXME: make more efficient
*/
DWORD WINAPI ThreadFunc( LPVOID lpParam )
{
    //Opens an output file for a record of moves
    //FIXME add the ability to undo moves
    out.open("moves.txt");
    //Sets up the board by assigning values to the Board[8][8] array
    SetUpBoard();
    //color is used to keep track of which player's turn it is; white (1) starts
    //Enter a while loop of normal game play
    while(player < 100)
    {
        //Goes through a player's turn
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
    //This thread runs the player's turn while WINAPI WinMain runs the message operations and redraws the board
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
    wcex.lpszClassName = "Chess";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "Chess",
                          "Chess",
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
    //Moves the window to the left side of the screen adn sets the size
    MoveWindow(hwnd, 10, 10, 660, 680, true);
    //Generate the texname for the texture
    FILE *fp = fopen("Picture 007.bmp", "rb");
    texture1 = generatetexture(fp);
    fclose(fp);



    /* program main loop */
    while (!bQuit)
    {
        while(!bQuit)
        {
            //Message handling
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
                //Determines if it is time to redraw the board - if so, redraws the board
                if (Message1 == "Piece moving" || Message1 == "selection" || Message1 == "beginning")
                {

                glClearColor(1.0, 1.0, 1.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
                ChangeBoardSize(hwnd);  /*Changes Board Size with the window*/
                /*May want to check to see if the size of the window has changed, rather than rerunnning the function*/
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
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
    //Closes thread
    CloseHandle( hThread );
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

//Message handling
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
        //If they click on the string
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

//Takes in the size of the window and changes the size of the viewport (where the graphics are printed) so that it is a square
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

//A player's turn
//color represents which color's turn it is, white or black
int PlayerTurn(int color)
{
    int x, y;
    int endy;
    int endx;
    int error = 1;
    //Gets input
    //Error equals 1 as long as there is an error and there is not valid input
    do
    {
        do
        {
            //mousex is larger than 7 if a square has not been clicked yet
            if(mousex > 7)
                error = 1;
            else
            {
                //Checks to make sure that the square has a piece of the correct color
                x = mousex;
                y = mousey;
                if ((Board[x][y].piecetype == 0) || (Board[x][y].color != color))
                {
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
        //Waits for person to select a different square

        while(mousey == y && x == mousex)
        {
        }
        //Then waits for OnLButtonDown to finish calculating the mousey and mousex.  Otherwise one of those is calculated before the other
        while (Message1 != "done calculating")
        {
        }

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
        //A big switch to select the appropraite function depending on which piece is moving
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
        //Resets everything so that it is ready for the next player's turn
        mousex = 100;
        mousey = 100;
        selection[0] = 100;
        selection[1] = 100;
    }
    while (error == 1);
    //enpassanttime stores the time since the move that made enpassant possible; if it is too large, enpassant is not possible
    if (enpassanttime < 2)
        enpassanttime++;
    //Change Message1 so that other thread knows to redraw board
    Message1 = "Piece moving";
    //Save a record to a text file
    out << x << " " << y << " " << endx << " " << endy << endl;

    return error;
}

void OnLButtonDown(int x, int y)
{
    //Included message to make sure that this finishes calculating before the other thread continues
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
