/**
 * 最简单的测试图片生成工具
 * Simplest Pic Gen
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 本程序可以生成多种RGB/YUV格式的测试图像。包括：
 * 灰阶图        [YUV420P]
 * 彩条图        [RGB24]
 * 彩色条纹图    [RGB24]
 * RGB渐变彩条图 [RGB24]
 * YUV渐变彩条图 [YUV420P]
 * 颜色视频      [RGB24][YUV444P]
 *
 * This software can generate several picture that used for
 * test:
 * Gray Bar Picture         [YUV420P]
 * Color Bar Picture        [RGB24]
 * Color Stripe Picture     [RGB24]
 * RGB Gradient Bar Picture [RGB24]
 * YUV Gradient Bar Picture [YUV420P]
 * All Color Video          [RGB24][YUV444P]
 *
 */

#include <stdio.h>
#include <stdlib.h>


/**
 * Generate Picture contains Stripe in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param r			Red component of stripe
 * @param g			Green component of stripe
 * @param b			Blue component of stripe
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_stripe(int width, int height,
	unsigned char r,unsigned char g,unsigned char b){

	unsigned char *data=NULL;
	char filename[100]={0};
	FILE *fp=NULL;
	int i=0,j=0;

	//Check
	if(width<=0||height<=0){
		printf("Error: Width, Height cannot be 0 or negative number!\n");
		printf("Default Param is used.\n");
		width=640;
		height=480;
	}

	data=(unsigned char *)malloc(width*height*3);

	sprintf(filename,"rgbstripe_%dx%d_rgb24.rgb",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}

	for(j=0;j<height;j++){
		for(i=0;i<width;i++){
			if(i%2!=0){
				data[(j*width+i)*3+0]=r;
				data[(j*width+i)*3+1]=g;
				data[(j*width+i)*3+2]=b;
			}else{//White
				data[(j*width+i)*3+0]=255;
				data[(j*width+i)*3+1]=255;
				data[(j*width+i)*3+2]=255;
			}
		}
	}
	fwrite(data,width*height*3,1,fp);
	fclose(fp);
	free(data);
	printf("Finish generate %s!\n",filename);
	return 0;
}


/**
 * Generate Picture contains Gray Bar changing from Black to White in YUV420P Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param ymin		the minimum value of luminance.
 * @param ymax		the maximum value of luminance.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_yuv420p_graybar(int width, int height,int barnum,unsigned char ymin,unsigned char ymax){

	int barwidth;
	float lum_inc;
	unsigned char lum_temp;
	int uv_width,uv_height;
	FILE *fp=NULL;
	unsigned char *data_y=NULL;
	unsigned char *data_u=NULL;
	unsigned char *data_v=NULL;
	int t=0,i=0,j=0;
	char filename[100]={0};

	//Check
	if(width<=0||height<=0||barnum<=0){
		printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
		printf("Default Param is used.\n");
		width=640;
		height=480;
		barnum=10;
	}
	if(width%barnum!=0){
		printf("Warning: Width cannot be divided by Bar Number without remainder!\n");
	}
	barwidth=width/barnum;
	lum_inc=((float)(ymax-ymin))/((float)(barnum-1));
	uv_width=width/2;
	uv_height=height/2;

	data_y=(unsigned char *)malloc(width*height);
	data_u=(unsigned char *)malloc(uv_width*uv_height);
	data_v=(unsigned char *)malloc(uv_width*uv_height);

	sprintf(filename,"graybar_%dx%d_yuv420p.yuv",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}

	//Output Info
	printf("Y, U, V value from picture's left to right:\n");
	for(t=0;t<(width/barwidth);t++){
		lum_temp=ymin+(char)(t*lum_inc);
		printf("%3d, 128, 128\n",lum_temp);
	}
	//Gen Data
	for(j=0;j<height;j++){
		for(i=0;i<width;i++){
			t=i/barwidth;
			lum_temp=ymin+(char)(t*lum_inc);
			data_y[j*width+i]=lum_temp;
		}
	}
	for(j=0;j<uv_height;j++){
		for(i=0;i<uv_width;i++){
			data_u[j*uv_width+i]=128;
		}
	}
	for(j=0;j<uv_height;j++){
		for(i=0;i<uv_width;i++){
			data_v[j*uv_width+i]=128;
		}
	}
	fwrite(data_y,width*height,1,fp);
	fwrite(data_u,uv_width*uv_height,1,fp);
	fwrite(data_v,uv_width*uv_height,1,fp);
	fclose(fp);
	free(data_y);
	free(data_u);
	free(data_v);
	printf("Finish generate %s!\n",filename);
    return 0;
}

/**
 * Generate Picture contains standard Color Bar in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_colorbar(int width, int height){
	
	unsigned char *data=NULL;
	int barwidth;
	char filename[100]={0};
	FILE *fp=NULL;
	int i=0,j=0;
	int lum;
	float r_coeff=0.299,g_coeff=0.587,b_coeff=0.114;

	//Check
	if(width<=0||height<=0){
		printf("Error: Width, Height cannot be 0 or negative number!\n");
		printf("Default Param is used.\n");
		width=640;
		height=480;
	}
	if(width%8!=0)
		printf("Warning: Width cannot be divided by Bar Number without remainder!\n");

	data=(unsigned char *)malloc(width*height*3);
	barwidth=width/8;

	sprintf(filename,"colorbar_%dx%d_rgb24.rgb",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}

	printf("Luminance (Y) component value of colors from left to right:\n");
	lum=r_coeff*255.0+g_coeff*255.0+b_coeff*255.0;
	printf("[White]  \tR,G,B=255,255,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*255.0+g_coeff*255.0+b_coeff*0.0;
	printf("[Yellow] \tR,G,B=255,255,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*0.0+g_coeff*255.0+b_coeff*255.0;
	printf("[Cyan]   \tR,G,B=  0,255,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*0.0+g_coeff*255.0+b_coeff*0.0;
	printf("[Green]  \tR,G,B=  0,255,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*255.0+g_coeff*0.0+b_coeff*255.0;
	printf("[Magenta]\tR,G,B=255,  0,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*255.0+g_coeff*0.0+b_coeff*0.0;
	printf("[Red]    \tR,G,B=255,  0,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*0.0+g_coeff*0.0+b_coeff*255.0;
	printf("[Blue]   \tR,G,B=  0,  0,255\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);
	lum=r_coeff*0.0+g_coeff*0.0+b_coeff*0.0;
	printf("[Black]  \tR,G,B=  0,  0,  0\t Y=%.3f*R+%.3f*G+%.3f*B=%3d\n",
		r_coeff,g_coeff,b_coeff,lum);

	for(j=0;j<height;j++){
		for(i=0;i<width;i++){
			int barnum=i/barwidth;
			switch(barnum){
			case 0:{
				data[(j*width+i)*3+0]=255;
				data[(j*width+i)*3+1]=255;
				data[(j*width+i)*3+2]=255;
				break;
				   }
			case 1:{
				data[(j*width+i)*3+0]=255;
				data[(j*width+i)*3+1]=255;
				data[(j*width+i)*3+2]=0;
				break;
				   }
			case 2:{
				data[(j*width+i)*3+0]=0;
				data[(j*width+i)*3+1]=255;
				data[(j*width+i)*3+2]=255;
				break;
				   }
			case 3:{
				data[(j*width+i)*3+0]=0;
				data[(j*width+i)*3+1]=255;
				data[(j*width+i)*3+2]=0;
				break;
				   }
			case 4:{
				data[(j*width+i)*3+0]=255;
				data[(j*width+i)*3+1]=0;
				data[(j*width+i)*3+2]=255;
				break;
				   }
			case 5:{
				data[(j*width+i)*3+0]=255;
				data[(j*width+i)*3+1]=0;
				data[(j*width+i)*3+2]=0;
				break;
				   }
			case 6:{
				data[(j*width+i)*3+0]=0;
				data[(j*width+i)*3+1]=0;
				data[(j*width+i)*3+2]=255;
				
				break;
				   }
			case 7:{
				data[(j*width+i)*3+0]=0;
				data[(j*width+i)*3+1]=0;
				data[(j*width+i)*3+2]=0;
				break;
				   }
			}

		}
	}
	fwrite(data,width*height*3,1,fp);
	fclose(fp);
	free(data);
	printf("Finish generate %s!\n",filename);
    return 0;
}

/**
 * Generate Picture contains Color Bar Changing from source color
 * to destination color in RGB24 Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param src_r		Red component of source color.
 * @param src_g		Green component of source color.
 * @param src_b		Blue component of source color.
 * @param dst_r		Red component of destination color.
 * @param dst_g		Green component of destination color.
 * @param dst_b		Blue component of destination color.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_rgb24_rgbgradient_bar(int width, int height,int barnum,
	unsigned char src_r,unsigned char src_g,unsigned char src_b,
	unsigned char dst_r,unsigned char dst_g,unsigned char dst_b){

	unsigned char *data=NULL;
	int barwidth;
	float r_inc,g_inc,b_inc;
	unsigned char r_temp,g_temp,b_temp;
	char filename[100]={0};
	FILE *fp=NULL;
	int t=0,i=0,j=0;

	//Check
	if(width<=0||height<=0||barnum<=0){
		printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
		printf("Default Param is used.\n");
		width=640;
		height=480;
	}
	if(width%barnum!=0)
		printf("Warning: Width cannot be divided by Bar Number without remainder!\n");
	

	data=(unsigned char *)malloc(width*height*3);
	barwidth=width/barnum;
	r_inc=((float)(dst_r-src_r))/((float)(barnum-1));
	g_inc=((float)(dst_g-src_g))/((float)(barnum-1));
	b_inc=((float)(dst_b-src_b))/((float)(barnum-1));

	sprintf(filename,"rgbgradientbar_%dx%d_rgb24.rgb",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}

	//Output Info
	printf("R, G, B value from picture's left to right:\n");
	for(t=0;t<(width/barwidth);t++){
		r_temp=src_r+(char)(t*r_inc);
		g_temp=src_g+(char)(t*g_inc);
		b_temp=src_b+(char)(t*b_inc);
		printf("%3d, %3d, %3d\n",r_temp,g_temp,b_temp);
	}

	for(j=0;j<height;j++){
		for(i=0;i<width;i++){
			t=i/barwidth;
			r_temp=src_r+(char)(t*r_inc);
			g_temp=src_g+(char)(t*g_inc);
			b_temp=src_b+(char)(t*b_inc);
			data[(j*width+i)*3+0]=r_temp;
			data[(j*width+i)*3+1]=g_temp;
			data[(j*width+i)*3+2]=b_temp;
		}
	}
	fwrite(data,width*height*3,1,fp);
	fclose(fp);
	free(data);
	printf("Finish generate %s!\n",filename);
	return 0;
}

/**
 * Generate Picture contains Color Bar Changing from source color
 * to destination color in YUV420P Format
 *
 * @param width		the width of picture.
 * @param height	the height of picture.
 * @param barnum	the number of Bars in the picture.
 * @param src_y		Luma component of source color.
 * @param src_u		U component of source color.
 * @param src_v		V component of source color.
 * @param dst_y		Luma component of destination color.
 * @param dst_u		U component of destination color.
 * @param dst_v		V component of destination color.
 * @return 0 if finished, -1 if there are errors.
 */
