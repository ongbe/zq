using System;
using System.ComponentModel;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Drawing;

namespace CurrencyBox
{
    [DesignTimeVisible(true), Designer(typeof(CurrencyBox.CurrencyBoxControlDesigner)),DefaultProperty("Text")]
    public class CurrencyBox: UserControl
    {
        #region Propiedades Privadas
        private bool BorraUnaVez = true;
        private bool _esNegativo;
        private bool[] cambioPosision;
        private char caracterDecimal;
        private int agruparNumerosCada;
        private char signoAgrupacion;
        private bool esSOCompatible;
        private bool keyProcesada;
        private bool _permiteNegativos;
        protected IntPtr _tema;
        private int _DigitosDecimales;
        private Color _ColorNegativo;
        private string _Text = "";
        private int nShiftLeft;
        private Point Posicion;
        private string textDisplay;
        private string singo;
        private string textCambiar;
        private char[] x;
        private char[] xz;
        private int xzn;
        private char[] y;
        private char[] yz;

        #endregion

        #region Overrides

        protected override void OnFontChanged(EventArgs e)
        {
            OnResize(EventArgs.Empty);
        }

        protected override void OnResize(EventArgs e)
        {
            base.Height = Font.Height + 7;
            base.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            Rectangle rectangle1 = base.ClientRectangle;
            ControlPaint.DrawBorder3D(pe.Graphics, rectangle1, Border3DStyle.Sunken);
            if ((esSOCompatible && (XPTheme.IsThemeActive() == 1)) &&
                ((_tema == IntPtr.Zero)))
            {
                _tema = XPTheme.OpenThemeData(IntPtr.Zero, "Edit");
            }
            IntPtr ptr1 = Font.ToHfont();
            IntPtr ptr2 = pe.Graphics.GetHdc();
            Win32Util.SetBkMode(ptr2, 1);
            Win32Util.SelectObject(ptr2, ptr1);
            if (!base.Enabled)
            {
                Win32Util.SetTextColor(ptr2, Color.FromName("ControlDarkDark"));
            }
            else if (textDisplay.IndexOf("-") != -1)
            {
                Win32Util.SetTextColor(ptr2, _ColorNegativo);
            }
            else
            {
                Win32Util.SetTextColor(ptr2, ForeColor);
            }
            if (_tema != IntPtr.Zero)
            {
                RECT rect1 = new RECT(base.ClientRectangle);
                IntPtr ptr3 = ptr2;
                XPTheme.DrawThemeBackground(_tema, ptr3, 1, base.Enabled ? 1 : 4, ref rect1, ref rect1);
                Rectangle rectangle2 = base.ClientRectangle;
                rectangle2.Inflate(-1, -1);
                Win32Util.FillRect(ptr2, rectangle2, BackColor);
            }
            Size size1 = Win32Util.GetTextExtent(ptr2, textDisplay);
            Win32Util.ExtTextOut(ptr2, (rectangle1.Width - size1.Width) - nShiftLeft, rectangle1.Y + 3, rectangle1,
                                 textDisplay);
            if (_DigitosDecimales > 0)
            {
                for (int num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                {
                    if (cambioPosision[num1])
                    {
                        Point point1 = new Point();
                        point1.Y = 3;
                        if (num1 < (_DigitosDecimales + 2) && textDisplay.Length > ((_DigitosDecimales + 2) - num1))
                        {
                            string text1 =
                                textDisplay.Substring(textDisplay.Length - ((_DigitosDecimales + 2) - num1),
                                                   (_DigitosDecimales + 2) - num1);
                            Size size2 = Win32Util.GetTextExtent(ptr2, text1);
                            point1.X = (rectangle1.Width - size2.Width) - nShiftLeft;
                        }
                        else
                        {
                            point1.X = rectangle1.Width - nShiftLeft;
                        }
                        Posicion = point1;
                        if (Focused)
                        {
                            SetCaretPos(Posicion.X, Posicion.Y);
                        }
                    }
                }
            }
            else
            {
                Point point2 = new Point();
                point2.Y = 3;
                point2.X = rectangle1.Width - nShiftLeft;
                Posicion = point2;
                if (Focused)
                {
                    SetCaretPos(Posicion.X, Posicion.Y);
                }
            }
            pe.Graphics.ReleaseHdc(ptr2);
            if (textCambiar != textDisplay)
            {
                textCambiar = textDisplay;
                OnTextChanged(EventArgs.Empty);
            }
        }

        protected override void OnGotFocus(EventArgs e)
        {
            if (_DigitosDecimales > 0)
            {
                for (int num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                {
                    cambioPosision[num1] = false;
                }
                cambioPosision[1] = true;
            }
            base.Invalidate();
            Size size1 = new Size(1, Font.Height);
            CreateCaret(base.Handle, IntPtr.Zero, size1.Width, size1.Height);
            SetCaretPos(Posicion.X, Posicion.Y);
            ShowCaret(base.Handle);
        }

        protected override void OnLostFocus(EventArgs e)
        {
            if (textDisplay == (singo + TextoInicial()))
            {
                textDisplay = TextoInicial();
                base.Invalidate();
            }
            HideCaret(base.Handle);
            DestroyCaret();
        }

        #endregion

        #region Importacion de Metodos Externos

        [DllImport("user32.dll")]
        public static extern int CreateCaret(IntPtr hwnd, IntPtr hbm, int cx, int cy);

        [DllImport("user32.dll")]
        public static extern int DestroyCaret();

        [DllImport("user32.dll")]
        public static extern int HideCaret(IntPtr hwnd);

        [DllImport("user32.dll")]
        public static extern int ShowCaret(IntPtr hwnd);

        [DllImport("user32.dll")]
        public static extern int SetCaretPos(int x, int y);

        #endregion

        #region Constructor

        public CurrencyBox()
        {
            this.Posicion = Point.Empty;
            this._tema = IntPtr.Zero;
            this._DigitosDecimales = 2;
            this.x = new char[40];
            this.y = new char[10];
            this.xz = new char[40];
            this.yz = new char[10];
            this.cambioPosision = new bool[12];
            this.textDisplay = "";
            this._Text = "";
            this.agruparNumerosCada = 3;
            this.signoAgrupacion = '.';
            this.caracterDecimal = ',';
            this.singo = "-";
            this._ColorNegativo = Color.Red;
            this.nShiftLeft = 4;
            this.textCambiar = "";
            PlatformID mid1 = Environment.OSVersion.Platform;
            Version version1 = Environment.OSVersion.Version;
            Version version2 = new Version("5.1.2600.0");
            this.esSOCompatible = (version1 >= version2) && (mid1 == PlatformID.Win32NT);
            this.InitializeComponent();
            base.SetStyle(ControlStyles.ResizeRedraw, true);
            base.SetStyle(ControlStyles.FixedHeight, true);
            base.SetStyle(ControlStyles.Selectable, true);
            base.SetStyle(ControlStyles.UserPaint, true);
            base.Height = this.Font.Height + 7;
            base.KeyPress += new KeyPressEventHandler(this.KeyPulsada);
            this.Init();

        }

        #endregion

        #region Propiedades Publicas
        [Category("Categoria"), Browsable(true), Description("Habilita Numeros Negativos"), DefaultValue(false)]
        public bool PermiteNumerosNegativos
        {
            get { return _permiteNegativos; }
            set
            {
                if (_permiteNegativos != value)
                {
                    _permiteNegativos = value;
                    base.Invalidate();
                }
            }
        }

        [Category("Categoria"), DefaultValue("Red"), Browsable(true), Description("Color para Numeros Negativos")]
        public Color ColorNumerosNegativos
        {
            get { return _ColorNegativo; }
            set
            {
                if (_ColorNegativo != value)
                {
                    _ColorNegativo = value;
                    base.Invalidate();
                }
            }
        }

        [Browsable(true), DefaultValue(2), Category("Categoria"), TypeConverter(typeof(DigitosDecimalesConverter)),
            Description("Numero de digitos despues del separador Decimal")]
        public int DigitosDespuesDelSimboloDecimal
        {
            get { return _DigitosDecimales; }
            set
            {
                _DigitosDecimales = value;
                string text1 = textDisplay;
                Init();
                textCambiar = text1;
                base.Invalidate();
            }
        }

        [DefaultValue("0.00"), Description(""), Browsable(true), DesignerSerializationVisibility(DesignerSerializationVisibility.Visible),
           Category("Categoria"), Bindable(true)]
        public override string Text
        {
            get
            {
                return this._Text;
            }
            set
            {
                
                //para que en tiempo de diseño sustituya el combre del control por "0.00"
                if ((value.Length > 10) && (value.Substring(0, 11).ToLower() == "currencybox"))
                {
                    value = "0.00";
                }
                if (this._Text != value)
                {
                    string text1 = "{0:#,##0";
                    
                    if (this._DigitosDecimales > 0)
                    {
                        text1 = text1 + ".";
                        for (int num1 = 0; num1 < this._DigitosDecimales; num1++)
                        {
                            text1 = text1 + "0";
                        }
                    }
                    text1 = text1 + "}";
                    this._Text = String.Format(text1, Convert.ToDecimal(value));
                    this.textDisplay = this._Text;
                    if (this.textDisplay.IndexOf("-") != -1)
                    {
                        this._esNegativo = true;
                    }
                    else
                    {
                        this._esNegativo = false;
                    }
                    base.Invalidate();
                }
                if (this.textCambiar != this.textDisplay)
                {
                    this.textCambiar = this.textDisplay;
                    this.OnTextChanged(EventArgs.Empty);
                }
            }

        }

        #endregion

        #region Metodos

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // CurrencyBox
            // 
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Cursor = System.Windows.Forms.Cursors.Default;
            this.Name = "CurrencyBox";
            this.Size = new System.Drawing.Size(138, 25);
            this.ResumeLayout(false);

        }

        protected void Init()
        {
            NumberFormatInfo info1 = CultureInfo.CurrentCulture.NumberFormat;
            caracterDecimal = info1.NumberDecimalSeparator.ToCharArray()[0];
            signoAgrupacion = info1.NumberGroupSeparator.ToCharArray()[0];
            agruparNumerosCada = info1.NumberGroupSizes[0];
            singo = info1.NegativeSign;
            _esNegativo = false;
            for (int num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
            {
                cambioPosision[num1] = false;
            }
            cambioPosision[1] = true;
            textDisplay = TextoInicial();
            textCambiar = textDisplay;
            _Text = textDisplay;
        }

        protected string TextoInicial()
        {
            string text1 = "";
            if (_DigitosDecimales > 0)
            {
                text1 = "0" + caracterDecimal.ToString();
                for (int num1 = 1; num1 <= _DigitosDecimales; num1++)
                {
                    text1 = text1 + "0";
                }
                return text1;
            }
            return "0";
        }

        protected void MostrarOriginal()
        {
            int num2 = textDisplay.Length - ((_DigitosDecimales > 0) ? (_DigitosDecimales + 1) : 0) - (_esNegativo ? 1 : 0);
            xzn = 0;
            int num1 = 1;
            while (num1 <= num2)
            {
                if ((num1%(agruparNumerosCada + 1)) != 0)
                {
                    xz[num1 - (num1/(agruparNumerosCada + 1))] =
                        textDisplay.Substring((num2 - num1) + (_esNegativo ? 1 : 0), 1).ToCharArray()[0];
                    xzn++;
                }
                num1++;
            }
            if (_DigitosDecimales > 0)
            {
                for (num1 = 1; num1 <= _DigitosDecimales; num1++)
                {
                    yz[num1] = textDisplay.Substring((num2 + num1) + (_esNegativo ? 1 : 0), 1).ToCharArray()[0];
                }
            }
        }

        protected void TextoValidado()
        {
            textDisplay = "";
            int num1 = 1;
            while (num1 <= (xzn + ((xzn - 1)/agruparNumerosCada)))
            {
                if ((num1%(agruparNumerosCada + 1)) == 0)
                {
                    x[num1] = signoAgrupacion;
                }
                else
                {
                    x[num1] = xz[num1 - (num1/(agruparNumerosCada + 1))];
                }
                textDisplay = x[num1].ToString() + textDisplay;
                num1++;
            }
            if (_DigitosDecimales > 0)
            {
                x[0] = caracterDecimal;
                textDisplay = textDisplay + x[0].ToString();
                for (num1 = 1; num1 <= _DigitosDecimales; num1++)
                {
                    y[num1] = yz[num1];
                    textDisplay = textDisplay + y[num1].ToString();
                }
            }
            if (_esNegativo)
            {
                textDisplay = singo + textDisplay;
            }
            _Text = textDisplay;
        }

        protected void CambiarPosicion(Keys k)
        {
            int num1;
            switch (k)
            {
                case Keys.End:
                    for (num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                    {
                        if (cambioPosision[num1])
                        {
                            break;
                        }
                    }
                    if (num1 < (_DigitosDecimales + 2))
                    {
                        cambioPosision[num1] = false;
                        cambioPosision[_DigitosDecimales + 2] = true;
                        base.Invalidate();
                    }
                    return;

                case Keys.Home:
                    for (num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                    {
                        if (cambioPosision[num1])
                        {
                            break;
                        }
                    }
                    if (num1 > 1)
                    {
                        cambioPosision[num1] = false;
                        cambioPosision[1] = true;
                        base.Invalidate();
                    }
                    return;

                case Keys.Left:
                    for (num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                    {
                        if (cambioPosision[num1])
                        {
                            break;
                        }
                    }
                    if (num1 > 1)
                    {
                        cambioPosision[num1] = false;
                        cambioPosision[num1 - 1] = true;
                        base.Invalidate();
                    }
                    return;

                case Keys.Up:
                    return;

                case Keys.Right:
                    for (num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                    {
                        if (cambioPosision[num1])
                        {
                            break;
                        }
                    }
                    if (num1 < (_DigitosDecimales + 2))
                    {
                        cambioPosision[num1] = false;
                        cambioPosision[num1 + 1] = true;
                        base.Invalidate();
                    }
                    return;

                case Keys.Decimal:
                    if (_DigitosDecimales <= 0)
                    {
                        return;
                    }
                    for (num1 = 1; num1 <= (_DigitosDecimales + 2); num1++)
                    {
                        if (cambioPosision[num1])
                        {
                            break;
                        }
                    }
                    break;

                default:
                    return;
            }
            if (num1 > 1)
            {
                cambioPosision[num1] = false;
                cambioPosision[1] = true;
            }
            else
            {
                cambioPosision[1] = false;
                cambioPosision[2] = true;
            }
            base.Invalidate();
        }

        protected void BorrarHaciaLaIzq()
        {
            if (BorraUnaVez)
            {
                if (cambioPosision[1])
                {
                    MostrarOriginal();
                    if ((xzn == 1) && (xz[1] != '0'))
                    {
                        xz[1] = '0';
                        TextoValidado();
                        base.Invalidate();
                    }
                    else if (xzn > 1)
                    {
                        for (int num1 = 1; num1 <= xzn; num1++)
                        {
                            xz[num1] = xz[num1 + 1];
                        }
                        xzn--;
                        TextoValidado();
                        base.Invalidate();
                    }
                }
                else
                {
                    int num2 = 1;
                    while (num2 <= (_DigitosDecimales + 2))
                    {
                        if (cambioPosision[num2])
                        {
                            break;
                        }
                        num2++;
                    }
                    if ((num2 > 2) && (num2 <= (_DigitosDecimales + 2)))
                    {
                        cambioPosision[num2] = false;
                        cambioPosision[num2 - 1] = true;
                        MostrarOriginal();
                        yz[num2 - 2] = '0';
                        TextoValidado();
                        base.Invalidate();
                    }
                    else if (num2 == 2)
                    {
                        cambioPosision[num2] = false;
                        cambioPosision[num2 - 1] = true;
                        base.Invalidate();
                    }
                }
                BorraUnaVez = false;
            }
            else
            {
                BorraUnaVez = true;
            }
        }

        protected void InsertarKey(char k)
        {
            if (cambioPosision[1])
            {
                if (xzn <= 20)
                {
                    MostrarOriginal();
                    if ((xzn == 1) && (xz[1] == '0'))
                    {
                        xz[1] = k;
                    }
                    else
                    {
                        for (int num1 = 1; num1 <= xzn; num1++)
                        {
                            xz[(xzn - num1) + 2] = xz[(xzn - num1) + 1];
                        }
                        xz[1] = k;
                        xzn++;
                    }
                    TextoValidado();
                    base.Invalidate();
                }
            }
            else
            {
                int num2 = 1;
                while (num2 <= (_DigitosDecimales + 2))
                {
                    if (cambioPosision[num2])
                    {
                        break;
                    }
                    num2++;
                }
                if ((num2 > 1) && (num2 < (_DigitosDecimales + 2)))
                {
                    cambioPosision[num2] = false;
                    cambioPosision[num2 + 1] = true;
                    MostrarOriginal();
                    yz[num2 - 1] = k;
                    TextoValidado();
                    base.Invalidate();
                }
            }
        }

        protected override bool IsInputKey(Keys keyData)
        {
            keyProcesada = true;
            switch (keyData)
            {
                case Keys.End:
                    CambiarPosicion(Keys.End);
                    break;

                case Keys.Home:
                    CambiarPosicion(Keys.Home);
                    break;

                case Keys.Left:
                    CambiarPosicion(Keys.Left);
                    return true;

                case Keys.Up:
                    CambiarPosicion(Keys.Left);
                    return true;

                case Keys.Right:
                    CambiarPosicion(Keys.Right);
                    return true;

                case Keys.Down:
                    CambiarPosicion(Keys.Right);
                    return true;

                case Keys.Delete:
                    {
                        string text1 = textDisplay;
                        Init();
                        textCambiar = text1;
                        base.Invalidate();
                        break;
                    }
                case Keys.Add:
                    if (_permiteNegativos)
                    {
                        CambiodeSigno(Keys.Add);
                        base.Invalidate();
                    }
                    break;

                case Keys.Subtract:
                case Keys.OemMinus:
                    if (_permiteNegativos)
                    {
                        CambiodeSigno(Keys.Subtract);
                        base.Invalidate();
                    }
                    break;

                case Keys.Back:
                    BorrarHaciaLaIzq();
                    break;

                case Keys.Escape:
                    break;

                default:
                    if (((Keys.Shift & keyData) != Keys.None) && ((Keys.Oemplus & keyData) == Keys.Oemplus))
                    {
                        if (_permiteNegativos)
                        {
                            CambiodeSigno(Keys.Add);
                            base.Invalidate();
                        }
                    }
                    else
                    {
                        if ((Keys.Control & keyData) != Keys.None)
                        {
                            return true;
                        }
                        if ((Keys.Alt & keyData) != Keys.None)
                        {
                            return true;
                        }
                        keyProcesada = false;
                    }
                    break;
            }
            return base.IsInputKey(keyData);
        }

        protected void CambiodeSigno(Keys k)
        {
            if (k == Keys.Add)
            {
                if (_esNegativo)
                {
                    MostrarOriginal();
                    _esNegativo = false;
                    TextoValidado();
                    base.Invalidate();
                }
            }
            else if ((k == Keys.Subtract) && !_esNegativo)
            {
                MostrarOriginal();
                _esNegativo = true;
                TextoValidado();
                base.Invalidate();
            }
        }

        protected override void WndProc(ref Message m)
        {
            bool flag1 = true;
            switch (m.Msg)
            {
                case 0x7b:
                    {
                        Point point1 = Win32Util.GetPointFromLPARAM((int) m.LParam);
                        point1 = base.PointToClient(point1);
                        break;
                    }
                case 0x31a:
                    if (_tema != IntPtr.Zero)
                    {
                        XPTheme.CloseThemeData(_tema);
                        _tema = IntPtr.Zero;
                    }
                    base.Invalidate();
                    break;
            }
            if (flag1)
            {
                base.WndProc(ref m);
            }
        }

        private void KeyPulsada(object sender, KeyPressEventArgs e)
        {
            if (!keyProcesada)
            {
                switch (e.KeyChar)
                {
                    case ',':
                    case '.':
                        CambiarPosicion(Keys.Decimal);
                        return;

                    case '-':
                    case '/':
                        return;

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        InsertarKey(e.KeyChar);
                        return;
                }
            }
        }

        #endregion

        #region Clases

        public class DigitosDecimalesConverter : Int16Converter
        {
            public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
            {
                return new StandardValuesCollection(new int[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
            }

            public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
            {
                return false;
            }

            public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
            {
                return true;
            }
        }

        public class CurrencyBoxControlDesigner : ControlDesigner
        {
            public override SelectionRules SelectionRules
            {
                get { return (base.SelectionRules & ~ (SelectionRules.BottomSizeable | SelectionRules.TopSizeable)); }
            }
        }

        #endregion
    }
}