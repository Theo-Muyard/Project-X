// ============================================================================================================================================================
// ---> Cursor.h - Header
// ============================================================================================================================================================

#ifndef FT_EDITOR_CURSOR_H
# define FT_EDITOR_CURSOR_H

// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================
# include <SDL3/SDL.h>

// ============================================================================================================================================================
// ---> Types
// ============================================================================================================================================================

typedef struct s_Buffer t_Buffer;

typedef struct	s_Cursor 
{
	int		line;
	int		col;

	int		x;
	int		y;

	bool	visible;
	Uint32	last_toggle;
	Uint32	last_action;
} t_Cursor;

// ============================================================================================================================================================
// ---> Functions
// ============================================================================================================================================================

/**
 * @brief Initialize the cursor module so that it can be used.
 * @return The cursor.
 */
t_Cursor	*ft_cursor_init(void);

/**
 * @brief Move the cursor to a given position.
 * @param cursor The cursor.
 * @param line The line (y) of the new position.
 * @param col The column (x) of the new position.
 */
void		ft_cursor_move_to(t_Cursor *cursor, int line, int col);

/**
 * @brief Move the cursor position to right.
 * @param cursor The cursor.
 * @param buffer The app buffer (editor).
 */
void		ft_cursor_move_right(t_Cursor *cursor, t_Buffer *buffer);

/**
 * @brief Move the cursor position to left.
 * @param cursor The cursor.
 * @param buffer The app buffer (editor).
 * 
 */
void		ft_cursor_move_left(t_Cursor *cursor, t_Buffer *buffer);

/**
 * @brief Move the cursor position to up.
 * @param cursor The cursor.
 * @param buffer The app buffer (editor).
 * 
 */
void		ft_cursor_move_up(t_Cursor *cursor, t_Buffer *buffer);

/**
 * @brief Move the cursor position to down.
 * @param cursor The cursor.
 * @param buffer The app buffer (editor).
 */
void		ft_cursor_move_down(t_Cursor *cursor, t_Buffer *buffer);

/**
 * @brief Toggle the cursor visibility.
 * @param cursor The cursor.
 * @param current_time The current time.
 */
void		ft_cursor_toggle_visibility(t_Cursor *cursor, Uint32 current_time);

/**
 * @brief Display the cursor in action.
 * @param cursor The cursor.
 * @param current_time The current time.
 */
void		ft_cursor_show_in_action(t_Cursor *cursor, Uint32 current_time);


/**
 * @brief Update the cursor position.
 * @param cursor The cursor.
 * @param buffer The app buffer (editor).
 */
void		ft_cursor_update_layout(t_Cursor *cursor, t_Buffer *buffer);

#endif