namespace Utilities {
	class Color {
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
}