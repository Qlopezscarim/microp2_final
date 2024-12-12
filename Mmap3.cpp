#include <iostream>
#include <cstdint>
#include <vector>
#include "Mmap.h"









void Mmap::Pokeball(uint16_t x_start, uint16_t y_start, float scale)
{
	// Draw the white semicircle (bottom half)
        add_list(1, x_start, y_start+(20*scale), 40*scale, 20*scale, 0xFFFF, 0); // White: 0xFFFF

        // Draw the red semicircle (top half)
        add_list(1, x_start, y_start, 40*scale, 20*scale, 0xF800, 0); // Red: 0xF800

        // Draw the black horizontal stripe
        add_list(1, x_start, y_start+(18*scale), 40*scale, 4*scale, 0x0000, 0); // Black: 0x0000

        // Draw the outer black circle (button outline)
        add_list(1, x_start+(15*scale), y_start+(15*scale), 10*scale, 10*scale, 0x0000, 0); // Black: 0x0000

        // Draw the inner white circle
        add_list(1, x_start+(17*scale), y_start+(17*scale), 6*scale, 6*scale, 0xFFFF, 0); // White: 0xFFFF
}

void Mmap::Trainer(uint16_t x_start, uint16_t y_start, float scale)
{
    // Draw the head (cap)
    add_list(1, x_start + (12 * scale), y_start, 16 * scale, 12 * scale, 0xF800, 2); // Red cap: 0xF800 (head slightly forward)

    // Draw the shoulders (overlap body for slight tilt effect)
    add_list(1, x_start + (8 * scale), y_start + (16 * scale), 24 * scale, 8 * scale, 0x8410, 2); // Gray shoulders: 0x8410

    // Draw the backpack
    add_list(1, x_start + (10 * scale), y_start + (10 * scale), 20 * scale, 16 * scale, 0x07E0, 2); // Green backpack: 0x07E0 (slightly angled back)

    // Draw the body (blue jacket)
    add_list(1, x_start + (8 * scale), y_start + (18 * scale), 24 * scale, 30 * scale, 0x001F, 2); // Blue body: 0x001F

    // Draw the left arm (human skin tone)
    add_list(1, x_start, y_start + (20 * scale), 8 * scale, 24 * scale, 0xF7DA, 2); // Corrected skin tone left arm: 0xFAD6

    // Draw the right arm (human skin tone)
    add_list(1, x_start + (32 * scale), y_start + (20 * scale), 8 * scale, 24 * scale, 0xF7DA, 2); // Corrected skin tone right arm: 0xFAD6
}

