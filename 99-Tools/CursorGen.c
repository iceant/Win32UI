#include <CursorGen.h>

// ����ļ�����
//https://mp.weixin.qq.com/s?__biz=MzkwNzMzMjIyNg==&mid=2247486257&idx=1&sn=1656090e498c22f391c00d0857f4b856&chksm=c0db94dcf7ac1dcacd4618240e05595b984a05b55d5f9f44d277aae41f4353b06bfa5e4294c4&cur_album_id=2474385342163419137&scene=189#wechat_redirect
// ������Դ
//https://blog.csdn.net/jinzhuojun/article/details/8007586
// ע������� @ bilibili ���ü��Ե��з���¼
// �ṹע������ https://learn.microsoft.com/en-us/previous-versions/ms997538(v=msdn.10)

// ע�������ʽ�������е� WrietByte �Ķ�Ӧ��ϵ

//typedef struct
//{
//    WORD           idReserved;   // Reserved (must be 0)
//    WORD           idType;       // Resource Type (1 for icons)									// 2 ָ���� cur ��꾲̬����ļ�
//    WORD           idCount;      // How many images?
//    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
//} ICONDIR, *LPICONDIR;

//typedef struct
//{
//    BYTE        bWidth;          // Width, in pixels, of the image
//    BYTE        bHeight;         // Height, in pixels, of the image
//    BYTE        bColorCount;     // Number of colors in image (0 if >=8bpp)
//    BYTE        bReserved;       // Reserved ( must be 0)
//    WORD        wPlanes;         // Color Planes
//    WORD        wBitCount;       // Bits per pixel
//    DWORD       dwBytesInRes;    // How many bytes in this resource?
//    DWORD       dwImageOffset;   // Where in the file is this image?
//} ICONDIRENTRY, *LPICONDIRENTRY;
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void Fwrite(FILE* f, char* data, int byte)				// һ��д�� byte ���ֽ�,��ֻдһ��
{
    if(1!=fwrite(data,byte,1,f))
    {
        perror("fwrite error");
        exit(1);
    }
}

void WriteByte(FILE* f, unsigned int val,int byte)		// ָ��д�뼸���ֽ�
{
    char data[4];
    assert(byte<=4);									// ��������������ش�������ֹ����ִ�С�������̳̿ɲ�
    memcpy((void*)data,(void*)&val,byte);
    Fwrite(f,data,byte);
}

