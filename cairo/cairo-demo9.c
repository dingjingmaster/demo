/*************************************************************************
> FileName: cairo-demo9.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 18:38:18
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <cairo.h>

int main(){
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char* fbp = 0;
    cairo_t* cr;
    cairo_surface_t *surface;

    // open the frame buffer file for reading & writing
    fbfd = open ( "/dev/fb0", O_RDWR );
    if (!fbfd) {
        printf ("Error: can't open framebuffer device.\n");
        exit (1);
    }
    printf ("The framebuffer device was opened successfully\n");

    if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        printf ("Error reading fixed information\n");
        close (fbfd);
        exit (2);
    }

    if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        printf ("Error reading variable information\n");
        close (fbfd);
        exit (3);
    }

    // print info about the buffer
    printf ("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // calculates size
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // map the device to memory 
    fbp = (char*) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((int)fbp == -1) {
        printf ("Error: failed to map framebuffer device to memory\n");
        close (fbfd);
        exit (4);
    }

    printf ("The framebuffer device was successfully mapped to memory\n");

    surface = cairo_image_surface_create_for_data (fbp, CAIRO_FORMAT_ARGB32, 
    vinfo.xres, vinfo.yres, finfo.line_length); 
    cr = cairo_create (surface);
    cairo_move_to (cr, 100, 100);
    cairo_line_to (cr, 300, 300);
    cairo_stroke (cr);
	
    munmap (fbp, screensize);
    close (fbfd);
    return 0;
}

