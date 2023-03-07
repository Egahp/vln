/*****************************************************************************
* @file         vln.c
* @brief        a tiny designed readline and libedit replacement
*               specifically for deeply embedded applications
*
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

/* includes ----------------------------------------------------------------*/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "vln.h"

// #pragma GCC push_options
// #pragma GCC optimize("Os")
// #pragma GCC optimize("-fjump-tables")

/* private types -----------------------------------------------------------*/
/* private macro -----------------------------------------------------------*/

#define vln_waitkey(__line, __c)              \
    do {                                      \
        while (0 == (__line)->sget((__c), 1)) \
            ;                                 \
    } while (0)

#if defined(CFG_VLN_DEBUG) && CFG_VLN_DEBUG

#define VLN_PARAM_CHECK(__expr, __ret) \
    do {                               \
        if (!(__expr)) {               \
            return __ret;              \
        }                              \
    } while (0)

#define vln_put(__line, __pbuf, __size, __ret)            \
    do {                                                  \
        uint16_t _size_ = (__size);                       \
        if (_size_ != (__line)->sput((__pbuf), _size_)) { \
            return (__ret);                               \
        }                                                 \
    } while (0)

#else

#define VLN_PARAM_CHECK(__expr, __ret) ((void)0)

#define vln_put(__line, __pbuf, __size, __ret) \
    do {                                       \
        (__line)->sput((__pbuf), (__size));    \
    } while (0)

#endif

/** @defgroup   vln_seqgen_alias seqgen_alias
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/
#define vln_seqgen_ris vln_seqgen_reset
#define vln_seqgen_cuu vln_seqgen_cursor_up
#define vln_seqgen_cud vln_seqgen_cursor_down
#define vln_seqgen_cuf vln_seqgen_cursor_forward
#define vln_seqgen_cub vln_seqgen_cursor_backward
#define vln_seqgen_cnl vln_seqgen_cursor_nextline
#define vln_seqgen_cpl vln_seqgen_cursor_prevline
#define vln_seqgen_cha vln_seqgen_cursor_absolute
#define vln_seqgen_cup vln_seqgen_cursor_position
#define vln_seqgen_ed  vln_seqgen_erase_display
#define vln_seqgen_el  vln_seqgen_erase_line
#define vln_seqgen_il  vln_seqgen_insert_line
#define vln_seqgen_dl  vln_seqgen_delete_line
#define vln_seqgen_dch vln_seqgen_delete_char
#define vln_seqgen_ech vln_seqgen_erase_char
#define vln_seqgen_su  vln_seqgen_scroll_up
#define vln_seqgen_sd  vln_seqgen_scroll_down
#define vln_seqgen_scp vln_seqgen_save_cursor_position
#define vln_seqgen_rcp vln_seqgen_restore_cursor_position
#define vln_seqgen_nsb vln_seqgen_normal_screen
#define vln_seqgen_asb vln_seqgen_alternate_screen
#define vln_seqgen_rss vln_seqgen_report_screen_size
/*---------------------------------------------------------------------------
* @}            vln_seqgen_alias seqgen_alias
----------------------------------------------------------------------------*/

/* private variables -------------------------------------------------------*/
/* private constants -------------------------------------------------------*/
static const uint8_t vtmap[25] = {
    VLN_EXEC_NUL,  /*!< 0           */
    VLN_EXEC_MVHM, /*!< 1  home     */
    VLN_EXEC_NUL,  /*!< 2  insert   */
    VLN_EXEC_DEL,  /*!< 3  delete   */
    VLN_EXEC_MVED, /*!< 4  end      */
    VLN_EXEC_NUL,  /*!< 5  page up  */
    VLN_EXEC_NUL,  /*!< 6  page dn  */
    VLN_EXEC_MVHM, /*!< 7  home     */
    VLN_EXEC_MVED, /*!< 8  end      */
    VLN_EXEC_NUL,  /*!< 9           */
    VLN_EXEC_NUL,  /*!< 10          */
    VLN_EXEC_F1,   /*!< 11 F1       */
    VLN_EXEC_F2,   /*!< 12 F2       */
    VLN_EXEC_F3,   /*!< 13 F3       */
    VLN_EXEC_F4,   /*!< 14 F4       */
    VLN_EXEC_F5,   /*!< 15 F5       */
    VLN_EXEC_NUL,  /*!< 16          */
    VLN_EXEC_F6,   /*!< 17 F6       */
    VLN_EXEC_F7,   /*!< 18 F7       */
    VLN_EXEC_F8,   /*!< 19 F8       */
    VLN_EXEC_F9,   /*!< 20 F9       */
    VLN_EXEC_F10,  /*!< 21 F10      */
    VLN_EXEC_NUL,  /*!< 22          */
    VLN_EXEC_F11,  /*!< 23 F11      */
    VLN_EXEC_F12,  /*!< 24 F12      */
};

static const uint8_t xmap[20] = {
    VLN_EXEC_NUL,  /*!<             */
    VLN_EXEC_PRVH, /*!< A up        */
    VLN_EXEC_NXTH, /*!< B down      */
    VLN_EXEC_MVRT, /*!< C right     */
    VLN_EXEC_MVLT, /*!< D left      */
    VLN_EXEC_NUL,  /*!< E           */
    VLN_EXEC_MVED, /*!< F end       */
    VLN_EXEC_NUL,  /*!< G           */
    VLN_EXEC_MVHM, /*!< H home      */
    VLN_EXEC_NUL,  /*!< I           */
    VLN_EXEC_NUL,  /*!< J           */
    VLN_EXEC_NUL,  /*!< K           */
    VLN_EXEC_NUL,  /*!< L           */
    VLN_EXEC_NUL,  /*!< M           */
    VLN_EXEC_NUL,  /*!< N           */
    VLN_EXEC_NUL,  /*!< O           */
    VLN_EXEC_F1,   /*!< P F1        */
    VLN_EXEC_F2,   /*!< Q F2        */
    VLN_EXEC_F3,   /*!< R F3        */
    VLN_EXEC_F4    /*!< S F4        */
};

