#include <stdlib.h>
#include <assert.h>
#include "libpng/png.h"
#include "graphics/FsImage2D.h"
#include "sys/io/FsVFS.h"

#define FS_PNG_SIG_BITS 8

NS_FS_BEGIN 


static bool fs_png_check(const unsigned char* buf,unsigned int len)
{
	return png_sig_cmp((png_byte*)buf,(png_size_t)0,len)==0;
}


static void fs_png_read_data(png_structp png_ptr,png_bytep data,png_size_t length)
{
	png_size_t check;
	FsFile* file=(FsFile*)png_get_io_ptr(png_ptr);
	check=file->read(data,length);
	if(check!=length)
	{
		png_error(png_ptr,"Read Error");
	}
}
static void fs_png_flush(png_structp png_ptr)
{
}


static void fs_png_write_data(png_structp png_ptr,png_bytep data,png_size_t length)
{
	png_size_t check;

	FsFile* file=(FsFile*)png_get_io_ptr(png_ptr);
	check=file->write(data,length);

	if(check!=length)
	{
		FS_TRACE_WARN("Write Error,Except %d Bytes,But Only %d Bytes Write",length,check);
		png_error(png_ptr,"write Error");
	}
}


static void fs_png_cexcept_error(png_structp png_ptr,png_const_charp msg)
{
	if(png_ptr)
	{
		FS_TRACE_WARN("libpng error: %s\n",msg);
	}

}

Image2D* FsUtil_PngReader(FsFile* file)
{
	png_structp png_ptr=NULL;
	png_infop info_ptr=NULL;

	png_uint_32 width=0,height=0;
	int bit_depth=0, color_type=0;
	double gamma;
	png_uint_32 channels=0;
	png_uint_32 row_bytes=0;

	png_byte*  image_data=NULL;
	png_byte** row_pointers=NULL;
	png_uint_32 i,col,row;


	unsigned char sig_bytes[FS_PNG_SIG_BITS];
	file->read(sig_bytes,FS_PNG_SIG_BITS);
	if(!fs_png_check(sig_bytes,FS_PNG_SIG_BITS))
	{
		FS_TRACE_WARN("File Is Not Png Format");
		return NULL;
	}

	png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,
			(png_error_ptr)fs_png_cexcept_error,(png_error_ptr)NULL);

	if(!png_ptr)
	{
		FS_TRACE_WARN("Can't Alloc Memory For Png_ptr");
		return NULL;
	}
	info_ptr=png_create_info_struct(png_ptr);
	if(!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,NULL,NULL);
		FS_TRACE_WARN("Can't Alloc Memory For Png_info");
		return NULL;
	}

	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
		if(image_data)
		{
			free(image_data);
			image_data=NULL;

		}
		if(row_pointers)
		{
			free(row_pointers);
			image_data=NULL;
		}
		FS_TRACE_WARN("Load Png Error");
		return NULL;
	}

	/* set custom io read func */
	png_set_read_fn(png_ptr,(png_voidp)file,fs_png_read_data);

	/* already read the 8 signature byte */
	png_set_sig_bytes(png_ptr,FS_PNG_SIG_BITS);

	/* read  the file information */
	png_read_info(png_ptr,info_ptr);

	/* get size and bit-depth of the PNG-image */
	png_get_IHDR(png_ptr,info_ptr,
			&width,&height,&bit_depth,&color_type,
			NULL,NULL,NULL);

	/* downgrade 16-bit images to 8 bit */
	if(bit_depth==16)
	{
		png_set_strip_16(png_ptr);
	}

	/* transform paletted image into full-color rgb*/
	if(color_type==PNG_COLOR_TYPE_PALETTE)
	{
		png_set_expand(png_ptr);
	}

	/* expand imaged to bit-depth 8 (only applicable for grayscale images*/
	if(color_type==PNG_COLOR_TYPE_GRAY&&bit_depth<8)
	{
		png_set_expand(png_ptr);
	}
	/* transform transparency maps into full alpha-channel */
	if(png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}

	/* transform grayscale images into full-color*/
	if(color_type==PNG_COLOR_TYPE_GRAY||
			color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
	}

	/* only if file has a file gamma, we do a correction */
	if(png_get_gAMA(png_ptr,info_ptr,&gamma))
	{
		png_set_gamma(png_ptr,(double)2.2,gamma);
	}

	/* all transformations have been registered; now update info_ptr data,
	 * get row_bytes and channels,and allocate image memory */
	png_read_update_info(png_ptr,info_ptr);


	/* get the new color-type and bit-depth(after expansion/stripping)*/
	png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,
			NULL,NULL,NULL);

	//channels=png_get_channels(png_ptr,info_ptr);
	
	if (color_type==PNG_COLOR_TYPE_RGB)
	{
		channels=3;
	}
	else if (color_type==PNG_COLOR_TYPE_RGB_ALPHA)
	{
		channels=4;
	}
	else 
	{
		channels=0;
		FS_TRACE_WARN("Channels Unkowns");
		png_error(png_ptr,"Channels Unkowns");
	}
	
	/* row_bytes=(width * channels * bit_depth/8) */
	row_bytes=png_get_rowbytes(png_ptr,info_ptr);

	assert(bit_depth==8);
	assert(row_bytes==width*channels*bit_depth/8);

	image_data=(png_bytep) malloc(row_bytes*height*sizeof(png_byte));
	if(image_data==NULL)
	{
		FS_TRACE_WARN("Can't Alloc Memory For Image Data");
		png_error(png_ptr,"Can't Alloc Memory For image_data");

	}

	row_pointers=(png_bytepp) malloc(height* sizeof(png_bytep));
	if(row_pointers==NULL)
	{
		FS_TRACE_WARN("Can't Alloc Memory For Row Pointers");
		png_error(png_ptr,"Can't Alloc Memory For row_pointers");
	}

	for(i=0;i<height;i++)
	{
		row_pointers[i]=image_data+i*row_bytes;
	}

	/* now read the whole image */
	png_read_image(png_ptr,row_pointers);

	/* read rest of file and get additional chunks in info_ptr*/
	png_read_end(png_ptr,NULL);

	/* clean up */
	free(row_pointers);
	row_pointers=NULL;
	png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)NULL);



	Image2D* img=NULL;
	if(channels==3)
	{
		img=Image2D::create(width,height,Image2D::PIXEL_RGB888);
	}
	else 
	{
		assert(channels==4);
		img=Image2D::create(width,height,Image2D::PIXEL_RGBA8888);
	}

	//FS_TRACE_WARN("width=%d,height=%d,channels=%d",width,height,channels);

	png_bytep src=image_data;

	for(row=0;row<height;row++)
	{
		for(col=0;col<width;col++)
		{
			unsigned char v[4]={0,0,0,255};
			for(i=0;i<channels;i++)
			{
				v[i]=*src++;
			}
			img->setColor(col,row,Color(v[0],v[1],v[2],v[3]));
		}
	}
	if(image_data)
	{
		free(image_data);
	}
	return img;
}

