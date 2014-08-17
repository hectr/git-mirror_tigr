// TIGR - TIny GRaphics Library
//        ^^   ^^
//
// rawr.


// TODO:
// save png

#ifndef __TIGR_H__
#define __TIGR_H__

#ifdef __cplusplus
extern "C" {
#endif

// Compiler configuration.
#ifdef _MSC_VER
#define TIGR_INLINE static __forceinline
#else
#define TIGR_INLINE static inline
#endif

// Bitmaps ----------------------------------------------------------------

// This struct contains one pixel.
typedef struct {
	unsigned char b, g, r, a;
} TPixel;

// A Tigr bitmap.
typedef struct Tigr {
	int w, h;		// width/height (unscaled)
	TPixel *pix;	// pixel data
	void *handle;	// OS window handle, NULL for off-screen bitmaps.
} Tigr;

// Creates a new empty window. (title is UTF-8)
Tigr *tigrWindow(int w, int h, const char *title, int flags);

// Creates an empty off-screen bitmap.
Tigr *tigrBitmap(int w, int h);

// Deletes a window/bitmap.
void tigrFree(Tigr *bmp);

// Returns non-zero if the user requested to close a window.
int tigrClosed(Tigr *bmp);

// Displays a window's contents on-screen.
void tigrUpdate(Tigr *bmp);


// Drawing ----------------------------------------------------------------

// Helper for reading/writing pixels.
// For high performance, just write to bmp->pix yourself.
TPixel tigrGet(Tigr *bmp, int x, int y);
void tigrPlot(Tigr *bmp, int x, int y, TPixel pix);

// Clears a bitmap to a color.
void tigrClear(Tigr *bmp, TPixel color);

// Fills in a solid rectangle.
void tigrFill(Tigr *bmp, int x, int y, int w, int h, TPixel color);

// Draws a line.
void tigrLine(Tigr *bmp, int x0, int y0, int x1, int y1, TPixel color);

// Copies bitmap data.
// dx/dy = dest co-ordinates
// sx/sy = source co-ordinates
// w/h   = width/height
void tigrBlit(Tigr *dest, Tigr *src, int dx, int dy, int sx, int sy, int w, int h);

// Same as tigrBlit, but blends with the bitmap alpha channel,
// and uses the 'alpha' variable to fade out.
void tigrBlitAlpha(Tigr *dest, Tigr *src, int dx, int dy, int sx, int sy, int w, int h, float alpha);

// Same as tigrBlit, but tints the source bitmap with a color.
void tigrBlitTint(Tigr *dest, Tigr *src, int dx, int dy, int sx, int sy, int w, int h, TPixel tint);

// Helper for making colors.
TIGR_INLINE TPixel tigrRGB(unsigned char r, unsigned char g, unsigned char b)
{
	TPixel p; p.r = r; p.g = g; p.b = b; p.a = 0xff; return p;
}

// Helper for making colors.
TIGR_INLINE TPixel tigrRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	TPixel p; p.r = r; p.g = g; p.b = b; p.a = a; return p;
}


// Font printing ----------------------------------------------------------

typedef struct {
	int code, x, y, w, h;
} TigrGlyph;

typedef struct {
	Tigr *bitmap;
	int numGlyphs;
	TigrGlyph *glyphs;
} TigrFont;

// Loads a font. The font bitmap should contain all characters 
// for the given codepage, excluding the first 32 control codes.
// Supported codepages:
//     0    - Regular 7-bit ASCII
//     1252 - Windows 1252
TigrFont *tigrLoadFont(Tigr *bitmap, int codepage);

// Frees a font.
void tigrFreeFont(TigrFont *font);

// Prints UTF-8 text onto a bitmap.
void tigrPrint(Tigr *dest, TigrFont *font, int x, int y, TPixel color, const char *text, ...);

// Returns the width/height of a string.
int tigrTextWidth(TigrFont *font, const char *text);
int tigrTextHeight(TigrFont *font, const char *text);

// The built-in font.
extern TigrFont *tfont;


// User Input -------------------------------------------------------------

// Key scancodes. For letters/numbers, use ASCII ('A'-'Z' and '0'-'9').
typedef enum {
	TK_PAD0,TK_PAD1,TK_PAD2,TK_PAD3,TK_PAD4,TK_PAD5,TK_PAD6,TK_PAD7,TK_PAD8,TK_PAD9,
	TK_PADMUL,TK_PADADD,TK_PADENTER,TK_PADSUB,TK_PADDOT,TK_PADDIV,
	TK_F1,TK_F2,TK_F3,TK_F4,TK_F5,TK_F6,TK_F7,TK_F8,TK_F9,TK_F10,TK_F11,TK_F12,
	TK_BACKSPACE,TK_TAB,TK_RETURN,TK_SHIFT,TK_CONTROL,TK_ALT,TK_PAUSE,TK_CAPSLOCK,
	TK_ESCAPE,TK_SPACE,TK_PAGEUP,TK_PAGEDN,TK_END,TK_HOME,TK_LEFT,TK_UP,TK_RIGHT,TK_DOWN,
	TK_INSERT,TK_DELETE,TK_LWIN,TK_RWIN,TK_APPS,TK_NUMLOCK,TK_SCROLL,TK_LSHIFT,TK_RSHIFT,
	TK_LCONTROL,TK_RCONTROL,TK_LALT,TK_RALT,TK_SEMICOLON,TK_EQUALS,TK_COMMA,TK_MINUS,
	TK_DOT,TK_SLASH,TK_BACKTICK,TK_LSQUARE,TK_BACKSLASH,TK_RSQUARE,TK_TICK,
} TKey;

// Returns mouse input for a window.
void tigrMouse(Tigr *bmp, int *x, int *y, int *buttons);

// Reads the keyboard for a window.
// Returns non-zero if a key is pressed/held.
// tigrKeyDown tests for the initial press, tigrKeyHeld repeats each frame.
int tigrKeyDown(Tigr *bmp, int key);
int tigrKeyHeld(Tigr *bmp, int key);

// Reads character input for a window.
// Returns the Unicode value of the last key pressed, or 0 if none.
int tigrReadChar(Tigr *bmp);


// Bitmap I/O -------------------------------------------------------------

// Loads a PNG, from either a file or memory. (fileName is UTF-8)
// On error, returns NULL and sets errno.
Tigr *tigrLoadImage(const char *fileName);
Tigr *tigrLoadImageMem(const void *data, int length);


// Helpers ----------------------------------------------------------------

// Returns the amount of time elapsed since tigrTime was last called,
// or zero on the first call.
float tigrTime();

// Displays an error message and quits. (UTF-8)
// 'bmp' can be NULL.
void tigrError(Tigr *bmp, const char *message, ...);

// Reads an entire file into memory. (fileName is UTF-8)
// Free it yourself after with 'free'.
// On error, returns NULL and sets errno.
// TIGR will automatically append a NUL terminator byte
// to the end (not included in the length)
void *tigrReadFile(const char *fileName, int *length);

// Decompresses DEFLATEd zip/zlib data into a buffer.
// Returns non-zero on success.
int tigrInflate(void *out, unsigned outlen, const void *in, unsigned inlen);

// Decodes a single UTF8 codepoint and returns the next pointer.
const char *tigrDecodeUTF8(const char *text, int *cp);

// Encodes a single UTF8 codepoint and returns the next pointer.
char *tigrEncodeUTF8(char *text, int cp);

#ifdef __cplusplus
}
#endif
	
#endif // __TIGR_H__