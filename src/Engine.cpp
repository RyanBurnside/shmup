#include "Engine.h"
#include <SFML/System.hpp>

// Define a function to run arbitrary Lisp expressions
cl_object lisp(const std::string & call) {
 return cl_safe_eval(c_string_to_object(call.c_str()), Cnil, Cnil);
}

// Initialize the static object containers below
std::list<EnemyObject> Engine::players;
std::list<EnemyObject> Engine::player_shots;
std::list<EnemyObject> Engine::enemies;
std::list<EnemyObject> Engine::enemy_shots;

// Begin Engine implementation
Engine::Engine(int argc, char** argv)
{
  //ctor
  // Create the main window
    window.create(sf::VideoMode(600, 800), "Testing");

    window.setVerticalSyncEnabled(1);
    window.setVisible(true);

    cl_boot(argc, argv);
    atexit(cl_shutdown);

    // System specific DEFUNs
    DEFUN("add-new-enemy", add_new_enemy, 0);
    DEFUN("add-new-enemy-shot", add_new_enemy_shot, 0);
    DEFUN("add-new-player", add_new_player, 0);
    DEFUN("add-new-player-shot", add_new_player_shot, 0);
    DEFUN("get-direction-to-player", get_direction_to_player, 3);
    // Here we expose the C++ side to Lisp registering functions
    DEFUN("get-x", enemy_object_get_x, 0);
    DEFUN("get-y", enemy_object_get_y, 0);
    DEFUN("get-speed", enemy_object_get_speed, 0);
    DEFUN("get-direction", enemy_object_get_direction, 0);
    DEFUN("get-stepper", enemy_object_get_stepper, 0);
    DEFUN("get-dead", enemy_object_get_dead, 0);
    DEFUN("get-working-instance", enemy_object_get_working_instance, 0);

    DEFUN("set-x", enemy_object_set_x, 2);
    DEFUN("set-y", enemy_object_set_y, 2);
    DEFUN("set-position", enemy_object_set_position, 3);
    DEFUN("set-speed", enemy_object_set_speed, 2);
    DEFUN("set-direction", enemy_object_set_direction, 2);
    DEFUN("set-stepper", enemy_object_set_stepper, 2);
    DEFUN("set-color", enemy_object_set_color, 4);
    DEFUN("set-size", enemy_object_set_size, 2);
    DEFUN("set-dead", enemy_object_set_dead, 1);
    DEFUN("set-working-instance", enemy_object_set_working_instance, 1);

    // Load the user defined lisp vales
    lisp("(load \"src//initialize.lisp\")");

    //Example of how to return string from ecl
    //cl_object a = lisp("(coerce \"Testing making and returning a string from ecl!\n\" 'base-string)");
    //std::cout << a->base_string.self << std::endl;

    // Load sfml resources below
    if(!shot_texture.loadFromFile("Resources/Sprites/shot.png"))
    {
      std::cout << "Shot texture could not load" << std::endl;
      exit(EXIT_FAILURE);
    }

    // We assume bullet sprites are 32 x 32 but can be scaled
    shot_texture_sprite.setTexture(shot_texture);
    shot_texture_sprite.setOrigin(16.0, 16.0);

    // Populate players
    for(int i = 0; i < 4; ++i)
    {
      players.push_back(EnemyObject());
      if(i != 0){players.back().set_dead(true);}
    }
    players.begin()->set_position(300, 400);
}

Engine::~Engine()
{
  //dtor

}

void Engine::do_title_step(sf::Event& evnt)
{
}

void Engine::do_game_step(sf::Event& evnt)
{

  // update players
  for(EnemyObject &p: players){p.update();}
  for(EnemyObject &e: enemies){e.update();}
  for(EnemyObject &p: player_shots){p.update();}
  for(EnemyObject &e: enemy_shots){e.update();}

  static float ticker = 0;
  static int counter = 0;
  counter++;

  if(counter % 5 == 0)
  {
    ticker += .2;
    shot_burst(sf::Mouse::getPosition(window).x,
               sf::Mouse::getPosition(window).y, ticker, M_PI * 2.0, 20, enemy_shots);
  }
  flag_offscreen_shots();
  cull_dead(enemies);
  cull_dead(enemy_shots);
  cull_dead(player_shots);
}

void Engine::do_score_step(sf::Event& evnt)
{
}

void Engine::draw_players()
{

}

void Engine::draw_enemies()
{

}