static const uint8_t c0map[32] = {
    VLN_EXEC_NUL,  /*!< @ NUL */
    VLN_EXEC_MVHM, /*!< A SOH */
    VLN_EXEC_MVLT, /*!< B STX */
    VLN_EXEC_ALN,  /*!< C ETX */
    VLN_EXEC_DEL,  /*!< D EOT */
    VLN_EXEC_MVED, /*!< E ENQ */
    VLN_EXEC_MVRT, /*!< F ACK */
    VLN_EXEC_ALN,  /*!< G BEL */
    VLN_EXEC_BS,   /*!< H BS  */
    VLN_EXEC_NUL,  /*!< I HT  */
    VLN_EXEC_NLN,  /*!< J LF  */
    VLN_EXEC_DELN, /*!< K VT  */
    VLN_EXEC_CLR,  /*!< L FF  */
    VLN_EXEC_NLN,  /*!< M CR  */
    VLN_EXEC_NXTH, /*!< N SO  */
    VLN_EXEC_NLN,  /*!< O SI  */
    VLN_EXEC_PRVH, /*!< P DLE */
    VLN_EXEC_NUL,  /*!< Q DC1 */
    VLN_EXEC_NUL,  /*!< R DC2 */
    VLN_EXEC_NUL,  /*!< S DC3 */
    VLN_EXEC_NUL,  /*!< T DC4 */
    VLN_EXEC_DHLN, /*!< U NAK */
    VLN_EXEC_NUL,  /*!< V SYN */
    VLN_EXEC_DLWD, /*!< W ETB */
    VLN_EXEC_NUL,  /*!< X CAN */
    VLN_EXEC_NUL,  /*!< Y EM  */
    VLN_EXEC_ALN,  /*!< Z SUB */
    VLN_EXEC_NUL,  /*!< [ ESC */
    VLN_EXEC_NUL,  /*!< \ FS  */
    VLN_EXEC_NUL,  /*!< ] GS  */
    VLN_EXEC_SWNM, /*!< ^ RS  */
    VLN_EXEC_HELP, /*!< - US  */
};
/* private functions prototype ---------------------------------------------*/
/* exported variables ------------------------------------------------------*/
/* exported constants ------------------------------------------------------*/
/* private functions -------------------------------------------------------*/

/*****************************************************************************
* @brief        print decimal integer (0 - 65535)
* 
* @param[in]    buf         print buf
* @param[in]    idx         print idx
* @param[in]    value       16bit unsigned integer
* 
*****************************************************************************/
static void
vln_print_integer(uint8_t *buf, size_t *idx, uint16_t value)
{
    uint8_t *intbuf = (uint8_t[5]){};
    uint8_t *outbuf = buf + *idx;
    size_t count = 0;

    do {
        const uint8_t digit = (uint8_t)(value % 10);
        *intbuf++ = (uint8_t)('0' + digit);
        value /= 10;
        count++;
    } while (value);

    do {
        *outbuf++ = *--intbuf;
        (*idx)++;
    } while (--count);
}

/*****************************************************************************
* @brief        parse decimal integer (0 - 65535)
* 
* @param[in]    buf         parse buf
* @param[out]   value       16bit unsigned integer pointer
* 
*****************************************************************************/
static void vln_parse_integer(uint8_t *buf, uint16_t *value)
{
    *value = 0;

    while (('0' <= *buf) && (*buf <= '9')) {
        *value *= 10;
        *value += *buf - '0';
        buf++;
    }
}

/** @addtogroup vln_seq_generation
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief        Reset to Initial State
*****************************************************************************/
__unused static void vln_seqgen_reset(uint8_t *buf, size_t *idx)
{
    buf[*idx + 0] = '\e';
    buf[*idx + 1] = 'c';
    *idx += 2;
}

/*****************************************************************************
* @brief        output csi prefix
*****************************************************************************/
__unused static void vln_seqgen_csi(uint8_t *buf, size_t *idx)
{
    buf[*idx + 0] = '\e';
    buf[*idx + 1] = '[';
    *idx += 2;
}

/*****************************************************************************
* @brief        output csi 0 pn
*****************************************************************************/
__unused static void vln_seqgen_csi0(uint8_t *buf, size_t *idx, char c)
{
    vln_seqgen_csi(buf, idx);
    buf[(*idx)++] = c;
}

/*****************************************************************************
* @brief        output csi 1 pn
*****************************************************************************/
__unused static void vln_seqgen_csi1(uint8_t *buf, size_t *idx, char c, uint16_t pn)
{
    vln_seqgen_csi(buf, idx);
    vln_print_integer(buf, idx, pn);
    buf[(*idx)++] = c;
}

/*****************************************************************************
* @brief        output csi 2 pn
*****************************************************************************/
__unused static void vln_seqgen_csi2(uint8_t *buf, size_t *idx, char c, uint16_t pn, uint16_t pn2)
{
    vln_seqgen_csi(buf, idx);
    vln_print_integer(buf, idx, pn);
    buf[(*idx)++] = ';';
    vln_print_integer(buf, idx, pn);
    buf[(*idx)++] = c;
}

