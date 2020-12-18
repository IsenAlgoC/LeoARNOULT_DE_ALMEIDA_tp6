#include <stdio.h>   /* pour les entr�es-sorties */
#include <string.h>  /* pour les manipulations de cha�nes de caract�res */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 3.0
#define SQUELET
/**************************************************************************/
/* Compl�ter votre nom ici                                                */
/*   Nom : Arnoult de almeida                        Pr�nom :   L�o       */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le r�pertoire stock� en m�moire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire *rep, Enregistrement enr)
{
#ifdef IMPL_TAB
	int idx;

	if (rep->nb_elts < MAX_ENREG) //si il y a encore de la place
	{	
		*(rep->tab + rep->nb_elts ) = enr; // on ajoute au bout de la liste le contact
		rep->nb_elts += 1; // on augmente le nb d'�l�ment
		modif = true; // pour d�clencher la sauvegarde
	}
	else {
		return(ERROR);
	}

	
#else
#ifdef IMPL_LIST

	bool inserted = false;
	if (rep->nb_elts == 0) { // si c'est le premier contact
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) { // si pas de pb d'allocation
			rep->nb_elts += 1;  // on augmente le nb d'�l�ment
			modif = true; //flag de modification
			rep->est_trie = true; //on marque que la liste est tri� comme il n'y a qu'un maillon
			return(OK);
		}

	}
	else {
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) { // si la liste a plus d'un maillon
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = false; // pour que la liste soit trier apr�s
			return(OK);
		}
	}


#endif
	
#endif


	return(OK);

} /* fin ajout */
  /**********************************************************************/
  /* supprime du r�pertoire l'enregistrement dont l'indice est donn� en */
  /*   param�tre       et place modif = true                            */
  /**********************************************************************/
#ifdef IMPL_TAB
void supprimer_un_contact_dans_rep(Repertoire *rep, int indice) {

	// compl�ter code ici pour tableau
	if (rep->nb_elts >= 1)		//s'il y a au moins un element dans le tableau
	{						

		for (int i = indice; i < rep->nb_elts; i++) {
			*(rep->tab + i) = *(rep->tab + i + 1); //on d�cale les valeurs du tableau
		}


		rep->nb_elts -= 1;		//on d�cr�mente le compteur d'�l�ment
		modif = true;
	}

	return;
} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du r�pertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif � vrai                              */
  /************************************************************************/
  // complet

	int supprimer_un_contact_dans_rep_liste(Repertoire *rep, SingleLinkedListElem *elem) {
	
	int ret=DeleteLinkedListElem(rep->liste, elem); //on appelle la fonction DeleteLinkedListElem qui supprime l'element
	if (ret == 1) { // si pas de pb
		rep->nb_elts--; // on d�cr�mente le nb d'�lement
		modif = true;
	}

	return (0); 
	}
#endif
#endif


  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement sur une ligne � l'�cran  */
  /* ex Dupont, Jean                 0320304050                         */
  /**********************************************************************/
void affichage_enreg(Enregistrement enr)
{
	printf("%s, %s				%s", enr.nom, enr.prenom, enr.tel); //simple print des elements


} /* fin affichage_enreg */
  /**********************************************************************/
  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
  /* pour les listes                                                    */
  /* ex | Dupont                |Jean                  |0320304050      */
  /**********************************************************************/
void affichage_enreg_frmt(Enregistrement enr)
{

	printf("|%-30s      |%-30s      |%-20s\n", enr.nom, enr.prenom, enr.tel); // %-30s permet de compl�t� par un espace 
	//si pour que la chaine fasse toujours la m�me taille : ici 30
} /* fin affichage_enreg */

  /**********************************************************************/
  /* test si dans l'ordre alphabetique, un enregistrement est apres     */
  /* un autre                                                           */
  /**********************************************************************/
