#pragma once
class Vector2
{
	class Vector2D {
	private:
		int x;
		int y;
	public:
		Vector2D() {
			x = 0;
			y = 0;
		};
		Vector2D(int x, int y) {
			this->x = x;
			this->y = y;
		};
		int X() { return x; };
		int Y() { return y; };
		void Position(int x, int y) {
			this->x = x;
			this->y = y;
		};
	};

};

