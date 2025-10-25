// ============================================================================================================================================================
// ---> Navbar.h - Header
// ============================================================================================================================================================

#ifndef FT_NAVBAR_H
# define FT_NAVBAR_H

// ============================================================================================================================================================
// ---> Includes
// ============================================================================================================================================================

# include <SDL3/SDL.h>

// ============================================================================================================================================================
// ---> Types
// ============================================================================================================================================================

typedef struct	s_AppContext t_AppContext;

typedef struct s_SubItem
{
	char		*label;
	SDL_FRect	rect;
	bool		hovered;
	void		(*on_click)(t_AppContext *app);
} t_SubItem;

typedef struct s_NavItem
{
	char		*label;
	SDL_FRect	rect;
	bool		hovered;
	bool		active;
	t_SubItem	*sub_items;
	int			sub_count;
} t_NavItem;

typedef struct s_Navbar
{
	t_NavItem	*nav_items;
	int			nav_count;
	bool		need_update;
} t_Navbar;

// ============================================================================================================================================================
// ---> Functions
// ============================================================================================================================================================

/**
 * @brief Initialize the navbar module so that it can be used.
 * @param app The app contexte.
 * @return 0 on success & 1 on failure
 */
int		ft_navbar_init(t_AppContext *app);

/**
 * @brief Event handling for the navbar module.
 * @param app The app contexte.
 * @param event The SDL event.
 */
void	ft_navbar_handle_event(t_AppContext *app, SDL_Event *event);

/**
 * @brief Update the navbar module.
 * @param app The app contexte.
 */
void	ft_navbar_update(t_AppContext *app);

/**
 * @brief Create the navbar module rendering.
 * @param app The app contexte.
 */
void	ft_navbar_render(t_AppContext *app);

/**
 * @brief Clean the navbar module to prevent memory leaks.
 * @param app The app contexte.
 */
void	ft_navbar_cleanup(t_AppContext *app);

#endif