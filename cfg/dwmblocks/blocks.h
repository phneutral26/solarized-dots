//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/				/*Update Interval*/	/*Update Signal*/
	{"  󰓇   ", "~/scripts/dwm_bar/song.sh || echo 'Not running'",		5,	0},
	//{"󰤨   ",   "~/scripts/dwm_bar/network.sh",				10,	0},
	{"󰋑   ",   "~/scripts/dwm_bar/battery.sh",				10,	0},
	//{"󰕾   ",   "~/scripts/dwm_bar/volume.sh",				1,	0},
	{"󰃭   ",   "date '+%b %d %a, %Y'",					30,	0},
	{"󰥔   ",   "date '+%R' ",						5,	0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "    |    ";
static unsigned int delimLen = 9;
