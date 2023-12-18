/*************************************************************************
> FileName: x-grab-key2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 18 Dec 2023 05:14:50 PM CST
 ************************************************************************/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <xcb/xcb.h>
#include <xcb/xcbext.h>
#include <xcb/xcb_event.h>
#include <xcb/xinput.h>
#include <xcb/xkb.h>

#define NAME ""
#define VERSION ""
#define ARR_LEN(x) (sizeof(x)/sizeof((x)[0]))

const uint16_t default_rate = 70;
const uint16_t default_delay = 250;

typedef struct InputEventMask {
	xcb_input_event_mask_t info;
	xcb_input_xi_event_mask_t mask;
} InputEventMask;

static bool is_hierarchy_event(const xcb_generic_event_t *event, const xcb_query_extension_reply_t *xinput_info);
static bool set_repeat_rate_and_delay(xcb_connection_t *connection, uint16_t rate, uint16_t delay);
static bool str_to_uint16(const char *str, uint16_t *res);
static void usage(char *progname, int exit_code);
static void version(void);
static void err(char *fmt, ...);

static bool
is_hierarchy_event(const xcb_generic_event_t *event, const xcb_query_extension_reply_t *xinput_info)
{
	if (XCB_EVENT_RESPONSE_TYPE(event) != XCB_GE_GENERIC) {
		return false;
	}

	xcb_ge_generic_event_t *generic_event = (xcb_ge_generic_event_t *) event;
	if (generic_event->extension != xinput_info->major_opcode
	    || generic_event->event_type != XCB_INPUT_HIERARCHY) {
		return false;
	}

	xcb_input_hierarchy_event_t *hierarchy_event = (xcb_input_hierarchy_event_t *) generic_event;
	if (!(hierarchy_event->flags & (XCB_INPUT_HIERARCHY_MASK_MASTER_ADDED | XCB_INPUT_HIERARCHY_MASK_SLAVE_ADDED))) {
		return false;
	}

	return true;
}

static bool
set_repeat_rate_and_delay(xcb_connection_t *connection, uint16_t rate, uint16_t delay)
{
	uint16_t repeat_interval;
	const uint8_t per_key_repeat[ARR_LEN(((xcb_xkb_set_controls_request_t *)0)->perKeyRepeat)] = {0};
	xcb_generic_error_t *error;
	xcb_void_cookie_t cookie;

	if (rate > 1000 || rate < 1) {
		return false;
	}
	repeat_interval = 1000 / rate;

	/* This just bluntly reapplies the rate and delay settings to the (emulated)
	 * core keyboard. In the future one may set the configuration on the devices
	 * individually using the deviceid from the XCB_INPUT_HIERARCHY event.
	 *
	 * Also, are you f*** kidding xcb?! Why can't I just pass a struct instead
	 * of having to specify each request argument individually. Xlib handles
	 * this way better: not only does XkbSetControls() allow to pass a struct,
	 * there is also a XkbSetAutoRepeatRate() function which makes this process
	 * even simpler.
	 */
	cookie = xcb_xkb_set_controls_checked(connection, XCB_XKB_ID_USE_CORE_KBD,
	                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                      XCB_XKB_BOOL_CTRL_REPEAT_KEYS,
	                                      delay, repeat_interval,
	                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, per_key_repeat);
	error = xcb_request_check(connection, cookie);
	if (error) {
		fprintf(stderr, "Cannot set keyboard repeat rate and delay: %d\n", error->error_code);
		return false;
	}

	return true;
}

static bool
str_to_uint16(const char *str, uint16_t *res)
{
	char *end;
	long int conversion;

	conversion = strtol(str, &end, 10);
	if (((conversion == LONG_MIN || conversion == LONG_MAX) && errno == ERANGE)
	    || conversion < 0
	    || conversion > UINT16_MAX
	    || end == str
	    || *end != '\0') {
		return false;
	}

	*res = (uint16_t) conversion;
	return true;
}