/*****************************************************************************
* @brief        output csi 3 pn
*****************************************************************************/
__unused static void vln_seqgen_csi3(uint8_t *buf, size_t *idx, char c, uint16_t pn, uint16_t pn2, uint16_t pn3)
{
    vln_seqgen_csi(buf, idx);
    vln_print_integer(buf, idx, pn);
    buf[(*idx)++] = ';';
    vln_print_integer(buf, idx, pn);
    buf[(*idx)++] = ';';
    vln_print_integer(buf, idx, pn);
    buf[(*idx)++] = c;
}

/*****************************************************************************
* @brief        Cursor Up Pn Times (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_cursor_up(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'A', pn);
}

/*****************************************************************************
* @brief        Cursor Down Pn Times (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_cursor_down(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'B', pn);
}

/*****************************************************************************
* @brief        Cursor Forward Pn Times (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_cursor_forward(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'C', pn);
}

/*****************************************************************************
* @brief        Cursor Backward Pn Times (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_cursor_backward(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'D', pn);
}

/*****************************************************************************
* @brief        Cursor Next Line Pn Times (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_cursor_nextline(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'E', pn);
}

/*****************************************************************************
* @brief        Cursor Preceding Line Pn Times (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_cursor_prevline(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'F', pn);
}

/*****************************************************************************
* @brief        Cursor Character Absolute [column] (default = [row,1])
*****************************************************************************/
__unused static void vln_seqgen_cursor_absolute(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'G', pn);
}

/*****************************************************************************
* @brief        Cursor Position [row;column] (default = [1,1])
*****************************************************************************/
__unused static void vln_seqgen_cursor_position(uint8_t *buf, size_t *idx, uint16_t pn1, uint16_t pn2)
{
    vln_seqgen_csi2(buf, idx, 'H', pn1, pn2);
}

/*****************************************************************************
* @brief        Erase in Display
*                   Pn = 0 → Erase Below (default)
*                   Pn = 1 → Erase Above
*                   Pn = 2 → Erase All
*                   Pn = 3 → Erase Saved Lines (xterm)
*****************************************************************************/
__unused static void vln_seqgen_erase_display(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'J', pn);
}

/*****************************************************************************
* @brief        Erase in Line
*                   Pn = 0 → Erase to Right (default)
*                   Pn = 1 → Erase to Left
*                   Pn = 2 → Erase All
*****************************************************************************/
__unused static void vln_seqgen_erase_line(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'K', pn);
}

/*****************************************************************************
* @brief        Insert Pn Line(s) (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_insert_line(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'L', pn);
}

/*****************************************************************************
* @brief        Delete Pn Line(s) (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_delete_line(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'M', pn);
}

/*****************************************************************************
* @brief        Delete Pn Character(s) (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_delete_char(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'P', pn);
}

/*****************************************************************************
* @brief        Erase Pn Character(s) (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_erase_char(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'X', pn);
}

/*****************************************************************************
* @brief        Scroll up Pn lines (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_scroll_up(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'S', pn);
}

/*****************************************************************************
* @brief        Scroll down Pn lines (default = 1)
*****************************************************************************/
__unused static void vln_seqgen_scroll_down(uint8_t *buf, size_t *idx, uint16_t pn)
{
    vln_seqgen_csi1(buf, idx, 'T', pn);
}

/*****************************************************************************
* @brief        Save Current Cursor Position
*****************************************************************************/
__unused static void vln_seqgen_save_cursor_position(uint8_t *buf, size_t *idx)
{
    vln_seqgen_csi0(buf, idx, 's');
}

/*****************************************************************************
* @brief        Restore Saved Cursor Position
*****************************************************************************/
__unused static void vln_seqgen_restore_cursor_position(uint8_t *buf, size_t *idx)
{
    vln_seqgen_csi0(buf, idx, 'u');
}

/*****************************************************************************
* @brief        DEC Private Mode Reset use alternate screen buffer
*****************************************************************************/
__unused static void vln_seqgen_alternate_screen(uint8_t *buf, size_t *idx)
{
    vln_seqgen_csi(buf, idx);
    buf[(*idx) + 0] = '?';
    buf[(*idx) + 1] = '4';
    buf[(*idx) + 2] = '7';
    buf[(*idx) + 3] = 'h';
    (*idx) += 4;
}

/*****************************************************************************
* @brief        DEC Private Mode Set use normal screen buffer
*****************************************************************************/
__unused static void vln_seqgen_normal_screen(uint8_t *buf, size_t *idx)
{
    vln_seqgen_csi(buf, idx);
    buf[(*idx) + 0] = '?';
    buf[(*idx) + 1] = '4';
    buf[(*idx) + 2] = '7';
    buf[(*idx) + 3] = 'l';
    (*idx) += 4;
}

/*****************************************************************************
* @brief        Report the size of the text area in characters 
*               as CSI 8;row;column t
*****************************************************************************/
__unused static void vln_seqgen_report_screen_size(uint8_t *buf, size_t *idx)
{
    vln_seqgen_csi(buf, idx);
    buf[(*idx) + 0] = '1';
    buf[(*idx) + 1] = '8';
    buf[(*idx) + 2] = 't';
    (*idx) += 3;
}

/*---------------------------------------------------------------------------
* @}            vln_seq_generation
----------------------------------------------------------------------------*/

