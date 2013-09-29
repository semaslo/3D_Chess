#ifndef GENERATETEXTURE_H_INCLUDED
#define GENERATETEXTURE_H_INCLUDED
//Geneterates a texname for a texture

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

#endif // GENERATETEXTURE_H_INCLUDED
