// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

#include <stdlib.h>

#include "main.h"
#include "utils.h"

#include "cursor.h"
#include "buffer.h"

// ============================================================================================================================================================
// ---> Init
// ============================================================================================================================================================

t_Cursor	*ft_cursor_init(void)
{
	t_Cursor	*cursor;

	cursor = malloc(sizeof(t_Cursor));
	
	if (NULL == cursor)
	{
		ft_log_error("maloc");
		exit(EXIT_FAILURE);
	}
	*cursor = (t_Cursor)
	{
		.col = 0,
		.line = 0,
		.x = TREE_W + (MARGIN * 5),
		.y = NAVBAR_H + MARGIN,
		.last_toggle = 0,
		.visible = true
	};

	return (cursor);
}

// ============================================================================================================================================================
// ---> Cursor functions
// ============================================================================================================================================================

void		ft_cursor_move_to(t_Cursor *cursor, int line, int col)
{

	cursor->line = line;
	cursor->col = col;

	ft_cursor_show_in_action(cursor, SDL_GetTicks());
}


void		ft_cursor_move_right(t_Cursor *cursor, t_Buffer *buffer)
{
	if ((cursor->col + 1) <= buffer->lines[cursor->line].len)
	{
		cursor->col++;
	}
	else if ((cursor->line + 1) < buffer->nb_lines)
	{
		cursor->line++;
		cursor->col = 0;
	}

	ft_cursor_show_in_action(cursor, SDL_GetTicks());
}


void		ft_cursor_move_left(t_Cursor *cursor, t_Buffer *buffer)
{
	if ((cursor->col) > 0)
	{
		cursor->col--;
	}
	else if ((cursor->line) > 0)
	{
		cursor->line--;
		cursor->col = buffer->lines[cursor->line].len;
	}

	ft_cursor_show_in_action(cursor, SDL_GetTicks());
}


void		ft_cursor_move_up(t_Cursor *cursor, t_Buffer *buffer)
{
	if ((cursor->line) > 0)
	{
		cursor->line--;
		if (cursor->col > buffer->lines[cursor->line].len)
		{
			cursor->col = buffer->lines[cursor->line].len;
		}
	}

	ft_cursor_show_in_action(cursor, SDL_GetTicks());
}


void		ft_cursor_move_down(t_Cursor *cursor, t_Buffer *buffer)
{
	if (cursor->line + 1 < buffer->nb_lines)
	{
		cursor->line++;
		if (cursor->col > buffer->lines[cursor->line].len)
		{
			cursor->col = buffer->lines[cursor->line].len;
		}
	}

	ft_cursor_show_in_action(cursor, SDL_GetTicks());
}


void		ft_cursor_toggle_visibility(t_Cursor *cursor, Uint32 current_time)
{
	if ((current_time - cursor->last_action) < 500)
	{
		cursor->visible = true;
	}
	else if ((current_time - cursor->last_toggle) > 500)
	{
		cursor->visible = !cursor->visible;
		cursor->last_toggle = current_time;
	}
}


void		ft_cursor_show_in_action(t_Cursor *cursor, Uint32 current_time)
{
	cursor->visible = true;
	cursor->last_action = current_time;
}


void		ft_cursor_update_layout(t_Cursor *cursor, t_Buffer *buffer)
{
	int			i;

	cursor->x = TREE_W + (MARGIN * 7);
	i = 0;
	while (i < cursor->col)
	{
		cursor->x += buffer->lines[cursor->line].chars[i].w;
		++i;
	}

	cursor->y = NAVBAR_H + MARGIN + ((cursor->line - buffer->first_line) * LINE_H);
}