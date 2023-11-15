#include "qwirkles_functions.h"
/**     DANS CERTAINES FONCTION ON FAIT (VALEUR / 6)%6 pour que la valeur 36 (coeur jaune)
    soit compté comme si s'etait 0 car je suis un peu bête parfois et j'ai mal codé la définition des tuile */


void partie(JOUEUR listeJoueur[4],short int nbJoueur,short int mode,short int loading){

    /**Initialisation des variables */
    short int plateau[12][26];      //tableau 2D stockant les tuiles sur le plateau
    short int plateauPourScore[12][26];
    short int pioche[108];          //tableau stockant la pioche
    short int partieEnCours;        //variable pour savoir si la partie est toujours en cours
    short int finDeTour;            //variable pour savoir si le tour est fini
    int tour;                       //permet de savoir a quelle tour on est
    short int i;
    short int j;
    short int choix;                //variable du choix dans le menu de la partie
    short int nbPiocheRestante;     //la variable pour savoir où on en est dans la pioche
    int touche;                     //la variable pour la touche appuyé
    short int tuileInitiale;        //la variable pour savoir si la première tuile a été placé


    /**affectation de base */
    tuileInitiale=0;
    partieEnCours=1;
    nbPiocheRestante =0;
    tour=0;
    choix=0;

    if(loading==0){
        /** generation de la pioche */
        piocheMelange(pioche,mode);

        /**  Initialisation du plateau avec que la case vide (la case vide c'est le 0)  **/
        for(i=0;i<12;i++){
            for(j=0;j<26;j++){
                plateau[i][j]=0;
            }
        }


        /** Première affectation de la tuile de la pioche dans le premier joueur
            On est obligé car sinon il y a un bug dans son affectation            */
        if(listeJoueur[0].tuiles[0]==0){
            nbPiocheRestante = affectationPioche(listeJoueur,tour,pioche,nbPiocheRestante,0,nbJoueur,mode);
        }
    }


    /** load de la partie */
    else{ //%hi c'est pour les short int
        FILE* sauvegarde; //pointeur du fichier txt
        sauvegarde = fopen("sauvegarde.txt","r+");
        if(sauvegarde==NULL){exit(1);}

        fscanf(sauvegarde,"%hi",&mode);
        fscanf(sauvegarde,"%hi",&nbJoueur);
            for(i=0;i<nbJoueur;i++){
                fscanf(sauvegarde,"%19s%*c",listeJoueur[i].name);
                //fgets(listeJoueur[i].name,20,sauvegarde);
                fscanf(sauvegarde,"%d",&listeJoueur[i].score);
                for(j=0;j<6;j++){
                    fscanf(sauvegarde,"%hi",&listeJoueur[i].tuiles[j]);
                }
            }
        for(i=0;i<108-72*mode;i++){
            fscanf(sauvegarde,"%hi",&pioche[i]);
        }
        fscanf(sauvegarde,"%hi",&nbPiocheRestante);
        fscanf(sauvegarde,"%hi",&tuileInitiale);
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 26; j++) {
                fscanf(sauvegarde, "%hi", &plateau[i][j]);
                plateauPourScore[i][j] = plateau[i][j];
            }
        }
        fscanf(sauvegarde,"%d",&tour);
        fclose(sauvegarde);

    }



    affichageContourPlateau();//affichage des contours du plateau
    while(partieEnCours){
        /**remplissage de la main de tuiles du joueur */
        for(i=0;i<6;i++){
            if(listeJoueur[tour%nbJoueur].tuiles[i]==0){
                nbPiocheRestante = affectationPioche(listeJoueur,tour,pioche,nbPiocheRestante,i,nbJoueur,mode);
            }
        }
        /**detection de fin de partie*/
        i=0;
        while(listeJoueur[tour%nbJoueur].tuiles[i]==0){i++;}
        if(i>4){partieEnCours=0;finDeTour=1;}//si le joueur a ses 6 tuiles de vide alors findutour et fin de partie


        tuilesEnMain(listeJoueur[tour%nbJoueur]); //affichage des tuiles en main

        /**affichage du nom du joueur qui joue */
        placementEcriture(49,1);
        printf("                     ");
        placementEcriture(49,1);
        printf("%s",listeJoueur[tour%nbJoueur].name);

        finDeTour=0; //reset de la variable de la boucle du joueur qui joue
        while(!finDeTour){

            touche = keyDetection(); //detection de la touche qui est entré : entree/espace = 1,  z=2,q=3,s=4,d=5, les autres=0
            if(touche!=0){


                choix = choixMenuJeu(touche,choix); //permet de savoir quelle action on choisit
                flecheMenuJeu(choix);               //affiche la fleche dans le menu des choix
                tuilesEnMain(listeJoueur[tour%nbJoueur]);   //affiche la main du joueur

                /**detection choix apres entree*/
                if(touche==1){   //si on choisit de faire l'action
                        if(choix>=0 && choix <6){ //si c'est une des tuiles
                            tuileInitiale = placementTuile(listeJoueur,plateau,plateauPourScore,tour,nbJoueur,choix,tuileInitiale);
                            effacementFlecheJeu();
                        }
                        if(choix==6){ //si c'est le bouton fin de tour
                            finDeTour=1;
                        }
                }

                /**detection echange*/
                else if(touche==6 && choix>=0 && choix <6 && listeJoueur[tour%nbJoueur].tuiles[choix]!=0 && nbPiocheRestante>0){

                    nbPiocheRestante--;
                    for(i=nbPiocheRestante;i<108-72*mode;i++){
                        pioche[i]=pioche[i+1]; //on décale la pioche vers la gauche de 1
                    }
                    pioche[108-72*mode] =listeJoueur[tour%nbJoueur].tuiles[choix]; //on reaffecte la tuile dans la pioche
                    listeJoueur[tour%nbJoueur].tuiles[choix]=0; //reset de la tuile a changer
                    melangertuiles(nbPiocheRestante,pioche,mode); //melange de la pioche
                }
                else if(touche==7){
                    sauvegarder(mode,nbJoueur,listeJoueur,pioche,nbPiocheRestante,tuileInitiale,plateau,tour);
                        placementEcriture(39,15);
                        printf("          Partie Sauvegard%ce !",130);
                        printf("Appuyer sur t pour sauvegarder");
                }


                /**Affichage du plateau */
                for(int k=0;k<26;k++){                      //longueur du plateau
                    for(int l=0;l<12;l++){                  //hauteur du plateau
                        placementEcriture(k+4,l+3); //positionnement du curseur dans la console pour ecrire a une position précise
                        if(plateau[l][k]!=0){               // si la tuile n'est pas vide
                           affichageTuile(plateau[l][k]);   //affichage de la tuile
                        }
                    }
                }
                affichageScore(listeJoueur,nbJoueur,35,9);
                touche=0; //reset de la touche appuyé
            }
        }
        for(i=0;i<12;i++){
            for(j=0;j<26;j++){
                plateauPourScore[i][j] = plateau[i][j];
            }
        }




        tour++;//on incrémente le tour de 1
    }

    system("cls");

    placementEcriture(0,0);
    printf("-------Partie Terminée-------\n  SCORE FINAL");

    affichageScore(listeJoueur,nbJoueur,3,3);


    placementEcriture(0,10);
}


int main(){
    zoomConsole();
    JOUEUR listeJoueur[4];
    short int nbJoueur;
    short int mode;             //mode 0 = normal       mode 1 = dégradé
    srand(time(NULL));          // vrai aléatoire fait a partir du temps du pc
    short int loading;

    mode =0;
    nbJoueur=4;
    while(1){                       //boucle pourqu'on revienne a chaque fin de partie au menu principale
        loading = menuPrincipal();
        if(loading==0){
            mode = choixDuMode(); //choix du mode de partie
            nbJoueur = nombreJoueurs(); //choix du nombre de joueur
        joueurInit(listeJoueur,nbJoueur);
        }
        system("cls");
        partie(listeJoueur,nbJoueur,mode,loading);
        getch();
        system("cls");
    }
    return 0;
}