int gen_yuv420p_yuvgradient_bar(int width, int height,int barnum,
	unsigned char src_y,unsigned char src_u,unsigned char src_v,
	unsigned char dst_y,unsigned char dst_u,unsigned char dst_v){

	int uv_width,uv_height;
	unsigned char *data_y=NULL;
	unsigned char *data_u=NULL;
	unsigned char *data_v=NULL;
	FILE *fp=NULL;
	int barwidth,uv_barwidth;
	float y_inc,u_inc,v_inc=0;
	unsigned char y_temp,u_temp,v_temp=0;
	char filename[100]={0};
	int t=0,i=0,j=0;
	//Check
	if(width<=0||height<=0||barnum<=0){
		printf("Error: Width, Height or Bar Number cannot be 0 or negative number!\n");
		printf("Default Param is used.\n");
		width=640;
		height=480;
	}
	if(width%barnum!=0)
		printf("Warning: Width cannot be divided by Bar Number without remainder!\n");

	uv_width=width/2;
	uv_height=height/2;
	data_y=(unsigned char *)malloc(width*height);
	data_u=(unsigned char *)malloc(uv_width*uv_height);
	data_v=(unsigned char *)malloc(uv_width*uv_height);
	barwidth=width/barnum;
	uv_barwidth=barwidth/(width/uv_width);
	y_inc=((float)(dst_y-src_y))/((float)(barnum-1));
	u_inc=((float)(dst_u-src_u))/((float)(barnum-1));
	v_inc=((float)(dst_v-src_v))/((float)(barnum-1));

	sprintf(filename,"yuvgradientbar_%dx%d_yuv420p.yuv",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}

	//Output Info
	printf("Y, U, V value from picture's left to right:\n");
	for(t=0;t<(width/barwidth);t++){
		y_temp=src_y+(char)(t*y_inc);
		u_temp=src_u+(char)(t*u_inc);
		v_temp=src_v+(char)(t*v_inc);
		printf("%3d, %3d, %3d\n",y_temp,u_temp,v_temp);
	}

	//Gen Data
	for(j=0;j<height;j++){
		for(i=0;i<width;i++){
			t=i/barwidth;
			y_temp=src_y+(char)(t*y_inc);
			data_y[j*width+i]=y_temp;
		}
	}
	for(j=0;j<uv_height;j++){
		for(i=0;i<uv_width;i++){
			t=i/uv_barwidth;
			u_temp=src_u+(char)(t*u_inc);
			data_u[j*uv_width+i]=u_temp;
		}
	}
	for(j=0;j<uv_height;j++){
		for(i=0;i<uv_width;i++){
			t=i/uv_barwidth;
			v_temp=src_v+(char)(t*v_inc);
			data_v[j*uv_width+i]=v_temp;
		}
	}
	fwrite(data_y,width*height,1,fp);
	fwrite(data_u,uv_width*uv_height,1,fp);
	fwrite(data_v,uv_width*uv_height,1,fp);
	fclose(fp);
	free(data_y);
	free(data_u);
	free(data_v);
	printf("Finish generate %s!\n",filename);
	return 0;
}