/*void Mmap::Charizard(uint16_t x_start, uint16_t y_start, float scale)
{
    // Head (Orange)
    add_list(1, x_start + (20 * scale), y_start, 20 * scale, 18 * scale, 0xFC00, 0); // Orange head: 0xFC00
    add_list(1, x_start + (26 * scale), y_start + (4 * scale), 4 * scale, 6 * scale, 0x001F, 0); // Blue eye (left): 0x001F
    add_list(1, x_start + (34 * scale), y_start + (4 * scale), 4 * scale, 6 * scale, 0x001F, 0); // Blue eye (right): 0x001F
    add_list(1, x_start + (30 * scale), y_start + (12 * scale), 8 * scale, 4 * scale, 0x0000, 0); // Black mouth outline: 0x0000

    // Horns (Orange)
    add_list(1, x_start + (18 * scale), y_start - (6 * scale), 6 * scale, 8 * scale, 0xFC00, 0); // Left horn
    add_list(1, x_start + (36 * scale), y_start - (6 * scale), 6 * scale, 8 * scale, 0xFC00, 0); // Right horn

    // Neck (Orange)
    add_list(1, x_start + (24 * scale), y_start + (18 * scale), 12 * scale, 12 * scale, 0xFC00, 0); // Neck

    // Body (Orange)
    add_list(1, x_start + (10 * scale), y_start + (30 * scale), 40 * scale, 50 * scale, 0xFC00, 0); // Main body

    // Belly (Yellow)
    add_list(1, x_start + (15 * scale), y_start + (38 * scale), 30 * scale, 34 * scale, 0xFFE0, 0); // Yellow belly: 0xFFE0

    // Left Wing (Blue interior, black outline)
    add_list(1, x_start - (20 * scale), y_start + (10 * scale), 20 * scale, 60 * scale, 0x001F, 0); // Blue interior
    add_list(1, x_start - (22 * scale), y_start + (10 * scale), 2 * scale, 60 * scale, 0x0000, 0);  // Black outline

    // Right Wing (Blue interior, black outline)
    add_list(1, x_start + (50 * scale), y_start + (10 * scale), 20 * scale, 60 * scale, 0x001F, 0); // Blue interior
    add_list(1, x_start + (70 * scale), y_start + (10 * scale), 2 * scale, 60 * scale, 0x0000, 0);  // Black outline

    // Arms (Orange with claws)
    add_list(1, x_start, y_start + (40 * scale), 10 * scale, 20 * scale, 0xFC00, 0);  // Left arm
    add_list(1, x_start + (50 * scale), y_start + (40 * scale), 10 * scale, 20 * scale, 0xFC00, 0); // Right arm
    add_list(1, x_start + (2 * scale), y_start + (55 * scale), 8 * scale, 5 * scale, 0xFFFF, 0);    // Left claws (white)
    add_list(1, x_start + (52 * scale), y_start + (55 * scale), 8 * scale, 5 * scale, 0xFFFF, 0);   // Right claws (white)

    // Legs (Orange with yellow toes)
    add_list(1, x_start + (8 * scale), y_start + (80 * scale), 15 * scale, 30 * scale, 0xFC00, 0);  // Left leg
    add_list(1, x_start + (37 * scale), y_start + (80 * scale), 15 * scale, 30 * scale, 0xFC00, 0); // Right leg
    add_list(1, x_start + (10 * scale), y_start + (105 * scale), 12 * scale, 5 * scale, 0xFFE0, 0); // Left toes
    add_list(1, x_start + (39 * scale), y_start + (105 * scale), 12 * scale, 5 * scale, 0xFFE0, 0); // Right toes

    // Tail (Orange with flame)
    add_list(1, x_start + (25 * scale), y_start + (70 * scale), 10 * scale, 40 * scale, 0xFC00, 0); // Orange tail
    add_list(1, x_start + (28 * scale), y_start + (110 * scale), 6 * scale, 10 * scale, 0xF800, 0); // Red flame base
    add_list(1, x_start + (26 * scale), y_start + (118 * scale), 10 * scale, 6 * scale, 0xFFE0, 0); // Yellow flame highlight
}*/

