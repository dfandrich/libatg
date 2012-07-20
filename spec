atg - a tiny gui

Specification

atg is a small, simple GUI library/toolkit for SDL, providing things like buttons and clickables to allow you to concentrate on your program logic.

atg is loosely based on Spiffy's GUI, though genericised.

atg Event Queue:
	atg will poll SDL for events, and if these events trigger any atg elements, these elements' events will be placed on the queue.  In any case all SDL events will be placed on the atg queue, in case the application wishes to further process them.  Note that if the application calls SDL_PollEvent() or SDL_WaitEvent(), these events will not be 'seen' by atg.  Thus, the application should instead call atg_poll_event() or atg_wait_event().  Alternatively, after calling SDL_PollEvent() or SDL_WaitEvent(), the application can pass the returned event to atg with atg_do_event().

Custom Widgets:
	atg can be extended with custom widgets.  If a widget's type is ATG_CUSTOM, atg will use its callbacks to render, handle clicks, and free.
	An example of this is the 'selector' widget created in widget.c.  It consists of a box containing four buttons.  When one of these buttons is clicked, the value is set (written through the "userdata" pointer), and the selected option is highlighted.
	The render callback is left at the default; since the widget was created with atg_create_element_box, this is atg_render_box.  Consequently, the box and its contents are rendered in the normal way.  This is usually what you want when your widget is based on a box.
	The click-handler callback, selector_match_click_callback, uses atg__match_click_recursive to pass the click to the child elements (the four buttons), but then catches their events itself and interprets them, instead of passing them back down the stack.  It generates a value-change event if the selected value has changed.
	The free callback is left at the default (atg_free_box).
