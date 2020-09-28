#include "gatxt.h"

static gaint color_on = 0; /* off by default */
static gaint scheme   = 0;   /* color scheme */

/* Normal colors */
static char *black   = "[30m";
static char *red     = "[31m";
static char *green   = "[32m";
static char *yellow  = "[33m";
static char *blue    = "[34m";
static char *magenta = "[35m";
static char *cyan    = "[36m";
static char *white   = "[37m";
static char *reset   = "[39m";

static char *normal  = "[0m";
static char *bold    = "[1m";

/* Normal colors */
/* static char *Black   = "[90m"; */
/* static char *Red     = "[91m"; */
/* static char *Green   = "[92m"; */
/* static char *Yellow  = "[93m"; */
/* static char *Blue    = "[94m"; */
/* static char *Magenta = "[95m"; */
/* static char *Cyan    = "[96m"; */
/* static char *White   = "[97m"; */

static char buffer[256];
#define COLORIZE(c) snprintf(buffer,255,"%s%s%s",c,str,reset)

void gatxt(char *color) {
	if ( !color_on ) return;
	if ( color==NULL ) {
		printf("%s",reset);
		return;
	}

	/* Normal */
	if ( color[0]=='b' && 
		color[2]=='a' ) printf("%s",black);
	else if ( color[0]=='r' ) printf("%s",red);
	else if ( color[0]=='g' ) printf("%s",green);
	else if ( color[0]=='y' ) printf("%s",yellow);
	else if ( color[0]=='b' ) printf("%s",blue);
	else if ( color[0]=='m' ) printf("%s",magenta);
	else if ( color[0]=='c' ) printf("%s",cyan);
	else if ( color[0]=='w' ) printf("%s",white);

	else if ( color[0]=='o' ) printf("%s",normal);
	else if ( color[0]=='*' ) printf("%s",bold);

	/* Bright colors */
	else if ( color[0]=='B' && 
		color[2]=='a' ) printf("%s",black);
	else if ( color[0]=='R' ) printf("%s",red);
	else if ( color[0]=='G' ) printf("%s",green);
	else if ( color[0]=='Y' ) printf("%s",yellow);
	else if ( color[0]=='B' ) printf("%s",blue);
	else if ( color[0]=='M' ) printf("%s",magenta);
	else if ( color[0]=='C' ) printf("%s",cyan);
	else if ( color[0]=='W' ) printf("%s",white);

}


char *gatxts(char *str, char *color) { /* colorize the string */


	if ( !color_on ) return str;

	/* Normal */
	if ( color[0]=='b' && 
		color[2]=='a' ) COLORIZE(black);
	else if ( color[0]=='r' ) COLORIZE(red);
	else if ( color[0]=='g' ) COLORIZE(green);
	else if ( color[0]=='y' ) COLORIZE(yellow);
	else if ( color[0]=='b' ) COLORIZE(blue);
	else if ( color[0]=='m' ) COLORIZE(magenta);
	else if ( color[0]=='c' ) COLORIZE(cyan);
	else if ( color[0]=='w' ) COLORIZE(white);

	/* Bright colors */
	else if ( color[0]=='B' && 
		color[2]=='a' ) COLORIZE(black);
	else if ( color[0]=='R' ) COLORIZE(red);
	else if ( color[0]=='G' ) COLORIZE(green);
	else if ( color[0]=='Y' ) COLORIZE(yellow);
	else if ( color[0]=='B' ) COLORIZE(blue);
	else if ( color[0]=='M' ) COLORIZE(magenta);
	else if ( color[0]=='C' ) COLORIZE(cyan);
	else if ( color[0]=='W' ) COLORIZE(white);

	buffer[255] = '\0';
	return (char *) buffer;

}


char *gatxtl(char *str, gaint level) { /* colorize according to level */

	if ( scheme==0 ) {
		if (level==-1) return gatxts(str,"Green"); /* prompt */
		if (level==0 ) return gatxts(str,"Red");
		if (level==1 ) return gatxts(str,"magenta");
		if (level==2 ) return gatxts(str,"yellow");
	}
	else if ( scheme==1 ) {
		if (level==-1) return gatxts(str,"Green"); /* prompt */
		if (level==0) return gatxts(str,"Red");
		if (level==1) return gatxts(str,"magenta");
		if (level==2) return gatxts(str,"blue");
	}
	else if ( scheme==2 ) {
		if (level==-1) return gatxts(str,"Blue"); /* prompt */
		if (level==0) return gatxts(str,"black");
		if (level==1) return gatxts(str,"magenta");
		if (level==2) return gatxts(str,"white");
	} 
	return (str);
}