/*void Mmap::Charizard(uint16_t x_start, uint16_t y_start, float scale)
{
    // Head (Orange)
    add_list(1, x_start + (20 * scale), y_start + (110 * scale), 20 * scale, 18 * scale, 0xFC00, 0); // Orange head: 0xFC00
    add_list(1, x_start + (26 * scale), y_start + (106 * scale), 4 * scale, 6 * scale, 0xF800, 0); // Red eye (left): 0xF800
    add_list(1, x_start + (34 * scale), y_start + (106 * scale), 4 * scale, 6 * scale, 0xF800, 0); // Red eye (right): 0xF800
    add_list(1, x_start + (30 * scale), y_start + (98 * scale), 8 * scale, 4 * scale, 0x0000, 0); // Black mouth outline: 0x0000
    // Horns (Orange)
    add_list(1, x_start + (18 * scale), y_start + (116 * scale), 6 * scale, 8 * scale, 0xFC00, 0); // Left horn
    add_list(1, x_start + (36 * scale), y_start + (116 * scale), 6 * scale, 8 * scale, 0xFC00, 0); // Right horn
    // Neck (Orange)
    add_list(1, x_start + (24 * scale), y_start + (92 * scale), 12 * scale, 12 * scale, 0xFC00, 0); // Neck
    // Body (Orange)
    add_list(1, x_start + (10 * scale), y_start + (40 * scale), 40 * scale, 50 * scale, 0xFC00, 0); // Main body
    // Belly (Yellow)
    add_list(1, x_start + (15 * scale), y_start + (52 * scale), 30 * scale, 34 * scale, 0xFFE0, 0); // Yellow belly: 0xFFE0
    // Left Wing (Red interior, black outline)
    add_list(1, x_start - (20 * scale), y_start + (50 * scale), 20 * scale, 60 * scale, 0xF800, 0); // Red interior
    add_list(1, x_start - (22 * scale), y_start + (50 * scale), 2 * scale, 60 * scale, 0x0000, 0);  // Black outline
    // Right Wing (Red interior, black outline)
    add_list(1, x_start + (50 * scale), y_start + (50 * scale), 20 * scale, 60 * scale, 0xF800, 0); // Red interior
    add_list(1, x_start + (70 * scale), y_start + (50 * scale), 2 * scale, 60 * scale, 0x0000, 0);  // Black outline
    // Arms (Orange with white claws)
    add_list(1, x_start, y_start + (60 * scale), 10 * scale, 20 * scale, 0xFC00, 0);  // Left arm
    add_list(1, x_start + (50 * scale), y_start + (60 * scale), 10 * scale, 20 * scale, 0xFC00, 0); // Right arm
    add_list(1, x_start + (2 * scale), y_start + (75 * scale), 8 * scale, 5 * scale, 0xFFFF, 0);    // Left claws (white)
    add_list(1, x_start + (52 * scale), y_start + (75 * scale), 8 * scale, 5 * scale, 0xFFFF, 0);   // Right claws (white)
    // Legs (Orange with yellow toes)
    add_list(1, x_start + (8 * scale), y_start + (0 * scale), 15 * scale, 30 * scale, 0xFC00, 0);  // Left leg
    add_list(1, x_start + (37 * scale), y_start + (0 * scale), 15 * scale, 30 * scale, 0xFC00, 0); // Right leg
    add_list(1, x_start + (10 * scale), y_start + (25 * scale), 12 * scale, 5 * scale, 0xFFE0, 0); // Left toes
    add_list(1, x_start + (39 * scale), y_start + (25 * scale), 12 * scale, 5 * scale, 0xFFE0, 0); // Right toes
    // Tail (Orange with flame)
    add_list(1, x_start + (25 * scale), y_start + (20 * scale), 10 * scale, 40 * scale, 0xFC00, 0); // Orange tail
    add_list(1, x_start + (28 * scale), y_start + (0 * scale), 6 * scale, 10 * scale, 0xF800, 0); // Red flame base
    add_list(1, x_start + (26 * scale), y_start + (-8 * scale), 10 * scale, 6 * scale, 0xFFE0, 0); // Yellow flame highlight
}*/