void Engine::draw_shot(EnemyObject& e, float additional_rotation)
{
  static sf::Color col;
  static float scale = e.get_size() / shot_texture.getSize().y;
  scale = e.get_size() / shot_texture.getSize().y;
  shot_texture_sprite.setPosition(e.get_x(), e.get_y());
  shot_texture_sprite.setScale(scale, scale);
  float rot = e.get_direction() / (2.0 * M_PI) * 360.0;
  float rot2 = additional_rotation / (2.0 * M_PI) * 360.0;
  shot_texture_sprite.setRotation(rot + rot2);
  col.r = e.rgba[0];
  col.g = e.rgba[1];
  col.b = e.rgba[2];
  col.a = e.rgba[3];
  shot_texture_sprite.setColor(col);
  window.draw(shot_texture_sprite);
}

void Engine::draw_shots()
{
  for(EnemyObject& e : enemy_shots){draw_shot(e, shot_spin);}
  for(EnemyObject& e : player_shots){draw_shot(e, shot_spin);}
}

void Engine::draw()
{
  // Use the current value of GAME_MODE to draw the proper things
  shot_spin += shot_spin_step;
  static const float TAU = 2.0 * M_PI;
  while(shot_spin < 0){shot_spin += TAU;}
  while(shot_spin > TAU){shot_spin -= TAU;}

  switch(GAME_MODE)
  {
  case TITLE:
    break;
  case GAME:
    draw_enemies();
    draw_players();
    draw_shots();
    break;
  case SCORE:
    break;
  }
}

void Engine::update()
{
  sf::Event event;

  while(Engine::window.isOpen())
  {
    window.clear();
    while(window.pollEvent(event))
    {
      // Abort on x press
      if(event.type == sf::Event::Closed){window.close();}
    }

    switch(GAME_MODE)
    {
    case TITLE:
      do_title_step(event);
      break;
    case GAME:
      do_game_step(event);
      break;
    case SCORE:
      do_score_step(event);
      break;
    }
    draw();
    window.display();
  }
}

void Engine::clear_lists()
{
  enemies.clear();
  player_shots.clear();
  enemy_shots.clear();
}

void Engine::flag_offscreen_shots()
{
  static sf::FloatRect screen(0, 0, window.getSize().x, window.getSize().y);
  for(EnemyObject& s : player_shots)
  {
    if(!screen.intersects(s.get_hitbox())){s.set_dead(true);}
  }

  for(EnemyObject& s : enemy_shots)
  {
    if(!screen.intersects(s.get_hitbox())){s.set_dead(true);}
  }
}

void Engine::cull_dead(std::list<EnemyObject> &lis)
{
  // Remove all EnemyObjects if their dead flag is true
  lis.remove_if([](EnemyObject& e)
                  {return e.get_dead();});
}

void Engine::shot_burst(float x, float y, float aim_direction, float arc,
                    float num_copies, std::list<EnemyObject> &lis)
{
  float start_angle = aim_direction - (arc / 2.0);
  float step_angle = arc / (num_copies - 1.0);

  for(float i = 0; i < num_copies; ++i)
  {
    EnemyObject e;
    e.set_size(24);
    e.set_position(x, y);
    e.set_step_command("DO-NOTHING", "COMMON-LISP-USER");
    e.set_speed(5.0);
    e.set_direction(start_angle + (i * step_angle));
    lis.push_back(e);
  }
}

// The following functions expose class creation to ECL's macros

cl_object push_and_return_id(std::list<EnemyObject>& r)
{
  // EnemyObjects turn into things depending on what container they are pushed
  // into
  r.push_back(EnemyObject());
  return ecl_make_foreign_data(ECL_NIL, sizeof(EnemyObject*), r.back().get_self());
}

cl_object add_new_enemy()
{
  return push_and_return_id(Engine::enemies);
}

cl_object add_new_enemy_shot()
{
  return push_and_return_id(Engine::enemy_shots);
}

cl_object add_new_player()
{
  return push_and_return_id(Engine::players);
}

cl_object add_new_player_shot()
{
  return push_and_return_id(Engine::player_shots);
}

cl_object get_direction_to_player(cl_object player_number, cl_object x,
                                    cl_object y)
{

  if(Engine::players.size() == 0)
  {
    return ecl_make_double_float(M_PI * .5);
  }

  float index = ecl_to_int(player_number);
  if(Engine::players.size() - 1 < index)
  {
    // Invalid player index, using player number 1
    index = 0;
  }

  std::list<EnemyObject>::iterator e = Engine::players.begin();
  std::advance(e, index);

  // Set the aim to the first player if the others are dead
  if(e->get_dead()){e = Engine::players.begin();}

  float dif_x = e->get_x() - ecl_to_float(x);
  float dif_y = e->get_y() - ecl_to_float(y);
  if(dif_x - dif_y == 0){dif_x = 0; dif_y = 1;}

  return ecl_make_double_float(atan2(dif_y, dif_x));
}