/** @addtogroup vln_edit
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/
/*****************************************************************************
* @brief        refresh line
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_edit_refresh(vln_t *vln)
{
    size_t idx;
    uint8_t seq[16];

    uint16_t curoff;
    uint16_t pptoff;
    uint16_t diff;

    char *linebuff;
    uint16_t linesize;

    linebuff = vln->ln.buff->pbuf;

    if (vln->ln.buff == NULL) {
        linesize = 0;
        curoff = 0;
        pptoff = 0;
    } else {
        linesize = vln->ln.buff->size;
        curoff = vln->ln.curoff;
        pptoff = vln->ln.pptoff;
    }

    if ((pptoff + curoff) >= vln->term.col) {
        diff = pptoff + curoff - vln->term.col + 1;
        linebuff += diff;
        linesize -= diff;
        curoff -= diff;
    }

    if ((pptoff + linesize) > vln->term.col) {
        linesize -= (pptoff + linesize) - vln->term.col;
    }

    /*!< move to prompt end */
    idx = 0;
    vln_seqgen_cursor_absolute(seq, &idx, pptoff + 1);
    vln_put(vln, seq, idx, -1);

    if (!vln->ln.buff->mask) {
        /*!< output linbuff */
        vln_put(vln, linebuff, linesize, -1);
    }

    /*!< erase to end of display and restor cursor */
    idx = 0;
    vln_seqgen_erase_display(seq, &idx, 0);
    vln_seqgen_cursor_absolute(seq, &idx, pptoff + curoff + 1);
    vln_put(vln, seq, idx, -1);

    return 0;
}

/*****************************************************************************
* @brief        clear screen and refresh line
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_edit_clear(vln_t *vln)
{
    vln_put(vln, "\e[2J\e[1;1H", 10, -1);

    /*!< output prompt */
    vln_put(vln, vln->prompt, vln->ln.pptlen, -1);

    return vln_edit_refresh(vln);
}

/*****************************************************************************
* @brief        insert a character
* @retval int               0:Success -1:Error        
*****************************************************************************/
static int vln_edit_insert(vln_t *vln, char c)
{
    if (vln->ln.buff->size < vln->ln.lnmax) {
        if (vln->ln.buff->size != vln->ln.curoff) {
            memmove(vln->ln.buff->pbuf + vln->ln.curoff + 1,
                    vln->ln.buff->pbuf + vln->ln.curoff,
                    vln->ln.buff->size - vln->ln.curoff);
        }

        vln->ln.buff->pbuf[vln->ln.curoff++] = c;
        vln->ln.buff->size++;

        if (vln->ln.buff->size != vln->ln.curoff) {
            return vln_edit_refresh(vln);
        }

        if (vln->ln.buff->mask) {
            c = ' ';
        }
        vln_put(vln, &c, 1, -1);

        return 0;
    }
    return 0;
}

/*****************************************************************************
* @brief        delete cursor left character
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_backspace(vln_t *vln)
{
    if ((vln->ln.curoff > 0) && (vln->ln.buff->size > 0)) {
        memmove(vln->ln.buff->pbuf + vln->ln.curoff - 1,
                vln->ln.buff->pbuf + vln->ln.curoff,
                vln->ln.buff->size - vln->ln.curoff);
        vln->ln.buff->size--;
        vln->ln.curoff--;
        return vln_edit_refresh(vln);
    }
    return 0;
}

/*****************************************************************************
* @brief        delete cursor right character
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_delete(vln_t *vln)
{
    if ((vln->ln.curoff < vln->ln.buff->size) && (vln->ln.buff->size > 0)) {
        memmove(vln->ln.buff->pbuf + vln->ln.curoff,
                vln->ln.buff->pbuf + vln->ln.curoff + 1,
                vln->ln.buff->size - vln->ln.curoff - 1);
        vln->ln.buff->size--;
        return vln_edit_refresh(vln);
    }

    return 0;
}

/*****************************************************************************
* @brief        move cursor to left
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_moveleft(vln_t *vln)
{
    if (vln->ln.curoff > 0) {
        vln->ln.curoff--;
        return vln_edit_refresh(vln);
    }

    return 0;
}

/*****************************************************************************
* @brief        move cursor to right
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_moveright(vln_t *vln)
{
    if (vln->ln.curoff < vln->ln.buff->size) {
        vln->ln.curoff++;
        return vln_edit_refresh(vln);
    }

    return 0;
}

/*****************************************************************************
* @brief        move cursor to start of line
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_movehome(vln_t *vln)
{
    vln->ln.curoff = 0;
    return vln_edit_refresh(vln);
}

/*****************************************************************************
* @brief        move cursor to end of line
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_moveend(vln_t *vln)
{
    vln->ln.curoff = vln->ln.buff->size;
    return vln_edit_refresh(vln);
}

/*****************************************************************************
* @brief        delete the whole line
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_delline(vln_t *vln)
{
    vln->ln.buff->size = 0;
    vln->ln.curoff = 0;
    return vln_edit_refresh(vln);
}

/*****************************************************************************
* @brief        delete from cursor to end of line
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_delend(vln_t *vln)
{
    vln->ln.buff->size = vln->ln.curoff;
    return vln_edit_refresh(vln);
}

/*****************************************************************************
* @brief        delete previosu word
* @retval int               0:Success -1:Error    
*****************************************************************************/
static int vln_edit_delword(vln_t *vln)
{
    uint16_t curoff = vln->ln.curoff;

    while ((curoff > 0) && (vln->ln.buff->pbuf[curoff] == ' ')) {
        curoff--;
    }

    while ((curoff > 0) && (vln->ln.buff->pbuf[curoff] != ' ')) {
        curoff--;
    }

    memmove(vln->ln.buff->pbuf + curoff,
            vln->ln.buff->pbuf + vln->ln.curoff,
            vln->ln.buff->size - vln->ln.curoff + 1);
    vln->ln.buff->size -= vln->ln.curoff - curoff;
    vln->ln.curoff = curoff;

    return vln_edit_refresh(vln);
}

