#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include "bitmaps/bitmaps.include"
#include "bitmask/bitmask.include"
#include "bitmaps/neko/neko.include"
#include "bitmask/neko/neko.mask.include"

#define MAX_TICK                9999
#define DEFAULT_FOREGROUND      "black"
#define DEFAULT_BACKGROUND      "white"
#define MIKU_DANGER1            0
#define MIKU_DANGER2            1
#define MIKU_DANGER3            2
#define MAID_ALERT1             3
#define MAID_PC1                4
#define MAID_PC2                5
#define MAID_PC3                6
#define MAID_WELCOME1           7
#define MAID_WELCOME2           8
#define MAID_WELCOME            9
#define MAID_MOVE1              10
#define MAID_MOVE2              11
#define MAID_MOVE3              12
#define NEKO_KAKI1              13
#define NEKO_KAKI2              14
#define MAXDISPLAYNAME          (64 + 5)
#define NOTDEFINED              (-1)

int BITMAP_WIDTH = 0;
int BITMAP_HEIGHT = 0;
char    *ClassName = "Asisten";
char    *ProgramName;

Display *theDisplay;
int     theScreen;
unsigned int    theDepth;
Window  theRoot;
Window  theWindow;
char    *WindowName = NULL;
Window  theTarget = None;
char    *TargetName = NULL;
Cursor  theCursor;

unsigned int    WindowWidth;
unsigned int    WindowHeight;

XColor  theForegroundColor;
XColor  theBackgroundColor;

/* Types of animals */
#define BITMAPTYPES 1

typedef struct _AnimalDefaults {
  char *name;
  int speed, idle, bitmap_width, bitmap_height;
  long time;
  int off_x, off_y;
} AnimalDefaultsData;

AnimalDefaultsData AnimalDefaultsDataTable[] =
{
  { "miku", 13, 6, 32, 32, 225000L, 0, 0 }
};

char    *Foreground = NULL;             /*   foreground */
char    *Background = NULL;             /*   background */
long    IntervalTime = 0L;              /*   time       */
double  NekoSpeed = (double)0;          /*   speed      */
int     IdleSpace = 0;                  /*   idle       */
int     NekoMoyou = NOTDEFINED;         /*   tora       */
int     NoShape = NOTDEFINED;           /*   noshape    */
int     ReverseVideo = NOTDEFINED;      /*   reverse    */
int     ToWindow = NOTDEFINED;          /*   towindow   */
int     ToFocus = NOTDEFINED;           /*   tofocus    */
int     XOffset=0,YOffset=0;            /* X and Y offsets for cat from mouse pointer. */

Bool    DontMapped = True;

int     NekoTickCount;          /* 猫動作カウンタ */
int     NekoStateCount;         /* 猫同一状態カウンタ */
int     NekoState;              /* 猫の状態 */

int     MouseX;                 /* マウスＸ座標 */
int     MouseY;                 /* マウスＹ座標 */

int     PrevMouseX = 0;         /* 直前のマウスＸ座標 */
int     PrevMouseY = 0;         /* 直前のマウスＹ座標 */
Window  PrevTarget = None;      /* 直前の目標ウィンドウのＩＤ */

int     NekoX;                  /* 猫Ｘ座標 */
int     NekoY;                  /* 猫Ｙ座標 */

int     NekoMoveDx;             /* 猫移動距離Ｘ */
int     NekoMoveDy;             /* 猫移動距離Ｙ */

int     NekoLastX;              /* 猫最終描画Ｘ座標 */
int     NekoLastY;              /* 猫最終描画Ｙ座標 */
GC      NekoLastGC;             /* 猫最終描画 GC */
/* Variables used to set how quickly the program will chose to raise itself. */
/* Look at Interval(), Handle Visiblility Notify Event */
int     RaiseWindowDelay=0;

Pixmap  Danger1Xbm, Danger2Xbm, Danger3Xbm, MaidAlert1Xbm, MaidPc1Xbm, MaidPc2Xbm, MaidPc3Xbm, MaidWelcome1Xbm, MaidWelcome2Xbm, MaidWelcomeXbm, MaidMove1Xbm, MaidMove2Xbm, MaidMove3Xbm;
Pixmap  Danger1Msk, Danger2Msk, Danger3Msk, MaidAlert1Msk, MaidPc1Msk, MaidPc2Msk, MaidPc3Msk, MaidWelcome1Msk, MaidWelcome2Msk, MaidWelcomeMsk, MaidMove1Msk, MaidMove2Msk, MaidMove3Msk;

GC      Danger1GC, Danger2GC, Danger3GC, MaidAlert1GC, MaidPc1GC, MaidPc2GC, MaidPc3GC, MaidWelcome1GC, MaidWelcome2GC, MaidWelcomeGC, MaidMove1GC, MaidMove2GC, MaidMove3GC;


