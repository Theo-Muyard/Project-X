// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

#include <stdlib.h>

#include "main.h"
#include "utils.h"

#include "buffer.h"

// ============================================================================================================================================================
// ---> Functions
// ============================================================================================================================================================

t_Buffer	*ft_buffer_create(void)
{
	t_Buffer	*buffer;

	buffer = malloc(sizeof(t_Buffer));
	if (NULL == buffer)
	{
		ft_log_error("maloc");
		exit(EXIT_FAILURE);
	}

	buffer->nb_lines = 1;
	buffer->lines = malloc(buffer->nb_lines * sizeof(t_Line));
	if (NULL == buffer->lines)
	{
		ft_log_error("malloc");
		free(buffer);
		exit(EXIT_FAILURE);
	}

	buffer->first_line = 0;
	buffer->lines[0].len = 0;
	buffer->lines[0].chars = NULL;

	return (buffer);
}


void		ft_buffer_destroy(t_Buffer *buffer)
{
	int	i;

	if (NULL == buffer)
	{
		return;
	}

	i = 0;
	while (i < buffer->nb_lines)
	{
		free(buffer->lines[i].chars);
		++i;
	}
	free(buffer->lines);
	free(buffer);
}


void		ft_buffer_clear(t_Buffer *buffer)
{
	int	i;

	if (NULL == buffer)
	{
		return;
	}

	i = 0;
	while (i < buffer->nb_lines)
	{
		free(buffer->lines[i].chars);
		buffer->lines[i].len = 0;
		buffer->lines[i].chars = NULL;
		++i;
	}

	buffer->lines = realloc(buffer->lines, sizeof(t_Line));
	if (NULL == buffer->lines)
	{
		ft_buffer_destroy(buffer);
		ft_log_error("malloc");
		exit(EXIT_FAILURE);
	}

	buffer->nb_lines = 1;
}


void		ft_buffer_insert_char(t_Buffer *buffer, int line, int col, int codepoint)
{
	int		line_len;
	int		i;
	t_Line	*buffer_line;

	line_len = buffer->lines[line].len;
	buffer_line = &buffer->lines[line];

	if (col < 0 || col >= line_len)
	{
		col = line_len;
	}
	buffer_line->chars = realloc(buffer_line->chars, (line_len + 1) * sizeof(t_Char));

	if (NULL == buffer_line->chars)
	{
		ft_buffer_destroy(buffer);
		ft_log_error("realloc");
		exit(EXIT_FAILURE);
	}
	
	i = line_len;
	while (i > col)
	{
		buffer_line->chars[i] = buffer_line->chars[i - 1];
		--i;
	}
	buffer_line->chars[col].codepoint = codepoint;
	buffer_line->len++;
}


void		ft_buffer_delete_char(t_Buffer *buffer, int line, int col)
{
	int		i;
	int		line_len;
	t_Line	*buffer_line;

	line_len = buffer->lines[line].len;
	buffer_line = &buffer->lines[line];

	col--;

	if (col < 0 || col >= line_len)
	{
		col = line_len - 1;
	}

	if (line_len == 1)
	{
		free(buffer_line->chars);
		buffer_line->chars = NULL;
		buffer_line->len = 0;
		return;
	}

	if (line_len - 1 < 0)
	{
		ft_buffer_delete_line(buffer, line);
		return;
	}

	i = col;
	while (i < line_len - 1)
	{
		buffer_line->chars[i] = buffer_line->chars[i + 1];
		++i;
	}
	buffer_line->chars = realloc(buffer_line->chars, (line_len - 1) * sizeof(t_Char));

	if (NULL == buffer_line->chars)
	{
		ft_log_error("realloc");
		ft_buffer_destroy(buffer);
		exit(EXIT_FAILURE);
	}
	buffer_line->len--;
}


void		ft_buffer_split_line(t_Buffer *buffer, int line, int col)
{
	int		i;
	int		old_line_len;
	t_Line	*old_line;
	t_Char	*temp;

	old_line_len = buffer->lines[line].len;
	old_line = &buffer->lines[line];
	temp = malloc((old_line_len - col) * sizeof(t_Char));

	if (NULL == temp)
	{
		ft_log_error("malloc");
		ft_buffer_destroy(buffer);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < old_line_len - col)
	{
		temp[i] = old_line->chars[i + col];
		++i;
	}

	ft_buffer_insert_empty_line(buffer, line + 1);

	old_line = &buffer->lines[line];
	i = 0;
	while (i < old_line_len - col)
	{
		ft_buffer_insert_char(buffer, line + 1, i, temp[i].codepoint);
		++i;
	}

	old_line->len = col;
	old_line->chars = realloc(old_line->chars, col * sizeof(t_Char));
}


