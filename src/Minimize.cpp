//******************************************************************************

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include "Determinize.h"
#include "Minimize.h"
#include "FA_tools.h"

////////////////////////////////////////////////////////////////////////////////
using namespace std;

sAutoNDE Minimize(const sAutoNDE& at){
  
  sAutoNDE r;
  sAutoNDE ad;
  ad = Determinize(at);

  etat_t e;
  etatset_t nf, f ,temp, ins_partition, ins_trans;
  vector<etatset_t> partitions, partitions_temp;
  map<unsigned int, etatset_t> map_part;

   for(unsigned int i = 0; i < ad.nb_etats; i++) { // On crée les partitions de basent : Etats non-finaux et finaux
      etatset_t::iterator it1 = ad.finaux.find(i);
      if(it1 != (ad.finaux.end())){
        f.insert(i);
       }
       else{
        nf.insert(i);
       }
   }


   cout << endl << "Partition des etats finaux: " << f << endl;
   cout << "Partition des etats non-finaux: " << nf << endl;

  partitions.push_back(nf);
  partitions.push_back(f);

  bool partitionne = true; 
  partitions_temp = partitions;

  while(partitionne == true){ //Tant qu'on partitionne on reste dans le while
    partitionne = false;
    for(unsigned int i = 0; i < partitions.size(); i++){
      for(unsigned int j = 0; j < ad.nb_symbs; j++){
        map_part.clear(); //On vide la map dés qu'on change de symbole
        for(etatset_t::iterator it1 = partitions[i].begin(); it1 != partitions[i].end(); ++it1){
            unsigned int k;
            for(k=0 ; k < partitions.size(); k++){
              etatset_t::iterator it2 = partitions[k].find(*(ad.trans[*it1][j].begin()));// On va chercher dans la matrice de transition la valeur correspondante à l'état
              if( it2 != partitions[k].end()){
                break; //Dés qu'on trouve l'état de destination dans une des partitions on break
              }
            }
            map<unsigned int, etatset_t>::iterator it3 = map_part.find(k);
            if(it3 == map_part.end()){ // Si on ne trouve pas la partition dans le map on entre dans le for
              ins_partition.clear(); // On clear le set de partition temporaire pour le re-remplir
              ins_partition.insert(*it1); // On insère la partition dans un Set temporaire
              map_part.insert(make_pair(k,ins_partition));//On insère l'état de départ et la partition de destination dans la map
            }
            else {
              it3->second.insert(*it1);
            }
        }
        if(map_part.size()>1){ // Si on a ajouté des élements à notre map alors cela veut dire qu'on partitionne, on rentre dans le if et on break
          partitionne = true;
          break;
        }
      }
      if(partitionne == true){
        partitions_temp[i] = map_part.begin()->second; //Si on partitionne, on rajoute la  première partition contenue dans la map à la position i dans le conteneur temporaire de partitions
        for(map<unsigned int, etatset_t>::iterator it4 = next(map_part.begin()); it4 != map_part.end(); ++it4){ // On parcours ensuite la map et on ajoute toutes les autres partitions contenus dedans
          partitions_temp.push_back(it4 ->second);

        }
       map_part.clear();
      }
    }
    if(partitionne == true){// Le conteneur  principale de partitions "sauvegarde" les partitions obtenues et stockés jusqu'ici dans le conteneur temporaire
      partitions = partitions_temp;
    }

  }

  cout << "Etats de l'automate minimise: " << partitions << endl;

  r.nb_etats = partitions.size();
  r.trans.resize(r.nb_etats);
  r.nb_symbs = ad.nb_symbs;
  int e_fin = 0;

  // On va maintenant reconstruire l'automate à partir des nouveaux états que nous avons trouvés

  for(unsigned int i = 0; i < partitions.size(); i++){
    for(etatset_t::iterator it1=partitions[i].begin() ; it1 != partitions[i].end() ; ++it1){
      etatset_t::iterator it2 = ad.finaux.find(*it1);
      if(it2 != ad.finaux.end()){
        r.finaux.insert(i); //Chaques états finales de l'automate de base est un état finale de l'automate minimisé
        e_fin++;
      }
      if(ad.initial == *it1){ 
        r.initial = i; //L'état initial de l'automate de base est l'état initial de l'automate minimisé
        }
    }

    r.trans[i].resize(r.nb_symbs); // On s'occupe maintenant de remplire les transitions
    for(unsigned int j = 0; j < r.nb_symbs; j++){
      e = *(partitions[i].begin()); // e prend la valeur du première état de la partition du conteneur à la position i
      unsigned int k;
      ins_trans.clear();
      for(k = 0; k < partitions.size(); k++){ //
        etatset_t::iterator it3 = partitions[k].find(*(ad.trans[e][j].begin())); //On break dés que l'on arrive à la fin de la partition
        if(it3 != partitions[k].end()){
          break;
        }
      }
      ins_trans.insert(k);
      r.trans[i][j] = ins_trans; //On insère dans la matrice les transitions stockés temporairement aupravant
    }
  }
  r.nb_finaux = e_fin;



  return r;
}

//******************************************************************************