/**
 * Convert RGB24 format to BMP format
 *
 * @param rgb24path		path of input RGB24 file.
 * @param bmppath		path of output BMP file
 * @param width			the width of picture.
 * @param height		the height of picture.
 * @return 0 if finished, -1 if there are errors.
 */
int rgb24_to_bmp(const char *rgb24path,const char *bmppath,int width,int height)
{
	typedef struct 
	{  
		long imageSize;
		long blank;
		long startPosition;
	}BmpHead;

	typedef struct
	{
		long  Length;
		long  width;
		long  height;
		unsigned short  colorPlane;
		unsigned short  bitColor;
		long  zipFormat;
		long  realSize;
		long  xPels;
		long  yPels;
		long  colorUse;
		long  colorImportant;
	}InfoHead;
	
	int i=0,j=0;
	BmpHead m_BMPHeader={0};
	InfoHead  m_BMPInfoHeader={0};
	char bfType[2]={'B','M'};
	int header_size=sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
	unsigned char *rgb24_buffer=NULL;
	FILE *fp_rgb24=NULL,*fp_bmp=NULL;
	
	if((fp_rgb24=fopen(rgb24path,"rb"))==NULL){
		printf("Error: Cannot open input RGB24 file.\n");
		return -1;
	}
	if((fp_bmp=fopen(bmppath,"wb"))==NULL){
		printf("Error: Cannot open output BMP file.\n");
		return -1;
	}
	
	rgb24_buffer=(unsigned char *)malloc(width*height*3);
	fread(rgb24_buffer,1,width*height*3,fp_rgb24);

	m_BMPHeader.imageSize=3*width*height+header_size;
	m_BMPHeader.startPosition=header_size;

	m_BMPInfoHeader.Length=sizeof(InfoHead); 
	m_BMPInfoHeader.width=width;
	//BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
	m_BMPInfoHeader.height=-height;
	m_BMPInfoHeader.colorPlane=1;
	m_BMPInfoHeader.bitColor=24;
	m_BMPInfoHeader.realSize=3*width*height;

	fwrite(bfType,1,sizeof(bfType),fp_bmp);
	fwrite(&m_BMPHeader,1,sizeof(m_BMPHeader),fp_bmp);
	fwrite(&m_BMPInfoHeader,1,sizeof(m_BMPInfoHeader),fp_bmp);

	//BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
	//It saves pixel data in Little Endian
	//So we change 'R' and 'B'
	for(j =0;j<height;j++){
		for(i=0;i<width;i++){
			char temp=rgb24_buffer[(j*width+i)*3+2];
			rgb24_buffer[(j*width+i)*3+2]=rgb24_buffer[(j*width+i)*3+0];
			rgb24_buffer[(j*width+i)*3+0]=temp;
		}
	}
	fwrite(rgb24_buffer,3*width*height,1,fp_bmp);
	fclose(fp_rgb24);
	fclose(fp_bmp);
	free(rgb24_buffer);
	printf("Finish generate %s!\n",bmppath);
	return 0;
}