bool est_sup(Enregistrement enr1, Enregistrement enr2)
{
	if (_strcmpi(enr1.nom, enr2.nom) < 0) { // si le nom1 est sup�rieur � enr2 dans l'alphabet
		return(true);
	}
	else if (_strcmpi(enr1.nom, enr2.nom) > 0) {
		return(false);
	}
	else {
		if (_strcmpi(enr1.prenom, enr2.prenom) < 0) {
			return(true);
		}
		else if (_strcmpi(enr1.prenom, enr2.prenom) > 0) {
			return(false);
		}
		else { return true; } // en cas de nom et prenom == on retourne true
	}
}
 
/*********************************************************************/
/*   Tri Alphabetique du tableau d'enregistrements                   */
/*********************************************************************/

void trier(Repertoire *rep)
{

#ifdef IMPL_TAB

	int boucle = 1;
	while (boucle) //bouche jusqu'� ce que �a soit trier
	{
		boucle = 0;
		for (int j = 0; j < rep->nb_elts - 1; j++) //on parcourt la liste
		{
			if (!est_sup(*(rep->tab + j), *(rep->tab + j + 1))) // si un mot est sup�rieur � une lettre est sup � une autre
			{
				Enregistrement tmp = *(rep->tab + j);
				*(rep->tab + j) = *(rep->tab + j + 1); // on d�cale la liste
				*(rep->tab + j + 1) = tmp;
				boucle = 1;
			}
		}
	}

	
	
#else
#ifdef IMPL_LIST
	// ajouter code ici pour Liste
	// rien � faire !
	// la liste est toujours tri�e
#endif
#endif


	rep->est_trie = true;

} /* fin trier */

  /**********************************************************************/
  /* recherche dans le r�pertoire d'un enregistrement correspondant au  */
  /*   nom � partir de l'indice ind                                     */
  /*   retourne l'indice de l'enregistrement correspondant au crit�re ou*/
  /*   un entier n�gatif si la recherche est n�gative				    */
  /**********************************************************************/

int rechercher_nom(Repertoire *rep, char nom[], int ind)
{
	int i = ind;		    /* position (indice) de d�but de recherche dans tableau/liste rep */
	int ind_fin;			/* position (indice) de fin de tableau/liste rep */

	char tmp_nom[MAX_NOM];	/* 2 variables temporaires dans lesquelles */
	char tmp_nom2[MAX_NOM];	/* on place la chaine recherchee et la chaine lue dans le */
							/* tableau, afin de les convertir en majuscules et les comparer */
	bool trouve = false;		


#ifdef IMPL_TAB
	ind_fin = rep->nb_elts;
	strcpy_s(tmp_nom, _countof(tmp_nom), nom); // on copie la chaine de caract�re de nom vers tmp_nom
	while ((!trouve) && (i < ind_fin)) {
		strcpy_s(tmp_nom2, _countof(tmp_nom2), rep->tab[i].nom); // on met dans
		if (_strcmpi(tmp_nom, tmp_nom2) == 0) { // si les 2 chaines de caract�res avec maj son �gal on change la valeur de trouve
			trouve = true;
		}
		else { i++; } // si on ne trouve pas le mot et que l'on est encore <ind_fin on incr�mente i et on reboucle
	}
	
#else
#ifdef IMPL_LIST
	ind_fin = rep->nb_elts;
	strcpy_s(tmp_nom, _countof(tmp_nom), nom); // on copie la chaine de caract�re de nom vers tmp_nom
	while ((!trouve) && (i < ind_fin)) {
		strcpy_s(tmp_nom2, _countof(tmp_nom2), GetElementAt(rep->liste,i)->pers.nom); 
		if (_strcmpi(tmp_nom, tmp_nom2) == 0) { // si les 2 chaines de caract�res avec maj son �gal on change la valeur de trouve
			trouve = true;
		}
		else { i++; } // si on ne trouve pas le mot et que l'on est encore <ind_fin on incr�mente i et on reboucle
	}						// ajouter code ici pour Liste
	
#endif
#endif

	return((trouve) ? i : -1);
} /* fin rechercher_nom */

  /*********************************************************************/
  /* Supprimer tous les caracteres non num�riques de la chaines        */
  /*********************************************************************/
