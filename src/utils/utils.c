// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

#include <stdio.h>

#include "main.h"
#include "utils.h"

// ============================================================================================================================================================
// ---> Functions
// ============================================================================================================================================================

void			ft_log_error(const char *msg)
{
	fprintf(stderr, "ERROR:%s\n", msg);
}


char			*ft_putnbr(int nbr)
{
	char	buffer[12];
	int		i;
	int		is_negative = 0;

	i = 0;
	
	if (nbr == 0)
		return strdup("0");

	if (nbr < 0)
	{
		is_negative = 1;
		nbr *= -1;
	}

	while (nbr > 0)
	{
		buffer[i++] = '0' + (nbr % 10);
		nbr /= 10;
	}

	if (is_negative)
		buffer[i++] = '-';

	buffer[i] = '\0';

	for (int j = 0; j < i / 2; j++)
	{
		char tmp = buffer[j];
		buffer[j] = buffer[i - j - 1];
		buffer[i - j - 1] = tmp;
	}

	return strdup(buffer);
}


void			ft_set_render_color(SDL_Renderer *renderer, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// ============================================================================================================================================================
// ---> App
// ============================================================================================================================================================

int				ft_app_init(t_AppContext *app)
{
	if (false == SDL_Init(SDL_INIT_VIDEO))
	{
		ft_log_error(SDL_GetError());
		return (1);
	}

	if (false == TTF_Init())
	{
		ft_log_error(SDL_GetError());
		return (1);
	}

	app->theme.font = TTF_OpenFont("assets/Roboto/static/Roboto-Regular.ttf", 15);
	if (NULL == app->theme.font)
	{
		ft_log_error(SDL_GetError());
		return (1);
	}

	app->window = SDL_CreateWindow(NAME, 800, 600, SDL_WINDOW_RESIZABLE);
	if (NULL == app->window)
	{
		ft_log_error(SDL_GetError());
		SDL_Quit();
		return (1);
	}

	app->renderer = SDL_CreateRenderer(app->window, NULL);
	if (NULL == app->renderer)
	{
		ft_log_error(SDL_GetError());
		SDL_DestroyWindow(app->window);
		SDL_Quit();
		return (1);
	}
	app->current_file_path = NULL;
	return (0);
}


void			ft_app_destroy(t_AppContext *app)
{
	SDL_DestroyRenderer(app->renderer);
	SDL_StopTextInput(app->window);
	SDL_DestroyWindow(app->window);
	SDL_Quit();

	TTF_CloseFont(app->theme.font);
	TTF_Quit();
}

// ============================================================================================================================================================
// ---> Text
// ============================================================================================================================================================

t_TextMetrics	ft_draw_text(const t_AppContext *app, const char *text, int x, int y, SDL_Color color)
{
	SDL_Texture		*texture;
	SDL_Surface		*surface;
	SDL_FRect		dst;
	t_TextMetrics	metrics;

	surface = TTF_RenderText_Blended(app->theme.font, text, 0, color);

	metrics = ft_get_text_metrics(app, text);
	texture = SDL_CreateTextureFromSurface(app->renderer, surface);
	dst = (SDL_FRect){x, y, metrics.w, metrics.h};

	SDL_RenderTexture(app->renderer, texture, NULL, &dst);

	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);
	return (metrics);
}


t_TextMetrics	ft_draw_text_codepoint(const t_AppContext *app, Uint32 codepoint, int x, int y, SDL_Color color)
{
	SDL_Texture		*texture;
	SDL_Surface		*surface;
	SDL_FRect		dst;
	t_TextMetrics	metrics;

	surface = TTF_RenderGlyph_Blended(app->theme.font, codepoint, color);

	metrics = ft_get_text_metrics_codepoint(app, codepoint);
	texture = SDL_CreateTextureFromSurface(app->renderer, surface);
	dst = (SDL_FRect){x, y, metrics.w, TTF_GetFontHeight(app->theme.font)};

	SDL_RenderTexture(app->renderer, texture, NULL, &dst);

	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);
	return (metrics);
}

// ============================================================================================================================================================
// ---> Metrics & utf-8/unicode
// ============================================================================================================================================================

t_TextMetrics	ft_get_text_metrics(const t_AppContext *app, const char *text)
{
	t_TextMetrics	returned;

	TTF_GetStringSize(app->theme.font, text, 0, &returned.w, &returned.h);
	return (returned);
}


t_TextMetrics	ft_get_text_metrics_codepoint(const t_AppContext *app, const Uint32 codepoint)
{
	t_TextMetrics	returned;
	int				minx, maxx;
	int				miny, maxy;
	int				advance;

	TTF_GetGlyphMetrics(app->theme.font, codepoint, &minx, &maxx, &miny, &maxy, &advance);
	returned.w = advance;
	returned.h = TTF_GetFontHeight(app->theme.font);

	return (returned);
}


Uint32			utf8_to_codepoint(const char *utf8, int *out_advance)
{
	unsigned char c = utf8[0];
	if (c < 0x80) {
		if (out_advance) *out_advance = 1;
		return c;
	}
	if ((c >> 5) == 0x6) {
		if (out_advance) *out_advance = 2;
		return ((c & 0x1F) << 6) | (utf8[1] & 0x3F);
	}
	if ((c >> 4) == 0xE) {
		if (out_advance) *out_advance = 3;
		return ((c & 0x0F) << 12) | ((utf8[1] & 0x3F) << 6) | (utf8[2] & 0x3F);
	}
	if ((c >> 3) == 0x1E) {
		if (out_advance) *out_advance = 4;
		return ((c & 0x07) << 18) | ((utf8[1] & 0x3F) << 12) | ((utf8[2] & 0x3F) << 6) | (utf8[3] & 0x3F);
	}
	if (out_advance) *out_advance = 1;
	return 0xFFFD;
}


int				codepoint_to_utf8(uint32_t cp, char out[4])
{
	if (cp <= 0x7F) {
		out[0] = cp;
		return 1;
	} else if (cp <= 0x7FF) {
		out[0] = 0xC0 | ((cp >> 6) & 0x1F);
		out[1] = 0x80 | (cp & 0x3F);
		return 2;
	} else if (cp <= 0xFFFF) {
		out[0] = 0xE0 | ((cp >> 12) & 0x0F);
		out[1] = 0x80 | ((cp >> 6) & 0x3F);
		out[2] = 0x80 | (cp & 0x3F);
		return 3;
	} else if (cp <= 0x10FFFF) {
		out[0] = 0xF0 | ((cp >> 18) & 0x07);
		out[1] = 0x80 | ((cp >> 12) & 0x3F);
		out[2] = 0x80 | ((cp >> 6) & 0x3F);
		out[3] = 0x80 | (cp & 0x3F);
		return 4;
	}
	return 0;
}