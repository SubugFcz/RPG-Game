//=================================================================
// The main program file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

// Projet includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <math.h>

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y - (LANDSCAPE_HEIGHT + MAX_BUILDING_HEIGHT))

// Helper function declarations
void playSound(char *wav);
int go_right(int x, int y);
int go_left(int x, int y);
int go_up(int x, int y);
int go_down(int x, int y);
void draw_game_over();
void draw_game_over2();
unsigned XY_KEY(int X, int Y);
void moveNPC();

int startIndicator;
int npcx;
int npcy;
int tempnpcx;
int tempnpcy;

/////////////////////////
// Struct of Player
/////////////////////////

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct
{
    int x, y;           // Current locations
    int px, py;         // Previous locations
    int walkCount;
    int lives;
    bool has_key;       // flag when obtained the key
    int game_solved;   // flag when game is complete
    bool talked_to_npc; // flag when you've talked to npc
    bool ramblin;

    // You may add more flags as needed

} Player;

/////////////////////////
// Get Action
/////////////////////////

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6

int get_action(GameInputs inputs)
{
    //******************
    // TODO: Implement
    //******************

    // 1. Check your action and menu button inputs and return the corresponding action value

    // 2. Check for your navigation switch inputs and return the corresponding action value

    // If no button is pressed, just return no action value

    if (inputs.ns_down)
    {
        return GO_DOWN;
    }
    if (inputs.ns_up)
    {
        return GO_UP;
    }
    if (inputs.ns_right)
    {
        return GO_RIGHT;
    }
    if (inputs.ns_left)
    {
        return GO_LEFT;
    }
    if (!inputs.b1)
    {
        startIndicator++;
        return ACTION_BUTTON;
    }
    if (!inputs.b2)
    {
        return MENU_BUTTON;
    }

    return NO_ACTION;
}

/////////////////////////
// Helper Functions:
/////////////////////////
// Feel free to define any helper functions here for update game