void	ft_buffer_cat_line(t_Buffer *buffer, int line)
{
	t_Line	*prev;
	t_Line	*curr;
	int		i;

	prev = &buffer->lines[line - 1];
	curr = &buffer->lines[line];

	i = 0;
	while (i < curr->len)
	{
		ft_buffer_insert_char(buffer, line - 1, prev->len + i, curr->chars[i].codepoint);
		++i;
	}

	ft_buffer_delete_line(buffer, line);
}



void		ft_buffer_insert_empty_line(t_Buffer *buffer, int line)
{
	int	i;

	if (line < 0 || line >= buffer->nb_lines)
	{
		line = buffer->nb_lines;
	}

	buffer->lines = realloc(buffer->lines, (buffer->nb_lines + 1) * sizeof(t_Line));
	if (NULL == buffer->lines)
	{
		ft_log_error("realloc");
		ft_buffer_destroy(buffer);
		exit(EXIT_FAILURE);
	}

	i = buffer->nb_lines;
	while (i > line)
	{
		buffer->lines[i] = buffer->lines[i - 1];
		--i;
	}
	buffer->lines[line].chars = NULL;
	buffer->lines[line].len = 0;
	buffer->nb_lines++;
}


void		ft_buffer_delete_line(t_Buffer *buffer, int line)
{
	int	i;

	if (line < 0 || line >= buffer->nb_lines)
	{
		line = buffer->nb_lines - 1;
	}

	if (buffer->nb_lines <= 1)
	{
		return;
	}
	free(buffer->lines[line].chars);

	i = line;
	while (i < buffer->nb_lines - 1)
	{
		buffer->lines[i] = buffer->lines[i + 1];
		++i;
	}

	buffer->lines = realloc(buffer->lines, (buffer->nb_lines - 1) * sizeof(t_Line));
	if (NULL == buffer->lines)
	{
		ft_log_error("realloc");
		ft_buffer_destroy(buffer);
		exit(EXIT_FAILURE);
	}
	buffer->nb_lines--;
}


void		ft_buffer_update_layout(t_Buffer *buffer, TTF_Font *font)
{
	int		i;
	int		j;
	int		x;
	t_Char	*ch;
	int				maxx, minx, maxy, miny, advance;

	i = buffer->first_line;
	while (i < buffer->first_line + buffer->nb_showed_lines)
	{
		if (i > buffer->nb_lines - 1)
		{
			break;
		}
		x = j = 0;
		while (j < buffer->lines[i].len)
		{
			ch = &buffer->lines[i].chars[j];
			ch->x = x;
			ch->y = LINE_H * (i - buffer->first_line);

			TTF_GetGlyphMetrics(font, ch->codepoint, &minx, &maxx, &miny, &maxy, &advance);
			ch->w = advance;
			ch->h = TTF_GetFontHeight(font);
			x += ch->w;
			++j;
		}
		++i;
	}
}


void		ft_buffer_display_layout(t_AppContext *app, t_Buffer *buffer, SDL_FRect area)
{
	int			i;
	int			j;
	t_Char		buffer_char;
	
	i = buffer->first_line;
	while (i < buffer->first_line + buffer->nb_showed_lines)
	{
		if (i > buffer->nb_lines - 1)
		{
			break;
		}
		j = 0;
		while (j < ft_buffer_get_line_length(buffer, i))
		{
			buffer_char = buffer->lines[i].chars[j];
			ft_draw_text_codepoint(app, buffer_char.codepoint, area.x + buffer_char.x, area.y + buffer_char.y, app->theme.text);
			++j;
		}
		++i;
	}
}


int			ft_buffer_get_line_length(t_Buffer *buffer, int line)
{
	return (buffer->lines[line].len);
}


int			ft_buffer_total_lines(t_Buffer *buffer)
{
	return (buffer->nb_lines);
}


t_Char		ft_buffer_get_char(t_Buffer *buffer, int line, int col)
{
	return (buffer->lines[line].chars[col]);
}