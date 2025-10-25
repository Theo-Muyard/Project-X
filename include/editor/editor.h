// ============================================================================================================================================================
// ---> Editor.h - Header
// ============================================================================================================================================================

#ifndef FT_EDITOR_H
# define FT_EDITOR_H

// ============================================================================================================================================================
// ---> Types
// ============================================================================================================================================================

typedef struct	s_AppContext t_AppContext;
typedef struct	s_Buffer t_Buffer;
typedef struct	s_Cursor t_Cursor;

typedef struct	s_EditorContext 
{
	t_Buffer	*buffer;
	t_Cursor	*cursor;
	bool		need_update;
} t_EditorContext;

// ============================================================================================================================================================
// ---> Editor.c
// ============================================================================================================================================================

/**
 * @brief Initialize the editor module so that it can be used.
 * @param app The app contexte.
 * @return 0 on success & 1 on failure.
 */
int		ft_editor_init(t_AppContext *app);

/**
 * @brief Events handling for the editor module.
 * @param app The app contexte.
 * @param event The SDL event.
 */
void	ft_editor_handle_event(t_AppContext *app, SDL_Event *event);

/**
 * @brief Update the editor module.
 * @param app The app contexte.
 * @param now The SDL time.
 */
void	ft_editor_update(t_AppContext *app, int now);

/**
 * @brief Create the editor module rendering.
 * @param app The app contexte.
 */
void	ft_editor_render(t_AppContext *app);

/**
 * @brief Clean the editor module to prevent memory leaks.
 * @param app The app contexte.
 */
void	ft_editor_cleanup(t_AppContext *app);

#endif