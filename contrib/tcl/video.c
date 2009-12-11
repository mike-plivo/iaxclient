/* 
 * video.c --
 *
 *	Implements a video widget integrated with iaxclient.
 *	Skeleton code from tkSquare.c.
 *
 *  Copyright (c) 2007  Mats Bengtsson
 *
 *  $Id: video.c,v 1.1 2009-06-09 08:10:55 cvs Exp $
 */

/*
 * Mac OS X build:
 * 
 *    gcc -Wall -g -DUSE_TCL_STUBS -DUSE_TK_STUBS  \
 *      -DTARGET_OS_MAC=1 -DTARGET_API_MAC_CARBON=1 -DMAC_OSX_TK \
 *      -I/Library/Frameworks/Tcl.framework/Headers/  \
 *      -I/Library/Frameworks/Tk.framework/Headers/  \
 *      -I/Users/matben/C/cvs/tcl/generic \
 *      -I/Users/matben/C/cvs/tk/generic \
 *      -I/Users/matben/C/cvs/tk/macosx \
 *      -c video.c
 *    gcc -dynamiclib -o video.dylib video.o \
 *      -L/Library/Frameworks/Tcl.framework/ -ltclstub8.4 \
 *      -L/Library/Frameworks/Tk.framework/ -ltkstub8.4
 *
 * Unix:
 * 
 *    gcc -Wall -g -DUSE_TCL_STUBS -DUSE_TK_STUBS -fpic -c video.c
 *    gcc -shared -o video.so video.o -ltclstub8.4 -ltkstub8.4
 *    
 */

#include "tkPort.h"
#define __NO_OLD_CONFIG
#include "tk.h"
#include "tkInt.h"

/*
 * A data structure of the following type is kept for each square
 * widget managed by this file:
 */

typedef struct {
    Tk_Window tkwin;		/* Window that embodies the square.  NULL
				 * means window has been deleted but
				 * widget record hasn't been cleaned up yet. */
    Display *display;		/* X's token for the window's display. */
    Tcl_Interp *interp;		/* Interpreter associated with widget. */
    Tcl_Command widgetCmd;	/* Token for square's widget command. */
    Tk_OptionTable optionTable;	/* Token representing the configuration
				 * specifications. */
    Tcl_Obj *widthObjPtr, *heightObjPtr; /* Width and height. */

    /*
     * Information used when displaying widget:
     */

    Tcl_Obj *bgBorderPtr;
    GC gc;			/* Graphics context for copying from
				 * off-screen pixmap onto screen. */
    Tcl_Obj *doubleBufferPtr;	/* Non-zero means double-buffer redisplay
				 * with pixmap;  zero means draw straight
				 * onto the display. */
    int updatePending;		/* Non-zero means a call to VideoDisplay
				 * has already been scheduled. */
} Video;

/*
 * Information used for argv parsing.
 */

static Tk_OptionSpec optionSpecs[] = {
    {TK_OPTION_BORDER, "-background", "background", "Background",
	    "#d9d9d9", Tk_Offset(Video, bgBorderPtr), -1, 0,
	    (ClientData) "white"},
    {TK_OPTION_SYNONYM, "-bg", (char *) NULL, (char *) NULL,
	    (char *) NULL, 0, -1, 0, (ClientData) "-background"},
    {TK_OPTION_BOOLEAN, "-dbl", "doubleBuffer", "DoubleBuffer",
	    "0", Tk_Offset(Video, doubleBufferPtr), -1},
    {TK_OPTION_SYNONYM, "-fg", (char *) NULL, (char *) NULL,
	    (char *) NULL, 0, -1, 0, (ClientData) "-foreground"},
    {TK_OPTION_PIXELS, "-height", "height", "Height", "150",
	    Tk_Offset(Video, heightObjPtr), -1},
    {TK_OPTION_PIXELS, "-width", "width", "Width", "200",
	    Tk_Offset(Video, widthObjPtr), -1},
    {TK_OPTION_END}
};

/*
 * Forward declarations for procedures defined later in this file:
 */

static int		VideoObjCmd(ClientData clientData,
			    Tcl_Interp *interp, int objc, 
			    Tcl_Obj * CONST objv[]);
static void		VideoDeletedProc(
			    ClientData clientData);
static int		VideoConfigure(Tcl_Interp *interp,
			    Video *videoPtr);
static void		VideoDestroy(char *memPtr);
static void		VideoDisplay(ClientData clientData);
static void		VideoObjEventProc(ClientData clientData,
			    XEvent *eventPtr);