void Mmap::Charizard(uint16_t x_start, uint16_t y_start, float scale)
{
    // Colors using abstraction formula
    uint16_t orange = ST7789_BLUE * 0.0 + ST7789_GREEN * 0.4 + ST7789_RED * 0.6;
    uint16_t yellow = ST7789_BLUE * 0.0 + ST7789_GREEN * 0.9 + ST7789_RED * 0.9;
    uint16_t red = ST7789_BLUE * 0.0 + ST7789_GREEN * 0.2 + ST7789_RED * 0.8;
    uint16_t blue = ST7789_BLUE * 0.9 + ST7789_GREEN * 0.1 + ST7789_RED * 0.0;
    uint16_t black = ST7789_BLUE * 0.0 + ST7789_GREEN * 0.0 + ST7789_RED * 0.0;
    uint16_t white = ST7789_BLUE * 0.9 + ST7789_GREEN * 0.9 + ST7789_RED * 0.9;

    // Tail (Orange with flame)
    add_list(1, x_start + (25 * scale), y_start + (70 * scale), 10 * scale, 40 * scale, orange, 0); // Tail
    add_list(1, x_start + (28 * scale), y_start + (110 * scale), 6 * scale, 10 * scale, red, 0); // Flame base
    add_list(1, x_start + (26 * scale), y_start + (118 * scale), 10 * scale, 6 * scale, yellow, 0); // Flame highlight

    // Legs (Orange with yellow toes)
    add_list(1, x_start + (8 * scale), y_start + (80 * scale), 15 * scale, 30 * scale, orange, 0);  // Left leg
    add_list(1, x_start + (37 * scale), y_start + (80 * scale), 15 * scale, 30 * scale, orange, 0); // Right leg
    add_list(1, x_start + (10 * scale), y_start + (105 * scale), 12 * scale, 5 * scale, yellow, 0); // Left toes
    add_list(1, x_start + (39 * scale), y_start + (105 * scale), 12 * scale, 5 * scale, yellow, 0); // Right toes

    // Arms (Orange with claws)
    add_list(1, x_start - (2 * scale), y_start + (40 * scale), 12 * scale, 20 * scale, orange, 0);  // Left arm
    add_list(1, x_start + (50 * scale), y_start + (40 * scale), 12 * scale, 20 * scale, orange, 0); // Right arm
    add_list(1, x_start + (2 * scale), y_start + (55 * scale), 8 * scale, 5 * scale, white, 0);    // Left claws
    add_list(1, x_start + (52 * scale), y_start + (55 * scale), 8 * scale, 5 * scale, white, 0);   // Right claws

    // Wings (Blue interior, black outline)
    add_list(1, x_start - (20 * scale), y_start + (10 * scale), 20 * scale, 60 * scale, blue, 0);  // Left wing interior
    add_list(1, x_start - (22 * scale), y_start + (10 * scale), 2 * scale, 60 * scale, black, 0);   // Left wing outline
    add_list(1, x_start + (50 * scale), y_start + (10 * scale), 20 * scale, 60 * scale, blue, 0);  // Right wing interior
    add_list(1, x_start + (70 * scale), y_start + (10 * scale), 2 * scale, 60 * scale, black, 0);   // Right wing outline

    // Body (Orange with yellow belly)
    add_list(1, x_start + (10 * scale), y_start + (30 * scale), 40 * scale, 50 * scale, orange, 0); // Main body
    add_list(1, x_start + (15 * scale), y_start + (38 * scale), 30 * scale, 34 * scale, yellow, 0); // Belly

    // Neck (Orange)
    add_list(1, x_start + (24 * scale), y_start + (18 * scale), 12 * scale, 12 * scale, orange, 0); // Neck

    // Horns (Orange)
    add_list(1, x_start + (18 * scale), y_start - (6 * scale), 6 * scale, 8 * scale, orange, 0); // Left horn
    add_list(1, x_start + (36 * scale), y_start - (6 * scale), 6 * scale, 8 * scale, orange, 0); // Right horn

    // Head (Orange with blue eyes and black mouth)
    add_list(1, x_start + (20 * scale), y_start, 20 * scale, 18 * scale, orange, 0); // Head
    add_list(1, x_start + (26 * scale), y_start + (4 * scale), 4 * scale, 6 * scale, blue, 0); // Left eye
    add_list(1, x_start + (34 * scale), y_start + (4 * scale), 4 * scale, 6 * scale, blue, 0); // Right eye
    add_list(1, x_start + (30 * scale), y_start + (12 * scale), 8 * scale, 4 * scale, black, 0); // Mouth outline
}


