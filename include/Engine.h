#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <ecl/ecl.h>
#include <iostream>
#include <string>
#include <list>
#include <algorithm>

#include "EnemyObject.h"

#define DEFUN(name,fun,args) \
 cl_def_c_function(c_string_to_object(name), \
 (cl_objectfn_fixed)fun, \
 args)

 // Define a function to run arbitrary Lisp expressions
cl_object lisp(const std::string & call);

class Engine
{
  public:
    Engine(int argc, char** argv);
    ~Engine();

    // SOme of the sfml system variables follow
    sf::RenderWindow window;
    sf::Texture shot_texture;
    sf::Sprite shot_texture_sprite;

    // Update is called once and enters the multimedia loop
    void update();

    static std::list<EnemyObject> players;
    static std::list<EnemyObject> enemies;
    static std::list<EnemyObject> enemy_shots;
    static std::list<EnemyObject> player_shots;

  protected:

  private:
    float shot_spin = 0;
    float shot_spin_step = .4;

    enum {TITLE, GAME, SCORE};
    unsigned int GAME_MODE = GAME;


    void draw();
    void draw_players();
    void draw_enemies();
    void draw_shot(EnemyObject& e, float additional_rotation = 0);
    void draw_shots();

    void do_score_step(sf::Event& evnt);
    void do_game_step(sf::Event& evnt);
    void do_title_step(sf::Event& evnt);
    void flag_offscreen_shots();
    void cull_dead(std::list<EnemyObject> &l);
    void clear_lists();

    // Shot burst uses the
    void shot_burst(float x, float y, float aim_direction, float arc,
                    float num_copies, std::list<EnemyObject> &lis);

};

// Prototype definitions that expose instance creation to ECL
cl_object add_new_enemy();
cl_object add_new_enemy_shot();
cl_object add_new_player();
cl_object add_new_player_shot();
cl_object set_working_instance(cl_object o);
cl_object get_working_instance();

cl_object get_direction_to_player(cl_object player_number, cl_object x,
                                    cl_object y);
#endif // ENGINE_H