void generate_ico_file(const char* filename,char * body,
                       int width, int height, int has_alpha)
{
    int x,y;
    int index=0;
    int Size=0;
    int offset = 6+ 1*16;								// 6 = 2 +2 +2����6��byte ��ico,curͨ�õ��ļ�ͷ�� 16= 1+1+1+1 +2 +2����һ��ͼƬ������ռ 16 ���ֽڣ� n��ͼƬ������ռ6*16���ֽڣ���n��ͼƬ����Ϊ�ڲ�ͬ�豸��ʹ�ò�ͬ�ͺŷֱ��ʵ�ͼƬ����һ��ͼƬ����һ�� icon ��cur��
    int bpp=32;											// 32λ��λͼ= 8λ alpha + 8 λ R�� 8 λ G�� 8 λ B

    FILE* outfile=fopen(filename,"wb");
    if(outfile==NULL){
        perror("fopen error");
        exit(1);
    }
    // icon�ļ�ͷ��������ͼƬ����
    WriteByte(outfile,0,2);								// idReserved ����λ��һ�ָ�ʽ���������������ļ���
    WriteByte(outfile,1,2);								// idType �ļ����ͣ������1 ������icon�ļ��������2 ����cur������ļ��������ļ���ʽһ����
    WriteByte(outfile,1,2);								// idCount ӵ�е�ͼƬ������iconΪ�ڲ�ͬ��Ļ��������ʾ����洢�����ʽ��ͼƬ�Ա�ѡ��
    // ����һ��ͼƬ��������Ϣ
    WriteByte(outfile,width,1);							// bWidth
    WriteByte(outfile,height,1);						// bHeight
    WriteByte(outfile,0,1);								// bColorCount
    WriteByte(outfile,0,1);								// bReservered
    WriteByte(outfile,0,2);								// wPlanes					// ������.cur�ȵ�λ�� X
    WriteByte(outfile,0,2);							// wBitCount					// ������.cur�ȵ�λ�� Y

    Size = 40 + height * ((width + 31) / 32 * 32 / 8 + width * 3);	//Note 4 bytes alignment		// ��������ļ�����ͷ��ͼ�����ݵ����� ��40=4+4+4+2+2+4*6 ���ֽڡ� ��width+31��/32�Ǽ���int ��AND λͼ������width / 32,���ǲ���ȷ��%32 �Ĳ��֣�����+31 Ȼ���ٳ��� 32 ��1��int ��4���ֽڣ���32bit,ÿ��bit��ʾһ�������Ƿ񱻹�դ���� AND ���ǣ����� * 32����һ����Ҫ����bit ��/8��ȷ���ж����ֽڡ�
    if (bpp == 32)
        Size += height * width;
    WriteByte(outfile,Size, 4);			//dwBytesInRes
    WriteByte(outfile,offset, 4);			//dwImageOffset

    WriteByte(outfile,40, 4);				//biSize
    WriteByte(outfile,width, 4);			//biWidth
    WriteByte(outfile,2 * height, 4);		//biHeight
    WriteByte(outfile,1, 2);				//biPlanes
    WriteByte(outfile,bpp, 2);				//biBitCount
    WriteByte(outfile,0, 4);				//biCompression
    WriteByte(outfile,0, 4);				//biSizeImage
    WriteByte(outfile,0, 4);				//biXPelsPerMeter
    WriteByte(outfile,0, 4);				//biYPelsPerMeter
    WriteByte(outfile,0, 4);				//biClrUsed
    WriteByte(outfile,0, 4);				//biClrImportant

    // XOR mask
    for (y=height - 1 ; y >= 0; --y) { 					// ������ӡ�߶ȾͲ����ȡ�� ��ȷ��Ϊ���ô�ӡ��windows���Ŷ�ȡ����.�������ң��������ϣ�����Ϊ��ͼƬ���Ŷ�ȡ֮�������ģ���Ҫ��ԭͼ��һ���������ݴ�ӡ���������һ��
        for (x = 0; x < width; ++x) {
            index = (y * width + x) * 4;
            WriteByte(outfile, body[index], 1);        //Blue
            WriteByte(outfile, body[index + 1], 1);    //Green
            WriteByte(outfile, body[index + 2], 1);    //Red
            WriteByte(outfile, has_alpha ? body[index + 3] : 255, 1); //Alpha
        }
    }

    // AND mask
    for (y = 0; y < (height * ((width + 31) / 32 * 32 / 8)); ++y) {
        WriteByte(outfile, 1, 1);						// 1 ����Ļ����ʾͼƬ��0����ʾͼƬ����ʾΪ����ͼƬû�С� ��Ԫ��դ�����ο�
    }

    fclose(outfile);
}

#define WIDTH   272
#define HEIGHT  272

int main()
{
    int image[WIDTH * HEIGHT];
    int i, j;
    for (i = 0; i < HEIGHT; ++i) {
        for (j = 0; j < WIDTH; ++j) {
            if(i>=10&&i<=20){									// 0x00000000����ȫ͸�� 0x5F000000 ���Կ���������ȫ͸������֪͸���ȿɵ���
                image[i*WIDTH+j]=0x5F000000;					// Alpha ͸�� 0x00, red: 00, green: 00, blue: 00  �� bit λ��Ӧ ARGB
            }else{
                image[i * WIDTH + j]  = 0xFFFF0000;  	// pure red icon, for test
            }
        }
    }

    generate_ico_file("testv11��ɫ5F͸��.cur", (char *)image, WIDTH, HEIGHT, 1);

    return 0;
}


