#include "EnemyObject.h"

// Set default value to 0 to be safe
EnemyObject* EnemyObject::working_instance = 0;

EnemyObject::EnemyObject()
{
  //ctor
}

EnemyObject::~EnemyObject()
{
  //dtor
}

// The following functions are interfaces to the Lisp side
cl_object enemy_object_get_x()
{
  return ecl_make_single_float(EnemyObject::working_instance->get_x());
}

cl_object enemy_object_get_y()
{
  return ecl_make_single_float(EnemyObject::working_instance->get_y());
}

cl_object enemy_object_get_speed()
{
  return ecl_make_single_float(EnemyObject::working_instance->get_speed());
}

cl_object enemy_object_get_direction()
{
  return ecl_make_single_float(EnemyObject::working_instance->get_direction());
}

cl_object enemy_object_get_stepper()
{
  return ecl_make_single_float(EnemyObject::working_instance->get_stepper());
}

cl_object enemy_object_get_dead()
{
  return ecl_make_bool(EnemyObject::working_instance->get_dead());
}

cl_object enemy_object_get_size()
{
  return ecl_make_single_float(EnemyObject::working_instance->get_size());
}

cl_object enemy_object_get_working_instance()
{
  return ecl_make_foreign_data(ECL_NIL, sizeof(EnemyObject*), EnemyObject::working_instance);
}

// SETTERS
void enemy_object_set_x(cl_object val, cl_object cumulative)
{
  EnemyObject::working_instance->set_x(ecl_to_float(val),
                                       ecl_to_bool(cumulative));
}

void enemy_object_set_y(cl_object val, cl_object cumulative)
{
  EnemyObject::working_instance->set_y(ecl_to_float(val),
                                       ecl_to_bool(cumulative));
}

void enemy_object_set_position(cl_object val, cl_object val2,
                               cl_object cumulative)
{
  EnemyObject::working_instance->set_position(ecl_to_float(val),
                                              ecl_to_float(val2),
                                              ecl_to_bool(cumulative));
}

void enemy_object_set_speed(cl_object val, cl_object cumulative)
{
  EnemyObject::working_instance->set_speed(ecl_to_float(val),
                                           ecl_to_bool(cumulative));
  EnemyObject::working_instance->recalculate_movement();
}

void enemy_object_set_direction(cl_object val, cl_object cumulative)
{
  EnemyObject::working_instance->set_direction(ecl_to_float(val),
                                               ecl_to_bool(cumulative));
  EnemyObject::working_instance->recalculate_movement();
}

void enemy_object_set_stepper(cl_object val, cl_object cumulative)
{
  EnemyObject::working_instance->set_stepper(ecl_to_int(val),
                                             ecl_to_bool(cumulative));
}

void enemy_object_set_dead(cl_object val)
{
  EnemyObject::working_instance->set_dead(ecl_to_bool(val));
}

void enemy_object_set_step_command(std::string val, std::string package)
{
  EnemyObject::working_instance->set_step_command(val, package.c_str());
}

void enemy_object_set_color(cl_object r, cl_object g, cl_object b, cl_object a)
{
  EnemyObject::working_instance->set_color(ecl_to_int(r), ecl_to_int(g), ecl_to_int(b), ecl_to_int(a));
}

void enemy_object_set_size(cl_object size, cl_object cumulative)
{
  EnemyObject::working_instance->set_size(ecl_to_float(size),
                                          ecl_to_bool(cumulative));
}

void enemy_object_set_working_instance(cl_object p)
{
  void* ptr = ecl_foreign_data_pointer_safe(p);
  if(ptr != 0)
  {
    EnemyObject::working_instance = static_cast<EnemyObject*>(ptr);
  }
  else
  {
    EnemyObject::working_instance = 0;
  }
}

