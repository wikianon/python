<!-- OverLIB 3.0 -->

<!-- Original:  Erik Bosrup (erik@bosrup.com)  -->
<!-- Web Site:  http://www.bosrup.com/web/overlib/  -->
<!-- Portions by:  Dan Steinman (dansteinman.com)  -->

var INARRAY=1; // Do nt change these constants
var CAPARRAY=2;
var STICKY=3;
var BACKGROUND=4;
var NOCLOSE=5;
var CAPTION=6;
var LEFT=7;
var RIGHT=8;
var CENTER=9;
var OFFSETX=10;
var OFFSETY=11;
var FGCOLOR=12;
var BGCOLOR=13;
var TEXTCOLOR=14;
var CAPCOLOR=15;
var CLOSECOLOR=16;
var WIDTH=17;
var BORDER=18;
var STATUS=19;
var AUTOSTATUS=20;
var AUTOSTATUSCAP=21;
var HEIGHT=22;
var CLOSETEXT=23;
var SNAPX=24;
var SNAPY=25;
var FIXX=26;
var FIXY=27;
var FGBACKGROUND=28;
var BGBACKGROUND=29;
var PADX=30;
var PADY=31;
var PADX2=32;
var PADY2=33;
var FULLHTML=34;
var ABOVE=35;
var BELOW=36;
var CAPICON=37;
var TEXTFONT=38;
var CAPTIONFONT=39;
var CLOSEFONT=40;
var TEXTSIZE=41;
var CAPTIONSIZE=42;
var CLOSESIZE=43;


////////////////////////////////////////////////////////////////////////////////////
// DEFAULT CONFIGURATION
// You don't have to change anything here if you don't want to. All of this can be
// changed on your html page or through an overLIB call.
////////////////////////////////////////////////////////////////////////////////////

// Main background color (the large area)
// Usually a bright color (white, yellow etc)
if (typeof ol_fgcolor == 'undefined') { var ol_fgcolor = "#CCCCFF";}

// Border color and color of caption
// Usually a dark color (black, brown etc)
if (typeof ol_bgcolor == 'undefined') { var ol_bgcolor = "#333399";}

// Text color
// Usually a dark color
if (typeof ol_textcolor == 'undefined') { var ol_textcolor = "#000000";}

// Color of the caption text
// Usually a bright color
if (typeof ol_capcolor == 'undefined') { var ol_capcolor = "#FFFFFF";}

// Color of "Close" when using Sticky
// Usually a semi-bright color
if (typeof ol_closecolor == 'undefined') { var ol_closecolor = "#9999FF";}

// Font face for the main text
if (typeof ol_textfont == 'undefined') { var ol_textfont = "Verdana,Arial,Helvetica";}

// Font face for the caption
if (typeof ol_captionfont == 'undefined') { var ol_captionfont = "Verdana,Arial,Helvetica";}

// Font face for the close text
if (typeof ol_closefont == 'undefined') { var ol_closefont = "Verdana,Arial,Helvetica";}

// Font size for the main text
if (typeof ol_textsize == 'undefined') { var ol_textsize = "2";}

// Font size for the caption
if (typeof ol_captionsize == 'undefined') { var ol_captionsize = "1";}

// Font size for the close text
if (typeof ol_closesize == 'undefined') { var ol_closesize = "1";}

// Width of the popups in pixels
// 100-300 pixels is typical
if (typeof ol_width == 'undefined') { var ol_width = "200";}

// How thick the ol_border should be in pixels
// 1-3 pixels is typical
if (typeof ol_border == 'undefined') { var ol_border = "1";}

// How many pixels to the right/left of the cursor to show the popup
// Values between 3 and 12 are best
if (typeof ol_offsetx == 'undefined') { var ol_offsetx = 10;}

// How many pixels to the below the cursor to show the popup
// Values between 3 and 12 are best
if (typeof ol_offsety == 'undefined') { var ol_offsety = 10;}

// Default text for popups
// Should you forget to pass something to overLIB this will be displayed.
if (typeof ol_text == 'undefined') { var ol_text = "Default Text"; }

// Default caption
// You should leave this blank or you will have problems making non caps popups.
if (typeof ol_cap == 'undefined') { var ol_cap = ""; }

// Decides if sticky popups are default.
// 0 for non, 1 for stickies.
if (typeof ol_sticky == 'undefined') { var ol_sticky = 0; }

// Default background image. Better left empty unless you always want one.
if (typeof ol_background == 'undefined') { var ol_background = ""; }

// Text for the closing sticky popups.
// Normal is "Close".
if (typeof ol_close == 'undefined') { var ol_close = "Close"; }