/*---------------------------------------------------------------------------
* @}            vln_edit
----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief        show help
* @retval int               0:Success -1:Error  
*****************************************************************************/
static int vln_help(vln_t *vln)
{
    if (sizeof(CFG_VLN_HELP) - 1) {
        vln_put(vln, CFG_VLN_HELP, sizeof(CFG_VLN_HELP) - 1, -1);
    }
    return 0;
}

/** @addtogroup vln_dispatch
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief        dispatch csi sequences
* 
* @param[in]    vln         
* @param[inout] c           character for dispatch
* @param[in]    pns         params for dispatch
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_dispatch_csi(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    uint8_t idx;

    if (*c == '~') {
        if (pns[0] >= sizeof(vtmap)) {
            pns[0] = 0;
        }

        *c = vtmap[pns[0]];
    } else if (*c == 't') {
        if (pns[0] == 8) {
            vln->term.nsupt = 0;
            vln->term.row = pns[1];
            vln->term.col = pns[2];
            /*!< 1.enter alternate screen buffer <esc>[?47h */
            /*!< 2.clear screen buffer           <esc>[2J   */
            /*!< 3.move cursor to (1,1)          <esc>[1;1H */
            /*!< 4.enter normal screen buffer    <esc>[?47l */
            vln_put(vln, "\e[?47h\e[2J\e[1;1H", 16, -1);
            vln_help(vln);
            vln_put(vln, "\e[?47l", 6, -1);
        }

        *c = VLN_EXEC_NUL;
    } else {
        idx = *c - 0x40;
        if (idx >= sizeof(xmap)) {
            idx = 0;
        }

        *c = xmap[idx];
    }

    return 0;
}

/*****************************************************************************
* @brief        dispatch o sequences
* 
* @param[in]    vln         
* @param[inout] c           character for dispatch
* @param[in]    pns         params for dispatch     
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_dispatch_o(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    (void)vln;
    (void)pns;
    *c = VLN_EXEC_NUL;

    return 0;
}

/*****************************************************************************
* @brief        dispatch alt sequences
* 
* @param[in]    vln         
* @param[inout] c           character for dispatch
* @param[in]    pns         params for dispatch     
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_dispatch_alt(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    (void)vln;
    (void)pns;
    *c = VLN_EXEC_NUL;

    return 0;
}

/*****************************************************************************
* @brief        dispatch ctrl sequences
* 
* @param[in]    vln         
* @param[inout] c           character for dispatch
* @param[in]    pns         params for dispatch     
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_dispatch_ctrl(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    (void)vln;
    (void)c;
    (void)pns;

    *c = c0map[*c];

    return 0;
}

/*---------------------------------------------------------------------------
* @}            vln_dispatch
----------------------------------------------------------------------------*/

/** @addtogroup vln_seq_execute
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief        parse csi sequences
* 
* @param[in]    vln         
* @param[out]   csiend      final csi seq character
* @param[out]   expre       extend prefix character
* @param[out]   pns         array of number param
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_seqexec_pcsi(vln_t *vln, uint8_t *csiend, uint8_t *expre, uint16_t *pns)
{
    uint8_t csibuf[8];
    uint8_t *pbuf = csibuf;

    vln_waitkey(vln, pbuf);

    if (('0' <= *pbuf) && (*pbuf <= '9')) {
        pbuf++;
    } else if ('?' == *pbuf) {
        *expre = *pbuf;
    } else if ((('a' <= *pbuf) && (*pbuf <= 'z')) ||
               (('A' <= *pbuf) && (*pbuf <= 'Z'))) {
        *csiend = *pbuf;
        return 0;
    }

    for (;;) {
        vln_waitkey(vln, pbuf);

        if (('0' <= *pbuf) && (*pbuf <= '9')) {
            pbuf++;
        } else if (*pbuf == ';') {
            pbuf = csibuf;
            vln_parse_integer(csibuf, pns++);
        } else if ((*pbuf == '~') ||
                   (('a' <= *pbuf) && (*pbuf <= 'z')) ||
                   (('A' <= *pbuf) && (*pbuf <= 'Z'))) {
            *csiend = *pbuf;
            vln_parse_integer(csibuf, pns++);
            return 0;
        } else {
            return -1;
        }
    }
}

/*****************************************************************************
* @brief        execute CSI sequences
* 
* @param[in]    vln         
* @param[out]   c           character for dispatch
* @param[out]   pns         array of number param
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_seqexec_csi(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    uint8_t expre = 0;

    if (vln_seqexec_pcsi(vln, c, &expre, pns)) {
        return -1;
    }

    if (expre) {
        *c = VLN_C0_NUL;
    }

    return vln_dispatch_csi(vln, c, pns);
}

/*****************************************************************************
* @brief        execute ALT sequences
* 
* @param[in]    vln         
* @param[inout]   c         character for dispatch
* @param[out]   pns         array of number param
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_seqexec_alt(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    return vln_dispatch_alt(vln, c, pns);
}

/*****************************************************************************
* @brief        execute O sequences
* 
* @param[in]    vln         
* @param[out]   c           character for dispatch
* @param[out]   pns         array of number param
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_seqexec_o(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    vln_waitkey(vln, c);

    return vln_dispatch_o(vln, c, pns);
}

/*****************************************************************************
* @brief        execute sequences
* 
* @param[in]    vln         
* @param[inout] c           character for dispatch
* @param[out]   pns         array of number param
* 
* @retval int               0:Success -1:Error
*****************************************************************************/
static int vln_seqexec(vln_t *vln, uint8_t *c, uint16_t *pns)
{
    vln_waitkey(vln, c);

    switch (*c) {
        case 'O':
            return vln_seqexec_o(vln, c, pns);
        case '[':
            return vln_seqexec_csi(vln, c, pns);
        default:
            return vln_seqexec_alt(vln, c, pns);
    }

    return 0;
}