/////////////////////////
// Update Game
/////////////////////////

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{

    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;

    MapItem *item = NULL;

    int x = Player.x;
    int y = Player.y;

    //******************
    // TODO: Implement
    //******************

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.

    switch (action)
    {

        
    case MENU_BUTTON:
        Player.ramblin = true;
        draw_border2();

        return FULL_DRAW;

    case GO_UP:
        // TODO: Implement
        // 1. Check the item north of the player
        // 2. Make sure to not walk through walls
        // 3. If it is not a wall, the walk up by updating player's coordinates

        MapItem *itemNorth = get_north(x, y);
        if (itemNorth->walkable)
        {
            Player.y -= 1;
            Player.walkCount++;
            Player.ramblin = false;
            return FULL_DRAW;
        }
        else
        {
            if (itemNorth->type == WALL2)
            {
                if (Player.ramblin)
                {
                    Player.y -= 1;
                    Player.walkCount++;
                    Player.ramblin = false;
                    return FULL_DRAW;
                }
                else
                {
                    break;
                }
            }
            break;
        }
    case GO_LEFT:
        // TODO: Implement

        MapItem *itemWest = get_west(x, y);
        if (itemWest->walkable)
        {
            Player.x -= 1;
            Player.walkCount++;
            Player.ramblin = false;
            return FULL_DRAW;
        }
        else
        {
            if (itemWest->type == WALL2)
            {
                if (Player.ramblin)
                {
                    Player.x -= 1;
                    Player.walkCount++;
                    Player.ramblin = false;
                    return FULL_DRAW;
                }
                else
                {
                    break;
                }
            }
            break;
        }

    case GO_DOWN:
        // TODO: Implement

        MapItem *itemSouth = get_south(x, y);
        if (itemSouth->walkable)
        {
            Player.y += 1;
            Player.walkCount++;
            Player.ramblin = false;
            return FULL_DRAW;
        }
        else
        {
            if (itemSouth->type == WALL2)
            {
                if (Player.ramblin)
                {
                    Player.y += 1;
                    Player.walkCount++;
                    Player.ramblin = false;
                    return FULL_DRAW;
                }
                else
                {
                    break;
                }
            }
            break;
        }

    case GO_RIGHT:

        MapItem *itemEast = get_east(x, y);
        if (itemEast->walkable)
        {
            Player.x += 1;
            Player.walkCount++;
            Player.ramblin = false;
            return FULL_DRAW;
        }
        else
        {
            if (itemEast->type == WALL2)
            {
                if (Player.ramblin)
                {
                    Player.x += 1;
                    Player.walkCount++;
                    Player.ramblin = false;
                    return FULL_DRAW;
                }
                else
                {
                    break;
                }
            }
            break;
        }

    case ACTION_BUTTON:

        //******************
        // TODO: Implement
        //******************

        // 1. Check if near NPC,
        //     - if so, mark the player has talked and give instructions on what to do
        //     - if the game is solved (defeated Buzz), give the player the key
        //     - return FULL_DRAW to redraw the scene
        int playx = Player.x;
        int playy = Player.y;
        Player.ramblin = false;

        int hashp = (playx + playy) * (playx + playy + 1) / 2 + playy;
        int hashna = (npcx-1 + npcy) * (npcx-1 + npcy + 1) / 2 + npcy;
        int hashnb = (npcx+1 + npcy) * (npcx+1 + npcy + 1) / 2 + npcy;
        int hashnc = (npcx + npcy+1) * (npcx + npcy+1 + 1) / 2 + npcy+1;
        int hashnd = (npcx + npcy-1) * (npcx + npcy-1 + 1) / 2 + npcy-1;

        if (get_active_map_index() == 0 && (hashp == hashna || hashp == hashnb || hashp == hashnc || hashp == hashnd))
        {
            if (Player.talked_to_npc)
            {
                if (Player.game_solved==2)
                {
                    if (Player.has_key)
                    {
                        const char *lines[] = {"You have the key ",
                                               "go get your.     ",
                                               "PPPPPRIZZZEEEE   "};
                        long_speech(lines, 3);
                    }
                    else
                    {
                        const char *lines[] = {"Thank God!       ",
                                               "Take this....    ",
                                               "Go Explore and   ",
                                               "I am so grateful!"};
                        long_speech(lines, 4);
                        Player.has_key = true;
                    }
                }
                else
                {
                    const char *lines[] = {"Are you still....",
                                           "Confuse?         ",
                                           "Find the cave!   ",
                                           "and.. Kill him!.."};
                    long_speech(lines, 4);
                }
            }
            else
            {
                const char *lines[] = {"Help me....      ",
                                       "Inside the Cave..",
                                       "Get some Water...",
                                       "Use it to.. Kill.",
                                       "The BUZZ........."};
                long_speech(lines, 5);
                Player.talked_to_npc = true;
            }
        }
        else

            // 2. Check if near a door
            //    - if the player has the key, you win the game
            //    - if not, show speech bubbles that the play needs to get the key
            //    - return FULL_DRAW to redraw the scene

            if (get_active_map_index() == 0 && (hashp == 1001 || hashp == 1046 || hashp == 1092 || hashp == 1139))
            {
                if (Player.has_key)
                {
                    return GAME_OVER;
                }
            }
            else

                // 3. Check if on Buzz's cave
                //    - if the player has talked to the npc, then start the speech bubble to fight buzz
                //    - set the players coordinates for the small map
                //    - and set the map to the small map
                //    - return FULL_DRAW to redraw the scene

                if (get_active_map_index() == 0 && (hashp == 345 || hashp == 372 || hashp == 371 || hashp == 399))
                {
                    if (Player.talked_to_npc)
                    {   
                        tempnpcx=npcx;
                        tempnpcy=npcy;
                        const char *lines[] = {"Be carefull..       "};
                        long_speech(lines, 1);
                        set_active_map(1);
                        Player.x = 11;
                        Player.y = 16;
                    }
                }
                else

                    // 4. Check if on a stairs
                    //    - if so, move the player the correct coordinates
                    //    - and set the map back to the main big map
                    //    - return FULL_DRAW to redraw the scene
                    if (get_active_map_index() == 1 && hashp == 394)
                    {
                        set_active_map(0);
                        npcx=tempnpcx;
                        npcy=tempnpcy;
                        Player.x = 5;
                        Player.y = 21;
                    }
                    else

                        // 5. Check if at the right spell to end the game
                        //    - if so, use speech bubbles to declare that Buzz has been defeated
                        //    - and flag that the game is solved
                        //    - return FULL_DRAW to redraw the scene
                        if (get_active_map_index() == 1 && (hashp == 264 || hashp == 265 || hashp == 312 || hashp == 313) && (Player.game_solved != 2))
                        {
                            const char *lines[] = {"I love water...  ",
                                                   "But not this...  ",
                                                   "Water.           ",
                                                   "ARRRGGFHHH.      ",
                                                   ".....            "};
                            long_speech(lines, 5);
                            Player.game_solved++;
                            add_slain_buzz2(13, 9);
                        }
                        else if (get_active_map_index() == 1 && (hashp == 164 || hashp == 165 || hashp == 202 || hashp == 203) && (Player.game_solved != 2))
                        {
                            const char *lines[] = {"Earth??......    ",
                                                   "Water??..        ",
                                                   "and you choose.  ",
                                                   "Fire????  .      ",
                                                   "ARRRGGFHHH       "};
                            long_speech(lines, 5);
                            Player.game_solved++;
                            add_slain_buzz1(9, 9);
                        }else if (get_active_map_index() == 1 && (hashp == 389 || hashp == 390 || hashp == 447 || hashp == 448) && (Player.game_solved != 2))
                        {
                            const char *lines[] = {"Are you kidding? ",
                                                   "I love earth     ",
                                                   "Mother nature.   ",
                                                   "give me strength!",
                                                   "Minus 1 <3       "};
                            long_speech(lines, 5);
                            Player.lives--;
                        }else 
                        {
                            return FULL_DRAW;
                        }

        // Feel free to add more functions as you make the game!

        return FULL_DRAW;
    }

    return NO_RESULT;
}

