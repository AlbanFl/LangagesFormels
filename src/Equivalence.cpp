//******************************************************************************

#include <iostream>
#include <cassert>

#include "Equivalence.h"
#include "Accept.h"
#include "ToGraph.h"
#include "Determinize.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction

	std::string sr ="aa";
	
	ToGraph(at, "./test_avant.txt");
	/*********************** REARANGEMENT DE L'AUTOMATE ***********************/
	std::cout << " lolilol " << std::endl;
	
	sAutoNDE at2 = Determinize(at);
	
	std::cout << at2.trans.size() << " et epsilon " << at2.epsilon.size() << std::endl;
	
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
	
	ToGraph(at2, "./test_apres.txt");
	
	/*********************** TROUVER L'EXPRESSION RATIONNELLE ***********************/
	
	
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
  //TODO définir cette fonction

  return true;
}

//******************************************************************************