/*---------------------------------------------------------------------------
* @}            vln_seq_execute
----------------------------------------------------------------------------*/

#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY

/** @addtogroup vln_history
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief        reverse memcpy 4byte align
* 
* @param[in]    dst         
* @param[in]    src         
* @param[in]    n           byte
* 
*****************************************************************************/
static void vln_rvsememcpy4(void *dst, const void *src, size_t n)
{
    uint32_t *pout = dst;
    uint32_t *pin = (uint32_t *)src - 1;

    for (; n > 0; n -= 4) {
        *pout++ = *pin--;
    }
}

/*****************************************************************************
* @brief        align 4 up
* 
* @param[in]    size        size
* 
* @retval                   align size
*****************************************************************************/
static uint32_t vln_align4(uint32_t size)
{
    if (size & 0x3) {
        return (size & ~0x3) + 4;
    } else {
        return size;
    }
}

/*****************************************************************************
* @brief        cache linbuff to history list
* 
* @param[in]    vln         
* 
*****************************************************************************/
static void vln_history_cache(vln_t *vln)
{
    uint16_t unused;
    uint16_t offset;
    uint16_t remain;
    uint16_t lnsize;
    uint8_t *dst;

    /*!< fifo unused size */
    unused = vln->hist.size - (vln->hist.cache - vln->hist.out);

    /*!< line and size total size, alignup 4 */
    lnsize = vln_align4(vln->ln.buff->size + 4);

    /*!< free the oldest history */
    if (lnsize > unused) {
        vln->hist.out += lnsize - unused;
    }

    /*!< fifo write offset */
    offset = vln->hist.cache & vln->hist.mask;

    /*!< unused size before ringloop */
    remain = vln->hist.size - offset;
    remain = remain > lnsize ? lnsize : remain;

    dst = (uint8_t *)(vln->ln.buff) + lnsize;

    vln_rvsememcpy4(vln->hist.pbuf + offset, dst, remain);
    vln_rvsememcpy4(vln->hist.pbuf, dst - remain, lnsize - remain);

    vln->hist.in = vln->hist.cache + lnsize;
}

/*****************************************************************************
* @brief        store linebuff to history list
* 
* @param[in]    vln         
*                
*****************************************************************************/
static void vln_history_store(vln_t *vln)
{
    if (vln->ln.buff->size > 0) {
        vln_history_cache(vln);

        /*!< update cache index */
        vln->hist.cache = vln->hist.in;
    }
}

/*****************************************************************************
* @brief        load special index history to linebuff
* 
* @param[in]    vln         
* @param[in]    index       history index
* 
* @retval                   0:Success -1:Error
*****************************************************************************/
static int vln_history_load(vln_t *vln, uint16_t index)
{
    uint16_t used;
    uint16_t offset;
    uint16_t remain;
    uint16_t lnsize;
    uint16_t histin;
    uint16_t count;
    uint16_t prevused;

    if (vln->hist.index == 0) {
        vln_history_cache(vln);
    }

    lnsize = 0;
    count = index;
    histin = vln->hist.in;
    prevused = vln->hist.in - vln->hist.out;

    do {
        histin -= lnsize;

        /*!< fifo used size */
        used = histin - vln->hist.out;

        if ((used == 0) || (used > prevused)) {
            return 0;
        }

        prevused = used;

        /*!< fifo read offset */
        offset = (histin - 4) & vln->hist.mask;

        /*!< read fisrt 4byte and alignup 4 */
        lnsize = vln_align4(((uint32_t *)(vln->hist.pbuf + offset))[0] + 4);

        if (lnsize > used) {
            return 0;
        }

    } while (count--);

    offset = (histin - lnsize) & vln->hist.mask;

    /*!< unused size before ringloop */
    remain = vln->hist.size - offset;
    remain = remain > lnsize ? lnsize : remain;

    /*!< temp value */
    lnsize = lnsize - remain;

    vln_rvsememcpy4((uint8_t *)(vln->ln.buff) + lnsize,
                    vln->hist.pbuf + offset + remain, remain);

    vln_rvsememcpy4(vln->ln.buff, vln->hist.pbuf + lnsize, lnsize);

    vln->ln.curoff = vln->ln.buff->size;
    vln->hist.index = index;
    return vln_edit_refresh(vln);
}

/*****************************************************************************
* @brief        load prev history to linebuff
* 
* @param[in]    vln         
*
* @retval                   0:Success -1:Error
*****************************************************************************/
static int vln_history_loadprev(vln_t *vln)
{
    uint16_t lnhist = vln->hist.index + 1;

    return vln_history_load(vln, lnhist);
}

/*****************************************************************************
* @brief        load next history to linebuff
* 
* @param[in]    vln         
*
* @retval                   0:Success -1:Error
*****************************************************************************/
static int vln_history_loadnext(vln_t *vln)
{
    uint16_t lnhist = vln->hist.index > 0 ? vln->hist.index - 1 : 0;

    return vln_history_load(vln, lnhist);
}

/*---------------------------------------------------------------------------
* @}            vln_history
----------------------------------------------------------------------------*/
#endif

/*****************************************************************************
* @brief        calculate prompt strlen and cursor offset
*****************************************************************************/
static void vln_calculate_prompt(vln_t *vln)
{
    char c;
    uint8_t flag;

    flag = 0;
    vln->ln.pptlen = 0;
    vln->ln.pptoff = 0;

    while (1) {
        c = vln->prompt[vln->ln.pptlen];

        if (c == '\0') {
            return;
        } else if (c == '\e') {
            flag = 1;
        } else if (flag == 0) {
            vln->ln.pptoff++;
        } else if ((flag == 1) && (c == 'm')) {
            flag = 0;
        }

        vln->ln.pptlen++;
    }
}

