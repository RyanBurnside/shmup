#ifndef ENEMYOBJECT_H
#define ENEMYOBJECT_H
#include <ecl/ecl.h>
#include <math.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>



class EnemyObject
{
public:
  EnemyObject();
  virtual ~EnemyObject();

  /* This is set to the current object being updated
  This is so the Lisp functions update the object pointed to without
  needing a class pointer which ecl doesn't understand */

  int rgba[4] = {255, 255, 255, 255};

  // Used when changing the current instance
  static EnemyObject *working_instance;

  float get_x()
  {
    return x;
  }

  float get_y()
  {
    return y;
  }

  float get_speed()
  {
    return speed;
  }

  float get_direction()
  {
    return direction;
  }

  float get_stepper()
  {
    return stepper;
  }

  bool get_dead()
  {
   return dead;
  }

  float get_size()
  {
    return size;
  }

  EnemyObject* get_self()
  {
    return this;
  }

  sf::FloatRect& get_hitbox()
  {
    return hitbox;
  }

  void set_x(float val, bool cumulative = false)
  {
    cumulative ? x += val : x = val;
    recalculate_hitbox();
  }

  void set_y(float val, bool cumulative = false)
  {
    cumulative ? y += val : y = val;
    recalculate_hitbox();
  }

  void set_position(float val, float val2, bool cumulative = false)
  {
    set_x(val, cumulative);
    set_y(val2, cumulative);
    recalculate_hitbox();
  }

  void set_speed(float val, bool cumulative = false)
  {
    cumulative ? speed += val : speed = val;
    recalculate_movement();
  }

  void set_direction(float val, bool cumulative = false)
  {
    cumulative ? direction += val : direction = val;
    recalculate_movement();
  }

  void set_stepper(unsigned int val, bool cumulative = false)
  {
      cumulative ? stepper += val : stepper = val;
  }


  void set_dead(bool val)
  {
    dead = val;
  }

  void set_step_command(std::string val, std::string package)
  {
    step_command = ecl_make_symbol(val.c_str(), package.c_str());
  }

  void set_color(int r, int g, int b, int a = 255)
  {
      rgba[0] = r;
      rgba[1] = g;
      rgba[2] = b;
      rgba[3] = a;
  }

  void set_size(float s, bool cumulative = false)
  {
    cumulative ? size += s : size = s;
    recalculate_hitbox();
  }

  void recalculate_movement()
  {
    x_step = cos(direction) * speed;
    y_step = sin(direction) * speed;
  }

  void recalculate_hitbox()
  {
    hitbox.width = size;
    hitbox.height = size;
    hitbox.left = x - (size / 2.0);
    hitbox.top = y - (size / 2.0);
  }

  void move()
  {
    x += x_step;
    y += y_step;
    recalculate_hitbox();
  }

  // Do actions sends the stepper value out to Lisp to preform behaviors
  void do_lisp_behavior()
  {
      /* Grab the shared pointer and make it point to this instance for Lisp
      to execute behavior on (this avoids passing an Object* to ecl) */
      working_instance = this;
      cl_funcall(2, step_command, ecl_make_int(stepper));
      stepper++;
  }

  void update()
  {
    do_lisp_behavior();
    move();
  }

protected:

private:
  float x = 0;
  float y = 0;
  float speed = 0;
  float direction = 0;
  float x_step = 0;
  float y_step = 0;
  float size = 32;
  sf::FloatRect hitbox;

  // Used to call the update function from Lisp side
  cl_object step_command = ecl_make_symbol("+", "CL");

  // Stepper is iterated to move to the next action on update
   int stepper = 0;
   bool dead = false;
};

// The following functions are interfaces to the Lisp side
cl_object enemy_object_get_x();
cl_object enemy_object_get_y();
cl_object enemy_object_get_speed();
cl_object enemy_object_get_direction();
cl_object enemy_object_get_stepper();
cl_object enemy_object_get_dead();
cl_object enemy_object_get_size();
cl_object enemy_object_get_working_instance();

// SETTERS
void enemy_object_set_x(cl_object val, cl_object cumulative);
void enemy_object_set_y(cl_object val, cl_object cumulative);
void enemy_object_set_position(cl_object val, cl_object val2,
                                cl_object cumulative);
void enemy_object_set_speed(cl_object val, cl_object cumulative);
void enemy_object_set_direction(cl_object val, cl_object cumulative);
void enemy_object_set_stepper(cl_object val, cl_object cumulative);
void enemy_object_set_dead(cl_object);
void enemy_object_set_step_command(std::string val, std::string package);
void enemy_object_set_color(cl_object r, cl_object g, cl_object b, cl_object a);
void enemy_object_set_size(cl_object s, cl_object cumulative);
void enemy_object_set_working_instance(cl_object p);

#endif // ENEMYOBJECT_H
