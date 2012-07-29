#include "atg.h"
#include "atg_internals.h"

SDL_Surface *atg_render_button(const atg_element *e)
{
	if(!e) return(NULL);
	if(!((e->type==ATG_BUTTON)||(e->type==ATG_CUSTOM))) return(NULL);
	atg_button *b=e->elem.button;
	if(!b) return(NULL);
	SDL_Surface *content=atg_render_box(&(atg_element){.w=(e->w>4)?e->w-4:0, .h=(e->h>4)?e->h-4:0, .type=ATG_BOX, .elem.box=b->content, .clickable=false, .userdata=NULL});
	if(!content) return(NULL);
	bool borders=(content->w>8)&&(content->h>4);
	SDL_Surface *rv=SDL_CreateRGBSurface(SDL_HWSURFACE, content->w+(borders?4:0), content->h+(borders?4:0), content->format->BitsPerPixel, content->format->Rmask, content->format->Gmask, content->format->Bmask, content->format->Amask);
	if(!rv)
	{
		SDL_FreeSurface(content);
		return(NULL);
	}
	SDL_FillRect(rv, &(SDL_Rect){.x=0, .y=0, .w=rv->w, .h=rv->h}, SDL_MapRGBA(rv->format, b->content->bgcolour.r, b->content->bgcolour.g, b->content->bgcolour.b, b->content->bgcolour.a));
	if(borders)
	{
		SDL_BlitSurface(content, NULL, rv, &(SDL_Rect){.x=2, .y=2});
		SDL_FillRect(rv, &(SDL_Rect){.x=2, .y=1, .w=rv->w-4, .h=1}, SDL_MapRGBA(rv->format, b->fgcolour.r, b->fgcolour.g, b->fgcolour.b, b->fgcolour.a));
		SDL_FillRect(rv, &(SDL_Rect){.x=2, .y=rv->h-2, .w=rv->w-4, .h=1}, SDL_MapRGBA(rv->format, b->fgcolour.r, b->fgcolour.g, b->fgcolour.b, b->fgcolour.a));
		SDL_FillRect(rv, &(SDL_Rect){.x=1, .y=2, .w=1, .h=rv->h-4}, SDL_MapRGBA(rv->format, b->fgcolour.r, b->fgcolour.g, b->fgcolour.b, b->fgcolour.a));
		SDL_FillRect(rv, &(SDL_Rect){.x=rv->w-2, .y=2, .w=1, .h=rv->h-4}, SDL_MapRGBA(rv->format, b->fgcolour.r, b->fgcolour.g, b->fgcolour.b, b->fgcolour.a));
	}
	else
		SDL_BlitSurface(content, NULL, rv, &(SDL_Rect){.x=0, .y=0});
	SDL_FreeSurface(content);
	return(rv);
}

void atg_click_button(struct atg_event_list *list, struct atg_element *element, SDL_MouseButtonEvent button, __attribute__((unused)) unsigned int xoff, __attribute__((unused)) unsigned int yoff)
{
	atg_ev_trigger trigger;
	trigger.e=element;
	trigger.button=button.button;
	atg__push_event(list, (atg_event){.type=ATG_EV_TRIGGER, .event.trigger=trigger});
}

atg_button *atg_create_button(const char *label, atg_colour fgcolour, atg_colour bgcolour)
{
	atg_button *rv=malloc(sizeof(atg_button));
	if(rv)
	{
		rv->fgcolour=fgcolour;
		rv->content=atg_create_box(ATG_BOX_PACK_HORIZONTAL, bgcolour);
		if(rv->content)
		{
			atg_element *l=atg_create_element_label(label, 12, fgcolour);
			if(l)
			{
				if(atg_pack_element(rv->content, l))
				{
					atg_free_element(l);
					atg_free_box_box(rv->content);
					free(rv);
					return(NULL);
				}
			}
			else
			{
				atg_free_box_box(rv->content);
				free(rv);
				return(NULL);
			}
		}
		else
		{
			free(rv);
			return(NULL);
		}
	}
	return(rv);
}

atg_element *atg_create_element_button(const char *label, atg_colour fgcolour, atg_colour bgcolour)
{
	atg_element *rv=malloc(sizeof(atg_element));
	if(!rv) return(NULL);
	atg_button *b=atg_create_button(label, fgcolour, bgcolour);
	if(!b)
	{
		free(rv);
		return(NULL);
	}
	rv->w=rv->h=0;
	rv->type=ATG_BUTTON;
	rv->elem.button=b;
	rv->clickable=false; /* because it generates ATG_EV_TRIGGER events instead */
	rv->hidden=false;
	rv->cache=false;
	rv->cached=NULL;
	rv->userdata=NULL;
	rv->render_callback=atg_render_button;
	rv->match_click_callback=NULL;
	rv->free_callback=atg_free_button;
	return(rv);
}

atg_button *atg_copy_button(const atg_button *b)
{
	if(!b) return(NULL);
	atg_button *rv=malloc(sizeof(atg_button));
	if(!rv) return(NULL);
	*rv=*b;
	rv->content=b->content?atg_copy_box(b->content):NULL;
	return(rv);
}

void atg_free_button(atg_element *e)
{
	if(!e) return;
	atg_button *button=e->elem.button;
	if(button)
	{
		atg_free_box_box(button->content);
	}
	free(button);
}