/*****************************************************************************
* @brief        wait switch to altscreen
* @retval int               0:Success -1:Error
*****************************************************************************/
__unused static int vln_wait_altscreen(vln_t *vln)
{
    uint32_t c = 0;

    for (;;) {
        vln_waitkey(vln, &c);

        if ((c == VLN_C0_CR) || (c == VLN_C0_LF)) {
            vln_put(vln, "\e[?47h", 6, -1);
            vln->term.alt = 1;
            return 0;
        }
    }
}

/*****************************************************************************
* @brief        readline internal
* 
* @param[in]    vln         
* 
* @retval char*             line pointer
*****************************************************************************/
static char *vln_inernal(vln_t *vln)
{
#if defined(CFG_VLN_XTERM) && CFG_VLN_XTERM
    /*!< wait switch to altscreen */
    if (vln->term.altnsupt == 0) {
        vln_wait_altscreen(vln);
    }
#endif

    /*!< output prompt */
    vln_put(vln, vln->prompt, vln->ln.pptlen, NULL);

    while (1) {
        uint8_t c = 0;
        uint16_t pn[4];

#if defined(CFG_VLN_XTERM) && CFG_VLN_XTERM
        /*!< wait switch to altscreen */
        if (vln->term.altnsupt == 0) {
            vln_wait_altscreen(vln);
        }
#endif

        /*!< get a key */
        vln_waitkey(vln, &c);

        if (c & 0x80) {
            /*!< not support 8bit code */
            continue;
        }

        if (VLN_G0_BEG <= c) {
            if (c <= VLN_G0_END) {
                /*!< printable characters */
                if (vln_edit_insert(vln, c)) {
                    return NULL;
                }
                continue;
            } else {
                /*!< delete */
                c = VLN_EXEC_BS;
            }
        } else if (VLN_C0_ESC == c) {
            if (vln_seqexec(vln, &c, pn)) {
                return NULL;
            }
        } else {
            if (vln_dispatch_ctrl(vln, &c, pn)) {
                return NULL;
            }
        }

        /*!< hanlder control */
        switch (c) {
            /*!< ignore */
            default:
            case VLN_EXEC_NUL:
                break;

            /*!< clear screen and refresh line */
            case VLN_EXEC_CLR:
                if (vln_edit_clear(vln)) {
                    return NULL;
                }
                break;

            /*!< newline */
            case VLN_EXEC_NLN:
#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY
                vln_history_store(vln);
#endif
                return vln->ln.buff->pbuf;

            /*!< abortline */
            case VLN_EXEC_ALN:
                vln->ln.buff->size = 0;
                return vln->ln.buff->pbuf;

            /*!< delete */
            case VLN_EXEC_DEL:
                if (vln_edit_delete(vln)) {
                    return NULL;
                }
                break;

            /*!< backspace */
            case VLN_EXEC_BS:
                if (vln_edit_backspace(vln)) {
                    return NULL;
                }
                break;

            /*!< move right */
            case VLN_EXEC_MVRT:
                if (vln_edit_moveright(vln)) {
                    return NULL;
                }
                break;

            /*!< move left */
            case VLN_EXEC_MVLT:
                if (vln_edit_moveleft(vln)) {
                    return NULL;
                }
                break;

            /*!< move end */
            case VLN_EXEC_MVED:
                if (vln_edit_moveend(vln)) {
                    return NULL;
                }
                break;

            /*!< move home */
            case VLN_EXEC_MVHM:
                if (vln_edit_movehome(vln)) {
                    return NULL;
                }
                break;

            /*!< fetch next history */
            case VLN_EXEC_NXTH:
#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY
                if (vln_history_loadnext(vln)) {
                    return NULL;
                }
#endif
                break;

            /*!< fetch prev history */
            case VLN_EXEC_PRVH:
#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY
                if (vln_history_loadprev(vln)) {
                    return NULL;
                }
#endif
                break;

            /*!< delete prev word */
            case VLN_EXEC_DLWD:
                if (vln_edit_delword(vln)) {
                    return NULL;
                }
                break;

            /*!< delete whole line */
            case VLN_EXEC_DHLN:
                if (vln_edit_delline(vln)) {
                    return NULL;
                }
                break;

            /*!< delete cursor to end */
            case VLN_EXEC_DELN:
                if (vln_edit_delend(vln)) {
                    return NULL;
                }
                break;

            /*!< list completions */
            case VLN_EXEC_SCPT:
                break;

            /*!< auto complete */
            case VLN_EXEC_ACPT:
                break;

            /*!< switch to normal screen */
            case VLN_EXEC_SWNM:
#if defined(CFG_VLN_XTERM) && CFG_VLN_XTERM
                if (vln->term.nsupt == 0) {
                    vln_put(vln, "\e[?47l", 6, NULL);
                    vln->term.alt = 0;
                }
#endif
                break;

            case VLN_EXEC_HELP:
                if (vln_help(vln)) {
                    return NULL;
                }
                vln->ln.buff->size = 0;
                return vln->ln.buff->pbuf;
        }
    }
}

/* exported functions ------------------------------------------------------*/

/*****************************************************************************
* @brief        clear screen and refresh line
*****************************************************************************/
void vln_clear(vln_t *vln)
{
    VLN_PARAM_CHECK(NULL != vln, );

    vln_edit_clear(vln);
}