// Default vertical alignment for popups.
// It's best to leave RIGHT here. Other options are LEFT and CENTER.
if (typeof ol_hpos == 'undefined') { var ol_hpos = RIGHT; }

// Default status bar text when a popup is invoked.
if (typeof ol_status == 'undefined') { var ol_status = ""; }

// If the status bar automatically should load either text or caption.
// 0=nothing, 1=text, 2=caption
if (typeof ol_autostatus == 'undefined') { var ol_autostatus = 0; }

// Default height for popup. Often best left alone.
if (typeof ol_heigh == 'undefined') { var ol_height = -1; }

// Horizontal grid spacing that popups will snap to.
// 0 makes no grid, anything else will cause a snap to that grid spacing.
if (typeof ol_snapx == 'undefined') { var ol_snapx = 0; }

// Vertical grid spacing that popups will snap to.
// 0 makes no grid, andthing else will cause a snap to that grid spacing.
if (typeof ol_snapy == 'undefined') { var ol_snapy = 0; }

// Sets the popups horizontal position to a fixed column.
// Anything above -1 will cause fixed position.
if (typeof ol_fixx == 'undefined') { var ol_fixx = -1; }

// Sets the popups vertical position to a fixed row.
// Anything above -1 will cause fixed position.
if (typeof ol_fixy == 'undefined') { var ol_fixy = -1; }

// Background image for the popups inside.
if (typeof ol_fgbackground == 'undefined') { var ol_fgbackground = ""; }

// Background image for the popups frame.
if (typeof ol_bgbackground == 'undefined') { var ol_bgbackground = ""; }

// How much horizontal left padding text should get by default when BACKGROUND is used.
if (typeof ol_padxl == 'undefined') { var ol_padxl = 1; }

// How much horizontal right padding text should get by default when BACKGROUND is used.
if (typeof ol_padxr == 'undefined') { var ol_padxr = 1; }

// How much vertical top padding text should get by default when BACKGROUND is used.
if (typeof ol_padyt == 'undefined') { var ol_padyt = 1; }

// How much vertical bottom padding text should get by default when BACKGROUND is used.
if (typeof ol_padyb == 'undefined') { var ol_padyb = 1; }

// If the user by default must supply all html for complete popup control.
// Set to 1 to activate, 0 otherwise.
if (typeof ol_fullhtml == 'undefined') { var ol_fullhtml = 0; }

// Allow overLIB to load usage images. Set to zero to stop.
if (typeof o3_tracker == 'undefined') { var o3_tracker = 1; }

// Default vertical position of the popup. Default should normally be BELOW.
// ABOVE only works when HEIGHT is defined.
if (typeof ol_vpos == 'undefined') { var ol_vpos = BELOW; }

// Default height of popup to use when placing the popup above the cursor.
if (typeof ol_aboveheight == 'undefined') { var ol_aboveheight = 0; }

// Default icon to place next to the popups caption.
if (typeof ol_caption == 'undefined') { var ol_capicon = ""; }



////////////////////////////////////////////////////////////////////////////////////
// END CONFIGURATION
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// INIT
////////////////////////////////////////////////////////////////////////////////////

// Runtime variables init. Used for runtime only, don't change, not for config!
var o3_text = "";
var o3_cap = "";
var o3_sticky = 0;
var o3_background = "";
var o3_close = "Close";
var o3_hpos = RIGHT;
var o3_offsetx = 2;
var o3_offsety = 2;
var o3_fgcolor = "";
var o3_bgcolor = "";
var o3_textcolor = "";
var o3_capcolor = "";
var o3_closecolor = "";
var o3_width = 100;
var o3_border = 1;
var o3_status = "";
var o3_autostatus = 0;
var o3_height = -1;
var o3_snapx = 0;
var o3_snapy = 0;
var o3_fixx = -1;
var o3_fixy = -1;
var o3_fgbackground = "";
var o3_bgbackground = "";
var o3_padxl = 0;
var o3_padxr = 0;
var o3_padyt = 0;
var o3_padyb = 0;
var o3_fullhtml = 0;
var o3_vpos = BELOW;
var o3_aboveheight = 0;
var o3_capicon = "";
var o3_textfont = "Verdana,Arial,Helvetica";
var o3_captionfont = "Verdana,Arial,Helvetica";
var o3_closefont = "Verdana,Arial,Helvetica";
var o3_textsize = "1";
var o3_captionsize = "1";
var o3_closesize = "1";

// Display state variables
var o3_x = 0;
var o3_y = 0;
var o3_allowmove = 0;
var o3_showingsticky = 0;
var o3_removecounter = 0;