/**
 * Generate a video in 256x256 and has 256 frames that contains all the colors.
 * Each color is shown in 1 pixel. They are mapped as follows:
 * In RGB24
 * R component's value is increasing with the growth of width (X-axis); 
 * G component's value is increasing with the growth of height (Y-axis);
 * B component's value is increasing with the growth of frame number (Z-axis).
 * In YUV444P
 * U component's value is increasing with the growth of width (X-axis); 
 * V component's value is increasing with the growth of height (Y-axis);
 * Y component's value is increasing with the growth of frame number (Z-axis).
 * 
 * This function now support to draw YUV444P/RGB24 format pixel.
 * 
 * @return 0 if finished, -1 if there are errors.
 */
int gen_allcolor_video(){

	unsigned char *data=NULL;
	char filename[100]={0};
	FILE *fp=NULL;
	int width=256,height=256,frames=256;
	int i=0,j=0,k=0;

	//From left to right (width, X-axis),R increasing from 0 to255 
	//From Top to bottom (height, Y-axis),G increasing from 0 to255 
	//From 0 to 255 frames (time, Z-axis),B increasing from 0 to255 
	data=(unsigned char *)malloc(width*height*3);
	sprintf(filename,"allcolor_xr_yg_zb_%dx%d_rgb24.rgb",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}
	for(k=0;k<frames;k++){
		for(j=0;j<height;j++){
			for(i=0;i<width;i++){
				data[(j*width+i)*3+0]=i;
				data[(j*width+i)*3+1]=j;
				data[(j*width+i)*3+2]=k;
			}
		}
		fwrite(data,width*height*3,1,fp);
		printf("Finish generate frame %d!\n",k);
	}
	fclose(fp);
	free(data);
	printf("Finish generate %s!\n",filename);

	//From left to right (width, X-axis),U increasing from 0 to255 
	//From Top to bottom (height, Y-axis),V increasing from 0 to255 
	//From 0 to 255 frames (time, Z-axis),Y increasing from 0 to255 
	data=(unsigned char *)malloc(width*height);
	sprintf(filename,"allcolor_xu_yv_zy_%dx%d_yuv444p.yuv",width,height);
	if((fp=fopen(filename,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}
	for(k=0;k<frames;k++){
		for(j=0;j<height;j++){//Y
			for(i=0;i<width;i++){
				data[j*width+i]=k;
			}
		}
		fwrite(data,width*height,1,fp);
		for(j=0;j<height;j++){//U
			for(i=0;i<width;i++){
				data[j*width+i]=i;
			}
		}
		fwrite(data,width*height,1,fp);
		for(j=0;j<height;j++){//V
			for(i=0;i<width;i++){
				data[j*width+i]=j;
			}
		}
		fwrite(data,width*height,1,fp);
		printf("Finish generate frame %d!\n",k);
	}
	fclose(fp);
	free(data);
	printf("Finish generate %s!\n",filename);

	return 0;
}





int main(int argc, char* argv[])
{
	//All picture's resolution is 1280x720
	//Gray Bar, from 16 to 235
	gen_yuv420p_graybar(1280,720,10,16,235);
	//Color Bar
	gen_rgb24_colorbar(1280,720);
	//10 bars, RGB changed from 255,0,0 to 0,0,255
	gen_rgb24_rgbgradient_bar(1280,720,10,255,0,0,0,0,255);
	//10 bars, RGB changed from 0,0,0 to 128,128,128
	gen_yuv420p_yuvgradient_bar(1280,720,10,0,0,0,128,128,128);
	//RGB24 to BMP
	rgb24_to_bmp("colorbar_1280x720_rgb24.rgb","colorbar_1280x720_rgb24.bmp",1280,720);
	//Red stripe
	gen_rgb24_stripe(1280,720,255,0,0);
	//Gen color video
	gen_allcolor_video();
	return 0;
}