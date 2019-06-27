#pragma once

void events()
{
	static SDL_Event e;
	while(SDL_PollEvent(&e)){
		switch(e.type){
		case SDL_QUIT:
			printf("Quitting now!\n");
			exit(0);
			return;
			break;
		case SDL_MOUSEMOTION:
			//printf("Mouse position (%d, %d)\n", event.motion.x, event.motion.y);
			return;
			break;
		case SDL_MOUSEBUTTONDOWN:
			//printf("Mouse click at (%d, %d)\n", event.button.x, event.button.y);
			return;
			break;
		case SDL_KEYDOWN:
			//printf("Key pressed\n");
			switch(e.key.keysym.sym){
			case SDLK_ESCAPE:
				printf("Quitting now!\n");
				exit(0);
				break;
			case SDLK_s:
				return;
				break;
			/*case SDLK_w:
				return;
				break;*/
			case SDLK_a:
				return;
				break;
			case SDLK_d:
				return;
				break;
			case SDLK_q:
				return;
				break;
			case SDLK_e:
				return;
				break;
			/*case SDLK_SPACE:
				return PLACE;
				break;*/
			default:
				return;
				break;
			}
			break;
		default:
			return;
			//printf("Unknown event!\n");
			break;
		}
	}
	return;
}