int FsUtil_PngWriter(FsFile* file,Image2D* img)
{
	png_uint_32 width=0,height=0,col=0,row=0,row_bytes=0;
	png_uint_32 bit_depth=0,i=0,channels=0;
	png_int_32	color_type=0;

	png_structp png_ptr=NULL;
	png_infop info_ptr=NULL;

	png_byte* png_pixels=NULL;
	png_byte** row_pointers=NULL;

	png_byte* pix_ptr=NULL;


	/* create png_struct */
	png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,
			(png_error_ptr)fs_png_cexcept_error,(png_error_ptr)NULL);
	if(!png_ptr)
	{
		FS_TRACE_WARN("Can't Alloc Memory For png_ptr");
		return -1;
	}

	/* create png_info_struct */
	info_ptr=png_create_info_struct(png_ptr);
	if(!info_ptr)
	{
		FS_TRACE_WARN("Can't Alloc Memory For info_ptr");
		png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
		return -1;
	}

	/* set long jmp */
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		if(png_pixels)
		{
			free(png_pixels);
		}
		if(row_pointers)
		{
			free(row_pointers);
		}
		png_destroy_write_struct(&png_ptr,&info_ptr);
		return -1;
	}


	/* get color_type,channels,bit_depth*/
	if(img->getPixelFormat()==Image2D::PIXEL_RGB888)
	{
		color_type=PNG_COLOR_TYPE_RGB;
		channels=3;
		bit_depth=8;
	}
	else  if(img->getPixelFormat()==Image2D::PIXEL_RGBA8888)
	{
		color_type=PNG_COLOR_TYPE_RGB_ALPHA;
		channels=4;
		bit_depth=8;
	}
	else 
	{
		png_error(png_ptr,"Can't Handle Image2D PixelFormat");
	}

	width=img->getWidth();
	height=img->getHeight();
	row_bytes=width*channels*(bit_depth/8);


	png_pixels=(png_byte*) malloc(row_bytes*height*sizeof(png_byte));
	if(png_pixels==NULL)
	{
		png_error(png_ptr,"Can't Alloc Memory For png_pixels");
	}

	pix_ptr=png_pixels;
	for(row=0;row<height;row++)
	{
		for(col=0;col<width;col++)
		{
			Color c=img->getColor(col,row);
			png_byte  v[4]={c.r,c.g,c.b,c.a};
			for(i=0;i<channels;i++)
			{
				*pix_ptr++=v[i];
			}
		}
	}

	/* set io func */
	png_set_write_fn(png_ptr,(png_voidp)file,fs_png_write_data,fs_png_flush);

	/* set png infomation */
	png_set_IHDR(png_ptr,info_ptr,width,height,bit_depth,color_type,
			PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE,
			PNG_FILTER_TYPE_BASE);

	/* write the file header information */
	png_write_info(png_ptr,info_ptr);


	/* allocate memory for row_pointers */
	row_pointers=(png_bytepp) malloc(height* sizeof(png_bytep));
	if(row_pointers==NULL)
	{
		png_error(png_ptr,"Can't Alloc Memory For row_pointers");
	}

	for(i=0;i<height;i++)
	{
		row_pointers[i]=png_pixels+i*row_bytes;
	}

	/* write out the entire image data in one call */
	png_write_image(png_ptr,row_pointers);

	/* write the additional chuncks to the png file */
	png_write_end(png_ptr,info_ptr);

	png_destroy_write_struct(&png_ptr,&info_ptr);
	if(row_pointers)
	{
		free(row_pointers);
	}
	if(png_pixels)
	{
		free(png_pixels);
	}

	return 0;
}

Image2D* FsUtil_PngReader(const char* filename)
{
	FsFile* file=VFS::createFile(filename);
	if(file==NULL)
	{
		FS_TRACE_WARN("Can't Open File(%s) For Image2D",filename);
		return NULL;
	}
	Image2D* ret=FsUtil_PngReader(file);
	file->decRef();
	return ret;
}

bool FsUtil_CheckPng(FsFile* file)
{
	int length=file->getLength();

	if(length< 8 )
	{
		return false;
	}


	uint8_t data[8];
	file->read(data,8);
	file->seek(0,FsFile::FS_SEEK_SET);

	return fs_png_check(data,8);
}

NS_FS_END














