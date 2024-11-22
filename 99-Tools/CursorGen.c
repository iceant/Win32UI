#include <CursorGen.h>

// 鼠标文件解析
//https://mp.weixin.qq.com/s?__biz=MzkwNzMzMjIyNg==&mid=2247486257&idx=1&sn=1656090e498c22f391c00d0857f4b856&chksm=c0db94dcf7ac1dcacd4618240e05595b984a05b55d5f9f44d277aae41f4353b06bfa5e4294c4&cur_album_id=2474385342163419137&scene=189#wechat_redirect
// 代码来源
//https://blog.csdn.net/jinzhuojun/article/details/8007586
// 注释添加自 @ bilibili 民用级脑的研发记录
// 结构注释来自 https://learn.microsoft.com/en-us/previous-versions/ms997538(v=msdn.10)

// 注意这个格式和下文中的 WrietByte 的对应关系

//typedef struct
//{
//    WORD           idReserved;   // Reserved (must be 0)
//    WORD           idType;       // Resource Type (1 for icons)									// 2 指的是 cur 鼠标静态光标文件
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

void Fwrite(FILE* f, char* data, int byte)				// 一次写入 byte 个字节,且只写一次
{
    if(1!=fwrite(data,byte,1,f))
    {
        perror("fwrite error");
        exit(1);
    }
}

void WriteByte(FILE* f, unsigned int val,int byte)		// 指定写入几个字节
{
    char data[4];
    assert(byte<=4);									// 如果它的条件返回错误，则终止程序执行——菜鸟教程可查
    memcpy((void*)data,(void*)&val,byte);
    Fwrite(f,data,byte);
}

void generate_ico_file(const char* filename,char * body,
                       int width, int height, int has_alpha)
{
    int x,y;
    int index=0;
    int Size=0;
    int offset = 6+ 1*16;								// 6 = 2 +2 +2，这6个byte 是ico,cur通用的文件头。 16= 1+1+1+1 +2 +2，是一个图片的索引占 16 个字节， n个图片的索引占6*16个字节，，n个图片的因为在不同设备上使用不同型号分辨率的图片，这一堆图片都在一个 icon 或cur里
    int bpp=32;											// 32位的位图= 8位 alpha + 8 位 R， 8 位 G， 8 位 B

    FILE* outfile=fopen(filename,"wb");
    if(outfile==NULL){
        perror("fopen error");
        exit(1);
    }
    // icon文件头，类型与图片个数
    WriteByte(outfile,0,2);								// idReserved 保留位，一种格式，可用区分其他文件。
    WriteByte(outfile,1,2);								// idType 文件类型，如果是1 ，就是icon文件，如果是2 则是cur鼠标光标文件，这类文件格式一样。
    WriteByte(outfile,1,2);								// idCount 拥有的图片个数，icon为在不同屏幕上正常显示，会存储多个格式的图片以备选择
    // 具体一个图片的索引信息
    WriteByte(outfile,width,1);							// bWidth
    WriteByte(outfile,height,1);						// bHeight
    WriteByte(outfile,0,1);								// bColorCount
    WriteByte(outfile,0,1);								// bReservered
    WriteByte(outfile,0,2);								// wPlanes					// 这里是.cur热点位置 X
    WriteByte(outfile,0,2);							// wBitCount					// 这里是.cur热点位置 Y

    Size = 40 + height * ((width + 31) / 32 * 32 / 8 + width * 3);	//Note 4 bytes alignment		// 这里计算文件索引头与图像数据的总数 ，40=4+4+4+2+2+4*6 个字节。 （width+31）/32是计算int 型AND 位图个数，width / 32,但是不能确定%32 的部分，所以+31 然后再除以 32 。1个int 有4个字节，共32bit,每个bit表示一个像素是否被光栅操作 AND 覆盖，行数 * 32计算一共需要多少bit ，/8是确定有多少字节。
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
    for (y=height - 1 ; y >= 0; --y) { 					// 调换打印高度就不会读取了 ，确定为倒置打印，windows倒着读取数据.从左往右，从下往上，所以为了图片倒着读取之后是正的，需要把原图第一行像素数据打印到倒数最后一行
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
        WriteByte(outfile, 1, 1);						// 1 在屏幕上显示图片，0则不显示图片，表示为整个图片没有。 三元光栅操作参考
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
            if(i>=10&&i<=20){									// 0x00000000是完全透明 0x5F000000 可以看出来不完全透明，可知透明度可调节
                image[i*WIDTH+j]=0x5F000000;					// Alpha 透明 0x00, red: 00, green: 00, blue: 00  各 bit 位对应 ARGB
            }else{
                image[i * WIDTH + j]  = 0xFFFF0000;  	// pure red icon, for test
            }
        }
    }

    generate_ico_file("testv11颜色5F透明.cur", (char *)image, WIDTH, HEIGHT, 1);

    return 0;
}


