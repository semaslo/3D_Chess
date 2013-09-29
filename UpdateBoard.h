#ifndef UPDATEBOARD_H_INCLUDED
#define UPDATEBOARD_H_INCLUDED

//Includes the OpenGL code to draw the board

#include "DrawSquares.h"
#include "DrawPawn.h"
#include "DrawKnight.h"
#include "DrawKing.h"
#include "DrawQueen.h"
#include "DrawRook.h"
#include "DrawBishop.h"
bool player = 1;
extern Piece Board[8][8];
int selection[2] = {100, 100};

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

#endif // UPDATEBOARD_H_INCLUDED
