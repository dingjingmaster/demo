#ifndef __TERM_EXAMPLE__
#define __TERM_EXAMPLE__

#define ESC "\033"

//Format text
#define RESET 		0 // Обычный
#define BRIGHT 		1 //Жирный
#define DIM		2  //Темный
#define UNDERSCORE	3 // Наклон
#define BLINK		4 //Подчеркнуто
#define REVERSE		5 // Моргать
#define HIDDEN		6  // Моргать

//Foreground Colours (text)

#define F_BLACK 	232
#define F_RED		31
#define F_GREEN		32
#define F_YELLOW	33
#define F_BLUE		34
#define F_MAGENTA 	35
#define F_CYAN		36
#define F_WHITE		37

//Background Colours
#define B_BLACK 	40
#define B_RED		41
#define B_GREEN		42
#define B_YELLOW	44
#define B_BLUE		44
#define B_MAGENTA 	45
#define B_CYAN		46
#define B_WHITE		47

#endif /*__TERM_EXAMPLE__*/
