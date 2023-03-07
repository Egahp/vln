/*****************************************************************************
* @file         vln_keycode.h
* @brief        The C0 and C1 control code or control character sets 
*               define control codes for use in text by computer systems 
*               that use ASCII and derivatives of ASCII. The codes 
*               represent additional information about the text, such as 
*               the position of a cursor, an instruction to start a new 
*               line, or a message that the text has been received.
* 
*               see https://en.wikipedia.org/wiki/ANSI_escape_code
*               see https://en.wikipedia.org/wiki/C0_and_C1_control_codes
*               see http://www.xfree86.org/4.8.0/ctlseqs.html#Definitions
*               see https://vt100.net/docs/vt100-ug/chapter3.html#T3-5
* @author       Egahp
* @version      1.0
* @date         2023.02.25
******************************************************************************
* @attention
* 
* <h2><center>&copy; Copyright 2021 Egahp.
* All rights reserved.</center></h2>
* 
* @htmlonly 
* <span style='font-weight: bold'>History</span> 
* @endhtmlonly
* Version|Author|Date|Biref
* ----|----|----|----
* 1.0|Egahp|2023.02.25|Create
*****************************************************************************/

/*!< VT101 User Guide                                                           */
/*!< APPENDIX C                                                                 */
/*!< <1>    Standard Key Character Codes (0x21-0x7E)                            */
/*!< Lowercase      Lowercase Code      Uppercase       Uppercasee Code         */
/*!< a              0x61                A               0x41                    */
/*!< b              0x62                B               0x42                    */
/*!< c              0x63                C               0x43                    */
/*!< d              0x64                D               0x44                    */
/*!< e              0x65                E               0x45                    */
/*!< f              0x66                F               0x46                    */
/*!< g              0x67                G               0x47                    */
/*!< h              0x68                H               0x48                    */
/*!< i              0x69                I               0x49                    */
/*!< j              0x6A                J               0x4A                    */
/*!< k              0x6B                K               0x4B                    */
/*!< l              0x6C                L               0x4C                    */
/*!< m              0x6D                M               0x4D                    */
/*!< n              0x6E                N               0x4E                    */
/*!< o              0x6F                O               0x4F                    */
/*!< p              0x70                P               0x50                    */
/*!< q              0x71                Q               0x51                    */
/*!< r              0x72                R               0x52                    */
/*!< s              0x73                S               0x53                    */
/*!< t              0x74                T               0x54                    */
/*!< u              0x75                U               0x55                    */
/*!< v              0x76                V               0x56                    */
/*!< w              0x77                W               0x57                    */
/*!< x              0x78                X               0x58                    */
/*!< y              0x79                Y               0x59                    */
/*!< z              0x7A                Z               0x5A                    */
/*!< 1              0x31                !               0x21                    */
/*!< 2              0x32                @               0x40                    */
/*!< 3              0x33                #               0x23                    */
/*!< 4              0x34                $               0x24                    */
/*!< 5              0x35                %               0x25                    */
/*!< 6              0x36                ^(circumflex)   0x5E                    */
/*!< 7              0x37                &               0x26                    */
/*!< 8              0x38                *               0x2A                    */
/*!< 9              0x39                (               0x28                    */
/*!< 0              0x30                )               0x29                    */
/*!< -(minus)       0x2D                _               0x5F                    */
/*!< =              0x3D                +               0x2B                    */
/*!< `(grave)       0x60                ~(tilde)        0x7E                    */
/*!< [              0x5B                {               0x7B                    */
/*!< ]              0x5D                }               0x7D                    */
/*!< ;              0x3B                :               0x3A                    */
/*!< '(spostrophe)  0x27                "               0x22                    */
/*!< \              0x5C                |               0x7C                    */
/*!< ,(comma)       0x2C                <               0x3C                    */
/*!< .(period)      0x2E                >               0x3E                    */
/*!< /              0x2F                ?               0x3F                    */
/*!<                                                                            */
/*!< <2>    Cursor Control KeyCodes                                             */
/*!<    Cursor Key      ANSI Mode Reset ANSI Mode Set   VT52 Mode               */
/*!<    ↑   Up          <esc>[A         <esc>OA         <esc>A                  */
/*!<    ↓   Down        <esc>[B         <esc>OB         <esc>B                  */
/*!<    →   Right       <esc>[C         <esc>OC         <esc>C                  */
/*!<    ←   Left        <esc>[D         <esc>OD         <esc>D                  */
/*!<                                                                            */
/*!< <3>    Control Codes Generated (0x00 - 0x1F, 0x7F)                         */
/*!<    Mnemonic        Code Hex        Key with CTRL   Dedicated Key           */
/*!<    NUL             0x00            Spcae Bar                               */
/*!<    SOH             0x01            A                                       */
/*!<    STX             0x02            B                                       */
/*!<    ETX             0x03            C                                       */
/*!<    EOT             0x04            D                                       */
/*!<    ENQ             0x05            E                                       */
/*!<    ACK             0x06            F                                       */
/*!<    BEL             0x07            G                                       */
/*!<    BS              0x08            H               BACK SPACE              */
/*!<    HT              0x09            I               TAB                     */
/*!<    LF              0x0A            J               LINE FEED               */
/*!<    VT              0x0B            K                                       */
/*!<    FF              0x0C            L                                       */
/*!<    CR              0x0D            M               RETURN (ENTER)          */
/*!<    SO              0x0E            N                                       */
/*!<    SI              0x0F            O                                       */
/*!<    DLE             0x10            P                                       */
/*!<    DC1             0x11            Q                                       */
/*!<    DC2             0x12            R                                       */
/*!<    DC3             0x13            S                                       */
/*!<    DC4             0x14            T                                       */
/*!<    NAK             0x15            U                                       */
/*!<    SYN             0x16            V                                       */
/*!<    ETB             0x17            W                                       */
/*!<    CAN             0x18            X                                       */
/*!<    EM              0x19            Y                                       */
/*!<    SUB             0x1A            Z                                       */
/*!<    ESC             0x1B            [                                       */
/*!<    FS              0x1C            \                                       */
/*!<    GS              0x1D            ]                                       */
/*!<    RS              0x1E            ~                                       */
/*!<    US              0x1F            ?                                       */
/*!<    DEL             0x7F                            DELETE                  */
/*!<                                                                            */
/*!< The following control charaters are generated differently from previous    */
/*!<    NUL             0x00            @                                       */
/*!<    RS              0x1E            ^                                       */
/*!<    US              0x1F            - &                                     */
/*!<                                                                            */
/*!< <4>    Auxiliary Keypad Codes                                              */
/*!<    Key     ANSI Numeric    ANSI Alternate  VT52 Numeric    VT52 Alternate  */
/*!<    0       0               <esc>Op         0               <esc>?p         */
/*!<    1       1               <esc>Oq         1               <esc>?q         */
/*!<    2       2               <esc>Or         2               <esc>?r         */
/*!<    3       3               <esc>Os         3               <esc>?s         */
/*!<    4       4               <esc>Ot         4               <esc>?t         */
/*!<    5       5               <esc>Ou         5               <esc>?u         */
/*!<    6       6               <esc>Ov         6               <esc>?v         */
/*!<    7       7               <esc>Ow         7               <esc>?w         */
/*!<    8       8               <esc>Ox         8               <esc>?x         */
/*!<    9       9               <esc>Oy         9               <esc>?y         */
/*!<    -       -               <esc>Om         -               <esc>?m         */
/*!<    ,       ,               <esc>Ol         ,               <esc>?l         */
/*!<    .       .               <esc>On         .               <esc>?n         */
/*!<    ENTER   CR              <esc>OM         CR              <esc>?M         */
/*!<    PF1     <esc>OP         <esc>OP         <esc>P          <esc>?P         */
/*!<    PF2     <esc>OQ         <esc>OQ         <esc>Q          <esc>?Q         */
/*!<    PF3     <esc>OR         <esc>OR         <esc>R          <esc>?R         */
/*!<    PF4     <esc>OS         <esc>OS         <esc>S          <esc>?S         */
/*!<                                                                            */
/*!<                                                                            */
/*!< vt sequences:                                                              */
/*!< <esc>[1~    - Home        <esc>[16~   -             <esc>[31~   - F17      */
/*!< <esc>[2~    - Insert      <esc>[17~   - F6          <esc>[32~   - F18      */
/*!< <esc>[3~    - Delete      <esc>[18~   - F7          <esc>[33~   - F19      */
/*!< <esc>[4~    - End         <esc>[19~   - F8          <esc>[34~   - F20      */
/*!< <esc>[5~    - PgUp        <esc>[20~   - F9          <esc>[35~   -          */
/*!< <esc>[6~    - PgDn        <esc>[21~   - F10                                */
/*!< <esc>[7~    - Home        <esc>[22~   -                                    */
/*!< <esc>[8~    - End         <esc>[23~   - F11                                */
/*!< <esc>[9~    -             <esc>[24~   - F12                                */
/*!< <esc>[10~   - F0          <esc>[25~   - F13                                */
/*!< <esc>[11~   - F1          <esc>[26~   - F14                                */
/*!< <esc>[12~   - F2          <esc>[27~   -                                    */
/*!< <esc>[13~   - F3          <esc>[28~   - F15                                */
/*!< <esc>[14~   - F4          <esc>[29~   - F16                                */
/*!< <esc>[15~   - F5          <esc>[30~   -                                    */
/*!<                                                                            */
/*!< xterm sequences:                                                           */
/*!< <esc>[A     - Up          <esc>[K     -             <esc>[U     -          */
/*!< <esc>[B     - Down        <esc>[L     -             <esc>[V     -          */
/*!< <esc>[C     - Right       <esc>[M     -             <esc>[W     -          */
/*!< <esc>[D     - Left        <esc>[N     -             <esc>[X     -          */
/*!< <esc>[E     -             <esc>[O     -             <esc>[Y     -          */
/*!< <esc>[F     - End         <esc>[P     - F1          <esc>[Z     -          */
/*!< <esc>[G     - Keypad 5    <esc>[Q     - F2                                 */
/*!< <esc>[H     - Home        <esc>[R     - F3                                 */
/*!< <esc>[I     -             <esc>[S     - F4                                 */
/*!< <esc>[J     -             <esc>[T     -                                    */
/*!<                                                                            */
/*!< Host -> Terminal input sequences                                           */
/*!< Pressing special keys on the keyboard, as well as outputting many          */
/*!< xterm CSI, DCS, or OSC sequences, often produces a CSI, DCS, or OSC        */
/*!< sequence,  sent from the terminal to the computer as though the user       */
/*!< typed it.                                                                  */
/*!< When typing input on a terminal keypresses outside the normal main         */
/*!< alphanumeric keyboard area can be sent to the host as ANSI sequences.      */
/*!< For keys that have an equivalent output function, such as the cursor       */
/*!< keys, these often mirror the output sequences. However, for most           */
/*!< keypresses there isn't an equivalent output sequence to use.               */
/*!<                                                                            */
/*!< There are several encoding schemes, and unfortunately most terminals       */
/*!< mix sequences from different schemes, so host software has to be able      */
/*!< to deal  with input sequences using any scheme. To complicate the          */
/*!< matter, the VT  terminals themselves have two schemes of input, normal     */
/*!< mode and application mode that can be switched by the application.         */
/*!<                                                                            */
/*!<                                                                            */
/*!< Pn     ->  Numeric Parameter                                               */
/*!<    A parameter that represents a number, designated by Pn.                 */
/*!< Ps     ->  Selective Parameter                                             */
/*!<    A parameter that selects a subfunction from a specified list of         */
/*!<    subfunctions, designated by Ps. In general, a control sequence with     */
/*!<    more than one selective parameter causes the same effect as several     */
/*!<    control sequences, each with one selective parameter, e.g.,             */
/*!<    CSI Psa; Psb; Psc F is identical to CSI Psa F CSI Psb F CSI Psc F.      */
/*!< Parameter String                                                           */
/*!<    A string of parameters separated by a semicolon -> ;                    */
/*!< Default                                                                    */
/*!<    A function-dependent value that is assumed when no explicit value,      */
/*!<    or a value of 0, is specified.                                          */
/*!< F      ->  Final character                                                 */
/*!<    A character whose bit combination terminates an escape or control       */
/*!<    sequence.                                                               */
/*!<                                                                            */
/*!< <esc> c        ->      RIS Reset to initial State                          */
/*!< <esc> N        ->      SS2 Single Shift Select of G2 Character Set         */
/*!< <esc> O        ->      SS3 Single Shift Select of G3 Character Set         */
/*!< <esc> P        ->      DCS Device Control String                           */
/*!< <esc> [        ->      CSI Control Sequence Introducer                         */
/*!< <esc> \        ->      ST  String Terminator                                       */
/*!< <esc> ]        ->      OSC Operating System Command                                    */
/*!<                                                                                            */
/*!< CSI Ps A       ->      CUU Cursor Up                       Cursor Up Ps Times                  */
/*!< CSI Ps B       ->      CUD Cursor Down                     Cursor Down Ps Times                */
/*!< CSI Ps C       ->      CUF Cursor Forward                  Cursor Forward Ps Times             */
/*!< CSI Ps D       ->      CUB Cursor Back                     Cursor Backward Ps Times            */
/*!< CSI Ps E       ->      CNL Cursor Next Line                Cursor Next Line Ps Times           */
/*!< CSI Ps F       ->      CPL Cursor Previous Line            Cursor Previous Line Ps Times       */
/*!< CSI Ps G       ->      CHA Cursor Horizontal Absolute      Cursor Character Absolute [column]  */
/*!< CSI Ps ; Ps H  ->      CUP Cursor Position                 Cursor Position [row;column]        */
/*!< CSI Ps J       ->      ED Erase in Display                                                     */
/*!<                            Ps = 0 -> Erase Below (default)                                     */
/*!<                            Ps = 1 -> Erase Above                                               */
/*!<                            Ps = 2 -> Erase All                                                 */
/*!<                            Ps = 3 -> Erase Saved Lines (xterm)                                 */
/*!< CSI Ps K       ->      EL Erase in Line                                                        */
/*!<                            Ps = 0 -> Erase to Right (default)                                  */
/*!<                            Ps = 1 -> Erase to Left                                             */
/*!<                            Ps = 2 -> Erase All                                                 */
/*!< CSI Ps L       ->      IL  Insert Line                     Insert Ps Lines                     */
/*!< CSI Ps M       ->      DL  Delete Line                     Delete Ps Lines                     */
/*!< CSI Ps S       ->      SU  Scroll Up                       Scroll up Ps lines                  */
/*!< CSI Ps T       ->      SD  Scroll Down                     Scroll down Ps lines                */
/*!< CSI Ps n       ->      DSR Device Status Report                                                */
/*!<                            Ps = 5 -> Status Report CSI 0 n                                     */
/*!<                            Ps = 6 -> Report Cursor Position [row;column] as CSI r ; c R        */
/*!< CSI s          ->      SCP Save Current Cursor Position                                        */
/*!< CSI u          ->      RCP Restore Saved Cursor Position                                       */
/*!< CSI Ps t       ->      WM  Window manipulation                                                 */
/*!<                            Ps = 18 -> Report the size of the text area in characters           */
/*!<                                as CSI 8 ; height ; width t                                     */
/*!< CSI ? Ps h     ->      DECSET DEC Private Mode Set                                             */
/*!<                            Ps = 47 -> UUse Alternate Screen Buffer                             */
/*!< CSI ? Ps l     ->      DECRST DEC Private Mode Reset                                           */
/*!<                            Ps = 47 -> Use Normal Screen Buffer                                 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __vln_keycode_h__
#define __vln_keycode_h__

/* includes ----------------------------------------------------------------*/