Pixmap Neko1Xbm, Neko2Xbm, Neko1Msk, Neko2Msk;
GC     Neko1GC, Neko2GC;

typedef struct {
    GC          *GCCreatePtr;
    Pixmap      *BitmapCreatePtr;
    char        *PixelPattern[BITMAPTYPES];
    Pixmap      *BitmapMasksPtr;
    char        *MaskPattern[BITMAPTYPES];
} BitmapGCData;
BitmapGCData    BitmapGCDataTable[] =
{
    { &Neko1GC, &Neko1Xbm,  kaki1_bits,
      &Neko1Msk, kaki1_bits },

    { &Neko2GC, &Neko2Xbm,  kaki2_bits,
      &Neko2Msk, kaki2_bits },

    { &MaidAlert1GC, &MaidAlert1Xbm,  maid_alert1_bits,
      &MaidAlert1Msk, maid_alert1_mask_bits },

    { &MaidWelcome1GC, &MaidWelcome1Xbm,  maid_welcome1_bits,
      &MaidWelcome1Msk, maid_welcome1_mask_bits },

    { &MaidWelcome2GC, &MaidWelcome2Xbm,  maid_welcome2_bits,
      &MaidWelcome2Msk, maid_welcome2_mask_bits },

    { &MaidWelcomeGC, &MaidWelcomeXbm,  maid_welcome_bits,
      &MaidWelcomeMsk, maid_welcome_mask_bits },

    { &MaidMove1GC, &MaidMove1Xbm,  maid_move1_bits,
      &MaidMove1Msk, maid_move1_mask_bits },

    { &MaidMove2GC, &MaidMove2Xbm,  maid_move2_bits,
      &MaidMove2Msk, maid_move2_mask_bits },

    { &MaidMove3GC, &MaidMove3Xbm,  maid_move3_bits,
      &MaidMove3Msk, maid_move3_mask_bits },

    { &Danger1GC, &Danger1Xbm,  danger1_bits,
      &Danger1Msk, danger1_mask_bits },
    { &Danger2GC, &Danger2Xbm,  danger2_bits,
      &Danger2Msk, danger2_mask_bits },
    { &Danger3GC, &Danger3Xbm,  danger3_bits,
      &Danger3Msk, danger3_mask_bits },

    { &MaidPc1GC, &MaidPc1Xbm,  maid_pc1_bits,
      &MaidPc1Msk, maid_pc1_mask_bits },

    { &MaidPc2GC, &MaidPc2Xbm,  maid_pc2_bits,
      &MaidPc2Msk, maid_pc2_mask_bits },

    { &MaidPc3GC, &MaidPc3Xbm,  maid_pc3_bits,
      &MaidPc3Msk, maid_pc3_mask_bits },

    { NULL, NULL, NULL, NULL, NULL }
};

void setNekoState(int state) {
  NekoState = state;

  if (NekoState == MAID_WELCOME1 || NekoState == MAID_WELCOME2 || NekoState == MAID_WELCOME) {
    BITMAP_WIDTH = 199;
    BITMAP_HEIGHT = 253;
  }
  else if (NekoState == MIKU_DANGER1 || NekoState == MIKU_DANGER2 || NekoState == MIKU_DANGER3) {
    BITMAP_WIDTH = 296;
    BITMAP_HEIGHT = 280;
  }
  else if (NekoState == MAID_ALERT1) {
    BITMAP_WIDTH = 300;
    BITMAP_HEIGHT = 168;
  }
  else if (NekoState == MAID_PC1 || NekoState == MAID_PC2 || NekoState == MAID_PC3) {
    BITMAP_WIDTH = 332;
    BITMAP_HEIGHT = 280;
  }
  else if (NekoState == MAID_MOVE1 || NekoState == MAID_MOVE2 || NekoState == MAID_MOVE3) {
    BITMAP_WIDTH = 260;
    BITMAP_HEIGHT = 191;
  }
  else if (NekoState == NEKO_KAKI1 || NekoState == NEKO_KAKI2) {
    BITMAP_WIDTH = 32;
    BITMAP_HEIGHT = 32;
  }
}

