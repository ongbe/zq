using System;
using System.ComponentModel;
using System.Drawing;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace CurrencyBox
{
    public enum Msg
    {
        // Fields
        WM_CONTEXTMENU = 0x7b,
        WM_COPY = 0x301,
        WM_NULL = 0,
        WM_REFLECT = 0x2000,
        WM_USER = 0x400,
        WM_XPTHEME = 0x31a
    }

    public enum PART_STATES_EDITTEXT
    {
        // Fields
        ASSIST = 7,
        DISABLED = 4,
        FOCUSED = 5,
        HOT = 2,
        NORMAL = 1,
        READONLY = 6,
        SELECTED = 3
    }

    public enum PARTS_EDIT
    {
        // Fields
        CARET = 2,
        EDITTEXT = 1
    }

    public enum Style
    {
        XpStyle,
        Normal
    }

    public struct RECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;

        public RECT(int l, int t, int r, int b)
        {
            left = l;
            top = t;
            right = r;
            bottom = b;
        }

        public RECT(Rectangle r)
        {
            left = r.Left;
            top = r.Top;
            right = r.Right;
            bottom = r.Bottom;
        }

        public Rectangle ToRectangle()
        {
            return new Rectangle(left, top, right, bottom);
        }
    }

    public class Win32Util
    {
        public static void ExtTextOut(IntPtr hdc, int x, int y, Rectangle clip, string str)
        {
            Win32Util.RECT rect1;
            rect1.top = clip.Top;
            rect1.left = clip.Left;
            rect1.bottom = clip.Bottom;
            rect1.right = clip.Right;
            Win32Util.Win32API.ExtTextOut(hdc, x, y, 4, ref rect1, str, str.Length, IntPtr.Zero);
        }

        public static void FillRect(IntPtr hdc, Rectangle clip, Color color)
        {
            Win32Util.RECT rect1;
            rect1.top = clip.Top;
            rect1.left = clip.Left;
            rect1.bottom = clip.Bottom;
            rect1.right = clip.Right;
            int num1 = (((color.B & 0xff) << 0x10) | ((color.G & 0xff) << 8)) | color.R;
            IntPtr ptr1 = Win32Util.Win32API.CreateSolidBrush(num1);
            Win32Util.Win32API.FillRect(hdc, ref rect1, ptr1);
        }

        public static int GET_X_LPARAM(int lParam)
        {
            return (lParam & 0xffff);
        }

        public static int GET_Y_LPARAM(int lParam)
        {
            return (lParam >> 0x10);
        }

        public static Point GetPointFromLPARAM(int lParam)
        {
            return new Point(Win32Util.GET_X_LPARAM(lParam), Win32Util.GET_Y_LPARAM(lParam));
        }

        public static Size GetTextExtent(IntPtr hdc, string str)
        {
            Win32Util.SIZE size1;
            size1.cx = 0;
            size1.cy = 0;
            Win32Util.Win32API.GetTextExtentPoint32(hdc, str, str.Length, ref size1);
            return new Size(size1.cx, size1.cy);
        }

        public static int HIGH_ORDER(int param)
        {
            return (param >> 0x10);
        }

        public static int LOW_ORDER(int param)
        {
            return (param & 0xffff);
        }

        public static void SelectObject(IntPtr hdc, IntPtr handle)
        {
            Win32Util.Win32API.SelectObject(hdc, handle);
        }

        public static void SetBkColor(IntPtr hdc, Color color)
        {
            int num1 = (((color.B & 0xff) << 0x10) | ((color.G & 0xff) << 8)) | color.R;
            Win32Util.Win32API.SetBkColor(hdc, num1);
        }

        public static void SetBkMode(IntPtr hdc, int mode)
        {
            Win32Util.Win32API.SetBkMode(hdc, mode);
        }

        public static void SetTextColor(IntPtr hdc, Color color)
        {
            int num1 = (((color.B & 0xff) << 0x10) | ((color.G & 0xff) << 8)) | color.R;
            Win32Util.Win32API.SetTextColor(hdc, num1);
        }

        private const int ETO_CLIPPED = 4;
        private const int ETO_OPAQUE = 2;
        public const int OPAQUE = 2;
        public const int TRANSPARENT = 1;

        [StructLayout(LayoutKind.Sequential)]
        private struct RECT
        {
            public int left;
            public int top;
            public int right;
            public int bottom;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct SIZE
        {
            public int cx;
            public int cy;
        }

        private class Win32API
        {
            // Methods
            [DllImport("gdi32")]
            internal static extern IntPtr CreateSolidBrush(int crColor);
            [DllImport("gdi32.dll")]
            public static extern int ExtTextOut(IntPtr hdc, int x, int y, int options, ref Win32Util.RECT clip, string str, int len, IntPtr spacings);
            [DllImport("User32.dll", CharSet = CharSet.Auto)]
            internal static extern int FillRect(IntPtr hDC, ref Win32Util.RECT rect, IntPtr hBrush);
            [DllImport("gdi32.dll")]
            public static extern int GetTextExtentPoint32(IntPtr hdc, string str, int len, ref Win32Util.SIZE size);
            [DllImport("gdi32.dll")]
            public static extern int SelectObject(IntPtr hdc, IntPtr hgdiObj);
            [DllImport("gdi32.dll")]
            public static extern int SetBkColor(IntPtr hdc, int color);
            [DllImport("gdi32.dll")]
            public static extern int SetBkMode(IntPtr hdc, int mode);
            [DllImport("gdi32.dll")]
            public static extern int SetTextColor(IntPtr hdc, int color);
        }
    }

    public class XPTheme
    {
        // Methods
        [DllImport("uxtheme.dll")]
        public static extern void CloseThemeData(IntPtr ht);

        [DllImport("uxtheme.dll")]
        public static extern void DrawThemeBackground(IntPtr ht, IntPtr hd, int iPartId, int iStateId, ref RECT rect,
                                                      ref RECT cliprect);

        [DllImport("uxtheme.dll")]
        public static extern void DrawThemeEdge(IntPtr ht, IntPtr hd, int iPartId, int iStateId, ref RECT destrect,
                                                int uedge, int uflags, ref RECT contentrect);

        [DllImport("uxtheme.dll")]
        public static extern void DrawThemeLine(IntPtr ht, IntPtr hd, int iStateId, ref RECT rect, int dwDtlFlags);

        [DllImport("uxtheme.dll")]
        public static extern void DrawThemeParentBackground(IntPtr h, IntPtr hDC, ref RECT rect);

        [DllImport("uxtheme.dll")]
        public static extern void DrawThemeText(IntPtr ht, IntPtr hd, int iPartId, int iStateId,
                                                [MarshalAs(UnmanagedType.LPTStr)] string psztext, int charcount,
                                                int dwtextflags, int dwtextflags2, ref RECT rect);

        [DllImport("uxtheme.dll")]
        public static extern bool IsAppThemed();

        [DllImport("uxtheme.dll")]
        public static extern int IsThemeActive();

        [DllImport("uxtheme.dll")]
        public static extern IntPtr OpenThemeData(IntPtr h, [MarshalAs(UnmanagedType.LPTStr)] string pszClassList);

        [DllImport("uxtheme.dll")]
        public static extern void SetWindowTheme(IntPtr h, [MarshalAs(UnmanagedType.LPTStr)] string pszSubAppName,
                                                 [MarshalAs(UnmanagedType.LPTStr)] string pszSubIdList);
    }       
}