/////////////////////////
// Draw Game
/////////////////////////

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{

    if(init == 999){

        uLCD.filled_rectangle(0, 0, 128, 128, BLACK);

        uLCD.text_string((char *)"HI", 8, 3, FONT_12X16, BLUE);
        uLCD.text_string((char *)"press button 1", 2, 6, FONT_12X16, WHITE);
        uLCD.text_string((char *)"to start", 6, 7, FONT_12X16, WHITE);
        uLCD.text_string((char *)"IAAD", 7, 12, FONT_8X8, DGREY);
        draw_buzz1(47, 72);
        draw_buzz2(74, 72);

        //playSound("musssic.wav");
        
        while(startIndicator == 0) {
        // 1. Read inputs -- implement this function:
        GameInputs inputs = read_inputs();
        
        // 2. Determine action (move, act, menu, etc.) -- implement this function:
        int action = get_action(inputs);
        
        // 3. Update game -- implement this function:
        int result = update_game(action);     
        }
    } else {
        // Draw game border first
    if (init)
        if (Player.ramblin)
        {
            draw_border2();
            if (get_active_map_index() == 0 && Player.walkCount % 6 == 5)
            {
            moveNPC();
            }
        }
        else
        {
            if (get_active_map_index() == 0 && Player.walkCount % 6 == 5)
            {
            moveNPC();
            }
            draw_border();
        }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i + 5) * 11 + 3;
            int v = (j + 4) * 11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            //            if (init && i == 0 && j == 0) // Only draw the player on init
            if (i == 0 && j == 0) // always draw the player
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem *curr_item = get_here(x, y);
                MapItem *prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
                else if (curr_item && curr_item->type == CLEAR)
                {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw)
                draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
    }
}

/////////////////////////
// Map Intialization
/////////////////////////