void InitScreen(char *DisplayName)
{
  XSetWindowAttributes  theWindowAttributes;
  unsigned long         theWindowMask;
  Window                        theTempRoot;
  int                           WindowPointX;
  int                           WindowPointY;
  unsigned int          BorderWidth;
  int                           event_base, error_base;

  if ((theDisplay = XOpenDisplay(DisplayName)) == NULL) {
    fprintf(stderr, "%s: Can't open display", ProgramName);
    if (DisplayName != NULL) {
      fprintf(stderr, " %s.\n", DisplayName);
    } else {
      fprintf(stderr, ".\n");
    }
    exit(1);
  }

 // GetResources();
  char  *resource;
  int           num;
  int loop;

  Foreground = DEFAULT_FOREGROUND;
  Background = DEFAULT_BACKGROUND;
  NekoMoyou = 0;
  IntervalTime = AnimalDefaultsDataTable[NekoMoyou].time;
  NekoSpeed = (double)(AnimalDefaultsDataTable[NekoMoyou].speed);
  IdleSpace = AnimalDefaultsDataTable[NekoMoyou].idle;

  XOffset = XOffset + AnimalDefaultsDataTable[NekoMoyou].off_x;
  YOffset = YOffset + AnimalDefaultsDataTable[NekoMoyou].off_y;

  NoShape = False;
  ReverseVideo = False;
  ToWindow = False;
  ToFocus = False;
 // GetResources();

  if (!NoShape && XShapeQueryExtension(theDisplay, &event_base, &error_base) == False) {
    fprintf(stderr, "Display not suported shape extension.\n");
    NoShape = True;
  }

  theScreen = DefaultScreen(theDisplay);
  theDepth = DefaultDepth(theDisplay, theScreen);

  theRoot = RootWindow(theDisplay, theScreen);

  XGetGeometry(theDisplay, theRoot, &theTempRoot,
               &WindowPointX, &WindowPointY,
               &WindowWidth, &WindowHeight,
               &BorderWidth, &theDepth);

  // SetupColors();
  XColor      theExactColor;
  Colormap    theColormap;

  theColormap = DefaultColormap(theDisplay, theScreen);

  if (!XAllocNamedColor(theDisplay, theColormap, Foreground, &theForegroundColor, &theExactColor)) {
      fprintf(stderr, "%s: Can't XAllocNamedColor(\"%s\").\n", ProgramName, Foreground);
      exit(1);
  }

  if (!XAllocNamedColor(theDisplay, theColormap, Background, &theBackgroundColor, &theExactColor)) {
      fprintf(stderr, "%s: Can't XAllocNamedColor(\"%s\").\n", ProgramName, Background);
      exit(1);
  }
  // SetupColors();

  theWindowAttributes.background_pixel = theBackgroundColor.pixel;
  theWindowAttributes.cursor = theCursor;
  theWindowAttributes.override_redirect = True;

  if (!ToWindow) {
    XChangeWindowAttributes(theDisplay, theRoot, CWCursor, &theWindowAttributes);
  }

  theWindowMask = CWBackPixel | CWCursor | CWOverrideRedirect;

  theWindow = XCreateWindow(theDisplay, theRoot, 0, 0,
                            BITMAP_WIDTH, BITMAP_HEIGHT,
                            0, theDepth, InputOutput, CopyFromParent,
                            theWindowMask, &theWindowAttributes);

  if (WindowName == NULL) WindowName = ProgramName;
  XStoreName(theDisplay, theWindow, WindowName);

  //InitBitmapAndGCs();
  BitmapGCData        *BitmapGCDataTablePtr;
  XGCValues           theGCValues;

  theGCValues.function = GXcopy;
  theGCValues.foreground = theForegroundColor.pixel;
  theGCValues.background = theBackgroundColor.pixel;
  theGCValues.fill_style = FillTiled;
  theGCValues.ts_x_origin = 0;
  theGCValues.ts_y_origin = 0;

  for (BitmapGCDataTablePtr=BitmapGCDataTable; BitmapGCDataTablePtr->GCCreatePtr!=NULL; BitmapGCDataTablePtr++) 
  {
       *(BitmapGCDataTablePtr->BitmapCreatePtr) = XCreatePixmapFromBitmapData(theDisplay, theRoot, BitmapGCDataTablePtr->PixelPattern[NekoMoyou], BITMAP_WIDTH, BITMAP_HEIGHT, theForegroundColor.pixel, theBackgroundColor.pixel, DefaultDepth(theDisplay, theScreen));

       theGCValues.tile = *(BitmapGCDataTablePtr->BitmapCreatePtr);

       *(BitmapGCDataTablePtr->BitmapMasksPtr) = XCreateBitmapFromData(theDisplay, theRoot, BitmapGCDataTablePtr->MaskPattern[NekoMoyou], BITMAP_WIDTH, BITMAP_HEIGHT);

       *(BitmapGCDataTablePtr->GCCreatePtr) = XCreateGC(theDisplay, theWindow, GCFunction | GCForeground | GCBackground | GCTile | GCTileStipXOrigin | GCTileStipYOrigin | GCFillStyle, &theGCValues);
   }
  //InitBitmapAndGCs();

  XSelectInput(theDisplay, theWindow, ExposureMask|VisibilityChangeMask|KeyPressMask);

  XFlush(theDisplay);
}

