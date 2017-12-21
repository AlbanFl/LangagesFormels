//******************************************************************************

#include <iostream>
#include <algorithm>

#include "Accept.h"
#include "FA_tools.h"

//using namespace std;

////////////////////////////////////////////////////////////////////////////////

//Verifie seulement si c'est deterministe, pas si c'est complet
bool EstDeterministe(const sAutoNDE &at) {
	//TODO définir cette fonction
	unsigned int cpt;
	for(size_t i = 0 ; i < at.trans.size() ; i++){
		cpt = 0;
		for(size_t j = 0 ; j < at.trans[i].size() ; j++){
			if(at.trans[i][j].size() > 1)
				return false;
			else if(at.trans[i][j].size() == 1)
				cpt ++;
		}
		if(cpt != at.nb_symbs)
			return false;
	}

	for(size_t k = 0 ; k < at.epsilon.size() ; k++){
		if(at.epsilon[k].size() > 0)
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Fermeture(const sAutoNDE &at, etatset_t &e) {
	//TODO définir cette fonction
	for(etatset_t::iterator it = e.begin(); it != e.end() ; it++){
		for(etatset_t::iterator iteps = at.epsilon[*it].begin(); iteps != at.epsilon[*it].end() ; iteps++)
			e.insert(*iteps);
	}	
}

////////////////////////////////////////////////////////////////////////////////

etatset_t Delta(const sAutoNDE &at, const etatset_t &e, symb_t c) {
	//TODO définir cette fonction
	etatset_t r;
	for(etatset_t::iterator it = e.begin(); it != e.end() ; it++){
		for(etatset_t::iterator ite = at.trans[*it][c].begin() ; ite != at.trans[*it][c].end() ; ite++)
			r.insert(*ite);
		for(etatset_t::iterator iteps = at.epsilon[*it].begin() ; iteps != at.epsilon[*it].end() ; iteps++)
			r.insert(*iteps);
	}
	return r;
}

////////////////////////////////////////////////////////////////////////////////

bool Accept(const sAutoNDE &at, std::string str) {
	//TODO définir cette fonction
	return Accepte_recurs(at, at.initial, str);
}

bool Accepte_recurs(const sAutoNDE &at, etat_t etat, std::string str){
	//si le mot en parametre est vide, on a réussi a recreer le mot passe en entree a partir du graphe
	if(str.size() < 1){
		for(etatset_t::iterator it = at.finaux.begin(); it != at.finaux.end() ; it++){
			// si l'état actuel est un état final
			if(etat == *it)
				return true;
		}
		// si on peut aller vers un autre état avec une transition epsilon, on relance depuis le nouvel état
		for(etatset_t::iterator iteps = at.epsilon[etat].begin(); iteps != at.epsilon[etat].end(); iteps++){
			if(Accepte_recurs(at, *iteps, str)){
				return true;
			}
		}
		//sinon, on retourne faux
		return false;
	}
	//on recupere le symbole entre en parametre
	unsigned int symbl = (int)(str[0]);
	symbl = symbl - ASCII_A;
	if(symbl > at.nb_symbs-1)
		return false;
	
	//On fait la fermeture a partir de l'etat actuel et du symbole voulu
	etatset_t suivants;
	suivants.insert(etat);
	suivants = Delta(at, suivants, symbl);

	//s'il n'y a pas de possibilité d'avancer de l'état actuel avec le symbole voulu ou une liaison epsilon : faux
	if(suivants.size() < 1){
		return false;
	}

	std::string newStr;
	newStr = str;
	newStr.erase(0, 1);

	for(etatset_t::iterator it = suivants.begin(); it != suivants.end() ; it++){
	//pour chaque transition possible
		for(etatset_t::iterator ite = at.trans[etat][symbl].begin(); ite != at.trans[etat][symbl].end(); ite++){
			//on verifie si la transition se fait par la lettre voulue, auquel cas on relance la fonction avec une lettre en moins
			if(*it == *ite){
				if(Accepte_recurs(at, *it, newStr) == true)
					return true;
			}
		}
	
		for(etatset_t::iterator iteps = at.epsilon[etat].begin(); iteps != at.epsilon[etat].end(); iteps++){
			//on verifie si la transition est une transition epsilone, auquel cas on relance la fonction avec le meme mot, mais le nouvel etat
			if(*it == *iteps){
				if(Accepte_recurs(at, *it, str) == true)
					return true;
			}
		}	
		
	}
	//si la fonction n'a jamais pu renvoyer true, c'est que le mot n'existe pas
	return false;

}

//******************************************************************************