/** @addtogroup vln
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/** @defgroup   vln_macros macros
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*!< <esc>[C   <esc>[PnC   <esc>[Pn;PnC   <esc>[Pn;Pn;PnC   <esc>[Pn;Pn;Pn;PnC  */
/*!< <esc>[?C  <esc>[?PnC  <esc>[?Pn;PnC  <esc>[?Pn;Pn;PnC  <esc>[?Pn;Pn;Pn;PnC */
/*!< <esc>[Pn~                                                                  */
#define VLN_SEQ_CSI(expre, x) ((0x01 << 16))
/*!< <esc>OC                                                                    */
#define VLN_SEQ_O(x)          ()
/*!< <esc>C                                                                     */
#define VLN_SEQ_ALT(x)        ()

#define VLN_CTRL(x)           ((x)-0x40)

/*---------------------------------------------------------------------------
* @}            vln_macros macros
----------------------------------------------------------------------------*/

/** @defgroup   vln_types types
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/
enum {
    /*!< C0 control codes for general use                                       */
    VLN_C0_NUL = 0x00, /*!< Null                      \0                        */
    VLN_C0_SOH = 0x01, /*!< Start of Heading                                    */
    VLN_C0_STX = 0x02, /*!< Start of Text                                       */
    VLN_C0_ETX = 0x03, /*!< End of Text                                         */
    VLN_C0_EOT = 0x04, /*!< End of Transmission                                 */
    VLN_C0_ENQ = 0x05, /*!< Enquiry                                             */
    VLN_C0_ACK = 0x06, /*!< Acknowledge                                         */
    VLN_C0_BEL = 0x07, /*!< Bell, Alert               \a                        */
    VLN_C0_BS = 0x08,  /*!< Backspace                 \b                        */
    VLN_C0_HT = 0x09,  /*!< Horizontal Tabulation     \t                        */
    VLN_C0_LF = 0x0A,  /*!< Line Feed                 \n                        */
    VLN_C0_VT = 0x0B,  /*!< Vertical Tabulation       \v                        */
    VLN_C0_FF = 0x0C,  /*!< Form Feed                 \f                        */
    VLN_C0_CR = 0x0D,  /*!< Carriage Return           \r                        */
    VLN_C0_SO = 0x0E,  /*!< Shift Out                                           */
    VLN_C0_SI = 0x0F,  /*!< Shift In                                            */
    VLN_C0_DLE = 0x10, /*!< Data Link Escape                                    */
    VLN_C0_DC1 = 0x11, /*!< Device Control One        XON                       */
    VLN_C0_DC2 = 0x12, /*!< Device Control Two                                  */
    VLN_C0_DC3 = 0x13, /*!< Device Control Three      XOFF                      */
    VLN_C0_DC4 = 0x14, /*!< Device Control Four                                 */
    VLN_C0_NAK = 0x15, /*!< Negative Acknowledge                                */
    VLN_C0_SYN = 0x16, /*!< Synchronous Idle                                    */
    VLN_C0_ETB = 0x17, /*!< End of Transmission Block                           */
    VLN_C0_CAN = 0x18, /*!< Cancel                                              */
    VLN_C0_EM = 0x19,  /*!< End of medium                                       */
    VLN_C0_SUB = 0x1A, /*!< Substitute                                          */
    VLN_C0_ESC = 0x1B, /*!< Escape                    \e                        */
    VLN_C0_FS = 0x1C,  /*!< File Separator                                      */
    VLN_C0_GS = 0x1D,  /*!< Group Separator                                     */
    VLN_C0_RS = 0x1E,  /*!< Record Separator                                    */
    VLN_C0_US = 0x1F,  /*!< Unit Separator                                      */
    /*!< While not technically part of the C0 control  character range, the     */
    /*!< following two characters are defined in ISO/IEC 2022 as always being   */
    /*!< available regardless of which sets of control characters and graphics  */
    /*!< characters have been registered. They can be thought of as having some */
    /*!< characteristics of control characters.                                 */
    VLN_C0_SP = 0x20,  /*!< Space                                               */
    VLN_G0_BEG = 0x20, /*!< G0 Code Start                                       */
    VLN_G0_END = 0x7E, /*!< G0 Code End                                         */
    VLN_C0_DEL = 0x7F, /*!< Delete                                              */
    /*!< Not support 8bit codes                                                 */
};

