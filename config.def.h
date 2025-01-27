/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *altbarcmd        = "polybar-launch"; /* Alternate bar launch command */
static const unsigned int gappx     = 18;        /* gaps between windows */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

/* rules */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",       NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",    NULL,     NULL,           0,         0,          0,          -1,        -1 },
	{ "kitty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,         NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.52; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "☰",      tile },    /* first entry is default */
	{ "⌀",      NULL },    /* no layout function means floating behavior */
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-show", "run", "-display-run", NULL };
/* static const char *termcmd[]  = { "kitty", "--listen-on", "unix:@kittysocket", NULL }; */
static const char *termcmd[]  = { "kitty", NULL };
static const char *lfcmd[]  = { "kitty", "lf", NULL };

/* custom func declarations */
static void unfloatvisible(const Arg *arg);
static void togglefullscr(const Arg *arg);

#include "movestack.c"
static Key keys[] = {
	/* modifier                key            function        argument */
	{ MODKEY,                  XK_Shift_R,    spawn,          {.v = roficmd } },
	{ MODKEY,                  XK_Return,     spawn,          {.v = termcmd } },
	{ MODKEY,                  XK_backslash,  spawn,          {.v = lfcmd } },
	/* { MODKEY,                  XK_b,          togglebar,      {0} }, */
	{ MODKEY,                  XK_j,          focusstack,     {.i = +1 } },
	{ MODKEY,                  XK_l,          focusstack,     {.i = +1 } },
	{ MODKEY,                  XK_k,          focusstack,     {.i = -1 } },
  { MODKEY,                  XK_h,          focusmaster,    {0} },
	{ MODKEY|ControlMask,      XK_j,          incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,      XK_k,          incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,      XK_h,          setmfact,       {.f = -0.02} },
	{ MODKEY|ControlMask,      XK_l,          setmfact,       {.f = +0.02} },
	{ MODKEY|ShiftMask,        XK_j,          movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,        XK_k,          movestack,      {.i = -1 } },
	{ MODKEY,                  XK_Tab,        view,           {0} },
	{ MODKEY,                  XK_q,          killclient,     {0} },
	{ MODKEY,                  XK_a,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                  XK_s,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                  XK_m,          setlayout,      {.v = &layouts[2]} },
  { MODKEY,                  XK_f,          togglefullscr,  {0} },
	{ MODKEY|ShiftMask,        XK_a,          unfloatvisible, {0} },
	{ MODKEY,                  XK_0,          view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,        XK_0,          tag,            {.ui = ~0 } },
	{ MODKEY,                  XK_comma,      focusmon,       {.i = -1 } },
	{ MODKEY,                  XK_period,     focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,        XK_comma,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,        XK_period,     tagmon,         {.i = +1 } },
	{ MODKEY,                  XK_equal,      changegaps,     {.i = -6 } },
	{ MODKEY,                  XK_minus,      changegaps,     {.i = +6 } },
	{ MODKEY|ShiftMask,        XK_equal,      setgaps,        {.i = 18  } },
	{ MODKEY|ShiftMask,        XK_minus,      setgaps,        {.i = 48  } },
	{ MODKEY|ShiftMask,        XK_u,          view_adjacent,  {.i = +1 } },
	{ MODKEY|ShiftMask,        XK_i,          view_adjacent,  {.i = -1 } },
	TAGKEYS(                   XK_1,                          0)
	TAGKEYS(                   XK_2,                          1)
	TAGKEYS(                   XK_3,                          2)
	TAGKEYS(                   XK_4,                          3)
	TAGKEYS(                   XK_5,                          4)
	TAGKEYS(                   XK_6,                          5)
	TAGKEYS(                   XK_grave,                      5)
	/* TAGKEYS(                   XK_7,                          6) */
	/* TAGKEYS(                   XK_8,                          7) */
	/* TAGKEYS(                   XK_9,                          8) */
	{ MODKEY|ShiftMask,        XK_q,          quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

/* Custom Funcs */
void
unfloatvisible(const Arg *arg)
{
    Client *c;

    for (c = selmon->clients; c; c = c->next)
        if (ISVISIBLE(c) && c->isfloating)
            c->isfloating = c->isfixed;

    if (arg && arg->v)
        setlayout(arg);
    else
        arrange(selmon);
}

void
togglefullscr(const Arg *arg)
{
  if(selmon->sel)
    setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}

// View next/previous tags
void
view_adjacent(const Arg *arg)
{
	int i, curtags;
	int seltag = 0;
	Arg a;

	curtags = selmon->tagset[selmon->seltags];
	for(i = 0; i < LENGTH(tags); i++)
		if(curtags & (1 << i)){
			seltag = i;
			break;
		}

	seltag = (seltag + arg->i) % (int)LENGTH(tags);
	if(seltag < 0)
		seltag += LENGTH(tags);

	a.i = (1 << seltag);
	view(&a);
}