Bool ProcessEvent()
{
    XEvent      theEvent;
    Bool        ContinueState = True;

    while (XPending(theDisplay)) 
    {
        XNextEvent(theDisplay,&theEvent);

        switch (theEvent.type) {
        case Expose:
            if (theEvent.xexpose.count == 0) { //exe disini
                //RedrawNeko();
                XFillRectangle(theDisplay, theWindow, NekoLastGC, 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
                XFlush(theDisplay);
                //RedrawNeko();
            }
            break;
        default:
            /* Unknown Event */
            break;
        }
    }

    return(ContinueState);
}

typedef struct {
    GC          *TickGCPtr;
    Pixmap      *TickMaskPtr;
} Animation;

Animation       AnimationPattern[][2] =
{
  { &Danger1GC, &Danger1Msk } ,
  { &Danger2GC, &Danger2Msk } ,
  { &Danger3GC, &Danger3Msk } ,

  { &MaidAlert1GC, &MaidAlert1Msk } ,

  { &MaidPc1GC, &MaidPc1Msk } ,
  { &MaidPc2GC, &MaidPc2Msk } ,
  { &MaidPc3GC, &MaidPc3Msk } ,

  { &MaidWelcome1GC, &MaidWelcome1Msk } ,
  { &MaidWelcome2GC, &MaidWelcome2Msk } ,
  { &MaidWelcomeGC, &MaidWelcomeMsk } ,

  { &MaidMove1GC, &MaidMove1Msk } ,
  { &MaidMove2GC, &MaidMove2Msk } ,
  { &MaidMove3GC, &MaidMove3Msk } ,

  { &Neko1GC, &Neko1Msk } ,
};
void DrawNeko(int x, int y, Animation DrawAnime)
{
    register GC         DrawGC = *(DrawAnime.TickGCPtr);
    register Pixmap     DrawMask = *(DrawAnime.TickMaskPtr);

    if ((x != NekoLastX) || (y != NekoLastY) || (DrawGC != NekoLastGC)) 
    {
      XWindowChanges    theChanges;

      theChanges.x = x;
      theChanges.y = y;
      XConfigureWindow(theDisplay, theWindow, CWX | CWY, &theChanges);
      
      if (NoShape == False) 
      {
        XShapeCombineMask(theDisplay, theWindow, ShapeBounding, 0, 0, DrawMask, ShapeSet);
      }
      if (DontMapped) 
      {
        XMapWindow(theDisplay, theWindow);
        DontMapped = 0;
      }
      XFillRectangle(theDisplay, theWindow, DrawGC, 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
    }

    XFlush(theDisplay);

    NekoLastX = x;
    NekoLastY = y;

    NekoLastGC = DrawGC;
}


void ProcessNeko()
{
  struct itimerval      Value;

  NekoX = (WindowWidth - BITMAP_WIDTH / 2) / 2;
  NekoY = (WindowHeight - BITMAP_HEIGHT / 2) / 2;

  NekoLastX = NekoX;
  NekoLastY = NekoY;

  timerclear(&Value.it_interval);
  timerclear(&Value.it_value);

  Value.it_interval.tv_usec = IntervalTime;
  Value.it_value.tv_usec = IntervalTime;

  setitimer(ITIMER_REAL, &Value, 0);

  do {
    printf("count: %d  %d:%d %d:%d\n", NekoTickCount, BITMAP_HEIGHT, BITMAP_WIDTH, NekoY, NekoX);

    if (NekoState == MIKU_DANGER1) {
        setNekoState(MIKU_DANGER2);
    }
    else if (NekoState == MIKU_DANGER2) {
        setNekoState(MIKU_DANGER3);
    }
    else {
        setNekoState(MIKU_DANGER1);
    }
    

    if (NekoTickCount == MAX_TICK) 
        NekoTickCount = 0;
    else {
        NekoTickCount++;
    }

    DrawNeko(NekoX, NekoY-300, AnimationPattern[NekoState][0]);


    //Interval();
    pause();
    if (RaiseWindowDelay>0) RaiseWindowDelay--;
    //Interval();

  } while (ProcessEvent());
}

int NekoErrorHandler(Display *dpy, XErrorEvent *err)
{
  if (err->error_code==BadWindow && (ToWindow || ToFocus)) {
    return 0;
  } else {
    char msg[80];
    XGetErrorText(dpy, err->error_code, msg, 80);
    fprintf(stderr, "%s: Error and exit.\n%s\n", ProgramName, msg);
    exit(1);
  }
}
static void NullFunction();
static void NullFunction()
{
  /* No Operation */
#if defined(SYSV) || defined(SVR4)
  signal(SIGALRM, NullFunction);
#endif /* SYSV || SVR4 */
}


int main()
{
  ProgramName = "anime";

  setNekoState(MIKU_DANGER1);

  XSetErrorHandler(NekoErrorHandler);

  char theDisplayName[MAXDISPLAYNAME];
  InitScreen(theDisplayName);

  signal(SIGALRM, NullFunction);

  ProcessNeko();
}