static void
err(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

static void
usage(char *progname, int exit_code)
{
	printf("Usage: %s [-V] [-r rate] [-d delay]\n", (progname == NULL) ? NAME : progname);
	exit(exit_code);
}

static void
version(void)
{
	printf(NAME " " VERSION "\n");
	exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
	int opt;
	uint16_t rate = default_rate, delay = default_delay;
	xcb_connection_t *connection;
	xcb_screen_t *screen;
	xcb_window_t root;
	const xcb_query_extension_reply_t *xinput_query;
	const xcb_query_extension_reply_t *xkb_query;
	InputEventMask input_mask;
	xcb_xkb_use_extension_cookie_t use_extension_cookie;
	xcb_xkb_use_extension_reply_t *use_extension_reply;
	xcb_generic_error_t *error;
	xcb_generic_event_t *event;

	while ((opt = getopt(argc, argv, "hVr:d:")) != -1) {
		switch(opt) {
		case 'h':
			usage(argv[0], EXIT_SUCCESS);
			break;
		case 'V':
			version();
			break;
		case 'r':
			if (!str_to_uint16(optarg, &rate)) {
				usage(argv[0], EXIT_FAILURE);
			}
			if (rate > 1000 || rate < 1) {
				err("Key repeat rate has to be between 1 and 1000.\n");
			}
			break;
		case 'd':
			if (!str_to_uint16(optarg, &delay)) {
				usage(argv[0], EXIT_FAILURE);
			}
			if (delay < 1) {
				err("Key repeat delay has to be greater than 0.\n");
			}
			break;
		}
	}

	connection = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(connection)) {
		err("Cannot connect to server.\n");
	}

	xinput_query = xcb_get_extension_data(connection, &xcb_input_id);
	if (!xinput_query->present) {
		err("Server does not support XInput.\n");
	}
	xkb_query = xcb_get_extension_data(connection, &xcb_xkb_id);
	if (!xkb_query->present) {
		err("Server does not support XKB.\n");
	}

	screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
	if (screen == NULL) {
		err("Cannot acquire screen.\n");
	}

	root = screen->root;

	/* This took me a bit of figuring out:
	 * xcb_input_xi_select_events() takes an argument of type struct
	 * xcb_input_event_mask_t, but looking at the header, the type has no space
	 * for the actual mask:
	 *
	 *      typedef struct xcb_input_event_mask_t {
	 *          xcb_input_device_id_t deviceid;
	 *          uint16_t              mask_len;
	 *      } xcb_input_event_mask_t;
	 *
	 *  In comparison, the corresponging XIEventMask in Xlib has a mask field.
	 *  The XCB XML protocol description *does* indicate a mask field of type list
	 *  (array in C terms):
	 *
	 *     <struct name="EventMask">
	 *         <field type="DeviceId" name="deviceid" altenum="Device" />
	 *         <field type="CARD16" name="mask_len" />
	 *         <list type="CARD32" name="mask" mask="XIEventMask">
	 *             <fieldref>mask_len</fieldref>
	 *         </list>
	 *     </struct>
	 *
	 * The EventMask struct does indeed have a mask field, which is an array of
	 * unit32_t's with a length of mask_len, though it seems we are on our own
	 * with xcb when it comes to constructing such a data structure. Doing a
	 * code search, there aren't that many examples. What they do is construct a
	 * new struct with both the xcb_input_event_mask_t as a field and the actual
	 * masks (usual just one value instead of a whole array) after that:
	 *
	 *     struct {
	 *         xcb_input_event_mask_t head;
	 *         xcb_input_xi_event_mask_t mask;
	 *     } mask;
	 *
	 *     mask m;
	 *     m.head.deviceid = XCB_INPUT_DEVICE_ALL;
	 *     m.head.mask_len = 1 // sizeof(m.mask) / sizeof(unit32_t) in case of multiple masks
	 *     m.mask = XCB_INPUT_EVENT_MASK_MOTION;
	 *
	 *     xcb_input_xi_select_events(conn, scr->root, 1, &m.head);
	 *
	 * When we receive such a list from the server, xcb generates iterator
	 * methods like xcb_input_xi_event_masks_iterator() and
	 * xcb_input_xi_event_mask_next(), but when we have to construct such data,
	 * we are on our own apparently.
	 */

	input_mask.info.deviceid = XCB_INPUT_DEVICE_ALL;
	input_mask.info.mask_len = 1;
	input_mask.mask = XCB_INPUT_XI_EVENT_MASK_HIERARCHY;
	xcb_input_xi_select_events(connection, root, 1, &input_mask.info);
	xcb_flush(connection);

	/* This took a while to figure out: before using the XKB extension, a call
	 * to xcb_xkb_use_extension() is required, otherwise normal XKB requests
	 * will return an Access Error. */
	use_extension_cookie = xcb_xkb_use_extension(connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);
	use_extension_reply = xcb_xkb_use_extension_reply(connection, use_extension_cookie, &error);
	if (error) {
		err("Cannot use XKB: %d\n", error->error_code);
	}
	free(use_extension_reply);

	/* Set repeat rate and delay once on startup. */
	set_repeat_rate_and_delay(connection, rate, delay);

	while ((event = xcb_wait_for_event(connection)) != NULL) {
		if (is_hierarchy_event(event, xinput_query)) {
			xcb_input_hierarchy_event_t *e = (xcb_input_hierarchy_event_t *) event;

			set_repeat_rate_and_delay(connection, rate, delay);

			/* In case one wants to set configuration on individual keyboards
			 * at some point, one can use an iterator like this:
			 *
			 *     xcb_input_hierarchy_info_iterator_t info = xcb_input_hierarchy_infos_iterator(e);
			 *     for (; info.rem > 0; xcb_input_hierarchy_info_next(&info)) {
			 *         // info.data
			 *     }
			 */
		}

		free(event);
	}

	xcb_flush(connection);
	xcb_disconnect(connection);
	return EXIT_SUCCESS;
}
