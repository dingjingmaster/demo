#define NEED_COLOR_CODE 1
#define NEED_COLOR_NAME 1
#include "color_name.h"
#include "dump_window.h"

static int default_bg = COLOR_BLACK;
static int default_fg = COLOR_WHITE;

static void test_background(void)
{
    NCURSES_COLOR_T f, b;
    int row;
    int chr;

    if (pair_content(0, &f, &b) == ERR) {
        printw("pair 0 contains no data\n");
    } else {
        printw("pair 0 contains (%d,%d)\n", (int) f, (int) b);
    }
    dump_window(stdscr);

    printw("Initializing pair 1 to red/%s\n", color_name(default_bg));
    init_pair(1, COLOR_RED, (NCURSES_COLOR_T) default_bg);
    bkgdset((chtype) (' ' | COLOR_PAIR(1)));
    printw("RED/BLACK\n");
    dump_window(stdscr);

    printw("Initializing pair 2 to %s/blue\n", color_name(default_fg));
    init_pair(2, (NCURSES_COLOR_T) default_fg, COLOR_BLUE);
    bkgdset((chtype) (' ' | COLOR_PAIR(2)));
    printw("This line should be %s/blue\n", color_name(default_fg));
    dump_window(stdscr);

    printw("Initializing pair 3 to %s/cyan (ACS_HLINE)\n", color_name(default_fg));
    init_pair(3, (NCURSES_COLOR_T) default_fg, COLOR_CYAN);
    printw("...and drawing a box which should be followed by lines\n");
    bkgdset(ACS_HLINE | (chtype) COLOR_PAIR(3));

    row = 7;
    mvprintw(row++, 10, "l");
    for (chr = 0; chr < 32; ++chr)
        AddCh(' ');
    printw("x\n");
    chr = 32;
    while (chr < 128) {
        if ((chr % 32) == 0)
            mvprintw(row++, 10, "x");
        AddCh((chr == 127) ? ' ' : chr);
        if ((++chr % 32) == 0)
            printw("x\n");
    }
    mvprintw(row++, 10, "m");
    for (chr = 0; chr < 32; ++chr)
        AddCh(' ');
    printw("j\n");
    dump_window(stdscr);

    bkgdset((chtype) (' ' | COLOR_PAIR(0)));
    printw("Default Colors\n");
    dump_window(stdscr);

    printw("Resetting colors to pair 1\n");
    bkgdset((chtype) (' ' | COLOR_PAIR(1)));
    printw("This line should be red/%s\n", color_name(default_bg));
    dump_window(stdscr);

    printw("Setting screen to pair 0\n");
    bkgd((chtype) (' ' | COLOR_PAIR(0)));
    dump_window(stdscr);

    printw("Setting screen to pair 1\n");
    bkgd((chtype) (' ' | COLOR_PAIR(1)));
    dump_window(stdscr);

    printw("Setting screen to pair 2\n");
    bkgd((chtype) (' ' | COLOR_PAIR(2)));
    dump_window(stdscr);

    printw("Setting screen to pair 3\n");
    bkgd((chtype) (' ' | COLOR_PAIR(3)));
    dump_window(stdscr);

    printw("Setting screen to pair 0\n");
    bkgd((chtype) (' ' | COLOR_PAIR(0)));
    dump_window(stdscr);
}

    static void
usage(int ok)
{
    static const char *msg[] =
    {
        "Usage: background [options]"
            ,""
            ,USAGE_COMMON
            ,"Options:"
#if HAVE_ASSUME_DEFAULT_COLORS
            ," -a       invoke assume_default_colors, repeat to use in init_pair"
#endif
            ," -b XXX   specify background color"
#if HAVE_USE_DEFAULT_COLORS
            ," -d       invoke use_default_colors, repeat to use in init_pair"
#endif
            ," -f XXX   specify foreground color"
            ," -l FILE  log window-dumps to this file"
    };
    size_t n;

    for (n = 0; n < SIZEOF(msg); n++)
        fprintf(stderr, "%s\n", msg[n]);

    ExitProgram(ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
/* *INDENT-OFF* */
VERSION_COMMON()
    /* *INDENT-ON* */

    int
main(int argc, char *argv[])
{
#if HAVE_ASSUME_DEFAULT_COLORS
    int a_option = 0;
#endif
#if HAVE_USE_DEFAULT_COLORS
    int d_option = 0;
#endif
    int ch;

    setlocale(LC_ALL, "");

    while ((ch = getopt(argc, argv, OPTS_COMMON "ab:df:l:")) != -1) {
        switch (ch) {
#if HAVE_ASSUME_DEFAULT_COLORS
            case 'a':
                ++a_option;
                break;
#endif
            case 'b':
                default_bg = color_code(optarg);
                break;
#if HAVE_USE_DEFAULT_COLORS
            case 'd':
                ++d_option;
                break;
#endif
            case 'f':
                default_fg = color_code(optarg);
                break;
            case 'l':
                if (!open_dump(optarg))
                    usage(FALSE);
                break;
            case OPTS_VERSION:
                show_version(argv);
                ExitProgram(EXIT_SUCCESS);
            default:
                usage(ch == OPTS_USAGE);
                /* NOTREACHED */
        }
    }
#if HAVE_USE_DEFAULT_COLORS && HAVE_ASSUME_DEFAULT_COLORS
    if (a_option && d_option) {
        fprintf(stderr, "Use either -a or -d option, but not both\n");
        ExitProgram(EXIT_FAILURE);
    }
#endif

    initscr();
    cbreak();
    noecho();

    if (has_colors()) {
        start_color();

#if HAVE_USE_DEFAULT_COLORS
        if (d_option) {
            printw("Using default colors...\n");
            use_default_colors();
            if (d_option > 1) {
                default_fg = -1;
                default_bg = -1;
            }
        }
#endif
#if HAVE_ASSUME_DEFAULT_COLORS
        if (a_option) {
            printw("Using assumed colors %s/%s...\n",
                    color_name(default_fg),
                    color_name(default_bg));
            assume_default_colors(default_fg, default_bg);
            if (a_option > 1) {
                default_fg = -1;
                default_bg = -1;
            }
        }
#endif

        test_background();

    } else {
        printw("This demo requires a color terminal");
        getch();
    }
    endwin();
    close_dump();
    ExitProgram(EXIT_SUCCESS);
}
