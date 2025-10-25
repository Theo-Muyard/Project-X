// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "utils.h"
#include "tinyfiledialogs.h"

#include "navbar.h"

#include "editor.h"
#include "buffer.h"

#define PATH_MAX 4096

// ============================================================================================================================================================
// ---> Static
// ============================================================================================================================================================

static void	ft_navbar_render_submenu(t_AppContext *app, t_NavItem *item)
{
	int				sub_x;
	int				sub_y;
	int				sub_rect_w;
	t_TextMetrics	sub_size;
	int				j;
	int				remaining_space;
	int				space;
	int				start_y;
	SDL_FRect		sub_hover_rect;

	j = 0;
	sub_rect_w = 0;
	while (j < item->sub_count)
	{
		int	w = ft_get_text_metrics(app, item->sub_items[j].label).w;
		if (sub_rect_w < w)
		{
			sub_rect_w = w;
		}
		++j;
	}
	
	SDL_FRect	sub_menu_rect = {item->rect.x, NAVBAR_H + 3, sub_rect_w + 80, (LINE_H * 1.5 * item->sub_count ) + 10};
	ft_set_render_color(app->renderer, app->theme.accent);
	SDL_RenderFillRect(app->renderer, &sub_menu_rect);

	remaining_space = sub_menu_rect.h - 20 - LINE_H * item->sub_count;
	space = 0;
	start_y = sub_menu_rect.y + 10;
	
	if (item->sub_count > 1)
	{
		space = remaining_space / (item->sub_count - 1);
	}
	else
	{
		start_y = sub_menu_rect.y + (sub_menu_rect.h - LINE_H) / 2;
	}

	j = 0;
	while (j < item->sub_count)
	{
		t_TextMetrics	sub_metrics = ft_get_text_metrics(app, item->sub_items[j].label);
		ft_set_render_color(app->renderer, app->theme.accent2);

		sub_y = start_y + j * (LINE_H + space) + (LINE_H - sub_metrics.h) / 2;
		sub_x = sub_menu_rect.x + (sub_menu_rect.w - sub_metrics.w )/ 2; 
		
		sub_size = ft_draw_text(app, item->sub_items[j].label, sub_x, sub_y, app->theme.text);

		item->sub_items[j].rect = (SDL_FRect){sub_x, sub_y, sub_size.w, sub_size.h};

		if (item->sub_items[j].hovered)
		{
			sub_hover_rect = (SDL_FRect){sub_menu_rect.x + 10, sub_y - (LINE_H - sub_metrics.h) / 2, sub_menu_rect.w - 20, LINE_H};
			ft_set_render_color(app->renderer, app->theme.text);
			(void)sub_hover_rect;
			SDL_RenderFillRect(app->renderer, &sub_hover_rect);
			ft_draw_text(app, item->sub_items[j].label, sub_x, sub_y, app->theme.accent);
		}
		++j;
	}
}


static void	ft_navbar_render_items(t_AppContext *app)
{
	t_NavItem	*item;
	SDL_FRect	item_hover_rect;
	int			i;
	int			x;
	int			y;

	x = 50;
	i = 0;
	while (i < app->navbar->nav_count)
	{
		item = &app->navbar->nav_items[i];
		ft_set_render_color(app->renderer, app->theme.text);
		y = (NAVBAR_H - ft_get_text_metrics(app, item->label).h) / 2;

		t_TextMetrics item_size = ft_draw_text(app, item->label, x, y, app->theme.text);

		item->rect = (SDL_FRect){x, y, item_size.w, item_size.h};

		if (item->hovered)
		{
			item_hover_rect = (SDL_FRect){x - 10, y - (LINE_H - item_size.h) / 2, item_size.w + 20, LINE_H};
			ft_set_render_color(app->renderer, app->theme.text);
			SDL_RenderFillRect(app->renderer, &item_hover_rect);
			ft_draw_text(app, item->label, x, y, app->theme.accent);
		}
		if (true == item->active)
		{
			ft_navbar_render_submenu(app, item);
		}

		++i;
		x += 20 + item_size.w;
	}
}


