namespace Platformer {
	class Game {
	public:
		Game() {};
		void Start() {
			bool running = true;
			Initialize();
			while (running) {
				//Draw
				Draw();
				running = false;
			}


			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Hack to get window to stay up
			SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }

			Quit();

		}
	private:
		//Global variables
		//Screen dimension constants
		const int SCREEN_WIDTH = 640;
		const int SCREEN_HEIGHT = 480;

		//The window we'll be rendering to
		SDL_Window* window = NULL;

		//The surface contained by the window
		SDL_Surface* screenSurface = NULL;

		//GameObjects
		Player player;
		GameObject floor;


		void Initialize() {
			//Initialize SDL and Create window
			window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (SDL_Init(SDL_INIT_VIDEO) < 0 || window == NULL)
			{
				printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
				return;
			}
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Game Objects
			player = Player(Vector2D(0, 0), 50, 50, Color(255, 255, 0));
			floor = GameObject(Vector2D(0, 500), 500, 100, Color(0, 100, 100));

		}

		void Update() {
			//Called every frame updates the game
		}

		void Draw() {
			SDL_FreeSurface(screenSurface);
			//Draw Background
			SDL_PixelFormat* format = (screenSurface->format);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 32, 255, 255));
			player.Draw(screenSurface);
			floor.Draw(screenSurface);
			SDL_Rect rect;
			int x = 0;
			rect.x = 0;
			rect.y = 0;
			rect.h = 1;
			rect.w = 1;
			SDL_Rect* rectP = &rect;
			SDL_FillRect(screenSurface, rectP, SDL_MapRGB(screenSurface->format, 0, 0, 0));
			SDL_Delay(10);
		}
		void Quit() {
			//Destroy window
			SDL_DestroyWindow(window);
			//Quit SDL subsystems
			SDL_Quit();
		}
	};
}