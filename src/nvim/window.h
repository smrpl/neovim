#ifndef NVIM_WINDOW_H
#define NVIM_WINDOW_H

#include <stdbool.h>

#include "nvim/buffer_defs.h"
#include "nvim/mark.h"

// Values for file_name_in_line()
#define FNAME_MESS      1       // give error message
#define FNAME_EXP       2       // expand to path
#define FNAME_HYP       4       // check for hypertext link
#define FNAME_INCL      8       // apply 'includeexpr'
#define FNAME_REL       16      /* ".." and "./" are relative to the (current)
                                   file instead of the current directory */
#define FNAME_UNESC     32      // remove backslashes used for escaping

/*
 * arguments for win_split()
 */
#define WSP_ROOM        1       // require enough room
#define WSP_VERT        2       // split vertically
#define WSP_TOP         4       // window at top-left of shell
#define WSP_BOT         8       // window at bottom-right of shell
#define WSP_HELP        16      // creating the help window
#define WSP_BELOW       32      // put new window below/right
#define WSP_ABOVE       64      // put new window above/left
#define WSP_NEWLOC      128     // don't copy location list

/*
 * Minimum screen size
 */
#define MIN_COLUMNS     12      // minimal columns for screen
#define MIN_LINES       2       // minimal lines for screen

/// Structure used by switch_win() to pass values to restore_win()
typedef struct {
  win_T *sw_curwin;
  tabpage_T *sw_curtab;
  bool sw_same_win;  ///< VIsual_active was not reset
  bool sw_visual_active;
} switchwin_T;

/// Execute a block of code in the context of window `wp` in tabpage `tp`.
/// Ensures the status line is redrawn and cursor position is valid if it is moved.
#define WIN_EXECUTE(wp, tp, block) \
  do { \
    win_T *const wp_ = (wp); \
    const pos_T curpos_ = wp_->w_cursor; \
    switchwin_T switchwin_; \
    if (switch_win_noblock(&switchwin_, wp_, (tp), true) == OK) { \
      check_cursor(); \
      block; \
    } \
    restore_win_noblock(&switchwin_, true); \
    /* Update the status line if the cursor moved. */ \
    if (win_valid(wp_) && !equalpos(curpos_, wp_->w_cursor)) { \
      wp_->w_redr_status = true; \
    } \
    /* In case the command moved the cursor or changed the Visual area, */ \
    /* check it is valid. */ \
    check_cursor(); \
    if (VIsual_active) { \
      check_pos(curbuf, &VIsual); \
    } \
  } while (false)

#ifdef INCLUDE_GENERATED_DECLARATIONS
# include "window.h.generated.h"
#endif
#endif  // NVIM_WINDOW_H
