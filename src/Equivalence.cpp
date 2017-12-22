//******************************************************************************

#include <iostream>
#include <cassert>

#include "Equivalence.h"
#include "Accept.h"
#include "Minimize.h"
#include "ToGraph.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction

	std::string sr ="aa";
	
	ToGraph(at, "./test2.txt");
	
	/*********************** REARANGEMENT DE L'AUTOMATE ***********************/
	
	//on push de nouveaux etats vides dans epsilon et trans
	std::vector<std::set<etat_t>> nouveau;
	std::vector<std::set<etat_t>> ancien;
	at.trans.push_back(nouveau);
	at.trans.push_back(nouveau);
	std::set<etat_t> nouveau_eps;
	std::set<etat_t> ancien_eps;
	at.epsilon.push_back(nouveau_eps);
	at.epsilon.push_back(nouveau_eps);
	
	//On avance tous les etats d'une "case" dans trans
	std::set<etat_t> nouveau_set;
	
	for(size_t i = 0 ; i < at.trans.size() ; i++){
		for(size_t j = 0; j < at.trans[i].size() ; j++){
			for(etatset_t::iterator it = at.trans[i][j].begin(); it != at.trans[i][j].end() ; it++){
				nouveau_set.insert(*it + 1);
				at.trans[i][j].erase(*it);
			}
			for(etatset_t::iterator ite = nouveau_set.begin(); ite != nouveau_set.end() ; ite++){
				at.trans[i][j].insert(*ite);
				nouveau_set.erase(*ite);
			}
		}
		
		ancien = at.trans[i];
		at.trans[i] = nouveau;
		nouveau = ancien;
		
		for(etatset_t::iterator it = at.epsilon[i].begin(); it != at.epsilon[i].end() ; it++){
			nouveau_set.insert(*it + 1);
			at.epsilon[i].erase(*it);
		}
		for(etatset_t::iterator ite = nouveau_set.begin(); ite != nouveau_set.end() ; ite++){
			at.epsilon[i].insert(*ite);
			nouveau_set.erase(*ite);
		}

		ancien_eps = at.epsilon[i];
		at.epsilon[i] = nouveau_eps;
		nouveau_eps = ancien_eps;
	}
	
	//Pour chaque etat final, on y fait une transition vers le dernier etat, et on le supprime de la liste des etats finaux
	for(etatset_t::iterator it = at.finaux.begin(); it != at.finaux.end() ; it++){
		at.epsilon[*it+1].insert(at.nb_etats + 1);
		at.finaux.erase(*it);
	}
	at.finaux.insert(at.nb_etats + 1);
	at.nb_finaux = 1;
	
	at.nb_etats += 2;
	
	at.epsilon[0].insert(at.initial + 1);
	at.initial = 0;
	
	ToGraph(at, "./test.txt");
	std::cout << at.trans.size() << " et epsilon " << at.epsilon.size() << std::endl;
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