static int		VideoWidgetObjCmd(ClientData clientData,
			    Tcl_Interp *, int objc, Tcl_Obj * CONST objv[]);


int
Video_Init(Tcl_Interp *interp)
{
    Tcl_CreateObjCommand(interp, "iaxclient::video", VideoObjCmd,
	    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    return TCL_OK;
}

/*
 * VideoSetFrame --
 * 
 *      This is supposed to draw a raw video frame into the widget.
 *      If sizes don't match then request new size but be sure not to
 *      draw outside the widget.
 */

int
VideoSetFrame(Video *videoPtr,
    int width, int height)
{
    
    /* The SDL sources contain a lot og bit blit functions... */
    
    
    
    
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * VideoObjCmd --
 *
 *	This procedure is invoked to process the "iaxclient::video" Tcl
 *	command.  It creates a new "iaxclient::video" widget.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	A new widget is created and configured.
 *
 *--------------------------------------------------------------
 */

static int
VideoObjCmd(
    ClientData clientData,	/* NULL. */
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj * CONST objv[])	/* Argument objects. */
{
    Video *videoPtr;
    Tk_Window tkwin;
    Tk_OptionTable optionTable;

    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "pathName ?options?");
	return TCL_ERROR;
    }

    tkwin = Tk_CreateWindowFromPath(interp, Tk_MainWindow(interp), 
	    Tcl_GetStringFromObj(objv[1], NULL), (char *) NULL);
    if (tkwin == NULL) {
	return TCL_ERROR;
    }
    Tk_SetClass(tkwin, "IAXCVideo");

    /*
     * Create the option table for this widget class.  If it has
     * already been created, the refcount will get bumped and just
     * the pointer will be returned.  The refcount getting bumped
     * does not concern us, because Tk will ensure the table is
     * deleted when the interpreter is destroyed.
     */

    optionTable = Tk_CreateOptionTable(interp, optionSpecs);

    /*
     * Allocate and initialize the widget record.  The memset allows
     * us to set just the non-NULL/0 items.
     */

    videoPtr			= (Video *) ckalloc(sizeof(Video));
    memset((void *) videoPtr, 0, (sizeof(Video)));

    videoPtr->tkwin		= tkwin;
    videoPtr->display		= Tk_Display(tkwin);
    videoPtr->interp		= interp;
    videoPtr->widgetCmd	= Tcl_CreateObjCommand(interp,
	    Tk_PathName(videoPtr->tkwin), VideoWidgetObjCmd,
	    (ClientData) videoPtr, VideoDeletedProc);
    videoPtr->gc		= None;
    videoPtr->optionTable	= optionTable;

    if (Tk_InitOptions(interp, (char *) videoPtr, optionTable, tkwin)
	    != TCL_OK) {
	Tk_DestroyWindow(videoPtr->tkwin);
	ckfree((char *) videoPtr);
	return TCL_ERROR;
    }

    Tk_CreateEventHandler(videoPtr->tkwin, ExposureMask|StructureNotifyMask,
	    VideoObjEventProc, (ClientData) videoPtr);
    if (Tk_SetOptions(interp, (char *) videoPtr, optionTable, objc - 2,
	    objv + 2, tkwin, NULL, (int *) NULL) != TCL_OK) {
	goto error;
    }
    if (VideoConfigure(interp, videoPtr) != TCL_OK) {
	goto error;
    }

    Tcl_SetObjResult(interp,
	    Tcl_NewStringObj(Tk_PathName(videoPtr->tkwin), -1));
    return TCL_OK;

error:
    Tk_DestroyWindow(videoPtr->tkwin);
    return TCL_ERROR;
}

/*
 *--------------------------------------------------------------
 *
 * VideoWidgetObjCmd --
 *
 *	This procedure is invoked to process the Tcl command
 *	that corresponds to a widget managed by this module.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *--------------------------------------------------------------
 */

