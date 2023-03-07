/*****************************************************************************
* @file         vln.h
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __vln_h__
#define __vln_h__

/* includes ----------------------------------------------------------------*/
#include <stdint.h>

#include "vln_keycode.h"

/** @addtogroup vln
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/

/** @defgroup   vln_macros macros
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/
#ifndef CFG_VLN_DEBUG
#define CFG_VLN_DEBUG 0
#endif

#ifndef CFG_VLN_DEFROW
#define CFG_VLN_DEFROW 20
#endif

#ifndef CFG_VLN_DEFCOL
#define CFG_VLN_DEFCOL 80
#endif

#ifndef CFG_VLN_HISTORY
#define CFG_VLN_HISTORY 1
#endif

#ifndef CFG_VLN_XTERM
#define CFG_VLN_XTERM 0
#endif

#ifndef CFG_VLN_NEWLINE
#define CFG_VLN_NEWLINE "\r\n"
#endif

#ifndef CFG_VLN_HELP
#define CFG_VLN_HELP                                            \
    "\r\n"                                                      \
    "\t+-------------------------------------+\r\n"             \
    "\t|           \e[1;34mVector Read Line\e[m          |\r\n" \
    "\t|                                     |\r\n"             \
    "\t| -> CTRL + ^  return normal screen   |\r\n"             \
    "\t| -> CTRL + L  clear screen           |\r\n"             \
    "\t| -> ENTER     end of line            |\r\n"             \
    "\t| -> TAB       auto complete          |\r\n"             \
    "\t| -> UP        fetch prev history     |\r\n"             \
    "\t| -> DOWN      fetch next history     |\r\n"             \
    "\t+-------------------------------------+\r\n"
#endif

/*---------------------------------------------------------------------------
* @}            vln_macros macros
----------------------------------------------------------------------------*/

/** @defgroup   vln_types types
-----------------------------------------------------------------------------
* @{
----------------------------------------------------------------------------*/
enum {
    VLN_SGR_NONE = 0,
    VLN_SGR_BLACK = 1,
    VLN_SGR_RED = 2,
    VLN_SGR_GREEN = 3,
    VLN_SGR_YELLOW = 4,
    VLN_SGR_BLUE = 5,
    VLN_SGR_MAGENTA = 6,
    VLN_SGR_CYAN = 7,
    VLN_SGR_WHITE = 8,
    VLN_SGR_DEFAULT = 10
};

typedef struct
{
    char *prompt; /*!< prompt pointer */
    uint16_t (*sput)(void *, uint16_t);
    uint16_t (*sget)(void *, uint16_t);

    struct
    {
        struct {
            uint16_t size; /*!< linesize         */
            uint16_t mask; /*!< now linemask     */
            char pbuf[0];  /*!< linebuff pointer */
        } *buff;

        uint16_t pptoff; /*!< prompt offset */
        uint16_t pptlen; /*!< prompt strlen */
        uint16_t lnmax;  /*!< linebuff max  */
        uint16_t curoff; /*!< cursor offset */
        uint16_t mask;   /*!< line mask     */
    } ln;

    struct
    {
        union {
            uint16_t altnsupt; /*!< is screen not supported but alt buffer */
            struct {
                uint8_t nsupt; /*!< is screen not supported */
                uint8_t alt;   /*!< is screen alt buffer */
            };
        };
        uint16_t row; /*!< terminal row     */
        uint16_t col; /*!< terminal column  */
    } term;

#if defined(CFG_VLN_HISTORY) && CFG_VLN_HISTORY
    struct {
        char *pbuf;     /*!< histfifo buffer  */
        uint16_t in;    /*!< histfifo in pos  */
        uint16_t out;   /*!< histfifo out pos */
        uint16_t mask;  /*!< histfifo mask    */
        uint16_t size;  /*!< histfifo size    */
        uint16_t cache; /*!< history cache    */
        uint16_t index; /*!< history index    */
    } hist;
#endif

} vln_t;

typedef struct {
    char *prompt;      /*!< prompt buffer pointer */
    char *history;     /*!< history buffer pointer */
    uint32_t histsize; /*!< history buffer size, must be a power of 2 */
    uint16_t (*sput)(void *, uint16_t);
    uint16_t (*sget)(void *, uint16_t);
} vln_init_t;

typedef struct
{
    union {
        uint16_t raw;
        struct
        {
            uint16_t foreground : 6;
            uint16_t bold       : 1;
            uint16_t underline  : 1;
            uint16_t background : 6;
            uint16_t blink      : 1;
            uint16_t inverse    : 1;
        };
    };
} vln_sgr_t;
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
extern int vln_init(vln_t *vln, vln_init_t *init);
extern char *vln(vln_t *vln, char *linebuff, uint32_t buffsize, uint32_t *linesize);
extern char *vln_isr(vln_t *vln, char *linebuff, uint32_t buffsize, uint32_t *linesize);
extern void vln_detect(vln_t *vln);
extern void vln_sgrset(char *buf, size_t *idx, uint16_t sgrraw);
extern void vln_clear(vln_t *vln);
extern void vln_mask(vln_t *vln, uint8_t mask);
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