void compact(char *s)
{
	int n = strlen(s);
	int i = 0;
	while (i != n) {
		if (isdigit(s[i]) == 0) { //on rentre dans le if si le i�me caract�re est num�rique
			n = strlen(s);
			for (int j = i; j < n; j++) { // on d�cale toutes les autres lettres
				*(s + j) = *(s + j + 1);
			}
		}
		else { i += 1; }
		
	}
	return;
}

/**********************************************************************/
/* sauvegarde le r�pertoire dans le fichier dont le nom est pass� en  */
/* argument                                                           */
/* retourne OK si la sauvegarde a fonctionn� ou ERROR sinon           */
/**********************************************************************/
int sauvegarder(Repertoire *rep, char nom_fichier[])
{
	FILE *fic_rep;
	char buffer[sizeof(Enregistrement) + 1];
	errno_t err;
#ifdef IMPL_TAB
	err = fopen_s(&fic_rep, nom_fichier, "w"); // on ouvre le fichier
	if (err != 0 || fic_rep == NULL) { // en cas de pb
		return ERROR;
	}
	else {
		for (int i = 0; i < rep->nb_elts; i++) {
			sprintf_s(buffer, sizeof(buffer),"%s%c%s%c%s\n", rep->tab[i].nom,SEPARATEUR, rep->tab[i].prenom,SEPARATEUR, rep->tab[i].tel);
			// on cr�e un buffer qui va ajouter tous les elements dans une chaine de caract�re
			fputs(buffer, fic_rep); // on ajoute la ligne au fichier
		}
		fclose(fic_rep); // on ferme le fichier
	}
#else
#ifdef IMPL_LIST
	err = fopen_s(&fic_rep, nom_fichier, "w");
	if (err != 0 || fic_rep == NULL) {
		return ERROR;
	}
	else {
		for (int i = 0; i < rep->nb_elts; i++) {
			sprintf_s(buffer, sizeof(buffer), "%s%c%s%c%s\n", GetElementAt(rep->liste, i)->pers.nom, SEPARATEUR, GetElementAt(rep->liste, i)->pers.prenom, SEPARATEUR, GetElementAt(rep->liste, i)->pers.tel);
			// on cr�e un buffer qui va ajouter tous les elements dans une chaine de caract�re
			fputs(buffer, fic_rep); // on ajoute la ligne au fichier
		}
		fclose(fic_rep);
	}
#endif
#endif

	return(OK);
} /* fin sauvegarder */


  /**********************************************************************/
  /*   charge dans le r�pertoire le contenu du fichier dont le nom est  */
  /*   pass� en argument                                                */
  /*   retourne OK si le chargement a fonctionn� et ERROR sinon         */
  /**********************************************************************/

int charger(Repertoire *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Enregistrement);
	char buffer[sizeof(Enregistrement) + 1];
	int idx = 0;

	char *char_nw_line;
	
	_set_errno(0);
	if ( ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL) )
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
				if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
							num_rec++;		/* element � priori correct, on le comptabilise */
					}
				}
#else
#ifdef IMPL_LIST
				Enregistrement enr;
				if (lire_champ_suivant(buffer, &idx, enr.nom, MAX_NOM, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, enr.prenom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, enr.tel, MAX_TEL, SEPARATEUR) == OK) {
							InsertElementAt(rep->liste, rep->liste->size, enr); // on ajoute l'element dans la liste chain�e

							num_rec++;
						}		/* element � priori correct, on le comptabilise */

					}
				}
#endif
#endif




			}

		}
		rep->nb_elts = num_rec;
		fclose(fic_rep);
		return(OK);
	}


} /* fin charger */