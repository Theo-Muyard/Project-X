// ============================================================================================================================================================
// ---> Utils.h - Header
// ============================================================================================================================================================

#ifndef FT_UTILS_H
# define FT_UTILS_H

// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

# include <SDL3/SDL.h>

// ============================================================================================================================================================
// ---> Types
// ============================================================================================================================================================

typedef struct s_AppContext t_AppContext;

typedef struct s_TextMetrics {
	int	w;
	int	h;
} t_TextMetrics;

// ============================================================================================================================================================
// ---> Functions
// ============================================================================================================================================================

/**
 * @brief Log an error message on stderr.
 * @param msg The error message to display.
 */
void			ft_log_error(const char *msg);

/**
 * @brief Convert a int in char*.
 * @param nbr The number to convert.
 * @return The number in char*.
 */
char			*ft_putnbr(int nbr);

/**
 * @brief Define the color used during rendering.
 * @param renderer The renderer of app.
 * @param color The color to use.
 */
void			ft_set_render_color(SDL_Renderer *renderer, SDL_Color color);

// ============================================================================================================================================================
// ---> App
// ============================================================================================================================================================

/**
 * @brief Init the app (SDL, TTF, window, renderer, fonts).
 * @param app The app context.
 * @return 0 on success, 1 on error.
 */
int				ft_app_init(t_AppContext *app);

/**
 * @brief Destroy the app to prevent memory leaks.
 * @param app The app context.
 */
void			ft_app_destroy(t_AppContext *app);

// ============================================================================================================================================================
// ---> Text
// ============================================================================================================================================================

/**
 * @brief Create and draw the text on the window.
 * @param app The app context.
 * @param text The text to print in UTF-8.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return A textMetrics with height & width of the drawn text.
 */
t_TextMetrics	ft_draw_text(const t_AppContext *app, const char *text, int x, int y, SDL_Color color);

/**
  * @brief Create and draw the text on the window.
 * @param app The app context.
 * @param text The text to print in codepoint (UNICODE).
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return A textMetrics with height & width of the drawn text.
 */
t_TextMetrics	ft_draw_text_codepoint(const t_AppContext *app, Uint32 codepoint, int x, int y, SDL_Color color);

// ============================================================================================================================================================
// ---> Metrics & utf-8/unicode
// ============================================================================================================================================================

/**
 * @brief Get the text metrics (width & height).
 * @param app The app context.
 * @param text The text in UTF-8.
 * @return A textMetrics with height & width.
 * 
 */
t_TextMetrics	ft_get_text_metrics(const t_AppContext *app, const char *text);

/**
 * @brief Get the text metrics (width & height).
 * @param app The app context.
 * @param codepoint The text in codepoint (UNICODE).
 * @return A textMetrics with height & width.
 * 
 */
t_TextMetrics	ft_get_text_metrics_codepoint(const t_AppContext *app, const Uint32 codepoint);

/**
 * @brief Convert a UTF-8 to codepoint (UNICODE).
 * @param utf8 The content in UTF-8.
 * @param out_advance A pointer to an int that will receive the number of bytes used consumed from the UTF-8 string to represent this codepoint.
 * @return The codepoint as a Uint32.
 * 
 */
Uint32		utf8_to_codepoint(const char *utf8, int *out_advance);

/**
 * @brief Convert a codepoint (UNICODE) to UTF-8.
 * @param utf8 The content in codepoint (UNICODE).
 * @param out A buffer of at least 4 bytes where the UTF-8 bytes will be writen.
 * @return The number of bytes used to represent the codepoint in UTF-8.
 * 
 */
int		codepoint_to_utf8(uint32_t cp, char out[4]);

#endif