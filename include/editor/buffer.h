// ============================================================================================================================================================
// ---> Buffer.h - HEader
// ============================================================================================================================================================

#ifndef FT_EDITOR_BUFFER_H
# define FT_EDITOR_BUFFER_H

// ============================================================================================================================================================
// ---> Types
// ============================================================================================================================================================

typedef struct s_AppContext t_AppContext;

typedef struct	s_Char 
{
	Uint32	codepoint;
	int		x;
	int		y;
	int		w;
	int		h;
} t_Char;

typedef struct	s_Line
{
	t_Char	*chars;
	int		len;
} t_Line;

typedef struct	s_Buffer
{
	t_Line	*lines;
	int		first_line;
	int		nb_showed_lines;
	int		nb_lines;
} t_Buffer;

// ============================================================================================================================================================
// ---> Functions
// ============================================================================================================================================================

/**
 * @brief Create the buffer structure.
 * @return The created buffer.
 */
t_Buffer	*ft_buffer_create(void);

/**
 * @brief Clean the buffer to prevent memory leaks.
 * @param buffer The buffer.
 */
void		ft_buffer_destroy(t_Buffer *buffer);

/**
 * @brief Clear the content of the buffer.
 * @param buffer The buffer.
 */
void		ft_buffer_clear(t_Buffer *buffer);

/**
 * @brief Insert the char at the specified column & line.
 * @param buffer The buffer.
 * @param line The line where the char is inserted.
 * @param col The column where the char is inserted.
 * @param codepoint The codepoint of the char to insert.
 */
void		ft_buffer_insert_char(t_Buffer *buffer, int line, int col, int codepoint);


/**
 * @brief Delete the char at the specified column & line.
 * @param buffer The buffer.
 * @param line The line where the char is deleteed.
 * @param col The column where the char is deleteed.
 * @param codepoint The codepoint of the char to delete.
 */
void		ft_buffer_delete_char(t_Buffer *buffer, int line, int col);

/**
 * @brief Split a line into buffer at the specified column & line.
 * @param buffer The buffer.
 * @param line The line that will be splited.
 * @param col The column that will be splited.
 */
void		ft_buffer_split_line(t_Buffer *buffer, int line, int col);

/**
 * @brief Concatenate a line into buffer at the specified line with the previous line.
 * @param buffer The buffer.
 * @param line The line to cat.
 */
void		ft_buffer_cat_line(t_Buffer *buffer, int line);


/**
 * @brief Insert an empty line into buffer.
 * @param buffer The buffer.
 * @param line The line that empty line will be added.
 */
void		ft_buffer_insert_empty_line(t_Buffer *buffer, int line);

/**
 * @brief Delete a line into buffer at specified line.
 * @param buffer The buffer.
 * @param line The line to delete.
 */
void		ft_buffer_delete_line(t_Buffer *buffer, int line);

/**
 * @brief Update the buffer layout.
 * @param buffer The buffer.
 */
void		ft_buffer_update_layout(t_Buffer *buffer, TTF_Font *font);

/**
 * @brief Display the buffer layout.
 * @param app The app context.
 * @param buffer The buffer.
 * @param area The area to display it.
 */
void		ft_buffer_display_layout(t_AppContext *app, t_Buffer *buffer, SDL_FRect area);

/**
 * @brief Get the line length.
 * @param buffer The buffer.
 * @param line The line that will get length.
 * @return The length of the line.
 */
int			ft_buffer_get_line_length(t_Buffer *buffer, int line);

/**
 * @brief Get the number of lines.
 * @param buffer The buffer.
 * @return The total of lines into buffer.
 */
int			ft_buffer_total_lines(t_Buffer *buffer);

/**
 * @brief Get the content into a specified charactere.
 * @param buffer The buffer.
 * @param line The line of the charactere.
 * @param col The column of the charactere.
 * @return A t_Char structure with the content of the charactere.
 */
t_Char		ft_buffer_get_char(t_Buffer *buffer, int line, int col);

#endif