void Mmap::Diglett(uint16_t x_start, uint16_t y_start, float scale)
{
    // Colors using abstraction formula
    uint16_t brown = ST7789_BLUE * 0.2 + ST7789_GREEN * 0.4 + ST7789_RED * 0.4;
    uint16_t pink = ST7789_BLUE * 0.8 + ST7789_GREEN * 0.5 + ST7789_RED * 0.7;
    uint16_t black = ST7789_BLUE * 0.0 + ST7789_GREEN * 0.0 + ST7789_RED * 0.0;
    uint16_t white = ST7789_BLUE * 0.9 + ST7789_GREEN * 0.9 + ST7789_RED * 0.9;
    uint16_t gray = ST7789_BLUE * 0.5 + ST7789_GREEN * 0.5 + ST7789_RED * 0.5;

    // Body (Brown Oval)
    add_list(1, x_start, y_start, 30 * scale, 40 * scale, brown, 0); // Main body
    
    // Eyes (Black with white highlights)
    add_list(1, x_start + (8 * scale), y_start + (20 * scale), 5 * scale, 8 * scale, black, 0);  // Left eye
    add_list(1, x_start + (17 * scale), y_start + (20 * scale), 5 * scale, 8 * scale, black, 0); // Right eye
    add_list(1, x_start + (9 * scale), y_start + (25 * scale), 2 * scale, 3 * scale, white, 0);  // Left eye highlight
    add_list(1, x_start + (18 * scale), y_start + (25 * scale), 2 * scale, 3 * scale, white, 0); // Right eye highlight

    // Nose (Pink Oval)
    add_list(1, x_start + (12 * scale), y_start + (12 * scale), 6 * scale, 4 * scale, pink, 0);  // Nose

    // Ground (Gray with uneven shapes for rocks)
    add_list(1, x_start - (10 * scale), y_start - (6 * scale), 50 * scale, 10 * scale, gray, 0);  // Main ground
    add_list(1, x_start - (6 * scale), y_start - (4 * scale), 8 * scale, 3 * scale, gray, 0);     // Rock 1
    add_list(1, x_start + (10 * scale), y_start - (5 * scale), 6 * scale, 3 * scale, gray, 0);    // Rock 2
    add_list(1, x_start + (22 * scale), y_start - (3 * scale), 4 * scale, 2 * scale, gray, 0);    // Rock 3
}

void Mmap::Terrain(uint16_t x_start, uint16_t y_start, float scale)
{
    // Colors using abstraction formula
    uint16_t grass = ST7789_BLUE * 0.1 + ST7789_GREEN * 0.9 + ST7789_RED * 0.1;  // Green grass
    uint16_t dirt = ST7789_BLUE * 0.3 + ST7789_GREEN * 0.3 + ST7789_RED * 0.5;  // Rocks
    uint16_t water = ST7789_BLUE * 0.4 + ST7789_GREEN * 0.6 + ST7789_RED * 0.8;  // Water blue
    
    // Ground (Grass area)
    add_list(1, x_start, y_start, 100 * scale, 50 * scale, grass, 7);  // Grass terrain
    
    // Dirt patch 1 (lower left corner)
    add_list(1, x_start + (10 * scale), y_start + (20 * scale), 30 * scale, 10 * scale, dirt, 0);  // Dirt patch
    
    // Dirt patch 2 (upper right corner)
    add_list(1, x_start + (60 * scale), y_start + (10 * scale), 23 * scale, 17 * scale, dirt, 0);  // Dirt patch
    
    // Water area (center bottom)
    add_list(1, x_start + (35 * scale), y_start + (30 * scale), 30 * scale, 15 * scale, water, 0); // Water
    
}

