//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//  Versao 09/2010
//
//**********************************************************

#include "Bmp.h"
#include <string.h>

Bmp::Bmp(const char *fileName)
{
   width = height = 0, rotacao = 0;
   data = NULL;
   if( fileName != NULL && strlen(fileName) > 0 )
   {
      load(fileName);
   }
   else
   {
      printf("Error: Invalid BMP filename");
   }
}

uchar* Bmp::getImage()
{
  return data;
}

int Bmp::getWidth(void)
{
  return width;
}

int Bmp::getHeight(void)
{
  return height;
}

int Bmp::getRotacao(void)
{
  return rotacao;
}

void Bmp::setRotacao(int r)
{
  rotacao = r;
}

int Bmp::getZoom(void)
{
  return zoom;
}

void Bmp::setZoom(int z)
{
   if(z>-5 && z<5) zoom = z;
}

void Bmp::init_rgb(){
   for(int i = 0; i < 3; i++){
      for(int j=0; j<256;j++){
         rgb[i][j] = 0;
      }
   }
}

void Bmp::leituraRGB(){
   int i, j;
   uchar *image = getImage();
   unsigned char r, g, b;

   for(int x=0; x<getHeight(); x++){
      for(int y=0; y<getWidth(); y++){
         i = x*bytesPerLine;
         j = y*3;

         r = image[i+j];
         g = image[i+j+1];
         b = image[i+j+2];
         rgb[0][r] += 1;
         rgb[1][g] += 1;
         rgb[2][b] += 1;

         Pixel *p = new Pixel(r, g, b, x, y);
         pixel[x][y] = p;
      }
   }
}

void Bmp::convertBGRtoRGB()
{
  unsigned char tmp;
  if( data != NULL )
  {
     for(int y=0; y<height; y++)
     for(int x=0; x<width*3; x+=3)
     {
        int pos = y*bytesPerLine + x;
        tmp = data[pos];
        data[pos] = data[pos+2];
        data[pos+2] = tmp;
     }
  }
  init_rgb();
  leituraRGB();
}

void Bmp::load(const char *fileName)
{
  FILE *fp = fopen(fileName, "rb");
  if( fp == NULL )
  {
     printf("\nErro ao abrir arquivo %s para leitura", fileName);
     return;
  }

  printf("\n\nCarregando arquivo %s", fileName);

  //le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
  //o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
  fread(&header.type,      sizeof(unsigned short int), 1, fp);
  fread(&header.size,      sizeof(unsigned int),       1, fp);
  fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
  fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
  fread(&header.offset,    sizeof(unsigned int),       1, fp);

  //le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
  fread(&info.size,        sizeof(unsigned int),       1, fp);
  fread(&info.width,       sizeof(int),                1, fp);
  fread(&info.height,      sizeof(int),                1, fp);
  fread(&info.planes,      sizeof(unsigned short int), 1, fp);
  fread(&info.bits,        sizeof(unsigned short int), 1, fp);
  fread(&info.compression, sizeof(unsigned int),       1, fp);
  fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
  fread(&info.xresolution, sizeof(int),                1, fp);
  fread(&info.yresolution, sizeof(int),                1, fp);
  fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
  fread(&info.impcolours,  sizeof(unsigned int),       1, fp);

  width = info.width;
  height = info.height;
  bits = info.bits;
  bytesPerLine = (3 * (width + 1) / 4) * 4;
  imagesize = bytesPerLine*height;
  int delta = bytesPerLine - (3 * width);
  rotacao = 0;
  zoom = 0;

   // CRIACAO DA MATRIZ DE PIXELS
  pixel = (Pixel***)malloc(height * sizeof(Pixel**));
  for(int i=0; i<height; i++){
      pixel[i] = (Pixel**)malloc(width * sizeof(Pixel*));
   }

/*
   for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
        pixel[i][j] = new Pixel(0,0,0,0,0);
    }
   }
 */


  printf("\nImagem: %dx%d - Bits: %d", width, height, bits);
  printf("\nbytesPerLine: %d", bytesPerLine);
  printf("\nbytesPerLine: %d", width * 3);
  printf("\ndelta: %d", delta);
  printf("\nimagesize: %d %d", imagesize, info.imagesize);

  if( header.type != 19778 )
  {
     printf("\nError: Arquivo BMP invalido");
     getchar();
     exit(0);
  }

  if( width*height*3 != imagesize )
  {
     printf("\nWarning: Arquivo BMP nao eh potencia de 2");
     getchar();
  }

  if( info.compression != 0 )
  {
     printf("\nError: Formato BMP comprimido nao suportado");
     getchar();
     return;
  }
  if( bits != 24 )
  {
     printf("\nError: Formato BMP com %d bits/pixel nao suportado", bits);
     getchar();
     return;
  }

  if( info.planes != 1 )
  {
     printf("\nError: Numero de Planes nao suportado: %d", info.planes);
     getchar();
     return;
  }

  data = new unsigned char[imagesize];
  fread(data, sizeof(unsigned char), imagesize, fp);

  fclose(fp);
}
