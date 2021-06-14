#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */

static const char *fonts[]          = { "Product Sans:size=15", "Noto Sans CJK JP:size=13", "Material Design Icons:size=15" };

static const unsigned int gappih    = 19;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 19;       /* vert inner gap between windows */
static const unsigned int gappoh    = 19;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 19;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;       /* 1 means no outer gap when there is only one window */

static const char fg[]         = "#202631";
static const char bg[]         = "#839496";
static const char acc[]        = "#93a1a1";

static const char *colors[][3]      = {
	/*                   fg  bg   border */
	[SchemeNorm]     = { fg, bg,  bg  },
	[SchemeSel]      = { bg, acc, acc },
	[SchemeStatus]   = { fg, bg,  "#000000"  }, // Statusbar right
	[SchemeTagsSel]  = { fg, acc, "#000000"  }, // Tagbar left selected
	[SchemeTagsNorm] = { fg, bg,  "#000000"  }, // Tagbar left unselected
	[SchemeInfoSel]  = { fg, bg,  "#000000"  }, // infobar middle  selected
	[SchemeInfoNorm] = { fg, bg,  "#000000"  }, // infobar middle  unselected
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance     title           tags mask   isfloating  isterminal  noswallow  monitor */
	{ "Nightly",  NULL,       NULL,           1 << 2,       0,        0,          0,         -1 },
	{ "Alacritty",NULL,       NULL,           0,            0,        1,          1,         -1 },
	{ "discord",  NULL,       NULL,           1 << 3,       0,        0,          0,         -1 },
	{ "mupdf",    NULL,       NULL, 	  0,            0,        0,          1,         -1 },
	{ NULL,       NULL,       "Event Tester", 0,            0,        0,          1,         -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *browser[]   = { "firefox", NULL };
static const char *discord[]   = { "discord", NULL };
static const char *dmenucmd[]  = { "dmenu_run", NULL };
static const char *emacs_d[]   = { "emacsclient", "-c", NULL };
static const char *files[]     = { "thunar", NULL };
static const char *flameshot[] = { "flameshot", "gui", NULL };
static const char *lock[]      = { "slock", NULL };
static const char *pavu[]      = { "pavucontrol", NULL };
static const char *pdf[]       = { "zathura", NULL };
static const char *spotify[]   = { "spotify", NULL };
static const char *telegram[]  = { "telegram-desktop", NULL};
static const char *termcmd[]   = { "alacritty", NULL };

static const char *vol_up[]    = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+2%", NULL };
static const char *vol_down[]  = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-2%", NULL };
static const char *vol_mute[]  = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };

static const char *play_tggl[] = { "playerctl", "play-pause", NULL };
static const char *play_next[] = { "playerctl", "next", NULL };
static const char *play_prev[] = { "playerctl", "previous", NULL };

static Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY|ShiftMask,             XK_q,                     killclient,     {0} },         // kill window
	{ MODKEY|ShiftMask,             XK_BackSpace,             spawn,          {.v = lock} }, // lock

	// open programs 
        { MODKEY,                       XK_d,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_e,                     spawn,          {.v = files } },
	{ MODKEY|ControlMask,           XK_d,                     spawn,          {.v = discord } },
	{ MODKEY|ControlMask,           XK_e,                     spawn,          {.v = emacs_d } },
	{ MODKEY|ControlMask,           XK_f,                     spawn,          {.v = browser } },
	{ MODKEY|ControlMask,           XK_p,                     spawn,          {.v = pavu } },
	{ MODKEY|ControlMask,           XK_s,                     spawn,          {.v = spotify } },
	{ MODKEY|ControlMask,           XK_t,                     spawn,          {.v = telegram } },
	{ MODKEY|ControlMask,           XK_z,                     spawn,          {.v = pdf } },
	{ MODKEY|ShiftMask,             XK_s,                     spawn,          {.v = flameshot } },

	// laptop audio control
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = vol_up } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = vol_down } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = vol_mute } },
	{ 0,                            XF86XK_AudioPlay,         spawn,          {.v = play_tggl } },
	{ 0,                            XF86XK_AudioNext,         spawn,          {.v = play_next } },
	{ 0,                            XF86XK_AudioPrev,         spawn,          {.v = play_prev } },

	// window layout and control
	{ MODKEY|ShiftMask,             XK_space,                 togglefloating, {0} },                // toggle float
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} }, // tiling
	{ MODKEY,                       XK_f,                     setlayout,      {.v = &layouts[1]} }, // float
	{ MODKEY,                       XK_m,                     setlayout,      {.v = &layouts[2]} }, // monocle

	{ MODKEY,                       XK_h,                     setmfact,       {.f = -0.05} }, // decrease master size
	{ MODKEY,                       XK_l,                     setmfact,       {.f = +0.05} }, // increase master size

	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } }, // focus down
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } }, // focus up

	{ MODKEY,                       XK_b,                     togglebar,      {0} }, // show bar

	{ MODKEY,                       XK_o,                     incnmaster,     {.i = +1 } }, // more master nodes
	{ MODKEY,                       XK_p,                     incnmaster,     {.i = -1 } }, // less measter nodes

	{ MODKEY,                       XK_z,                     zoom,           {0} }, // zoom on selected window

	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1 } }, // focus monitor left
	{ MODKEY,                       XK_period,                focusmon,       {.i = +1 } }, // focus monitor right
	{ MODKEY|ShiftMask,             XK_comma,                 tagmon,         {.i = -1 } }, // tag monitor left
	{ MODKEY|ShiftMask,             XK_period,                tagmon,         {.i = +1 } }, // tag monitor right

	{ MODKEY,                       XK_Tab,                   view,           {0} },         // view last tag
	{ MODKEY,                       XK_0,                     view,           {.ui = ~0 } }, // view all tags
	{ MODKEY|ShiftMask,             XK_0,                     tag,            {.ui = ~0 } }, // tag applications
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	TAGKEYS(                        XK_7,                                     6)
	TAGKEYS(                        XK_8,                                     7)
	TAGKEYS(                        XK_9,                                     8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, // move window
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} }, // resize window
	{ ClkTagBar,            0,              Button1,        view,           {0} }, // view tag
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} }, // toggle multiple tags
};