void Mmap::Terrain2(uint16_t x_start, uint16_t y_start, float scale)
{
    uint16_t tree_trunk = ST7789_BLUE * 0.1 + ST7789_GREEN * 0.3 + ST7789_RED * 0.3; // Brown tree trunk
    uint16_t tree_leaves = ST7789_BLUE * 0.1 + ST7789_GREEN * 0.7 + ST7789_RED * 0.1; // Green tree leaves

    // Tree Trunk (center of the terrain)
    add_list(1, x_start + (5 * scale), y_start, 10 * scale, 20 * scale, tree_trunk, 0); // Tree trunk

    // Tree Leaves (top of the trunk, circular shape)
    add_list(1, x_start, y_start + ((33-17) * scale), 20 * scale, 15 * scale, tree_leaves, 0); // Tree leaves

	// Darker Dirt patch 2 (random placement on the upper right)
    //add_list(1, x_start + (50 * scale), y_start + (10 * scale), 22 * scale, 18 * scale, dark_dirt, 0);  // Dark dirt patch

}

void Mmap::RockyTerrain(uint16_t x_start, uint16_t y_start, float scale)
{
    // Colors using abstraction formula
    uint16_t grass = ST7789_BLUE * 0.1 + ST7789_GREEN * 0.9 + ST7789_RED * 0.1;  // Green grass
    uint16_t rock = ST7789_BLUE * 0.3 + ST7789_GREEN * 0.3 + ST7789_RED * 0.5;  // Grey rock
    uint16_t dark_rock = ST7789_BLUE * 0.2 + ST7789_GREEN * 0.2 + ST7789_RED * 0.4; // Darker rock
    uint16_t dirt = ST7789_BLUE * 0.2 + ST7789_GREEN * 0.4 + ST7789_RED * 0.2;  // Light brown dirt

    // Ground (Grass area)
    add_list(1, x_start, y_start, 100 * scale, 50 * scale, grass, 0);  // Grass terrain

    // Rocky patches scattered around
    add_list(1, x_start + (15 * scale), y_start + (25 * scale), 20 * scale, 10 * scale, rock, 0);  // Rock patch 1
    add_list(1, x_start + (50 * scale), y_start + (15 * scale), 25 * scale, 12 * scale, dark_rock, 0);  // Rock patch 2
    add_list(1, x_start + (30 * scale), y_start + (35 * scale), 18 * scale, 10 * scale, rock, 0);  // Rock patch 3
    add_list(1, x_start + (70 * scale), y_start + (40 * scale), 20 * scale, 8 * scale, dark_rock, 0);  // Rock patch 4

    // Small dirt patches to give variation in the terrain
    //add_list(1, x_start + (5 * scale), y_start + (40 * scale), 10 * scale, 5 * scale, dirt, 0);  // Dirt patch 1
    //add_list(1, x_start + (40 * scale), y_start + (50 * scale), 10 * scale, 6 * scale, dirt, 0);  // Dirt patch 2
}



void Mmap::GDirection(char x, char y)
{
	int8_t joystick_x = x - '0' - 5;
        int8_t joystick_y = y - '0' - 5;

        joystick_x = -joystick_x;
        joystick_y = -joystick_y;

	if(joystick_x>0 && abs(joystick_x) > abs(joystick_y))
	{
		cur_direction = "R";
	}
	else if(joystick_x<0 && abs(joystick_x) > abs(joystick_y))
        {
		cur_direction = "L";
        }
	else if(joystick_y<0)
	{
		cur_direction = "F";
	}
	else if(joystick_y>0)
	{
		cur_direction = "B";
	}
}


void Mmap::Background()
{
	uint16_t grass = ST7789_BLUE * 0.1 + ST7789_GREEN * 0.9 + ST7789_RED * 0.1;  // Green grass                                                                                                                           // Colors using abstraction formula
	add_list(1, 0, 0, 240, 320, grass, 2); // Tree leaves
}


