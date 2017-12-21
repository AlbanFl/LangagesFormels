//******************************************************************************

#include <iostream>
#include <cassert>

#include "Equivalence.h"
#include "Accept.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction

  std::string sr;

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