/*****************************************************************************
* @brief        enable or disable mask mode
*****************************************************************************/
void vln_mask(vln_t *vln, uint8_t mask)
{
    VLN_PARAM_CHECK(NULL != vln, );

    vln->ln.mask = mask ? 1 : 0;

    if (NULL != vln->ln.buff) {
        vln->ln.buff->mask = vln->ln.mask;
        vln_edit_refresh(vln);
    }
}

/*****************************************************************************
* @brief        Fill Character Attributes SGR set to sgrraw
* 
* @param[in]    buf         buffer for fill
* @param[in]    idx         pointer to buffer index
* @param[in]    sgrraw      sgr attributes, 0 to reset
* 
*****************************************************************************/
void vln_sgrset(char *buf, size_t *idx, uint16_t sgrraw)
{
    vln_sgr_t sgr = { .raw = sgrraw };
    vln_seqgen_csi((void *)buf, idx);

    if (sgrraw == 0) {
        buf[(*idx)++] = 'm';
        buf[*idx] = '\0';
        return;
    }

    if (sgr.bold) {
        buf[(*idx) + 0] = '1';
        buf[(*idx) + 1] = ';';
        (*idx) += 2;
    }

    if (sgr.underline) {
        buf[(*idx) + 0] = '4';
        buf[(*idx) + 1] = ';';
        (*idx) += 2;
    }

    if (sgr.blink) {
        buf[(*idx) + 0] = '5';
        buf[(*idx) + 1] = ';';
        (*idx) += 2;
    }

    if (sgr.inverse) {
        buf[(*idx) + 0] = '8';
        buf[(*idx) + 1] = ';';
        (*idx) += 2;
    }

    if (sgr.foreground) {
        buf[(*idx) + 0] = '3';
        buf[(*idx) + 1] = '0' + (sgr.foreground - 1);
        buf[(*idx) + 2] = ';';
        (*idx) += 3;
    }

    if (sgr.background) {
        buf[(*idx) + 0] = '4';
        buf[(*idx) + 1] = '0' + (sgr.background - 1);
        buf[(*idx) + 2] = ';';
        (*idx) += 3;
    }

    buf[(*idx) - 1] = 'm';
    buf[*idx] = '\0';
}

/*****************************************************************************
* @brief        readline
* 
* @param[in]    vln         
* @param[in]    linebuff    linebuff pointer
* @param[in]    buffsize    linebuff size
* @param[in]    linesize    readline size
* 
* @retval char*             line pointer
*****************************************************************************/
char *vln(vln_t *vln, char *linebuff, uint32_t buffsize, uint32_t *linesize)
{
    char *ret;
    VLN_PARAM_CHECK(NULL != vln, NULL);
    VLN_PARAM_CHECK(NULL != linebuff, NULL);
    VLN_PARAM_CHECK(buffsize > 5, NULL);
    VLN_PARAM_CHECK(NULL != linesize, NULL);

    vln->ln.buff = (void *)linebuff;
    vln->ln.buff->size = 0;
    vln->ln.buff->mask = vln->ln.mask;
    vln->ln.lnmax = buffsize - 5; /*!< reserved for \0 and size */
    vln->ln.curoff = 0;

#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY
    vln->hist.index = 0;
#endif

    /*!< calculate prompt offset and size */
    vln_calculate_prompt(vln);

    ret = vln_inernal(vln);

    /*!< new line */
    vln_put(vln, CFG_VLN_NEWLINE, sizeof(CFG_VLN_NEWLINE) ? sizeof(CFG_VLN_NEWLINE) - 1 : 0, NULL);

    *linesize = vln->ln.buff->size;
    vln->ln.buff->pbuf[*linesize] = '\0';
    vln->ln.buff = NULL;

    return ret;
}

/*****************************************************************************
* @brief        init vln
* 
* @param[in]    vln         
* @param[in]    init        init param
* 
* @retval int               0:Success -1:Error              
*****************************************************************************/
int vln_init(vln_t *vln, vln_init_t *init)
{
    VLN_PARAM_CHECK(NULL != vln, -1);
    VLN_PARAM_CHECK(NULL != init, -1);
    VLN_PARAM_CHECK(NULL != init->sget, -1);
    VLN_PARAM_CHECK(NULL != init->sput, -1);
    VLN_PARAM_CHECK(NULL != init->prompt, -1);
    VLN_PARAM_CHECK(NULL != init->history, -1);
    VLN_PARAM_CHECK(!((init->histsize < 2) ||
                      (init->histsize & (init->histsize - 1))),
                    -1);

    vln->prompt = init->prompt;
    vln->sget = init->sget;
    vln->sput = init->sput;

#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY
    vln->hist.pbuf = init->history;
    vln->hist.in = 0;
    vln->hist.out = 0;
    vln->hist.mask = init->histsize - 1;
    vln->hist.size = init->histsize;
    vln->hist.cache = 0;
#endif

    vln->term.nsupt = 1;
    vln->term.alt = 0;
    vln->term.row = CFG_VLN_DEFROW;
    vln->term.col = CFG_VLN_DEFCOL;

    vln->ln.buff = NULL;

    return 0;
}

/*****************************************************************************
* @brief        detects if it is xterm, result will be processed automatically
*               if not xterm it will be blocked on next character
* 
* @param[in]    vln         
*****************************************************************************/
void vln_detect(vln_t *vln)
{
    uint8_t c;
    uint16_t pn[4];

    /*!< test sput and get screen size */
    if (5 != vln->sput("\e[18t", 5)) {
        return;
    }

    vln_waitkey(vln, &c);

    if (c == VLN_C0_ESC) {
        /*!< execute sequence */
        if (vln_seqexec(vln, &c, pn)) {
        }
    }
}

/************************ (C) COPYRIGHT 2021 Egahp **** END OF FILE *********/

// #pragma GCC pop_options