enum {
    VLN_EXEC_NUL = 0, /*!< Null                      */
    VLN_EXEC_CLR,     /*!< Clear screen refresh line */
    VLN_EXEC_NLN,     /*!< Newline                   */
    VLN_EXEC_ALN,     /*!< Abortline                 */
    VLN_EXEC_DEL,     /*!< Delete character          */
    VLN_EXEC_BS,      /*!< Backspace character       */
    VLN_EXEC_MVRT,    /*!< Move Right                */
    VLN_EXEC_MVLT,    /*!< Move Left                 */
    VLN_EXEC_MVED,    /*!< Move End                  */
    VLN_EXEC_MVHM,    /*!< Move Home                 */
    VLN_EXEC_NXTH,    /*!< Next History              */
    VLN_EXEC_PRVH,    /*!< Prev History              */
    VLN_EXEC_DLWD,    /*!< Delete prev word */
    VLN_EXEC_DHLN,    /*!< Delete whole line         */
    VLN_EXEC_DELN,    /*!< Delete cursor to end line */
    VLN_EXEC_SCPT,    /*!< List completions          */
    VLN_EXEC_ACPT,    /*!< Autocomplete              */
    VLN_EXEC_SWNM,    /*!< Switch to Normal Screen   */
    VLN_EXEC_HELP,    /*!< Show help                 */
    VLN_EXEC_F1,      /*!< Fucntion 1                */
    VLN_EXEC_F2,      /*!< Fucntion 2                */
    VLN_EXEC_F3,      /*!< Fucntion 3                */
    VLN_EXEC_F4,      /*!< Fucntion 4                */
    VLN_EXEC_F5,      /*!< Fucntion 5                */
    VLN_EXEC_F6,      /*!< Fucntion 6                */
    VLN_EXEC_F7,      /*!< Fucntion 7                */
    VLN_EXEC_F8,      /*!< Fucntion 8                */
    VLN_EXEC_F9,      /*!< Fucntion 9                */
    VLN_EXEC_F10,     /*!< Fucntion 10               */
    VLN_EXEC_F11,     /*!< Fucntion 11               */
    VLN_EXEC_F12,     /*!< Fucntion 12               */
};
/*---------------------------------------------------------------------------
* @}            vln_types types
----------------------------------------------------------------------------*/

/** @defgroup   vln_constants constants
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* @}            vln_constants constants
----------------------------------------------------------------------------*/

/** @defgroup   vln_variables variables
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* @}            vln_variables variables
----------------------------------------------------------------------------*/

/** @defgroup   vln_functions functions
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* @}            vln_functions functions
----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* @}            vln
----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/************************ (C) COPYRIGHT 2021 Egahp **** END OF FILE *********/