// Important locations for all maps
int cb_loc[2] = {5, 20}; // Location of the center of the cave

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // Initialize and sets current map to the first map
    Map *map = set_active_map(0);

    /////////////////////////
    // Initial Environmnet
    /////////////////////////

    // Adding random plants
    // pc.printf("Adding Plants!\r\n");
    // for (int i = map_width() + 3; i < map_area(); i += 39)
    // {
    //     add_plant(i % map_width(), i / map_width());
    // }

    for (int i = map_width() + 7; i < map_area(); i += 39)
    {
        add_plant(i % map_width()+3, i / map_width()+3);
    }

    // Adding wall borders
    // pc.printf("Adding walls!\r\n");
    add_wall(0, 0, HORIZONTAL, map_width());
    add_wall(0, map_height() - 1, HORIZONTAL, map_width());
    add_wall(0, 0, VERTICAL, map_height());
    add_wall(map_width() - 1, 0, VERTICAL, map_height());

    // Adding extra chamber borders
    // pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    add_door(33, 10, HORIZONTAL, 4);

    // Adding extra cave to Buzz's evil lair
    // pc.printf("Add cave\r\n");
    add_cave(cb_loc[0], cb_loc[1], 1, 1, 5, 5); // Cave is set as a 4x4 block to be bigger
    add_cave(cb_loc[0] + 1, cb_loc[1], 2, 1, 5, 5);
    add_cave(cb_loc[0], cb_loc[1] + 1, 3, 1, 5, 5);
    add_cave(cb_loc[0] + 1, cb_loc[1] + 1, 4, 1, 5, 5);

    // pc.printf("Initial environment completed\r\n");

    /////////////////////////////////
    // Characters and Items for the map
    /////////////////////////////////

    // Add NPC
    add_npc(npcx, npcy); // NPC is initialized to (x,y) = 10, 5. Feel free to move him around
    //***********************************
    // TODO: Implement As Needed
    //***********************************

    // Add any extra characters/items here for your project

    // Prints out map
    print_map();
}

void init_small_map()
{
    // Sets current map to the second map
    set_active_map(1);

    //***********************************
    // TODO: Implement
    //***********************************

    // 1. Add walls to the smaller map.
    //    Set the dimensions to be 16x16  <-- you may change to your liking, but must be smaller than the main map
    //    Hint: See how walls are initialized in the main map
    // Adding wall borders
    // pc.printf("Adding walls!\r\n");
    add_wall(0, 0, HORIZONTAL, map_width());
    add_wall(0, map_height() - 1, HORIZONTAL, map_width());
    add_wall(0, 0, VERTICAL, map_height());
    add_wall(map_width() - 1, 0, VERTICAL, map_height());

    add_wall2(2, 14, HORIZONTAL, 17);
    //
    // 2. Add your three spells at different locations
    // pc.printf("Adding spells!\r\n");
    add_earth(16, 12);
    add_fire(6, 12);
    add_water(11, 12);
    //
    // 3. Add Evil Buzz at the center of the map
    // pc.printf("Adding buzz!\r\n");
    add_buzz1(13, 9);
    add_buzz2(9, 9);

    // You may add any extra characters/items here for your project

    // Add stairs back to main (map 0)
    add_stairs(11, 16, 0, cb_loc[0], cb_loc[1]);
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // uLCD.filled_rectangle(64, 64, 74, 74, RED); //DELETE OR COMMENT THIS LINE  <-- It is a temporary indicator that your LCD is working before you implement your code

    // Initialize the maps
    npcx = 10;
    npcy = 5;

    maps_init();
    init_main_map();
    init_small_map();

    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = false;
    Player.game_solved = 0;
    Player.talked_to_npc = false;
    Player.walkCount = 0;
    startIndicator = 0;
    Player.lives = 2;

    // Initial drawing
    draw_game(999);
    draw_game(1);

    // Main game loop
    while (1)
    {

        ////////////////////////////////
        // TODO: Implement
        ////////////////////////////////

        // Timer to measure game update speed
        Timer t;
        t.start();

        // Actually do the game update:
        // 1. Read inputs
        GameInputs inputs = read_inputs();

        // 2. Determine action (get_action)
        int action = get_action(inputs);

        // 3. Update game (update_game)
        int result = update_game(action); // Set this variable "result" for the resulting state after update game

        // 3b. Check for game over based on update game result
        if (result == 1)
        {
            draw_game_over2();
            break;
        }
        if (Player.lives==0)
        {
            draw_game_over();
            break;
        }

        // 4. Draw screen to uLCD
        bool full_draw = false;
        if (result == FULL_DRAW)
            full_draw = true;
        draw_game(full_draw);

        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100)
            wait_ms(100 - dt);
    }
}

/////////////////////////////
// Advanced Features
/////////////////////////////

// Plays a wavfile
void playSound(char *wav)
{
    // open wav file
    FILE *wave_file;
    wave_file = fopen(wav, "r");

    if (wave_file != NULL)
    {
        printf("File opened successfully\n");

        // play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);

        // close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }

    printf("Could not open file for reading - %s\n", wav);
    return;
}

void moveNPC(){
    // map_erase(npcx, 5);
    add_plant(npcx, npcy);
    npcx++;
    npcy++;
    add_npc(npcx, npcy);
}
