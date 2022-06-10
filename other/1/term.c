#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "term.h"

/*ANSI/VT100 Terminal using example */


#define home() 			printf(ESC "[H") //Move cursor to the indicated row, column (origin at 1,1)
#define clrscr()		printf(ESC "[2J") //lear the screen, move to (1,1)
#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x);
#define visible_cursor() printf(ESC "[?251");
/*
Set Display Attribute Mode	<ESC>[{attr1};...;{attrn}m
*/
#define resetcolor() printf(ESC "[0m")
#define set_display_atrib(color) 	printf(ESC "[%dm",color)


void frame_draw () {
	home();
	set_display_atrib(B_BLUE);
//  ┌ ─ ┬ ─ ┐
//  │   │   │
//  ├ ─ ┼ ─ ┤
//  │   │   │
//  └ ─ ┴ ─ ┘
// ps axc --format pid,%cpu,%mem,command --sort=-%cpu --headers | head -n 5
//
//  ┏ ━ ┳ ━ ┓
//  ┃   ┃   ┃
//  ┣ ━ ╋ ━ ┫
//  ┃   ┃   ┃
//  ┗ ━ ┻ ━ ┛
//
//
//


//      12345678901234567890123456789012345678123456789012
puts(


     " ┏━┳━┓\n"
		 " ┃ ┃ ┃\n"
		 " ┣━╋━┫\n"
		 " ┃ ┃ ┃\n"
		 " ┗━┻━┛\n"

     " ┏━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
		 " ┃                        ┃                         ┃\n"
	   " ┗━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
	    "┌────────────────────────┬─────────────────────────┐\n" //0
	 		"│ PID %CPU %MEM COMMAND  │  PID %CPU %MEM COMMAND  │\n" //1
			"│                        │                         │\n" //2
		 	"│                        │                         │\n" //3
			"│                        │                         │\n" //4
	 		"│                        │                         │\n" //5
			"│                        │                         │\n" //6
			"│                        │                         │\n" //7
			"│                        │                         │\n" //8
			"└────────────────────────┴─────────────────────────┘\n" //9
);
	resetcolor();
}


void cpuload(){
//set_display_atrib(B_BLUE);
//char cpu[] = "teeeest";
//system("ps axc --format pid,%cpu,%mem,command --sort=-%cpu --headers | head -n 5");
//printf("%S",cpu[3]);
}

void memload(){
set_display_atrib(BRIGHT);
set_display_atrib(B_MAGENTA);
gotoxy(20,20);
printf("%s",system("ps axc --format pid,%cpu,%mem,command --sort=-%mem --headers | head -n 5"));
resetcolor();
}

void print_alarm (int alarm_error) {
	gotoxy(1,2);
	if (alarm_error) {
		set_display_atrib(BRIGHT);
		set_display_atrib(B_RED);
		printf("       !HIGHLOAD!      ");
	} else {
		set_display_atrib(BRIGHT);
		set_display_atrib(B_BLUE);
		printf("           OK          ");
	}
resetcolor();
}



void print_time_date (struct tm* tm_info) {
	char buffer[12];
	strftime(buffer, 12, "%d.%m.%y", tm_info);
	gotoxy(22,7)
	puts(buffer);
	strftime(buffer, 10, "%H:%M:%S", tm_info);
	gotoxy(22,8)
	puts(buffer);
	resetcolor();
}


void controller_emulator () {
	int alarm_counter = 0;
	time_t timer;
	struct tm* tm_info;
	srand(time(NULL));
while (1) {
	int alarm_counter;
	if ((alarm_counter%2) == 0){
 	print_alarm(1);
}
  else {
 	print_alarm(0);

}
alarm_counter += 1;
		time(&timer);
		tm_info = localtime(&timer);
//********************************//
		//print_time_date(tm_info);
		//cpuload ();
		memload();
//********************************//
		gotoxy(1,18);
		fflush(stdout);
		sleep(1);
	}
}

int main (void) {
	//home();
	clrscr();
	//frame_draw ();
	controller_emulator	();
	return 0;
}
