#ifndef DRAWSQUARES_H_INCLUDED
#define DRAWSQUARES_H_INCLUDED
//OpenGL code for board, include lighting and textures
unsigned int texture1;

void DrawSquares(int windowsize)
{
    float mat_gray[] = {0, 0, 0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_gray);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_gray);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_gray);
    float mat_shininess[] = {50};
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_QUADS);
    //glBindTexture(GL_TEXTURE_2D, texture2);
    for(int a=0; a<8; a+=2)
    {
        for(int b=0; b<8; b+=2)
        {
            glTexCoord2f(b/4.0, a/4.0);         glVertex3f((b*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, a/4.0);     glVertex3f(((b+1)*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, (a+1)/4.0); glVertex3f(((b+1)*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);
            glTexCoord2f(b/4.0, (a+1)/4.0);     glVertex3f((b*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);
        }
    }
    for(int a=1; a<8; a+=2)
    {
        for(int b=1; b<8; b+=2)
        {
            glTexCoord2f(b/4.0, a/4.0);     glVertex3f((b*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, a/4.0); glVertex3f(((b+1)*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, (a+1)/4.0); glVertex3f(((b+1)*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);
            glTexCoord2f(b/4.0, (a+1)/4.0); glVertex3f((b*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);

        }
    }
    float mat_white[] = {15.0, 15.0, 15.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
    glBindTexture(GL_TEXTURE_2D, texture1);
    for(int a=0; a<8; a+=2)
    {
        for(int b=1; b<8; b+=2)
        {
            glTexCoord2f(b/4.0, a/4.0); glVertex3f((b*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, a/4.0); glVertex3f(((b+1)*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, (a+1)/4.0); glVertex3f(((b+1)*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);
            glTexCoord2f(b/4.0, (a+1)/4.0); glVertex3f((b*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);
        }
    }
    for(int a=1; a<8; a+=2)
    {
        for(int b=0; b<8; b+=2)
        {
            glTexCoord2f(b/4.0, a/4.0); glVertex3f((b*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, a/4.0);glVertex3f(((b+1)*windowsize/8)/4.0, 0, (a*windowsize/8)/4.0);
            glTexCoord2f((b+1)/4.0, (a+1)/4.0); glVertex3f(((b+1)*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);
            glTexCoord2f(b/4.0, (a+1)/4.0); glVertex3f((b*windowsize/8)/4.0, 0, ((a+1)*windowsize/8)/4.0);

        }
    }


    glEnd();
}

#endif // DRAWSQUARES_H_INCLUDED
