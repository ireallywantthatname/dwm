#include "layouts.c"
#include "movestack.c"
#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 0; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *fonts[] = {"FiraCode Nerd Font:size=11"};
static const char dmenufont[] = "FiraCode Nerd Font:size=11";
static const char col_gray1[] = "#000000";
static const char col_gray2[] = "#000000";
static const char col_gray3[] = "#c0c0c0";
static const char col_gray4[] = "#000000";
static const char col_cyan[] = "#008080";
static const char *colors[][3] = {
  /*               fg         bg         border   */
  [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
  [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL};
const char *spcmd2[] = {"st",     "-n", "spfm",   "-g",
  "114x40", "-e", "ncspot", NULL};
const char *spcmd3[] = {"keepassxc", NULL};
static Sp scratchpads[] = {
  /* name          cmd  */
  {"spterm", spcmd1},
  {"spranger", spcmd2},
  {"keepassxc", spcmd3},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
  /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
  /* class      instance    title       tags mask     isfloating   monitor */
  {"Gimp", NULL, NULL, 0, 1, -1},
  {"Firefox", NULL, NULL, 1 << 8, 0, -1},
  {NULL, "spterm", NULL, SPTAG(0), 1, -1},
  {NULL, "spfm", NULL, SPTAG(1), 1, -1},
  {NULL, "keepassxc", NULL, SPTAG(2), 0, -1},
  {"Blueman-manager", "blueman-manager", "Bluetooth Devices", 0, 1, -1},
  {"Brave-browser", "brave-browser", NULL, 000000001, 0, -1},
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
  1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
  1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol     arrange function */
  {"[]=", tile}, /* first entry is default */
  {"><>", NULL}, /* no layout function means floating behavior */
  {"[M]", monocle},
  {"HHH", grid},

};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY, TAG)                                                      \
{MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
  {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
  {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
  {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
{                                                                            \
  .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
}

/* commands */
static char dmenumon[2] =
  "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
  "dmenu_run", "-m",      dmenumon, "-fn",    dmenufont, "-nb",     col_gray1,
  "-nf",       col_gray3, "-sb",    col_cyan, "-sf",     col_gray4, NULL};
static const char *termcmd[] = {"kitty", NULL};

static const Key keys[] = {
  /* modifier                     key        function        argument */
  {MODKEY, XK_p, spawn, {.v = dmenucmd}},
  {MODKEY, XK_Return, spawn, {.v = termcmd}},
  {MODKEY, XK_b, togglebar, {0}},
  {MODKEY, XK_j, focusstack, {.i = +1}},
  {MODKEY, XK_k, focusstack, {.i = -1}},
  {MODKEY, XK_i, incnmaster, {.i = +1}},
  {MODKEY, XK_d, incnmaster, {.i = -1}},
  {MODKEY, XK_h, setmfact, {.f = -0.04}},
  {MODKEY, XK_l, setmfact, {.f = +0.04}},
  {MODKEY | ShiftMask, XK_j, movestack, {.i = +1}},
  {MODKEY | ShiftMask, XK_k, movestack, {.i = -1}},
  {MODKEY | ShiftMask, XK_Return, zoom, {0}},
  {MODKEY, XK_Tab, view, {0}},
  {MODKEY, XK_q, killclient, {0}},
  {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
  {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
  {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
  {MODKEY, XK_g, setlayout, {.v = &layouts[3]}},

  {MODKEY, XK_space, setlayout, {0}},
  {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
  {MODKEY, XK_0, view, {.ui = ~0}},
  {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
  {MODKEY, XK_comma, focusmon, {.i = -1}},
  {MODKEY, XK_period, focusmon, {.i = +1}},
  {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
  {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
  {MODKEY, XK_y, togglescratch, {.ui = 0}},
  {MODKEY, XK_u, togglescratch, {.ui = 1}},
  {MODKEY, XK_x, togglescratch, {.ui = 2}},
  TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
  TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
  TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},

  /* Brightness control */
  {0, XF86XK_MonBrightnessUp, spawn, SHCMD("brightnessctl set +5%")},
  {0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 5%-")},

  /* Volume control */
  {0, XF86XK_AudioRaiseVolume, spawn,
    SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+")},
  {0, XF86XK_AudioLowerVolume, spawn,
    SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-")},
  {0, XF86XK_AudioMute, spawn,
    SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle")},

  {0, XF86XK_AudioPrev, spawn, SHCMD("playerctl previous")},
  {0, XF86XK_AudioNext, spawn, SHCMD("playerctl next")},
  {0, XK_Pause, spawn, SHCMD("playerctl play-pause")},

  /* Select area screenshot */
  {0, XK_Print, spawn,
    SHCMD("maim -s | xclip -selection clipboard -t image/png -i")},

  {MODKEY, XK_w, spawn, SHCMD("brave")},
  {MODKEY, XK_e, spawn, SHCMD("thunar")},
  {Mod4Mask, XK_b, spawn, SHCMD("blueman-manager")},
  {Mod4Mask, XK_c, spawn, SHCMD("code")},
  {Mod4Mask, XK_w, spawn, SHCMD("google-chrome-unstable")},
  {Mod4Mask, XK_o, spawn, SHCMD("libreoffice")},

  {MODKEY | ShiftMask, XK_r, spawn, SHCMD("systemctl reboot")},
  {MODKEY | ShiftMask, XK_s, spawn, SHCMD("systemctl suspend")},
  {MODKEY | ShiftMask, XK_p, spawn, SHCMD("systemctl poweroff")}};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function argument */
  {ClkLtSymbol, 0, Button1, setlayout, {0}},
  {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
  {ClkWinTitle, 0, Button2, zoom, {0}},
  {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
  {ClkClientWin, MODKEY, Button1, movemouse, {0}},
  {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
  {ClkClientWin, MODKEY, Button1, resizemouse, {0}},
  {ClkTagBar, 0, Button1, view, {0}},
  {ClkTagBar, 0, Button3, toggleview, {0}},
  {ClkTagBar, MODKEY, Button1, tag, {0}},
  {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
