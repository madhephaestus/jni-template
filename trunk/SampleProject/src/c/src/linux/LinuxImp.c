#include "v4j.h"

#include "v4l2uvc.h"
#include </usr/include/stdio.h>
#include </usr/include/string.h>
#include </usr/include/fcntl.h>
#include </usr/include/unistd.h>
#include </usr/include/errno.h>
#include </usr/include/sys/ioctl.h>
#include </usr/include/sys/mman.h>
#include </usr/include/sys/select.h>
#include </usr/include/linux/videodev.h>
#include </usr/include/linux/videodev2.h>
#include </usr/include/stdio.h>
#include </usr/include/jpeglib.h>
#include </usr/include/stdlib.h>



static struct vdIn vid;
static int isOpen = 0;
static int lastCapt = 0;

int deviceExist(){
	FILE * f;
	f=fopen(vid.videodevice, "r");
	if ( f ){
		fclose(f);
		return 1;
	}
	report_warning("\nnative:deviceExist() Device is missing: ");
	report_warning(vid.videodevice);
	return 0;
}

int initializeVideo(const char * device, int hight, int width){
	if(isOpen)
		closeVideo(device);
	if(init_videoIn(&vid, (char *)device,width,hight,V4L2_PIX_FMT_MJPEG) == 0 ){
		isOpen = 1;
		captureImage();
		return 1;
	}else if(init_videoIn(&vid, (char *)device,width,hight,V4L2_PIX_FMT_YUYV) == 0){
		report_warning("\nnative:initializeVideo() Hardware MJPEG unavailable, using YUYV...");
		isOpen = 1;
		captureImage();
		return 1;
	}
	return 0;
}
int closeVideo(const char * device){
	if(!isOpen)
		return 0;
	if(close_v4l2(&vid)==0){
		report_warning("\nClosing ok: ");
		report_warning((char *)device);
		report_warning("\n");
		isOpen=0;
		return 1;
	}
	return 0;
}

int captureImage(){
	if(deviceExist() == 0)
		closeVideo(vid.videodevice);
	if(!isOpen)
		return 0;
	//report_warning("\nStarting capture...");
	if( uvcGrab(&vid)==0){
		if(vid.formatIn == V4L2_PIX_FMT_YUYV)
			lastCapt = vid.framesizeIn;
		else
			lastCapt = vid.buf.bytesused;
		//fprintf(stderr, "%s: %d", "\ncapture OK",lastCapt);
		return lastCapt;
	}
	else{
		report_warning("\nnative:captureImage() Image capture failed\n");
		return 0;
	}

}



int getImage(char * imageArray){
	//report_warning("\nStarting copy...");
	//memcpy_picture(vid.tmpbuffer,(unsigned char*) imageArray,lastCapt);
	if(vid.formatIn == V4L2_PIX_FMT_YUYV){
		lastCapt=compress_yuyv_to_jpeg(&vid,(unsigned char*)imageArray,vid.framesizeIn,100);
	}else{
		lastCapt=memcpy_picture((unsigned char*) imageArray,vid.tmpbuffer,lastCapt);
	}
	//report_warning("\nCopy ok");
	return lastCapt;
}

#define OUTPUT_BUF_SIZE  4096

typedef struct {
    struct jpeg_destination_mgr pub; /* public fields */

    JOCTET * buffer;    /* start of buffer */

    unsigned char *outbuffer;
    int outbuffer_size;
    unsigned char *outbuffer_cursor;
    int *written;

} mjpg_destination_mgr;

typedef mjpg_destination_mgr * mjpg_dest_ptr;

/******************************************************************************
Description.:
Input Value.:
Return Value:
******************************************************************************/
void init_destination(j_compress_ptr cinfo)
{
    mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

    /* Allocate the output buffer --- it will be released when done with image */
    dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));

    *(dest->written) = 0;

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

/******************************************************************************
Description.: called whenever local jpeg buffer fills up
Input Value.:
Return Value:
******************************************************************************/
int empty_output_buffer(j_compress_ptr cinfo)
{
    mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;

    memcpy(dest->outbuffer_cursor, dest->buffer, OUTPUT_BUF_SIZE);
    dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
    *(dest->written) += OUTPUT_BUF_SIZE;

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

    return TRUE;
}

/******************************************************************************
Description.: called by jpeg_finish_compress after all data has been written.
              Usually needs to flush buffer.
Input Value.:
Return Value:
******************************************************************************/
void term_destination(j_compress_ptr cinfo)
{
    mjpg_dest_ptr dest = (mjpg_dest_ptr) cinfo->dest;
    size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

    /* Write any data remaining in the buffer */
    memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
    dest->outbuffer_cursor += datacount;
    *(dest->written) += datacount;
}

void dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written)
{
    mjpg_dest_ptr dest;

    if(cinfo->dest == NULL) {
        cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(mjpg_destination_mgr));
    }

    dest = (mjpg_dest_ptr) cinfo->dest;
    dest->pub.init_destination = init_destination;
    dest->pub.empty_output_buffer = empty_output_buffer;
    dest->pub.term_destination = term_destination;
    dest->outbuffer = buffer;
    dest->outbuffer_size = size;
    dest->outbuffer_cursor = buffer;
    dest->written = written;
}

/******************************************************************************
Description.: yuv2jpeg function is based on compress_yuyv_to_jpeg written by
              Gabriel A. Devenyi.
              It uses the destination manager implemented above to compress
              YUYV data to JPEG. Most other implementations use the
              "jpeg_stdio_dest" from libjpeg, which can not store compressed
              pictures to memory instead of a file.
Input Value.: video structure from v4l2uvc.c/h, destination buffer and buffersize
              the buffer must be large enough, no error/size checking is done!
Return Value: the buffer will contain the compressed data
******************************************************************************/
int compress_yuyv_to_jpeg(struct vdIn *vd, unsigned char *buffer, int size, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    unsigned char *line_buffer, *yuyv;
    int z;
    static int written;

    line_buffer = calloc(vd->width * 3, 1);
    yuyv = vd->framebuffer;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    /* jpeg_stdio_dest (&cinfo, file); */
    dest_buffer(&cinfo, buffer, size, &written);

    cinfo.image_width = vd->width;
    cinfo.image_height = vd->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    z = 0;
    while(cinfo.next_scanline < vd->height) {
        int x;
        unsigned char *ptr = line_buffer;

        for(x = 0; x < vd->width; x++) {
            int r, g, b;
            int y, u, v;

            if(!z)
                y = yuyv[0] << 8;
            else
                y = yuyv[2] << 8;
            u = yuyv[1] - 128;
            v = yuyv[3] - 128;

            r = (y + (359 * v)) >> 8;
            g = (y - (88 * u) - (183 * v)) >> 8;
            b = (y + (454 * u)) >> 8;

            *(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
            *(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
            *(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

            if(z++) {
                z = 0;
                yuyv += 4;
            }
        }

        row_pointer[0] = line_buffer;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    free(line_buffer);

    return (written);
}