static void	ft_navbar_create_items(
	t_Navbar	*nav,
	char		*label,
	SDL_FRect	rect,
	t_SubItem	*sub_items,
	int			sub_count,
	int			item_i)
{
	t_NavItem	*nav_item;
	int			i;

	nav_item = &nav->nav_items[item_i];
	nav_item->label = SDL_strdup(label);
	nav_item->active = false;
	nav_item->hovered = false;
	nav_item->rect = rect;
	nav_item->sub_count = sub_count;

	nav_item->sub_items = malloc(sub_count * sizeof(t_SubItem));
	if (NULL == nav_item->sub_items)
	{
		ft_log_error("maloc");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (i < sub_count)
	{
		nav_item->sub_items[i].label = sub_items[i].label;
		nav_item->sub_items[i].on_click = sub_items[i].on_click;
		nav_item->sub_items[i].hovered = false;
		nav_item->sub_items[i].rect = sub_items[i].rect;
		++i;
	}
}


static void	ft_navbar_new_file(t_AppContext *app)
{
	char	*filename;
	char	*folder_path;
	FILE	*file;
	char	full_path[PATH_MAX];

	(void)app;

	folder_path = tinyfd_selectFolderDialog("Select folder:", ".");
	if (NULL == folder_path)
	{
		return;
	}

	filename = tinyfd_inputBox("New file", "Enter the name of the new file:", "untitled.txt");

	if (NULL == filename)
	{
		return;
	}

	snprintf(full_path, PATH_MAX, "%s/%s",folder_path, filename);

	file = fopen(full_path, "r");
	if (file)
	{
		tinyfd_messageBox("Error", "File already exists.", "ok", "error", 1);
		fclose(file);
		return;
	}

	file = fopen(full_path, "w");
	if (NULL == file)
	{
		tinyfd_messageBox("Error", "Could not create file.", "ok", "error", 1);
		return;
	}
	fclose(file);
	app->current_file_path = SDL_strdup(full_path);
	if (NULL == app->current_file_path)
	{
		ft_log_error("strdup");
		exit(EXIT_FAILURE);
	}
	app->editor->need_update = true;
}


static void	ft_navbar_open_file(t_AppContext *app)
{
	char	*file_path;
	FILE	*file;
	char	*content;
	long	size;
	int		i;
	int		advance;
	Uint32	codepoint;

	file_path = tinyfd_openFileDialog("Open file", ".", 0, NULL, NULL, 0);
	if (NULL == file_path)
	{
		return;
	}
	file = fopen(file_path, "r");
	if (NULL == file)
	{
		tinyfd_messageBox("Error", "Cannot open file.", "ok", "error", 1);
		return;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	content = malloc(size + 1);
	if (NULL == content)
	{
		fclose(file);
		ft_log_error("maloc");
		exit(EXIT_FAILURE);
	}

	fread(content, 1, size, file);
	content[size] = '\0';
	fclose(file);

	free(app->current_file_path);
	app->current_file_path = SDL_strdup(file_path);

	ft_buffer_clear(app->editor->buffer);
	i = 0;
	while (content[i])
	{
		if (content[i] == '\n')
		{
			ft_buffer_insert_empty_line(app->editor->buffer, -1);
			++i;
		}
		else
		{
			advance = 0;
			codepoint = utf8_to_codepoint(&content[i], &advance);
			ft_buffer_insert_char(app->editor->buffer, app->editor->buffer->nb_lines - 1, -1, codepoint);
			i += advance;
		}
	}
	free(content);
	app->editor->need_update = true;
}


static void	ft_navbar_save_file(t_AppContext *app)
{
	FILE	*file;
	int		line;
	int		col;
	char	utf8[4];
	int		len;

	file = fopen(app->current_file_path, "w");
	if (NULL == file)
	{
		tinyfd_messageBox("Error", "Cannot save file.", "ok", "error", 1);
		return;
	}

	line = 0;
	while (line < app->editor->buffer->nb_lines)
	{
		col = 0;
		while (col < app->editor->buffer->lines[line].len)
		{
			len = codepoint_to_utf8(app->editor->buffer->lines[line].chars[col].codepoint, utf8);
			fwrite(utf8, 1, len, file);
			++col;
		}
		++line;
		if (line < app->editor->buffer->nb_lines)
		{
			fwrite((char *)"\n", 1, 1, file);
		}
	}
	fclose(file);
	tinyfd_messageBox("Success", "File saved.", "ok", NULL, 1);
}

// ============================================================================================================================================================
// ---> Init
// ============================================================================================================================================================

int		ft_navbar_init(t_AppContext *app)
{
	SDL_FRect	null_rect;

	app->navbar =  malloc(sizeof(t_Navbar));
	if (NULL == app->navbar)
	{
		ft_log_error("maloc");
		exit(EXIT_FAILURE);
	}

	app->navbar->nav_count = 2;
	app->navbar->nav_items = malloc(app->navbar->nav_count * sizeof(t_NavItem));
	if (NULL == app->navbar->nav_items)
	{
		ft_log_error("maloc");
		exit(EXIT_FAILURE);
	}

	null_rect = (SDL_FRect){0, 0, 0, 0};

	t_SubItem	file_sub_items[] = {
		{ .label = "New file", .rect = null_rect, .on_click = ft_navbar_new_file },
		{ .label = "Open file", .rect = null_rect, .on_click = ft_navbar_open_file },
		{ .label = "Open folder", .rect = null_rect },
		{ .label = "Save", .rect = null_rect, .on_click = ft_navbar_save_file },
	};

	t_SubItem	terminal_sub_items[] = {
		{ .label = "Switch view", .rect = null_rect }
	};

	ft_navbar_create_items(app->navbar, "File", null_rect, file_sub_items, 4, 0);
	ft_navbar_create_items(app->navbar, "Terminal", null_rect, terminal_sub_items, 1, 1);

	app->navbar->need_update = true;
	return (0);
}

// ============================================================================================================================================================
// ---> Handle event
// ============================================================================================================================================================

void	ft_navbar_handle_event(t_AppContext *app, SDL_Event *event)
{
	int			mx;
	int			my;
	int			i;
	int			j;
	t_NavItem	*item;
	t_SubItem	*sub_item;

	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
	{
		mx = event->button.x;
		my = event->button.y;
		i = 0;
		while (i < app->navbar->nav_count)
		{
			item = &app->navbar->nav_items[i];

			if (item->active)
			{
				j = 0;
				while (j < item->sub_count)
				{
					if (SDL_PointInRectFloat(&(SDL_FPoint){mx, my}, &item->sub_items[j].rect))
					{
						item->sub_items[j].on_click(app);
					}
					++j;
				}
			}

			if (SDL_PointInRectFloat(&(SDL_FPoint){mx, my}, &item->rect))
			{
				item->active = true;
			}
			else
			{
				item->active = false;
			}
			++i;
		}
	}

	if (event->type == SDL_EVENT_MOUSE_MOTION)
	{
		mx = event->motion.x;
		my = event->motion.y;

		i = 0;
		while (i < app->navbar->nav_count)
		{
			item = &app->navbar->nav_items[i];
			if (SDL_PointInRectFloat(&(SDL_FPoint){mx, my}, &item->rect))
			{
				if (false == item->hovered)
				{
					item->hovered = true;
				}
			}
			else
			{
				item->hovered = false;
			}

			if (item->active)
			{
				j = 0;
				while (j < app->navbar->nav_items[i].sub_count)
				{
					sub_item = &app->navbar->nav_items[i].sub_items[j];
					if (SDL_PointInRectFloat(&(SDL_FPoint){mx, my}, &sub_item->rect))
					{
						if (false == sub_item->hovered)
						{
							sub_item->hovered = true;
						}
					}
					else
					{
						sub_item->hovered = false;
					}
					++j;
				}
			}
			++i;
		}
	}
}

// ============================================================================================================================================================
// ---> Update
// ============================================================================================================================================================

void	ft_navbar_update(t_AppContext *app)
{
	(void)app;
	// No code here now (update it for later)
}

// ============================================================================================================================================================
// ---> Render
// ============================================================================================================================================================

void	ft_navbar_render(t_AppContext *app)
{
	SDL_FRect	navbar_rect;
	ft_set_render_color(app->renderer, app->theme.accent);

	navbar_rect = (SDL_FRect) {0, 0, (float)app->width, NAVBAR_H};
	SDL_RenderFillRect(app->renderer, &navbar_rect);

	ft_navbar_render_items(app);
}

// ============================================================================================================================================================
// ---> Clean
// ============================================================================================================================================================

void	ft_navbar_cleanup(t_AppContext *app)
{
	int	i;

	i = 0;
	while (i < app->navbar->nav_count)
	{
		free(app->navbar->nav_items[i].sub_items);
		free(app->navbar->nav_items[i].label);
		++i;
	}
	
	free(app->navbar->nav_items);
	free(app->navbar);
}