// Our layer
var over = null;

// Decide browser version
var ns4 = (document.layers)? true:false
var ie4 = (document.all)? true:false

// Microsoft Stupidity Check(tm).
if (ie4) {
	if (navigator.userAgent.indexOf('MSIE 5')>0) {
		var ie5 = true;
		} else {
		var ie5 = false;
	}
	} else {
	var ie5 = false;
}

// Capture events and set over to correct DOM position.
if ( (ns4) || (ie4) ) {
	if (ns4) over = document.overDiv
	if (ie4) over = overDiv.style
	document.onmousemove = mouseMove
	if (ns4) document.captureEvents(Event.MOUSEMOVE)
}

////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////

// overlib(arg0, ..., argN)
// Loads parameters into global runtime variables.
function overlib() {
	
	// Load defaults to runtime.
	o3_text = ol_text;
	o3_cap = ol_cap;
	o3_sticky = ol_sticky;
	o3_background = ol_background;
	o3_close = ol_close;
	o3_hpos = ol_hpos;
	o3_offsetx = ol_offsetx;
	o3_offsety = ol_offsety;
	o3_fgcolor = ol_fgcolor;
	o3_bgcolor = ol_bgcolor;
	o3_textcolor = ol_textcolor;
	o3_capcolor = ol_capcolor;
	o3_closecolor = ol_closecolor;
	o3_width = ol_width;
	o3_border = ol_border;
	o3_status = ol_status;
	o3_autostatus = ol_autostatus;
	o3_height = ol_height;
	o3_snapx = ol_snapx;
	o3_snapy = ol_snapy;
	o3_fixx = ol_fixx;
	o3_fixy = ol_fixy;
	o3_fgbackground = ol_fgbackground;
	o3_bgbackground = ol_bgbackground;
	o3_padxl = ol_padxl;
	o3_padxr = ol_padxr;
	o3_padyt = ol_padyt;
	o3_padyb = ol_padyb;
	o3_fullhtml = ol_fullhtml;
	o3_vpos = ol_vpos;
	o3_aboveheight = ol_aboveheight;
	o3_capicon = ol_capicon;
	o3_textfont = ol_textfont;
	o3_captionfont = ol_captionfont;
	o3_closefont = ol_closefont;
	o3_textsize = ol_textsize;
	o3_captionsize = ol_captionsize;
	o3_closesize = ol_closesize;
	
	o3_text = arguments[0];
	
	return overlib300();
}