static int
VideoWidgetObjCmd(
    ClientData clientData,		/* Information about square widget. */
    Tcl_Interp *interp,			/* Current interpreter. */
    int objc,				/* Number of arguments. */
    Tcl_Obj * CONST objv[])		/* Argument objects. */
{
    Video *videoPtr = (Video *) clientData;
    int result = TCL_OK;
    static CONST char *videoOptions[] = {"cget", "configure", (char *) NULL};
    enum {
	VIDEO_CGET, VIDEO_CONFIGURE
    };
    Tcl_Obj *resultObjPtr;
    int index;

    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "option ?arg arg...?");
	return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], videoOptions, "command",
	    0, &index) != TCL_OK) {
	return TCL_ERROR;
    }

    Tcl_Preserve((ClientData) videoPtr);
    
    switch (index) {
	case VIDEO_CGET: {
	    if (objc != 3) {
		Tcl_WrongNumArgs(interp, 2, objv, "option");
		goto error;
	    }
	    resultObjPtr = Tk_GetOptionValue(interp, (char *) videoPtr,
		    videoPtr->optionTable, objv[2], videoPtr->tkwin);
	    if (resultObjPtr == NULL) {
		result = TCL_ERROR;
	    } else {
		Tcl_SetObjResult(interp, resultObjPtr);
	    }
	    break;
	}
	case VIDEO_CONFIGURE: {
	    resultObjPtr = NULL;
	    if (objc == 2) {
		resultObjPtr = Tk_GetOptionInfo(interp, (char *) videoPtr,
			videoPtr->optionTable, (Tcl_Obj *) NULL,
			videoPtr->tkwin);
		if (resultObjPtr == NULL) {
		    result = TCL_ERROR;
		}
	    } else if (objc == 3) {
		resultObjPtr = Tk_GetOptionInfo(interp, (char *) videoPtr,
			videoPtr->optionTable, objv[2], videoPtr->tkwin);
		if (resultObjPtr == NULL) {
		    result = TCL_ERROR;
		}
	    } else {
		result = Tk_SetOptions(interp, (char *) videoPtr,
			videoPtr->optionTable, objc - 2, objv + 2,
			videoPtr->tkwin, NULL, (int *) NULL);
		if (result == TCL_OK) {
		    result = VideoConfigure(interp, videoPtr);
		}
		if (!videoPtr->updatePending) {
		    Tcl_DoWhenIdle(VideoDisplay, (ClientData) videoPtr);
		    videoPtr->updatePending = 1;
		}
	    }
	    if (resultObjPtr != NULL) {
		Tcl_SetObjResult(interp, resultObjPtr);
	    }
	}
    }
    Tcl_Release((ClientData) videoPtr);
    return result;

    error:
    Tcl_Release((ClientData) videoPtr);
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * VideoConfigure --
 *
 *	This procedure is called to process an argv/argc list in
 *	conjunction with the Tk option database to configure (or
 *	reconfigure) a square widget.
 *
 * Results:
 *	The return value is a standard Tcl result.  If TCL_ERROR is
 *	returned, then the interp's result contains an error message.
 *
 * Side effects:
 *	Configuration information, such as colors, border width,
 *	etc. get set for videoPtr;  old resources get freed,
 *	if there were any.
 *
 *----------------------------------------------------------------------
 */

