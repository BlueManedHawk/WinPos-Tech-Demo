
/* LICENSE
 *
 */

#include "SDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main ( void ) {

	/* This function doesn't do what the SDL wiki claims it does.  What it
	 * does is make it so that all logs below this priority are hidden. */
	SDL_LogSetAllPriority ( SDL_LOG_PRIORITY_INFO ) ;

	if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "SDL failed to\
			initialize!  Error given: %s" , SDL_GetError ( ) ) ; 
		return EXIT_FAILURE ; }
	atexit ( SDL_Quit ) ;
	
	SDL_DisableScreenSaver ( ) ;
	atexit ( SDL_EnableScreenSaver ) ;

	SDL_Rect Bounds ;
	if ( SDL_GetDisplayUsableBounds ( 0 , &Bounds ) < 0 ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Could not obtain\
			the bounds of the display, which are required for\
			correct operation.  Error given:  %s" ,\
			SDL_GetError ( ) ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }

	/* TODO: This repeats itself.  Can it not? */
	SDL_Rect ImageRect ;
	int AspectRatio = Bounds.w / Bounds.h ;
	if ( AspectRatio == 4 / 3 ) {
		/* Do nothing.  Skip. */ }
	else if ( AspectRatio > 4 / 3 ) {
		ImageRect.h = Bounds.h ;
		ImageRect.w = ImageRect.h * ( 4 / 3 ) ;
		ImageRect.y = 0 ;
		ImageRect.x = ( Bounds.w - ImageRect.w ) / 2 ; }
	else if ( AspectRatio < 4 / 3 ) {
		ImageRect.w = Bounds.w ;
		ImageRect.h = ImageRect.w * ( 4 / 3 ) ;
		ImageRect.x = 0 ;
		ImageRect.y = ( Bounds.h - ImageRect.h ) / 2 ; }

	SDL_Window * Window = SDL_CreateWindow ( "WinPos Tech Demo" ,\
		SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 640 , 480 ,\
		SDL_WINDOW_RESIZABLE ) ;
	if ( Window == NULL ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Failed to create\
			the window.  Error given: %s" , SDL_GetError ( ) ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }

	if ( SDL_FlashWindow ( Window , SDL_FLASH_UNTIL_FOCUSED ) < 0 ) {
		SDL_LogError ( SDL_LOG_CATEGORY_ERROR , "Couldn't flash the\
			window.  Error given:  %s" , SDL_GetError ( ) ) ; }

	SDL_Surface * Surface = SDL_CreateRGBSurfaceWithFormat ( 0 , Bounds.w ,\
		Bounds.h , 32 , SDL_PIXELFORMAT_RGBA32 ) ;
	if ( Surface == NULL ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't create\
			the main surface.  Error given:  %s" , SDL_GetError ( )\
			) ;
		SDL_DestroyWindow ( Window ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }
	if ( SDL_FillRect ( Surface , NULL , SDL_MapRGB ( Surface->format , 0\
					, 0 , 0 ) < 0 ) ) {
		SDL_LogError ( SDL_LOG_CATEGORY_ERROR , "Couldn't black out\
			the surface.  Uh, it's probably fine?  😬.  Error\
			given:  %s", SDL_GetError ( ) ) ; }
	SDL_Surface * Image = SDL_LoadBMP ( "./Assets/Image.bmp" ) ;
	if ( Image == NULL ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't create\
			the image surface.  Error given:  %s" , SDL_GetError (\
			) ) ;
		SDL_FreeSurface ( Surface ) ;
		SDL_DestroyWindow ( Window ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }
	if ( SDL_BlitSurface ( Image , NULL , Surface , &ImageRect ) < 0 ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't blit the\
			surfaces, making effects unobservable.  Error given:\
			  %s" , SDL_GetError ( ) ) ;
		SDL_FreeSurface ( Surface ) ;
		SDL_FreeSurface ( Image ) ;
		SDL_DestroyWindow ( Window ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }

	SDL_Renderer * Renderer = SDL_CreateRenderer ( Window , -1 ,\
		SDL_RENDERER_ACCELERATED ) ;
	if ( Renderer == NULL ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't create\
			the renderer.  Error given:  %s" , SDL_GetError ( ) )\
			;
		SDL_FreeSurface ( Surface ) ;
		SDL_FreeSurface ( Image ) ;
		SDL_DestroyWindow ( Window ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }

	SDL_Texture * Texture = SDL_CreateTextureFromSurface ( Renderer , \
		Surface ) ;
	if ( Texture == NULL ) {
		SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't get the\
			texture to work.  Error given:  %s" , SDL_GetError ( )\
			) ;
		SDL_FreeSurface ( Surface ) ;
		SDL_FreeSurface ( Image ) ;
		SDL_DestroyRenderer ( Renderer ) ;
		SDL_DestroyWindow ( Window ) ;
		SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
		SDL_Quit ( ) ;
		return EXIT_FAILURE ; }

	SDL_FreeSurface ( Surface ) ;
	SDL_FreeSurface ( Image ) ;

	SDL_Event e ;
	bool Quit = false ;
	SDL_Rect WindowRect ;
	if ( SDL_SetRenderDrawColor ( Renderer , 0 , 0 , 0 , 255 ) < 0 ) {
		SDL_LogError ( SDL_LOG_CATEGORY_ERROR , "Couldn't set the draw\
			color.  Is that okay?  I dunno.  Error given:  %s" ,\
			SDL_GetError ( ) ) ; }
	while ( 1 ) {

		if ( SDL_RenderClear ( Renderer ) < 0 ) {
			SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't\
				clear the renderer.  Error given:  %s" ,\
				SDL_GetError ( ) ) ;
			SDL_DestroyTexture ( Texture ) ;
			SDL_DestroyRenderer ( Renderer ) ;
			SDL_DestroyWindow ( Window ) ;
			SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
			SDL_Quit ( ) ;
			return EXIT_FAILURE ; }

		while ( SDL_PollEvent ( &e ) == 1 ) {
			if ( e.type == SDL_QUIT ) {
				Quit = true ; } }
		if ( Quit == true ) 
			break ;
		SDL_GetWindowSize ( Window , &WindowRect.w , &WindowRect.h ) ;
		SDL_GetWindowPosition ( Window , &WindowRect.x ,
			&WindowRect.y ) ;

		if ( SDL_RenderCopy ( Renderer , Texture , &WindowRect , NULL\
				) < 0 ) {
			SDL_LogCritical ( SDL_LOG_CATEGORY_ERROR , "Couldn't\
				copy to the buffer.  Error given:  %s" ,\
				SDL_GetError ( ) ) ;
			SDL_DestroyTexture ( Texture ) ;
			SDL_DestroyRenderer ( Renderer ) ;
			SDL_DestroyWindow ( Window ) ;
			SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
			SDL_Quit ( ) ;
			return EXIT_FAILURE ; }

		SDL_RenderPresent ( Renderer ) ; 
		
		SDL_Delay ( 27 ) ; /* Crude, but it works. */ }

	SDL_DestroyWindow ( Window ) ;
	SDL_QuitSubSystem ( SDL_INIT_VIDEO ) ;
	SDL_Quit ( ) ;

	return EXIT_SUCCESS ; }