// Clears popups if appropriate
function nd() {
	if ( o3_removecounter >= 1 ) { o3_showingsticky = 0 };
	if ( (ns4) || (ie4) ) {
		if ( o3_showingsticky == 0 ) {
			o3_allowmove = 0;
			hideObject(over);
			} else {
			o3_removecounter++;
		}
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// OVERLIB 3.00 FUNCTION
////////////////////////////////////////////////////////////////////////////////////

// This function decides what it is we want to display and how we want it done.
function overlib300() {
	
	// Make layer content
	var layerhtml;
	
	if (o3_background != "" || o3_fullhtml) {
		// Use background instead of box.
		layerhtml = ol_content_background(o3_text, o3_background, o3_fullhtml);
		} else {
		// They want a popup box.
		
		// Prepare popup background
		if (o3_fgbackground != "") {
			o3_fgbackground = "BACKGROUND=\""+o3_fgbackground+"\"";
		}
		if (o3_bgbackground != "") {
			o3_bgbackground = "BACKGROUND=\""+o3_bgbackground+"\"";
		}
		
		// Prepare popup colors
		if (o3_fgcolor != "") {
			o3_fgcolor = "BGCOLOR=\""+o3_fgcolor+"\"";
		}
		if (o3_bgcolor != "") {
			o3_bgcolor = "BGCOLOR=\""+o3_bgcolor+"\"";
		}
		
		// Prepare popup height
		if (o3_height > 0) {
			o3_height = "HEIGHT=" + o3_height;
			} else {
			o3_height = "";
		}
		
		// Decide which kinda box.
		if (o3_cap == "") {
			// Plain
			layerhtml = ol_content_simple(o3_text);
			} else {
			// With caption
			if (o3_sticky) {
				// Show close text
				layerhtml = ol_content_caption(o3_text, o3_cap, o3_close);
				} else {
				// No close text
				layerhtml = ol_content_caption(o3_text, o3_cap, "");
			}
		}
	}
	
	// We want it to stick!
	if (o3_sticky) {
		o3_showingsticky = 1;
		o3_removecounter = 0;
	}
	
	// Write layer
	layerWrite(layerhtml);
	
	// Prepare status bar
	if (o3_autostatus > 0) {
		o3_status = o3_text;
		if (o3_autostatus > 1) {
			o3_status = o3_cap;
		}
	}
	
	// When placing the layer the first time, even stickies may be moved.
	o3_allowmove = 0;
	
	// Show layer
	disp(o3_status);
	
	// Stickies should stay where they are.
	if (o3_sticky) {
		o3_allowmove = 0;
		return false;
		} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////
// LAYER GENERATION FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////

// Makes simple table without caption
function ol_content_simple(text) {
	txt = "<TABLE WIDTH="+o3_width+" BORDER=0 CELLPADDING="+o3_border+" CELLSPACING=0 "+o3_bgcolor+" "+o3_height+"><TR><TD><TABLE WIDTH=100% BORDER=0 CELLPADDING=2 CELLSPACING=0 "+o3_fgcolor+" "+o3_fgbackground+" "+o3_height+"><TR><TD VALIGN=TOP><FONT FACE=\""+o3_textfont+"\" COLOR=\""+o3_textcolor+"\" SIZE=\""+o3_textsize+"\">"+text+"</FONT></TD></TR></TABLE></TD></TR></TABLE>"
//	set_background("");
	return txt;
}
////////////////////////////////////////////////////////////////////////////////////
// HANDLING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////

// Displays the popup
function disp(statustext) {
	if ( (ns4) || (ie4) ) {
		if (o3_allowmove == 0) {
			placeLayer();
			showObject(over);
			o3_allowmove = 1;
		}
	}
	
	if (statustext != "") {
		self.status = statustext;
	}
}

// Decides where we want the popup.
function placeLayer() {
	var placeX, placeY;
	
	// HORIZONTAL PLACEMENT
	if (o3_fixx > -1) {
		// Fixed position
		placeX = o3_fixx;
		} else {
		// From mouse
		if (o3_hpos == CENTER) { // Center
			placeX = o3_x+o3_offsetx-(o3_width/2);
		}
		if (o3_hpos == RIGHT) { // Right
			placeX = o3_x+o3_offsetx;
		}
		if (o3_hpos == LEFT) { // Left
			placeX = o3_x-o3_offsetx-o3_width;
		}
		
		// Snapping!
		if (o3_snapx > 1) {
			var snapping = placeX % o3_snapx;
			if (o3_hpos == LEFT) {
				placeX = placeX - (o3_snapx + snapping);
				} else {
				// CENTER and RIGHT
				placeX = placeX + (o3_snapx - snapping);
			}
		}
	}
	
	
	
	// VERTICAL PLACEMENT
	if (o3_fixy > -1) {
		// Fixed position
		placeY = o3_fixy;
		} else {
		// From mouse
		if (o3_aboveheight > 0 && o3_vpos == ABOVE) {
			placeY = o3_y - (o3_aboveheight + o3_offsety);
			} else {
			// BELOW
			placeY = o3_y + o3_offsety;
		}
		
		// Snapping!
		if (o3_snapy > 1) {
			var snapping = placeY % o3_snapy;
			
			if (o3_aboveheight > 0 && o3_vpos == ABOVE) {
				placeY = placeY - (o3_snapy + snapping);
				} else {
				placeY = placeY + (o3_snapy - snapping);
			}
		}
	}
	
	// Actually move the object.
	moveTo(over, placeX, placeY);
}


// Moves the layer
function mouseMove(e) {
	if (ns4) {o3_x=e.pageX; o3_y=e.pageY;}
	if (ie4) {o3_x=event.x; o3_y=event.y;}
	if (ie5) {o3_x=event.x+document.body.scrollLeft; o3_y=event.y+document.body.scrollTop;}
	
	if (o3_allowmove) {
		placeLayer();
	}
}


////////////////////////////////////////////////////////////////////////////////////
// LAYER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////

// Writes to a layer
function layerWrite(txt) {
	if (ns4) {
		var lyr = document.overDiv.document
		lyr.write(txt)
		lyr.close()
	}
	else if (ie4) document.all["overDiv"].innerHTML = txt
}

// Make an object visible
function showObject(obj) {
	if (ns4) obj.visibility = "show"
	else if (ie4) obj.visibility = "visible"
}

// Hides an object
function hideObject(obj) {
	if (ns4) obj.visibility = "hide"
	else if (ie4) obj.visibility = "hidden"
	
	self.status = "";
}

// Move a layer
function moveTo(obj,xL,yL) {
	obj.left = xL
	obj.top = yL
}
