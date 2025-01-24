/*This source code copyrighted by Lazy Foo' Productions 2004-2024 and may not be redistributed without written permission.*/

//Libraries
//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

//#include <string>
#include <vector>

//#include <tmxlite/Map.hpp>
//#include <tmxlite/Layer.hpp>
//#include <tmxlite/TileLayer.hpp>
//#include <tmxlite/ObjectGroup.hpp>


//Enumrarators
	enum GameState {
		MainMenu,
		Running,
		Win,
		Lose,
	};

	enum Direction {
		Up,
		Down,
		Left,
		Right,
		Stationary,
		UpLeft,
		UpRight,
		DownLeft,
		DownRight,
	};
	//Classes

	//-Utilites
	class Vector2D {
	private:
		float x, y;
	public:
		Vector2D() {};
		Vector2D(float x, float y) {
			this->x = x;
			this->y = y;
		};
		void SetVector2D(float x, float y) {
			this->x = x;
			this->y = y;
		}
		void IncrementX(float x) {
			this->x += x;
		}
		void IncrementY(float y) {
			this->y += y;
		}
		float X() { return x; };
		float Y() { return y; };
	};

	class Color {//Stores rgb values for the game objects colour
		int r;
		int g;
		int b;
	public:
		Color() {};
		Color(int r, int g, int b) {
			this->r = r;
			this->g = g;
			this->b = b;
		};
		void SetColor(int r, int g, int b) {
			this->r = r;
			this->g = g;
			this->b = b;
		};
		void SetColor(Color color) {
			this->r = color.r;
			this->g = color.g;
			this->b = color.b;
		};

		int R() { return r; };
		int G() { return g; };
		int B() { return b; };
	};

	class Sprite {//Handles everything to do with rendering
	private:
		SDL_Rect rect;
		SDL_Surface* image = nullptr;
		SDL_Texture* texture = nullptr;
		SDL_Renderer* renderer = nullptr;
		int spriteSize;
		int spriteTileHeight;
		int spriteTileWidth;
		float animationSprite;
		SDL_RendererFlip flip;
		const float spriteOffsetX = 10;
		const float spriteOffsetY = 4;
	public:
		SDL_Rect* rectPointer = nullptr;
		Color color;
		Sprite() {};
		Sprite(int x, int y, int h, int w, Color color, SDL_Renderer* sdlRenderer, int spriteSize, int spriteTileHeight, int spriteTileWidth) {
			rect.x = x;
			rect.y = y;
			rect.h = h;
			rect.w = w;	
			rectPointer = &rect;
			this->color = color;
			renderer = sdlRenderer;
			this->spriteSize = spriteSize;
			this->spriteTileHeight = spriteTileHeight;
			this->spriteTileWidth = spriteTileWidth;
		};
		~Sprite() {

		}

		void UpdateSpritePosition(float x, float y) {
			rect.x = x;
			rect.y = y;
		}

		float AnimationSprite() {
			return animationSprite;
		}

		void AnimationSprite(int animationSprite) {
			this->animationSprite = animationSprite;
		}

		void IncrementAnimationSprite(float value) {
			this->animationSprite += value;
		}

		void Flip(bool value) {
			if (value) {
				flip = SDL_FLIP_HORIZONTAL;
				return;
			}
			flip = SDL_FLIP_NONE;
		}

		void Draw() {
			if (this->image != nullptr) {
				SDL_Rect sourceRect;
				sourceRect.x = (((int)animationSprite % spriteTileWidth) * spriteSize)+spriteOffsetX;
				sourceRect.y = (((int)animationSprite / spriteTileHeight) * spriteSize)+spriteOffsetY;
				sourceRect.h = spriteSize-(spriteOffsetY*2);
				sourceRect.w = spriteSize-(spriteOffsetX*2);

				SDL_RenderCopyEx(renderer, texture, &sourceRect, rectPointer, NULL, NULL, flip);

				//SDL_RenderCopy();
				return;
			}
			//rectPointer = &rect;
			//SDL_FillRect(screenSurface, rectPointer, SDL_MapRGB(screenSurface->format, color.R(), color.G(), color.B()));
		};

		void SetImage(const char* file) {
			this->image = IMG_Load(file);
			if (this->image == nullptr) {
				std::cout << "Sprite Image Error \n";
				return;
			}
			texture = SDL_CreateTextureFromSurface(renderer, image);
		};
	};

	class Collider {
	private:
		SDL_Rect* rect;
	public:
		Collider() {};
		Collider(SDL_Rect* rect) {
			this->rect = rect;

		}

		void ResetCollider(SDL_Rect* rect) {
			this->rect = rect;
		}

		bool isColliding(SDL_Rect* other) {
			//Checks collision with another rectangle
			if ((((rect->x + rect->w) > other->x) && (rect->x < (other->x + other->w))) && ((rect->y + rect->h) > other->y) && (rect->y < (other->y + other->h))) {
				return true;
			}
			/*if (((rect.x + rect.w) > other->x) && (rect.x < (other->x + other->w))) {
				std::cout << "Horizontal Collision" << std::endl;
			}
			if ((rect.x + rect.w) > other->x) {
				std::cout << "TR vs TL" << std::endl;
			}
			if (rect.x < (other->x + other->w)) {
				std::cout << "TL vs TR" << std::endl;
			}*/
			//std::cout << "X" << rect->x << std::endl;
			//std::cout << "X" << rect->w << std::endl;
				//std::cout << "X" << rect->x+rect->w << std::endl;
				//std::cout << "X" << other->x << std::endl;
			
			/*if (((rect.y + rect.h) > other->y) && (rect.y < (other->y + other->h))) {
				std::cout << "Vertical Collision" << std::endl;
			}*/
			
			return false;
		}
		Direction ColliderType(SDL_Rect* other) {

			////source: https://gamedev.stackexchange.com/questions/29786/a-simple-2d-rectangle-collision-algorithm-that-also-determines-which-sides-that
			float w = 0.5 * (rect->w + other->x);
			float h = 0.5 * (rect->h + other->h);
			float centreX = 0.5 * (rect->x + rect->w);
			float centreY = 0.5 * (rect->y + rect->h);
			float oCentreX = 0.5 * (other->x + other->w);
			float oCentreY = 0.5 * (other->y + other->h);
			float dx = centreX - oCentreX;
			float dy = centreY - oCentreY;

			if (abs(dx) <= w && abs(dy) <= h)
			{
				float wy = w * dy;
				float hx = h * dx;

				if (wy > hx) {
					if (wy > -hx) {
						//top collision
						return Up;
					}
					else {
						//left collision
						return Left;
					}
				}
				else {
					if (wy > -hx) {
						//right collision
						return Right;
					}
					else {
						//bottom collision
						return Down;
					}
				}
			}
		}
	};

	class SoundPlayer {
	private:
	public:
		SoundPlayer() {
			//do stuff
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1) {
				printf("audio mixer error", SDL_GetError());
			}
			//std::stringstream name;
		}

		void PlayMusic(const char* file) {
			//play
			Mix_PlayMusic(Mix_LoadMUS(file), -1);
		}
		void PlayEffect(const char* file) {
			//play
			Mix_PlayChannel(-1, Mix_LoadWAV(file), 0);
		}
	};

	/*class GameObjectList {
	private:
		int length;
		GameObject *list;
	public:
		GameObjectList(int size) {
			*list = new GameObject[size];
			length = size;
		}
		void SetItem(int position, GameObject item) {
			*list = item;
		}
		void AddItem(GameObject item) {
			GameObject* temp = list;
			length++;
			list = new GameObject[length];
			list = temp;
			list[length] = item;
			delete temp;
		}
		GameObject GetItem(int position) {
			return list[position];
		}
	};*/

	//-Base Class
	class GameObject {
	private:
		Collider collider;
	public:
		Vector2D position;
		Sprite* sprite;
		GameObject() {};
		GameObject(Vector2D position, int height, int width, Color color, SDL_Renderer* sdlrenderer, int spriteSize, int tileHeight, int tileWidth) {
			sprite = new Sprite(position.X(), position.Y(), height, width, color, sdlrenderer,spriteSize,tileHeight,tileWidth);
			this->position = position;
			collider = Collider(sprite->rectPointer);
		};

		~GameObject() {
			//delete sprite;
		}

		bool RunCollider(GameObject other) {
			collider.ResetCollider(sprite->rectPointer);
			if (collider.isColliding(other.sprite->rectPointer)) {
				//std::cout << "X difference: " << (this->sprite->rectPointer->x - other.sprite->rectPointer->x) << std::endl;
				//std::cout << "Y difference: " << (this->sprite->rectPointer->y - other.sprite->rectPointer->y) << std::endl;
				return true;;
			};
			//std::cout << "No Collision \n";
			//std::cout << "X: " << (sprite->rectPointer->x) << std::endl;
			//std::cout << "y: " << sprite->rectPointer->y << std::endl;
			//std::cout << "X: " << (other.sprite->rectPointer->x) << std::endl;
			//std::cout << "y: " << other.sprite->rectPointer->y << std::endl;
			return false;
		};

		Direction CollisionType(GameObject other) {
			return collider.ColliderType(other.sprite->rectPointer);
		}
	};

	//--GameObject Types
	class Player : public GameObject {
	private:
		int hp = 0;
		float speed = 1;
		Direction direction;
		
		void UpdateSprite() {
			sprite->UpdateSpritePosition(position.X(), position.Y());
		};

		void RunAnimation() {
			if (sprite->AnimationSprite() > 31 || sprite->AnimationSprite() < 16) {
				sprite->AnimationSprite(16);
				return;
			}
			sprite->IncrementAnimationSprite(0.025);
		}
	public:
		Player() {};
		Player(Vector2D position, int height, int width, Color color, SDL_Renderer* sdlrenderer) {
			sprite = new Sprite(position.X(), position.Y(), height, width, color, sdlrenderer, 32, 8, 8);
			this->position = position;
			sprite->SetImage("assets/player.png");
		};
		float animationSprite;
		float gravity = 0;
		int GetHealth() { return hp; };
		void SetHealth(int x) { hp = x; };
		void SetDirection(Direction direction) { this->direction = direction; };
		bool CanJump;
		void Move() {
			switch (direction) {
				case Up:
					position.IncrementY(-speed);
					break;
				case Down:
					position.IncrementY(speed);
					break;
				case Left:
					position.IncrementX(-speed);
					RunAnimation();
					//flip sprite
					//if (this->sprite->rectPointer->w > 0) { this->sprite->rectPointer->w = -this->sprite->rectPointer->w; }
					sprite->Flip(true);
					break;
				case Right:
					position.IncrementX(speed);
					RunAnimation();
					//this->sprite->rectPointer->w = abs(this->sprite->rectPointer->w);
					sprite->Flip(false);
					break;
				case UpLeft:
					position.IncrementY(-speed);
					position.IncrementX(-speed);
					break;
				case UpRight:
					position.IncrementY(-speed);
					position.IncrementX(speed);
					break;
				case DownLeft:
					position.IncrementY(speed);
					position.IncrementX(-speed);
					break;
				case DownRight:
					position.IncrementY(speed);
					position.IncrementX(speed);
					break;
				default:
					//Player stationary
					if (sprite->AnimationSprite() < 3) {
						sprite->IncrementAnimationSprite(0.005);
						break;
					}
					sprite->AnimationSprite(0);
					break;
			};
			UpdateSprite();
		};
	};

	class TiledMap {
	public:
		TiledMap(SDL_Renderer* sdlRenderer, std::string tiledMapFilename, int mapHeight, int mapWidth, std::vector<std::vector<int>> &map, int tileSize) {
			renderer = sdlRenderer;
			filename = tiledMapFilename;
			this->mapHeight = mapHeight;
			this->mapWidth = mapWidth;
			this->map = &map;
			this->tileSize = tileSize;
		}

		void init() {
			SDL_Surface* image = IMG_Load(filename.c_str());
			texture = SDL_CreateTextureFromSurface(renderer, image);
			SDL_FreeSurface(image);
			MakeTilemapCollider();
			tileOffsetX = 0;
			tileOffsetY = 0;
		}

		void RunCollider(Player* player) {
			//std::cout << "ooooh " << mapCollider.size() << "\n";
			for (int i = 0; i < mapCollider.size(); i++) {
				if (!mapCollider[i].RunCollider(*player)) { continue; }
				switch (mapCollider[i].CollisionType(*player)) {
					case Up:
						//turn off gravity
						//std::cout << "player up\n";
						player->gravity = 0;
						player->CanJump = true;
						break;
					case Down:
						//push player up?
						//std::cout << "player down\n";
						player->position.IncrementY(1);
						break;
					case Left:
						//push player left
						//std::cout << "player left\n";
						player->position.IncrementX(3);
						break;
					case Right:
						//push player right
						//std::cout << "player right\n";
						player->position.IncrementX(-3);
						break;
					default:
						//Do nothing
						break;
				}
			}
		}

		bool FinishLineCheck(Player* player) {
			for (int i = 0; i < winCollider.size(); i++) {
				if (winCollider[i].RunCollider(*player)) { std::cout << "Win State";  return true; }
			}
			return false;
		}

		void render() {
			//std::cout << "mapHeight: "<<mapWidth<<"\n";
			int mapPointer = 0;
			int winPointer = 0;
			int currentTile;
			for (int i = 0; i < mapHeight; i++) {
				for (int j = 0; j < mapWidth; j++) {
					currentTile = (*map)[i][j];
					SDL_Rect sourceRect;
					sourceRect.x = (currentTile % 25) * sourceTileSize;
					sourceRect.y = (currentTile / 25) * sourceTileSize;
					sourceRect.h = sourceTileSize;
					sourceRect.w = sourceTileSize;

					SDL_Rect renderRect;
					renderRect.x = (j * tileSize)+tileOffsetX;
					renderRect.y = (i * tileSize)+tileOffsetY;
					renderRect.h = tileSize;
					renderRect.w = tileSize;

					SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);

					//Update Collider
					if (currentTile == -1) { continue; }
					else if (currentTile == 261) {
						winCollider[winPointer].sprite->UpdateSpritePosition(renderRect.x, renderRect.y);
						winPointer++;
						continue;
					}
					mapCollider[mapPointer].sprite->UpdateSpritePosition(renderRect.x, renderRect.y);
					mapPointer++;
				}
			}
		}
		void clean() {
			//
			map->clear();
			mapCollider.clear();
			winCollider.clear();
		}

		void Move(Vector2D position) {
			tileOffsetX = position.X();
			tileOffsetY = position.Y();
			//MakeTilemapCollider();
		}
	private:
		int tileSize;
		const int sourceTileSize = 16;

		std::string filename;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		int mapHeight;
		int mapWidth;
		std::vector < std::vector<int>>* map;
		std::vector<GameObject> mapCollider;
		std::vector<GameObject> winCollider;

		float tileOffsetX;
		float tileOffsetY;

		void MakeTilemapCollider() {
			mapCollider.clear();
			winCollider.clear();
			for (int i = 0; i < mapHeight; i++) {
				for (int j = 0; j < mapWidth; j++) {
					if ((*map)[i][j] == -1) { continue; }
					else if ((*map)[i][j] == 261) {
						winCollider.push_back(GameObject(Vector2D(((j * tileSize) + tileOffsetX), ((i * tileSize) + tileOffsetY)), tileSize, tileSize, Color(), NULL, NULL, NULL, NULL));
						continue;
					}
					mapCollider.push_back(GameObject(Vector2D(((j * tileSize) + tileOffsetX), ((i * tileSize) + tileOffsetY)), tileSize, tileSize, Color(), NULL, NULL, NULL, NULL));
				}
			}
		}
	};

	class Enemy : GameObject {
	private:
		Collider collider;
		Collider head;
		Direction direction;
		float speed;
	public:
		Enemy(Vector2D position, int height, int width, Color color, SDL_Renderer* sdlrenderer, int spriteSize, int tileHeight, int tileWidth) {
			sprite = new Sprite(position.X(), position.Y(), height, width, color, sdlrenderer, spriteSize, tileHeight, tileWidth);
			this->position = position;
			this->collider = Collider(sprite->rectPointer);
			direction = Left;
			speed = 0.8;
		};

		void Move() {
			//
		};

		void Destroy() {
			//
		}
	};

	class EnemyContainer {
	private:
		std::vector<Enemy> enemies;
		const int tileSize = 16;
		float tileOffsetX;
		float tileOffsetY;
	public:
		EnemyContainer(std::vector < std::vector<int>>* map, int mapHeight, int mapWidth) {
			//generate enemies
			for (int i = 0; i < mapHeight; i++) {
				for (int j = 0; j < mapWidth; j++) {
					if ((*map)[i][j] == 100) {
						enemies.push_back(Enemy(Vector2D(((j * tileSize) + tileOffsetX), ((i * tileSize) + tileOffsetY)), tileSize, tileSize, Color(), NULL, NULL, NULL, NULL));
					}
				}
			}
		}
		

	};


	//-Main Game
	class Game {
	public:
		Game() {
			running = true;
		};
		void Start() {
			Initialize();
			while (running) {//Game Loop
				Update();
				Draw();
			}
			Quit();
		};
	private:
		bool running;
		//Global variables
		//Screen dimension constants
		const int SCREEN_WIDTH = 1920;
		const int SCREEN_HEIGHT = 1080;

		//The window we'll be rendering to
		SDL_Window* window = nullptr;

		//The surface contained by the window
		//DL_Surface* screenSurface = nullptr	;

		//Allows the drawwing of pixels on a window
		SDL_Renderer* renderer = nullptr;

		//Physics
		//float gravity;
		float maxVelocity;

		//GameObjects
		Player* player = nullptr;
		//Player playerCentre;
		//GameObject* floor;
		GameObject* grass;

		std::unique_ptr<TiledMap> tiledMap;

		std::ifstream level1Map;
		std::string mapContent;
		int mapHeight;
		int mapWidth;
		std::vector<std::vector<int>> map = { {} };

		float worldOffsetX;
		float worldOffsetY;

		SoundPlayer* soundPlayer;

		GameObject* background1;
		GameObject* background2;
		GameObject* background3;
		GameObject* background4;

		void Initialize() {
			//Initialize SDL and Create window
			window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (SDL_Init(SDL_INIT_VIDEO) < 0 || window == NULL)
			{
				printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
				return;
			}

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			//Get window surface
			//screenSurface = SDL_GetWindowSurface(window);

			//Physics
			//gravity = 0;
			maxVelocity = 0.5;


			//Game Objects //0.15
			player = new Player(Vector2D(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25), SCREEN_HEIGHT*0.05, SCREEN_HEIGHT*0.05, Color(132, 37, 147), renderer);
			//playerCentre = Player(Vector2D(SCREEN_WIDTH / 2 - 25+5, SCREEN_HEIGHT / 2 - 25+5), 40, 40, Color(0, 0, 0));
			//floor = new GameObject(Vector2D(-100, 350), 300, 5000, Color(139, 69, 19), renderer);
			//grass = new GameObject(Vector2D(-100, 350), 10, 5000, Color(124, 252, 100), renderer);

			

			mapContent = "";
			level1Map.open("assets/Level1.csv");
			mapHeight = 0;
			while (std::getline(level1Map, mapContent)) {
				//map[mapHeight].push_back()
				std::string temp;
				mapWidth = 0;
				for (int i = 0; i < mapContent.length(); i++) {
					if (mapContent[i] != ',') { temp += mapContent[i];  continue; }
					//std::cout << temp << ",";
					map[mapHeight].push_back(stoi(temp));
					temp = "";
					mapWidth++;
				}
				//std::cout << "\n\n";
				mapHeight++;
				map.push_back({});
			}
			
			tiledMap = std::unique_ptr<TiledMap>(new TiledMap(renderer, "assets/Assets.png", mapHeight, mapWidth, map, SCREEN_HEIGHT*0.050));
			tiledMap->init();

			soundPlayer = new SoundPlayer();
			soundPlayer->PlayMusic("assets/background.mp3");

			background1 = new GameObject(Vector2D(0, 0), SCREEN_HEIGHT, SCREEN_WIDTH, Color(), renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
			background1->sprite->SetImage("assets/Background_1.png");
			background2 = new GameObject(Vector2D(0, 0), SCREEN_HEIGHT, SCREEN_WIDTH, Color(), renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
			background2->sprite->SetImage("assets/Background_2.png");
			background3 = new GameObject(Vector2D(SCREEN_WIDTH, 0), SCREEN_HEIGHT, SCREEN_WIDTH, Color(), renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
			background3->sprite->SetImage("assets/Background_1.png");
			background4 = new GameObject(Vector2D(SCREEN_WIDTH, 0), SCREEN_HEIGHT, SCREEN_WIDTH, Color(), renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH);
			background4->sprite->SetImage("assets/Background_2.png");
		};

		void Input() {
			SDL_Event event;
			const Uint8* keyboardState = SDL_GetKeyboardState(0);
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]) {
					running = false;
				}
				else if (keyboardState[SDL_SCANCODE_A]) {
					//std::cout << "Player Left" << std::endl;
					player->SetDirection(Left);
				}
				else if (keyboardState[SDL_SCANCODE_D]) {
					//std::cout << "Player Right" << std::endl;
					player->SetDirection(Right);
				}
				else {
					//std::cout << "Player Stationary" << std::endl;
					player->SetDirection(Stationary);
				}
				if (keyboardState[SDL_SCANCODE_SPACE] && player->CanJump) {
					player->position.IncrementY(-80);
					//gravity = -4;
					player->CanJump = false;
					soundPlayer->PlayEffect("assets/jump.mp3");
					//sound
					//std::cout << "Jump" << "\n";
				};
				/*if (keyboardState[SDL_SCANCODE_P]) {
					std::cout << "Player Position reset\n";
					player->position.SetVector2D(SCREEN_WIDTH / 2, 0);
				}*/
			};
		};

		void CheckCollisions() {
			//player.RunCollider(floor);
			//if (player->RunCollider(*grass)) { gravity = 0; player->CanJump = true; }
			tiledMap->RunCollider(player);
		};

		void Update() {
			//Called every frame updates the game
			player->position.IncrementY(player->gravity);
			if (player->gravity < maxVelocity) { player->gravity += 0.0025; }
			Input();
			player->Move();
			CheckCollisions();

			if (player->position.X() < (SCREEN_WIDTH * 0.25) && worldOffsetX < 0) {
				worldOffsetX++;
				player->position.IncrementX(1);
			}
			else if (player->position.X() > (SCREEN_WIDTH * 0.75) && worldOffsetX > -8800) {
				worldOffsetX--;
				player->position.IncrementX(-1);
			}
			//std::cout << worldOffsetX << "\n";
			tiledMap->Move(Vector2D(worldOffsetX, worldOffsetY));
			//background1->sprite->UpdateSpritePosition((int)(worldOffsetX * 0.5)%SCREEN_WIDTH, 0);
			//std::cout<<"screens"<<((int)worldOffsetX / SCREEN_WIDTH)<<"\n";
			background1->sprite->UpdateSpritePosition((worldOffsetX * 0.5) - (SCREEN_WIDTH * ((int)(worldOffsetX / SCREEN_WIDTH/2)))*2, 0);
			background2->sprite->UpdateSpritePosition((worldOffsetX * 0.25) - (SCREEN_WIDTH * ((int)(worldOffsetX / SCREEN_WIDTH / 4))) * 2, 0);
			background3->sprite->UpdateSpritePosition((SCREEN_WIDTH + (worldOffsetX * 0.5)) - (SCREEN_WIDTH * ((int)(worldOffsetX / SCREEN_WIDTH / 6))) * 2, 0);
			background4->sprite->UpdateSpritePosition((SCREEN_WIDTH + (worldOffsetX * 0.25)) - (SCREEN_WIDTH * ((int)(worldOffsetX / SCREEN_WIDTH / 16))) * 2, 0);
			
			//std::cout << "Player: " << player->position.X() << " " << player->position.Y() << "\n";

			if (tiledMap->FinishLineCheck(player)) {
				running = false;
			}
		};

		void Draw() {
			SDL_RenderClear(renderer);
			//SDL_FreeSurface(screenSurface);
			//Draw Background
			//SDL_PixelFormat* format = (screenSurface->format);
			//SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 191, 255));
			
			//playerCentre.sprite.Draw(screenSurface);
			//floor->sprite->Draw(screenSurface);
			//grass->sprite->Draw(screenSurface);

			//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			//SDL_RenderFillRect(renderer, nullptr);

			background2->sprite->Draw();
			background4->sprite->Draw();
			
			background1->sprite->Draw();
			background3->sprite->Draw();

			tiledMap->render();
			player->sprite->Draw();

			//Updates the window
			SDL_RenderPresent(renderer);
			

			//Update the surface
			//SDL_UpdateWindowSurface(window);
		};

		void Quit() {
			tiledMap->clean();
			//Destroy window
			SDL_DestroyWindow(window);
			delete player;
			//Quit SDL subsystems
			SDL_Quit();
		};
	};

	//Start
	int main(int argc, char* args[])
	{
		Game game;
		game.Start();
		return 0;
	};