static int
VideoConfigure(
    Tcl_Interp *interp,			/* Used for error reporting. */
    Video *videoPtr)			/* Information about widget. */
{
    Tk_3DBorder bgBorder;
    int doubleBuffer;
    int width, height;
    
    /*
     * Set the background for the window and create a graphics context
     * for use during redisplay.
     */

    bgBorder = Tk_Get3DBorderFromObj(videoPtr->tkwin, 
	    videoPtr->bgBorderPtr);
    Tk_SetWindowBackground(videoPtr->tkwin,
	    Tk_3DBorderColor(bgBorder)->pixel);
    Tcl_GetBooleanFromObj(NULL, videoPtr->doubleBufferPtr, &doubleBuffer);
    if ((videoPtr->gc == None) && (doubleBuffer)) {
	XGCValues gcValues;
	gcValues.function = GXcopy;
	gcValues.graphics_exposures = False;
	videoPtr->gc = Tk_GetGC(videoPtr->tkwin,
		GCFunction|GCGraphicsExposures, &gcValues);
    }

    /*
     * Register the desired geometry for the window.  Then arrange for
     * the window to be redisplayed.
     */

    Tk_GetPixelsFromObj(NULL, videoPtr->tkwin, videoPtr->widthObjPtr, &width);
    Tk_GetPixelsFromObj(NULL, videoPtr->tkwin, videoPtr->heightObjPtr, &height);
    Tk_GeometryRequest(videoPtr->tkwin, width, height);
    if (!videoPtr->updatePending) {
	Tcl_DoWhenIdle(VideoDisplay, (ClientData) videoPtr);
	videoPtr->updatePending = 1;
    }
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * VideoObjEventProc --
 *
 *	This procedure is invoked by the Tk dispatcher for various
 *	events on squares.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	When the window gets deleted, internal structures get
 *	cleaned up.  When it gets exposed, it is redisplayed.
 *
 *--------------------------------------------------------------
 */

static void
VideoObjEventProc(
    ClientData clientData,	/* Information about window. */
    XEvent *eventPtr)		/* Information about event. */
{
    Video *videoPtr = (Video *) clientData;

    if (eventPtr->type == Expose || eventPtr->type == ConfigureNotify) {
	if (!videoPtr->updatePending) {
	    Tcl_DoWhenIdle(VideoDisplay, (ClientData) videoPtr);
	    videoPtr->updatePending = 1;
	}
    } else if (eventPtr->type == DestroyNotify) {
	if (videoPtr->tkwin != NULL) {
	    Tk_FreeConfigOptions((char *) videoPtr, videoPtr->optionTable,
		    videoPtr->tkwin);
	    if (videoPtr->gc != None) {
		Tk_FreeGC(videoPtr->display, videoPtr->gc);
	    }
	    videoPtr->tkwin = NULL;
	    Tcl_DeleteCommandFromToken(videoPtr->interp,
		    videoPtr->widgetCmd);
	}
	if (videoPtr->updatePending) {
	    Tcl_CancelIdleCall(VideoDisplay, (ClientData) videoPtr);
	}
	Tcl_EventuallyFree((ClientData) videoPtr, VideoDestroy);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * VideoDeletedProc --
 *
 *	This procedure is invoked when a widget command is deleted.  If
 *	the widget isn't already in the process of being destroyed,
 *	this command destroys it.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The widget is destroyed.
 *
 *----------------------------------------------------------------------
 */

static void
VideoDeletedProc(
    ClientData clientData)	/* Pointer to widget record for widget. */
{
    Video *videoPtr = (Video *) clientData;
    Tk_Window tkwin = videoPtr->tkwin;

    /*
     * This procedure could be invoked either because the window was
     * destroyed and the command was then deleted (in which case tkwin
     * is NULL) or because the command was deleted, and then this procedure
     * destroys the widget.
     */

    if (tkwin != NULL) {
	Tk_DestroyWindow(tkwin);
    }
}

/*
 *--------------------------------------------------------------
 *
 * VideoDisplay --
 *
 *	This procedure redraws the contents of a square window.
 *	It is invoked as a do-when-idle handler, so it only runs
 *	when there's nothing else for the application to do.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Information appears on the screen.
 *
 *--------------------------------------------------------------
 */

static void
VideoDisplay(
    ClientData clientData)	/* Information about window. */
{
    Video *videoPtr = (Video *) clientData;
    Tk_Window tkwin = videoPtr->tkwin;
    Pixmap pm = None;
    Drawable d;
    Tk_3DBorder bgBorder;
    int doubleBuffer;

    videoPtr->updatePending = 0;
    if (!Tk_IsMapped(tkwin)) {
	return;
    }

    /*
     * Create a pixmap for double-buffering, if necessary.
     */

    Tcl_GetBooleanFromObj(NULL, videoPtr->doubleBufferPtr, &doubleBuffer);
    if (doubleBuffer) {
	pm = Tk_GetPixmap(Tk_Display(tkwin), Tk_WindowId(tkwin),
		Tk_Width(tkwin), Tk_Height(tkwin),
		DefaultDepthOfScreen(Tk_Screen(tkwin)));
	d = pm;
    } else {
	d = Tk_WindowId(tkwin);
    }

    /*
     * Redraw the widget's background and border.
     */
    
    bgBorder = Tk_Get3DBorderFromObj(videoPtr->tkwin, 
	    videoPtr->bgBorderPtr);
    Tk_Fill3DRectangle(tkwin, d, bgBorder, 0, 0, Tk_Width(tkwin),
	    Tk_Height(tkwin), 0, TK_RELIEF_FLAT);

    /*
     * If double-buffered, copy to the screen and release the pixmap.
     */

    if (doubleBuffer) {
	XCopyArea(Tk_Display(tkwin), pm, Tk_WindowId(tkwin), videoPtr->gc,
		0, 0, (unsigned) Tk_Width(tkwin), (unsigned) Tk_Height(tkwin),
		0, 0);
	Tk_FreePixmap(Tk_Display(tkwin), pm);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * VideoDestroy --
 *
 *	This procedure is invoked by Tcl_EventuallyFree or Tcl_Release
 *	to clean up the internal structure of a square at a safe time
 *	(when no-one is using it anymore).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Everything associated with the square is freed up.
 *
 *----------------------------------------------------------------------
 */

static void
VideoDestroy(
    char *memPtr)		/* Info about square widget. */
{
    Video *videoPtr = (Video *) memPtr;
    
    ckfree((char *) videoPtr);
}
