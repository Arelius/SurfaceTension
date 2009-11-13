#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <scheme-private.h>
#include <box2d.h>

extern scheme* scheme_vm;
struct SystemManager;

void scheme_load_file_name(scheme* sc, const char* file);
b2Vec2 GetVec2(pointer p);
void declare_script_system(SystemManager* SM);

void RegisterSchemeFunc(scheme* sc, foreign_func func, const char* Docs);

#define car(p) pair_car(p)
#define cdr(p) pair_cdr(p)

#define caar(p)          car(car(p))
#define cadr(p)          car(cdr(p))
#define cdar(p)          cdr(car(p))
#define cddr(p)          cdr(cdr(p))
#define cadar(p)         car(cdr(car(p)))
#define caddr(p)         car(cdr(cdr(p)))
#define cadaar(p)        car(cdr(car(car(p))))
#define cadddr(p)        car(cdr(cdr(cdr(p))))
#define cddddr(p)        cdr(cdr(cdr(cdr(p))))


#endif //_SCRIPT_H_
