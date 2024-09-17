#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Window.hpp"
#include <SFML/Graphics.hpp>


enum class commands: char{
	up, down, left, right,
	r_left, r_right,
	eat, gen_e, sex
};


typedef struct{
	commands gens[1024];
	int hungry;
	char rotation;
	int current_tick;
	int max_ticks;
	int age;
} cell;


class CellTable{
	public:
		CellTable(sf::RenderWindow *win);
		void draw();
		void rand_cells();
		void update();
	private:
		sf::RenderWindow *window;
		cell *cells[100][100] = {};
};

