// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

#include <stdlib.h>

#include "main.h"
#include "utils.h"

#include "editor.h"
#include "buffer.h"
#include "cursor.h"

// ============================================================================================================================================================
// ---> Static functions
// ============================================================================================================================================================

static void	ft_auto_scroll(t_Buffer *buffer, t_Cursor *cursor)
{
	if (cursor->line < buffer->first_line)
	{
		buffer->first_line = cursor->line;
	}
	else if (cursor->line >= buffer->first_line + buffer->nb_showed_lines - 1)
	{
		buffer->first_line = cursor->line - buffer->nb_showed_lines + 1;
	}
}

// ============================================================================================================================================================
// ---> Init
// ============================================================================================================================================================

int		ft_editor_init(t_AppContext *app)
{
	app->editor = malloc(sizeof(t_EditorContext));
	if (NULL == app->editor)
	{
		ft_log_error("malloc");
		exit(EXIT_FAILURE);
	}
	app->editor->buffer = ft_buffer_create();
	app->editor->cursor = ft_cursor_init();

	app->editor->need_update = true;
	return (0);
}

// ============================================================================================================================================================
// ---> Handle event
// ============================================================================================================================================================

void	ft_editor_handle_event(t_AppContext *app, SDL_Event *event)
{
	t_Buffer	*buffer;
	t_Cursor	*cursor;
	int			line;
	int			col;

	buffer = app->editor->buffer;
	cursor = app->editor->cursor;
	line = app->editor->cursor->line;
	col = app->editor->cursor->col;

	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		switch (event->key.key)
		{
			case SDLK_BACKSPACE:
			{
				if (col > 0)
				{
					ft_buffer_delete_char(buffer, line, col);
					ft_cursor_move_left(cursor, buffer);
				}
				else if (line > 0)
				{
					if (buffer->first_line > 0)
					{
						buffer->first_line--;
					}
					ft_cursor_move_to(cursor, line - 1, buffer->lines[line - 1].len);
					ft_buffer_cat_line(buffer, line);
				}
				break;
			}
			case SDLK_RETURN:
			{
				if (col != buffer->lines[line].len)
				{
					ft_buffer_split_line(buffer, line, col);
					ft_cursor_move_to(cursor, line + 1, 0);
				}
				else
				{
					if (line == buffer->nb_lines - 1)
					{
						ft_buffer_insert_empty_line(buffer, -1);
					}
					else
					{
						ft_buffer_insert_empty_line(buffer, line + 1);
					}
					ft_cursor_move_down(cursor, buffer);
				}
				ft_auto_scroll(buffer, cursor);
				break;
			}
			case SDLK_UP:
			{
				ft_cursor_move_up(cursor, buffer);
				ft_auto_scroll(buffer, cursor);
				break;
			}
			case SDLK_DOWN:
			{
				ft_cursor_move_down(cursor, buffer);
				ft_auto_scroll(buffer, cursor);
				break;
			}
			case SDLK_LEFT:
			{
				ft_cursor_move_left(cursor, buffer);
				break;
			}
			case SDLK_RIGHT:
			{
				ft_cursor_move_right(cursor, buffer);
				break;
			}
		}
	}
	else if (event->type == SDL_EVENT_TEXT_INPUT)
	{
		ft_buffer_insert_char(buffer, line, col, utf8_to_codepoint(event->text.text, NULL));
		ft_cursor_move_right(cursor, buffer);
	}
}

// ============================================================================================================================================================
// ---> Update
// ============================================================================================================================================================

void	ft_editor_update(t_AppContext *app, int now)
{
	(void)now;
	t_Buffer	*buffer;
	t_Cursor	*cursor;

	buffer = app->editor->buffer;
	cursor = app->editor->cursor;

	ft_buffer_update_layout(buffer, app->theme.font);
	ft_cursor_update_layout(cursor, buffer);
	ft_cursor_toggle_visibility(cursor, now);
}

// ============================================================================================================================================================
// ---> Render
// ============================================================================================================================================================

void	ft_editor_render(t_AppContext *app)
{
	SDL_FRect		editor_rect;
	SDL_FRect		cursor_rect;
	SDL_FRect		line_counter;
	t_Buffer		*buffer;
	t_Cursor		*cursor;
	int				i;

	buffer = app->editor->buffer;
	cursor = app->editor->cursor;

	ft_set_render_color(app->renderer, app->theme.background);

	editor_rect = (SDL_FRect){TREE_W + (MARGIN * 7), NAVBAR_H + MARGIN, (float)app->width - TREE_W, (float)app->height -NAVBAR_H - 10};
	SDL_RenderFillRect(app->renderer, &editor_rect);

	line_counter = (SDL_FRect){TREE_W + MARGIN, NAVBAR_H + MARGIN, (MARGIN * 4), (float)app->height - NAVBAR_H};
	SDL_RenderFillRect(app->renderer, &line_counter);

	buffer->nb_showed_lines = editor_rect.h / LINE_H;

	if (NULL != app->current_file_path)
	{
		if (true == cursor->visible)
		{
			ft_set_render_color(app->renderer, app->theme.cursor);
			cursor_rect = (SDL_FRect){cursor->x, cursor->y, CURSOR_W, CURSOR_H};
			SDL_RenderFillRect(app->renderer, &cursor_rect);
		}

		ft_buffer_display_layout(app, buffer, editor_rect);

		i = buffer->first_line;
		while (i < buffer->first_line + buffer->nb_showed_lines)
		{
			if (i > buffer->nb_lines - 1)
			{
				break;
			}
			char *txt = ft_putnbr(i + 1);
			float x = line_counter.x + line_counter.w - ft_get_text_metrics(app, txt).w - 5;
			float y = line_counter.y + LINE_H * (i - buffer->first_line);

			ft_draw_text(app, txt, x, y, app->theme.text);
			free(txt);
			i++;
		}
	}
}

// ============================================================================================================================================================
// ---> Clean
// ============================================================================================================================================================

void	ft_editor_cleanup(t_AppContext *app)
{
	ft_buffer_destroy(app->editor->buffer);
	free(app->editor);
}