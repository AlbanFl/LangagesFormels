//******************************************************************************

#include <iostream>

#include "Determinize.h"
#include "FA_tools.h"
#include "Accept.h"

////////////////////////////////////////////////////////////////////////////////
using namespace std;

sAutoNDE Determinize(const sAutoNDE& at){

  sAutoNDE r;

  vector<set<etat_t> > nouveaux_etats; ///ce tableau de set d'états va permettre de faire la corrélation entre un état du graphe déterministe et les états de at
  set<etat_t> initial;
  initial.insert(at.initial); 
  r.nb_finaux = 0;
  for(set<etat_t>::iterator i = initial.begin(); i != initial.end(); i++) {
    for(set<etat_t>::iterator it = at.epsilon[(*i)].begin(); it != at.epsilon[(*i)].end(); it++) {
     ///Ces 2 boucles imbriquées permettent de trouver tous les états qu'on peut atteindre à partir de l'état initial avec que des liaisons epsilon.
      if(initial.find((*it)) == initial.end()) {
        initial.insert((*it));
      }
    }
  }
  nouveaux_etats.push_back(initial);
  for(set<etat_t>::iterator it = initial.begin(); it != initial.end(); it++) { ///Vérifie si l'état initial est un état final
          if(at.finaux.find(*it) != at.finaux.end() && r.finaux.find(nouveaux_etats.size()-1) == r.finaux.end()){
            r.finaux.insert(nouveaux_etats.size()-1);
            r.nb_finaux++;
          }
        }
  bool est_nouveau = true;

  for(unsigned int etat=0; etat<nouveaux_etats.size(); etat++) { ///pour chaque nouvel état...
    for(unsigned int symb=0; symb<at.nb_symbs; symb++) { ///...pour chaque symbole...
      set<etat_t> nouvel_etat;
      est_nouveau = true;
      for(set<etat_t>::iterator i = nouveaux_etats[etat].begin(); i != nouveaux_etats[etat].end(); i++) {///... on parcours tous les états de at que contient l'état testé...
        for(set<etat_t>::iterator it = at.trans[(*i)][symb].begin(); it != at.trans[(*i)][symb].end(); it++){///... et on vérifie dans la table de transition de at si il y'a des liaisons allant de l'état du graphe initial 
																			     ///     testé jusqu'à un autre état de at grâce à une laision du symbole testé
          if(nouvel_etat.find(((*it))) == nouvel_etat.end())
            nouvel_etat.insert((*it)); 
        }
      }
      for(set<etat_t>::iterator i = nouvel_etat.begin(); i != nouvel_etat.end(); i++){
        for(set<etat_t>::iterator it = at.epsilon[(*i)].begin(); it != at.epsilon[(*i)].end(); it++) { ///ajoute au set d'éléments de at les éléments que l'on peut atteindre grâce a des e-transitions
          if(nouvel_etat.find((*it)) == nouvel_etat.end()) {
            nouvel_etat.insert((*it));
          }
        }
      }
      int i = etat; 
      unsigned int check = 0;
      while(est_nouveau && i>= 0 && nouvel_etat.size() > 0) {/// vérifie si le set d'éléments est un nouvel élément du graphe déterministe
        if(nouveaux_etats[i].size() == nouvel_etat.size()) {
          for(set<etat_t>::iterator it = nouvel_etat.begin(); it != nouvel_etat.end(); it++) {
            if(nouveaux_etats[i].find((*it)) != nouveaux_etats[i].end())
              check++;
            }
            if(check == nouvel_etat.size())
              est_nouveau = false;
            check = 0;
        }
        if(est_nouveau)
          i--;
      }
      vector<set<etat_t> > sym;
      sym.resize(at.nb_symbs);
      r.trans.push_back(sym);
      if(est_nouveau && nouvel_etat.size() > 0) { ///insère le set d'éléments aux éléments du graphe déterministe si il est nouveau
        nouveaux_etats.push_back(nouvel_etat);
        r.trans[etat][symb].insert(nouveaux_etats.size()-1);
        for(set<etat_t>::iterator it = nouvel_etat.begin(); it != nouvel_etat.end(); it++) {
          if(at.finaux.find(*it) != at.finaux.end() && r.finaux.find(nouveaux_etats.size()-1) == r.finaux.end()){
            r.finaux.insert(nouveaux_etats.size()-1);
            r.nb_finaux++;
          }
        }

      }else if(nouvel_etat.size() > 0)
        r.trans[etat][symb].insert(i);
    }
  }
  r.nb_etats = nouveaux_etats.size();
  r.initial = 0;
  r.nb_symbs = at.nb_symbs;

  return r;
}

//******************************************************************************
