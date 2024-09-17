#include "celltable.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#define START_HUNGRY 50
#define MAX_AGE  1024
#define HUNGRY_UP 100


int mod(int a, int b){
	return (a % b + b) % b;
}

int rotate_to_x(int n){
	switch (n) {
		case 0:
			return 1;
		case 2:
			return -1;
	}
	return 0;
}


int rotate_to_y(int n){
	return abs(abs(rotate_to_x(n)) - 1);
}


int count_gen(commands* gens, commands command, int size){
	int ans = 0;
	for (int i = 0; i < size; i++){
		if (gens[i] == command) ans += 1;
	}
	return ans * 100;
}


CellTable::CellTable(sf::RenderWindow *win){
	window = win;
}

void CellTable::draw(){
	int red=0, green=0, blue=0;
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++)
		{
			if (cells[i][j] != nullptr) {
				red = count_gen(cells[i][j]->gens, commands::eat, cells[i][j]->max_ticks) % 256;
				green = count_gen(cells[i][j]->gens, commands::gen_e, cells[i][j]->max_ticks) % 256;
				blue = count_gen(cells[i][j]->gens, commands::sex, cells[i][j]->max_ticks) % 256;
				sf::RectangleShape rectangle(sf::Vector2f(10.f, 10.f));
				rectangle.setFillColor(sf::Color(red, green, blue));
				rectangle.setPosition(sf::Vector2f(i * 10.f, j * 10.f));
				window->draw(rectangle);
			}
		}
	}
}


void CellTable::update(){
	cell* new_cell;
	int r_number;
	cell* current_cell;
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++){
			if (cells[i][j] == nullptr) continue;
			if (cells[i][j]->age <= 0 || cells[i][j]->hungry <= 0){
				delete cells[i][j];
				cells[i][j] = nullptr;
				continue;
			}
			current_cell = cells[i][j];
			switch (cells[i][j]->gens[cells[i][j]->current_tick]) {
				case commands::up:
					if (cells[i][mod(j - 1, 100)] == nullptr){
						cells[i][mod(j - 1, 100)] = cells[i][j];
						cells[i][j] = nullptr;
						
					}
					break;
				case commands::down:
					if (cells[i][(j + 1) % 100] == nullptr){
						cells[i][(j + 1) % 100] = cells[i][j];
						cells[i][j] = nullptr;
					}
					break;
				case commands::left:
					if (cells[mod(i - 1, 100)][j] == nullptr){
						cells[mod(i - 1, 100)][j] = cells[i][j];
						cells[i][j] = nullptr;
					}
					break;
				case commands::right:
					if (cells[(i + 1) % 100][j] == nullptr){
						cells[(i + 1) % 100][j] = cells[i][j];
						cells[i][j] = nullptr;
					}
					break;
				case commands::gen_e:
					cells[i][j]->hungry += HUNGRY_UP;
					break;
				case commands::r_left:
					cells[i][j]->rotation = mod(cells[i][j]->rotation - 1, 4);
					break;
				case commands::r_right:
					cells[i][j]->rotation = (cells[i][j]->rotation + 1) % 4;
					break;
				case commands::eat:
					if (cells[mod(i + rotate_to_x(cells[i][j]->rotation), 100)][mod(i + rotate_to_y(cells[i][j]->rotation), 100)] != nullptr){
						cells[i][j]->hungry += cells[mod(i + rotate_to_x(cells[i][j]->rotation), 100)][mod(i + rotate_to_y(cells[i][j]->rotation), 100)]->hungry % 2;
						delete cells[mod(i + rotate_to_x(cells[i][j]->rotation), 100)][mod(i + rotate_to_y(cells[i][j]->rotation), 100)];
						cells[mod(i + rotate_to_x(cells[i][j]->rotation), 100)][mod(i + rotate_to_y(cells[i][j]->rotation), 100)] = nullptr;
					}
					break;
				case commands::sex:
					if (cells[i][j]->hungry < 100) break;
					cells[i][j]->hungry -= 50;
					if (cells[mod(i + rotate_to_x(cells[i][j]->rotation), 100)][mod(i + rotate_to_y(cells[i][j]->rotation), 100)] == nullptr){
						new_cell = new cell;
						std::memcpy(new_cell->gens, cells[i][j]->gens, 1024);
						new_cell->current_tick = 0;
						new_cell->max_ticks = cells[i][j]->max_ticks;
						new_cell->hungry = START_HUNGRY;
						new_cell->rotation = 0;
						new_cell->age = cells[i][j]->max_ticks * 2;
						r_number = rand();
						if (r_number % 3 == 0 && new_cell->max_ticks < 1024){
							new_cell->gens[new_cell->max_ticks] = static_cast<commands>(rand() % 10);
							new_cell->max_ticks += 1;
							new_cell->age += 2;
						}
						else if (r_number % 3 == 1){
							new_cell->gens[rand() % new_cell->max_ticks] =static_cast<commands>(rand() % 10);
						}
						cells[mod(i + rotate_to_x(cells[i][j]->rotation), 100)][mod(i + rotate_to_y(cells[i][j]->rotation), 100)] = new_cell;
					}
					break;
			}
			current_cell->current_tick = mod(current_cell->current_tick + 1, current_cell->max_ticks);
			current_cell->hungry--;
			current_cell->age--;
		}
	}
}


void CellTable::rand_cells(){
	cell *current_cell;
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 100; j++)
		{
			if (rand() % 2 == 1){
				current_cell = new cell;
				current_cell->hungry = START_HUNGRY;
				current_cell->current_tick = 0;
				current_cell->rotation = 0;
				current_cell->max_ticks = 5;
				current_cell->age = 10;

				for (int tick = 0; tick < 5; tick++) current_cell->gens[tick] = static_cast<commands>(rand() % 10);

				cells[i][j] = current_cell;
			}
		}
	}
}


