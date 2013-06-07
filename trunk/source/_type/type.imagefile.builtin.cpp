#include "_type/type.imagefile.builtin.h"

#include "userLogo_airplane_bin.h"
#include "userLogo_astronaut_bin.h"
#include "userLogo_ball_bin.h"
#include "userLogo_beach_bin.h"
#include "userLogo_butterfly_bin.h"
#include "userLogo_car_bin.h"
#include "userLogo_cat_bin.h"
#include "userLogo_chess_bin.h"
#include "userLogo_dirt_bike_bin.h"
#include "userLogo_dog_bin.h"
#include "userLogo_drip_bin.h"
#include "userLogo_duck_bin.h"
#include "userLogo_fish_bin.h"
#include "userLogo_frog_bin.h"
#include "userLogo_guest_bin.h"
#include "userLogo_guitar_bin.h"
#include "userLogo_horses_bin.h"
#include "userLogo_kick_bin.h"
#include "userLogo_lift_off_bin.h"
#include "userLogo_palm_tree_bin.h"
#include "userLogo_pink_flower_bin.h"
#include "userLogo_red_flower_bin.h"
#include "userLogo_skater_bin.h"
#include "userLogo_snowflake_bin.h"

const string basepath = "%APPDATA%/usericons/";

_map<_u8,string> imageId2filename = {
	{ 1 , basepath + "airplane.bmp" },
	{ 2 , basepath + "astronaut.bmp" },
	{ 3 , basepath + "ball.bmp" },
	{ 4 , basepath + "beach" },
	{ 5 , basepath + "butterfly.bmp" },
	{ 6 , basepath + "car.bmp" },
	{ 7 , basepath + "cat.bmp" },
	{ 8 , basepath + "chess.bmp" },
	{ 9 , basepath + "dirt bike.bmp" },
	{ 10 , basepath + "dog.bmp" },
	{ 11 , basepath + "drip.bmp" },
	{ 12 , basepath + "duck.bmp" },
	{ 13 , basepath + "fish.bmp" },
	{ 14 , basepath + "frog.bmp" },
	{ 15 , basepath + "guest.bmp" },
	{ 16 , basepath + "guitar.bmp" },
	{ 17 , basepath + "horses.bmp" },
	{ 18 , basepath + "kick.bmp" },
	{ 19 , basepath + "lift off.bmp" },
	{ 20 , basepath + "palm tree.bmp" },
	{ 21 , basepath + "pink flower.bmp" },
	{ 22 , basepath + "red flower.bmp" },
	{ 23 , basepath + "skater.bmp" },
	{ 24 , basepath + "snowflake.bmp" },
};

_map<_u8,const _u8*> imageId2imagedata = {
	{ 1 , userLogo_airplane_bin },
	{ 2 , userLogo_astronaut_bin },
	{ 3 , userLogo_ball_bin },
	{ 4 , userLogo_beach_bin },
	{ 5 , userLogo_butterfly_bin },
	{ 6 , userLogo_car_bin },
	{ 7 , userLogo_cat_bin },
	{ 8 , userLogo_chess_bin },
	{ 9 , userLogo_dirt_bike_bin },
	{ 10 , userLogo_dog_bin },
	{ 11 , userLogo_drip_bin },
	{ 12 , userLogo_duck_bin },
	{ 13 , userLogo_fish_bin },
	{ 14 , userLogo_frog_bin },
	{ 15 , userLogo_guest_bin },
	{ 16 , userLogo_guitar_bin },
	{ 17 , userLogo_horses_bin },
	{ 18 , userLogo_kick_bin },
	{ 19 , userLogo_lift_off_bin },
	{ 20 , userLogo_palm_tree_bin },
	{ 21 , userLogo_pink_flower_bin },
	{ 22 , userLogo_red_flower_bin },
	{ 23 , userLogo_skater_bin },
	{ 24 , userLogo_snowflake_bin },
};

_map<_u8,_u32> imageId2imagesize = {
	{ 1 , userLogo_airplane_bin_size },
	{ 2 , userLogo_astronaut_bin_size },
	{ 3 , userLogo_ball_bin_size },
	{ 4 , userLogo_beach_bin_size },
	{ 5 , userLogo_butterfly_bin_size },
	{ 6 , userLogo_car_bin_size },
	{ 7 , userLogo_cat_bin_size },
	{ 8 , userLogo_chess_bin_size },
	{ 9 , userLogo_dirt_bike_bin_size },
	{ 10 , userLogo_dog_bin_size },
	{ 11 , userLogo_drip_bin_size },
	{ 12 , userLogo_duck_bin_size },
	{ 13 , userLogo_fish_bin_size },
	{ 14 , userLogo_frog_bin_size },
	{ 15 , userLogo_guest_bin_size },
	{ 16 , userLogo_guitar_bin_size },
	{ 17 , userLogo_horses_bin_size },
	{ 18 , userLogo_kick_bin_size },
	{ 19 , userLogo_lift_off_bin_size },
	{ 20 , userLogo_palm_tree_bin_size },
	{ 21 , userLogo_pink_flower_bin_size },
	{ 22 , userLogo_red_flower_bin_size },
	{ 23 , userLogo_skater_bin_size },
	{ 24 , userLogo_snowflake_bin_size },
};