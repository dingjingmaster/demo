#include "dump_window.h"

static FILE *dumpfp;

int open_dump(const char *fn)
{
    int result = 0;
    close_dump();
    if ((dumpfp = fopen(fn, "a")) != 0)
        result = 1;
    return result;
}

void close_dump(void)
{
    if (dumpfp != 0) {
        fclose(dumpfp);
        dumpfp = 0;
    }
}

void dump_window(WINDOW *w)
{
    wgetch(w);
    if (dumpfp != 0) {
        int y, x;
        int oldy, oldx;
        int maxy, maxx;
        int pass;
        char *cvec = 0;
        char *avec = 0;
        char *pvec = 0;
        int ccnt = 0;
        int acnt = 0;
        int pcnt = 0;
        int endy = -1;
        int endx = -1;

        fprintf(dumpfp, "Window %p\n", (void *) w);

        getyx(w, oldy, oldx);
        getmaxyx(w, maxy, maxx);
        fprintf(dumpfp, "size     (%dx%d)\n", maxy, maxx);
        getbegyx(w, y, x);
        fprintf(dumpfp, "begin    (%dx%d)\n", maxy, maxx);
        getyx(w, y, x);
        fprintf(dumpfp, "position (%d,%d)\n", y, x);

        if (maxy > 0 && maxx > 0) {
            for (pass = 0; pass < 2; ++pass) {
                for (y = 0; y < maxy; ++y) {

                    if (cvec)
                        memset(cvec, 0, (size_t) maxx + 1);
                    if (avec)
                        memset(avec, 0, (size_t) maxx + 1);
                    if (pvec)
                        memset(pvec, 0, (size_t) maxx + 1);

                    for (x = 0; x < maxx; ++x) {
                        chtype data = mvwinch(w, y, x);
                        chtype temp;
                        char cc = (char) ((data & 0xff) ? (data & 0xff) : ' ');
                        char aa;
                        char pp;

                        temp = ((data & A_ATTRIBUTES) & (~A_COLOR));
                        if (temp) {
                            if (temp & A_ALTCHARSET) {
                                aa = (temp & A_BOLD) ? 'A' : 'a';
                            } else if (temp & A_STANDOUT) {
                                aa = (temp & A_BOLD) ? 'S' : 's';
                            } else if (temp & A_REVERSE) {
                                aa = (temp & A_BOLD) ? 'R' : 'r';
                            } else if (temp & A_UNDERLINE) {
                                aa = (temp & A_BOLD) ? 'U' : 'u';
                            } else {
                                aa = (temp & A_BOLD) ? 'b' : '?';
                            }
                        } else {
                            aa = ' ';
                        }
                        if (data & A_COLOR) {
                            if (PAIR_NUMBER((int) data) < 8) {
                                pp = (char) ('0' + PAIR_NUMBER((int) data));
                            } else {
                                pp = '*';
                            }
                        } else {
                            pp = ' ';
                        }

                        if (pass) {
                            if (cvec)
                                cvec[x] = cc;
                            if (avec)
                                avec[x] = aa;
                            if (pvec)
                                pvec[x] = pp;
                        } else {
                            if (cc != ' ' || aa != ' ' || pp != ' ') {
                                if (endx < x)
                                    endx = x;
                                if (endy < y)
                                    endy = y;
                            }
                            ccnt += (cc != ' ');
                            acnt += (aa != ' ');
                            pcnt += (pp != ' ');
                        }
                    }
                    if (pass) {
                        fprintf(dumpfp, "%3d", y + 1);
                        if (cvec)
                            fprintf(dumpfp, "\tc|%.*s|\n", maxx, cvec);
                        if (avec)
                            fprintf(dumpfp, "\ta|%.*s|\n", maxx, avec);
                        if (pvec)
                            fprintf(dumpfp, "\tp|%.*s|\n", maxx, pvec);
                    }
                }
                if (pass) {
                    free(cvec);
                    free(avec);
                    free(pvec);
                } else {
                    fprintf(dumpfp, "%d cells with characters\n", ccnt);
                    fprintf(dumpfp, "%d cells with video-attributes\n", acnt);
                    fprintf(dumpfp, "%d cells with color-attributes\n", pcnt);
                    if (endy < 0 || endx < 0)
                        break;
                    /* reduce the dump a little, ignore really blank cells */
                    maxx = endx + 1;
                    maxy = endy + 1;
                    if (ccnt)
                        cvec = malloc((size_t) maxx + 1);
                    if (acnt)
                        avec = malloc((size_t) maxx + 1);
                    if (pcnt)
                        pvec = malloc((size_t) maxx + 1);
                }
            }
        }
        wmove(w, oldy, oldx);
    }
}