void Mmap::TrainerWalkRight(uint16_t x_start, uint16_t y_start, float scale)
{
	// Draw the head (cap)
    add_list(1, x_start + (8 * scale), y_start, 12 * scale, 16 * scale, 0xF800, 2); // Red cap: 0xF800

    // Draw the shoulders (overlap body for slight tilt effect)
    add_list(1, x_start + (10 * scale), y_start + (8 * scale), 8 * scale, 24 * scale, 0x8410, 2); // Gray shoulders: 0x8410

    // Draw the backpack (now positioned to the left of the body)
    add_list(1, x_start - (6 * scale), y_start + (12 * scale), 16 * scale, 20 * scale, 0x07E0, 2); // Green backpack: 0x07E0

    // Draw the body (blue jacket)
    add_list(1, x_start + (10 * scale), y_start + (8 * scale), 30 * scale, 24 * scale, 0x001F, 2); // Blue body: 0x001F

    // Draw the left arm (human skin tone, now pointing slightly down-right)
    add_list(1, x_start + (13 * scale), y_start + (10 * scale), 24 * scale, 8 * scale, 0xF7DA, 2); // Left arm: 0xF7DA

    // Draw the right arm (human skin tone, now to the right of the body)
    add_list(1, x_start + (13 * scale), y_start + (30 * scale), 24 * scale, 8 * scale, 0xF7DA, 2); // Right arm: 0xF7DA
}

void Mmap::update_trainer()
{
	if(cur_direction != old_direction)
	{
		if(cur_direction=="F")
		{add_update_list(3,1,0,0,0,0,0);}
		else if(cur_direction=="B")
		{add_update_list(3,2,0,0,0,0,0);}
		else if(cur_direction=="R")
		{add_update_list(3,3,0,0,0,0,0);}
		else if(cur_direction=="L")
		{add_update_list(3,4,0,0,0,0,0);}
	}
	old_direction = cur_direction;
}

void Mmap::Check_TB()
{
	if(abs(x_displacement) > 240)
	{
		GTerrain(0);
		x_displacement = 0;
	}
	else if(abs(y_displacement) > 320)
	{
		//GTerrain(1);
		//y_displacement = 0;
	}
}

void Mmap::GTerrain(uint8_t to_spawn)
{
	std::cout << "--------------SPAWNING SOMETHING!-----------------\n";
        if(cur_direction == "L")
        {
                //we moving RIGHT IN REALITY (object moving left)

                
                /*std::srand(time(0)+1);
                uint16_t x_start = 0;
                x_start = x_start-5;
                std::srand(time(0)+2);
                uint16_t y_start = 20 + abs(std::rand())%260;
                Terrain2(x_start,y_start,1);*/
        }
        else if(cur_direction == "R")
        {
                //std::cout << "GOOD!--------------------------------------------";
                //we moving left

                
                std::srand(time(0)+1);
                uint16_t x_start = 239;
                x_start = x_start+5;
                std::srand(time(0)+2);
                uint16_t y_start = 20 + abs(std::rand())%260;
                
		uint16_t to_spawn = std::rand()%2;
		if(to_spawn == 0)
		{
                	//Terrain2(x_start,y_start,1);
			uint16_t to_spawn_2 = std::rand()%2;
			if(to_spawn_2 == 1)
			{
				Terrain(x_start,y_start,1);
			}
			else
			{
				Terrain2(x_start,y_start,1);
			}
		}
		else
		{
			SimpleWeed(x_start,y_start,1);
		}
                std::cout << "\nSpawned a tree at x_start: \t" << x_start << "y_start:\t " << y_start;
        }
        else if(cur_direction == "F")
        {
                //cur_direction = "F";
        }
        else if(cur_direction == "B")
        {
                //xcur_direction = "B";
        }
}

void Mmap::SimpleWeed(uint16_t x_start, uint16_t y_start, float scale)
{
    // Darker green color for the bush
    uint16_t dark_green = ST7789_BLUE * 0.1 + ST7789_GREEN * 0.7 + ST7789_RED * 0.1;

    // Base rectangle (wide and short)
    add_list(1, x_start, y_start, 20 * scale, 8 * scale, dark_green, 3);

    // Middle rectangle (slightly narrower, shifted up for layering effect)
    add_list(1, x_start + (3 * scale), y_start + (3 * scale), 14 * scale, 6 * scale, dark_green, 3);

}

