#include <stdio.h>
#include <getopt.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <alloca.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef TTY_GROUP_NAME
# include <grp.h>
#endif

#ifndef initialize_main
#ifndef __OS2__
#define initialize_main(ac, av)
#else
#define initialize_main(ac, av) \
    do { _wildcard (ac, av); _response (ac, av); } while (0)
#endif
#endif

/* If true, attempt to canonicalize hostnames via a DNS lookup. */
static bool do_lookup;

/* If true, display only a list of usernames and count of
   the users logged on.
   Ignored for 'who am i'.  */
static bool short_list;

/* If true, display only name, line, and time fields.  */
static bool short_output;

/* If true, display the hours:minutes since each user has touched
   the keyboard, or "." if within the last minute, or "old" if
   not within the last day.  */
static bool include_idle;

/* If true, display a line at the top describing each field.  */
static bool include_heading;

/* If true, display a '+' for each user if mesg y, a '-' if mesg n,
   or a '?' if their tty cannot be statted. */
static bool include_mesg;

/* If true, display process termination & exit status.  */
static bool include_exit;

/* If true, display the last boot time.  */
static bool need_boottime;

/* If true, display dead processes.  */
static bool need_deadprocs;

/* If true, display processes waiting for user login.  */
static bool need_login;

/* If true, display processes started by init.  */
static bool need_initspawn;

/* If true, display the last clock change.  */
static bool need_clockchange;

/* If true, display the current runlevel.  */
static bool need_runlevel;

/* If true, display user processes.  */
static bool need_users;

/* If true, display info only for the controlling tty.  */
static bool my_line_only;

/* The strftime format to use for login times, and its expected
   output width.  */
static char const *time_format;
static int time_format_width;

/* for long options with no corresponding short option, use enum */
enum
{
  LOOKUP_OPTION = CHAR_MAX + 1
};

static struct option const longopts[] = {
  {"all", no_argument, NULL, 'a'},
  {"boot", no_argument, NULL, 'b'},
  {"count", no_argument, NULL, 'q'},
  {"dead", no_argument, NULL, 'd'},
  {"heading", no_argument, NULL, 'H'},
  {"login", no_argument, NULL, 'l'},
  {"lookup", no_argument, NULL, LOOKUP_OPTION},
  {"message", no_argument, NULL, 'T'},
  {"mesg", no_argument, NULL, 'T'},
  {"process", no_argument, NULL, 'p'},
  {"runlevel", no_argument, NULL, 'r'},
  {"short", no_argument, NULL, 's'},
  {"time", no_argument, NULL, 't'},
  {"users", no_argument, NULL, 'u'},
  {"writable", no_argument, NULL, 'T'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

static void who (const char *filename, int options) {
    size_t n_users;
    STRUCT_UTMP *utmp_buf;

    if (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
        die (EXIT_FAILURE, errno, "%s", quotef (filename));
    if (short_list)
        list_entries_who (n_users, utmp_buf);
    else
        scan_entries (n_users, utmp_buf);

    free (utmp_buf);
}

int main(int argc, char* argv[]) {

    initialize_main (&argc, &argv);
    printf("%s", argv[1]);

    return 0;
}
