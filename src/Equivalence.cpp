//******************************************************************************

#include <iostream>
#include <cassert>
#include <string.h>

#include "Equivalence.h"
#include "Accept.h"
#include "Minimize.h"
#include "ToGraph.h"
#include "Determinize.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction

	std::string sr;
	
	/*********************** REARANGEMENT DE L'AUTOMATE (FONCTIONNEL) ***********************/
	
	sAutoNDE at2 = Determinize(at);
	
	//on push de nouveaux etats vides dans epsilon et trans
	std::vector<std::set<etat_t>> nouveau;
	std::vector<std::set<etat_t>> ancien;
	at2.trans.push_back(nouveau);
	at2.trans.push_back(nouveau);
	std::set<etat_t> nouveau_eps;
	at2.epsilon.push_back(nouveau_eps);
	at2.epsilon.push_back(nouveau_eps);
	
	//On avance tous les etats d'une "case" dans trans
	std::set<etat_t> nouveau_set;
	
	for(size_t i = 0 ; i < at2.trans.size() - 1 ; i++){
		for(size_t j = 0; j < at2.trans[i].size() ; j++){
			for(etatset_t::iterator it = at2.trans[i][j].begin(); it != at2.trans[i][j].end() ; it++){
				nouveau_set.insert(*it + 1);
				at2.trans[i][j].erase(*it);
			}
			for(etatset_t::iterator ite = nouveau_set.begin(); ite != nouveau_set.end() ; ite++){
				at2.trans[i][j].insert(*ite);
				nouveau_set.erase(*ite);
			}
		}
		
		ancien = at2.trans[i];
		at2.trans[i] = nouveau;
		nouveau = ancien;
	}
	
	//Pour chaque etat final, on y fait une transition vers le dernier etat, et on le supprime de la liste des etats finaux
	for(etatset_t::iterator it = at2.finaux.begin(); it != at2.finaux.end() ; it++){
		at2.epsilon[*it+1].insert(at2.nb_etats + 1);
		at2.finaux.erase(*it);
	}
	
	//On insère le nouvel état final dans la liste des états finaux, et on passe à 1 le nb d'états finaux
	at2.finaux.insert(at2.nb_etats + 1);
	at2.nb_finaux = 1;
	
	//On ajoute 2 au nb d'états
	at2.nb_etats += 2;
	
	//On crée une transition epsilon de 0 vers l'ancien état initial, et on change l'état initial
	at2.epsilon[0].insert(at2.initial + 1);
	at2.initial = 0;
	
	/*********************** TROUVER L'EXPRESSION RATIONNELLE (NON FONCTIONNEL) ***********************/
	
	//tableau qui va se remplit peu à peu en regardant comment aller de l'etat 0 à l'etat voulu
	string tab[at2.nb_etats + 1][at2.nb_etats + 1];
	for(unsigned int i = 0 ; i <= at2.nb_etats ; i++){
		for(unsigned int j = 0 ; j <= at2.nb_etats ; j++){
			tab[i][j] = "";
		}
	}
	string str_temp;
	for(unsigned int etat = 1 ; etat < at2.nb_etats ; etat++){
		str_temp = "";
		for(unsigned int i = 1 ; i < at2.nb_etats ; i++){
			if(at2.trans[etat][i].size() != 0){
				for(etatset_t::iterator it = at2.trans[etat][i].begin(); it != at2.trans[etat][i].end() ; it++){
					if(*it == etat){
						if(str_temp == ""){
							str_temp = (char)(i + ASCII_A);
						}else{
							str_temp = str_temp + "|" + (char)(i + ASCII_A);
						}
					}
				}
			}
		}
		tab[etat][etat] = "(" + str_temp + ")*";
		str_temp = "";
		for(unsigned int i = 1 ; i < at2.nb_etats ; i++){
			if(at2.trans[etat][i].size() != 0){
				for(etatset_t::iterator it = at2.trans[etat][i].begin(); it != at2.trans[etat][i].end() ; it++){
					if(tab[etat][*it] == ""){
						tab[etat][*it] = tab[etat][etat] + "." + (char)(i+ASCII_A);
					}
					else{	
						tab[etat][*it] = tab[etat][*it] + "|" + tab[etat][etat] + "." + (char)(i+ASCII_A);
					}
				}
			}
		}
	}
	
	sr = tab[0][at2.nb_etats-1];
	
	return sr;
}

////////////////////////////////////////////////////////////////////////////////

bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max, string word) {
  //TODO définir cette fonction
	if((Accept(a1, word) && Accept(a2, word)) || (!Accept(a1, word) && !Accept(a2, word))){ //si le mot est accept� ou refus� par les deux automates
		if(word.size() == word_size_max) // on stoppe la recursivit� si le mot est aussi long que la taille max
			return true;
		else{ //sinon
			for(unsigned int symb=ASCII_A; symb<ASCII_Z; symb++){ //pour chaque symbole de l'alphabet
				string test_word = word;
				string letter;
				letter = (char)symb;
				test_word = test_word+letter;
				 if (!PseudoEquivalent(a1, a2, word_size_max, test_word)) //on r�appelle la fonction avec le mot entr� en param�tre + le symbole 
					 return false; //si on se retrouve avec un false, on retourne false en cascade
			}
			return true;
		}
	} else
		return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
  
	sAutoNDE am1, am2;
	bool equivalent = true;

	am1 = Minimize(a1);
	am2 = Minimize(a2);
	cout << endl;
	if( am1.nb_etats == am2.nb_etats){
		if(am1.finaux == am2.finaux){
			for(unsigned int i; i < am1.trans.size(); i++){
				for(unsigned int j; j < am1.nb_symbs; j++){
					if(am1.trans[i][j] != am2.trans[i][j]){
						equivalent = false;
					}
				}
			}
		}
		else{
			equivalent = false;
		}
	}
	else{
		equivalent = false;
	}
  return equivalent;
  //On ne prend pas en compte le nommage des etats
}

//******************************************************************************
