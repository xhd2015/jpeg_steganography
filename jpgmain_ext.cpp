/**
 * This main file realizes extracting embeding/hiding string from the jpeg file by restore the DCT coefficients.
 */

#include <stdio.h>
#include <jpeglib.h>
#include <assert.h>
#include <string.h>


#define INFILE	"in.jpg"
#define OUTFILE "out.jpg"
#define B_H(byte) (((byte & 0x80)>>7)&0x1)
#define B_L(byte) (byte & 0x1)
int main()
{

//	  struct jpeg_compress_struct cinfo;
	  struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	 // cinfo.err = jpeg_std_error(&jerr);
	dinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */

	  //jpeg_create_compress(&cinfo);	
	jpeg_create_decompress(&dinfo);
	
	FILE *infile,*outfile;
	//outfile=fopen(OUTFILE,"wb");
	infile=fopen(OUTFILE,"rb");
	assert(infile!=NULL);
	//assert(outfile!=NULL);

//	jpeg_set_defaults(&cinfo);
	  jpeg_stdio_src(&dinfo, infile);
	  //jpeg_stdio_dest(&cinfo, outfile);

	jpeg_read_header(&dinfo,TRUE);
	
	//cinfo.dct_method=JDCT_ISLOW;

	jvirt_barray_ptr* coeff= jpeg_read_coefficients(&dinfo);
	//struct jvirt_barray_control ddcoeff=*dcoeff;
	//printf("%d\n",dcoeff->mem_buffer[0][0]);
	char str[]={'x','h','d','z','e','r','o',0};//"xhdzero";
	char buf[8]={0};
	printf("reading:\n");//,str);
	size_t b_lenstr=strlen(str);
	unsigned char rcurbyte=0;
	size_t rcuri=0,rcurii=0;
	int ever_accessed;
	for (int ci = 0; ci < 1; ci++)
	    {
		if(rcuri>=b_lenstr)break;
		JBLOCKARRAY buffer_one;
		JCOEFPTR blockptr_one;
		jpeg_component_info* compptr_one;
		compptr_one = dinfo.comp_info + ci;

		for (int by = 0; by < compptr_one->height_in_blocks; by++)
		{
		    buffer_one = (dinfo.mem->access_virt_barray)((j_common_ptr)&dinfo, coeff[ci], by, (JDIMENSION)1, TRUE);
		    for (int bx = 0; bx < compptr_one->width_in_blocks; bx++)
		    {
			blockptr_one = buffer_one[0][bx];
			for (int bi = 0; bi < 64; bi++)
			{
				ever_accessed=0;
				//printf("[%d]:%d  ",bi,blockptr_one[bi]);
				if(blockptr_one[bi]!=0)
				{
					ever_accessed=1;
					rcurbyte = (rcurbyte | B_L(blockptr_one[bi]));
					printf("%d",B_L(blockptr_one[bi]));
//					printf("curbyte = %x \n",rcurbyte);
				}
				if(ever_accessed)
				{
					rcurii++;
					if(rcurii==8)
					{
						rcurii=0;
						buf[rcuri] = rcurbyte;
						rcuri += 1;
						//printf("i curbyte = %c \n",rcurbyte);
						if(rcuri >= b_lenstr)goto end_ext;
						//printf("buf[%d] = %c \n",rcuri,buf[rcuri]);
						printf(" ");
						rcurbyte=0;
					}else{
						rcurbyte = (rcurbyte << 1);
					}
				}
			}
			//printf("\n");
		    }
		}
	    }

	end_ext:
		printf("\n");
	printf("%s\n",buf);	
	jpeg_finish_decompress(&dinfo);

	fclose(infile);
	jpeg_destroy_decompress(&dinfo);

	getchar();


}

/**
Allocate and initialize a JPEG compression object
Specify the destination for the compressed data (eg, a file)
Set parameters for compression, including image size & colorspace
jpeg_start_compress(...);
while (scan lines remain to be written) jpeg_write_scanlines(...);
jpeg_finish_compress(...);
Release the JPEG compression object

*/

/** decompression
Allocate and initialize a JPEG decompression object
Specify the source of the compressed data (eg, a file)
Call jpeg_read_header() to obtain image info
Set parameters for decompression
jpeg_start_decompress(...);
while (scan lines remain to be read) jpeg_read_scanlines(...);
jpeg_finish_decompress(...);
Release the JPEG decompression object

*/


/**DCT


*/

/**Error handler
Typical code for this step, if you are using the default error handler, is

        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        ...
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
*/


/***set options
        cinfo.image_width = Width;      / image width and height, in pixels/
        cinfo.image_height = Height;
        cinfo.input_components = 3;     / # of color components per pixel /
        cinfo.in_color_space = JCS_RGB; / colorspace of input image /
        jpeg_set_defaults(&cinfo);
        / Make optional parameter settings here /
*/
