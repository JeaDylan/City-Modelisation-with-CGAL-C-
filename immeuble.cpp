#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/draw_linear_cell_complex.h>
#include "My_linear_cell_complex_incremental_builder.h"
#include <stdlib.h>
#include "immeuble.h"
#include "lcc_def.h"

// Fonction de création des murs de droite et de gauche, fonction qui est utilisée deux fois pour la création d'une maison
void immeuble::murCote (LCC& lcc, Dart_handle D){

    Dart_handle dh1 = lcc.beta(D, 1);
    Dart_handle dh4 = lcc.beta(D, 0,0);

    Dart_handle dh2 = lcc.beta(D, 1,1);
    Dart_handle dh3 = lcc.beta(dh4, 0);

    Dart_handle dh6 = lcc.insert_cell_1_in_cell_2(dh4, dh1);
    Dart_handle dh5 = lcc.insert_cell_1_in_cell_2(dh2, dh3);

}

// fonction d'initialisation du sol
// on créé le quadrillage du sol
void immeuble::Bas(LCC& lcc, Dart_handle D){
    // création de 4 points sur les arrètes après récupération des arrêtes
    Point p1 = lcc.point(D);
    Point p2 = lcc.point(lcc.other_extremity(D));
    Point p3 = lcc.point(lcc.beta(lcc.beta(D, 1), 1));

    float x = p1.x();
    float y = p1.y();
    float z = p3.z();

    float lx = p2.x() - p1.x();
    float lz = p2.z() - p3.z();

    Dart_handle dh7 = lcc.insert_point_in_cell<1>(lcc.beta(D, 1), Point(x + lx, y, z + lz - 0.2));
    Dart_handle dh9 = lcc.insert_point_in_cell<1>(dh7, Point(x+lx, y, z+0.2));
    Dart_handle dh10 = lcc.insert_point_in_cell<1>(lcc.beta(D, 0), Point(x, y, z+0.2));
    Dart_handle dh8 = lcc.insert_point_in_cell<1>(dh10,Point(x, y, z + lz - 0.2));

    // on relie ces quatres points
    Dart_handle dh11 = lcc.insert_cell_1_in_cell_2(dh8, dh7);
    Dart_handle dh12 = lcc.insert_cell_1_in_cell_2(dh9, dh10);

    // on pose 4 points sur les nouvelles lignes

    Dart_handle dh14 = lcc.insert_point_in_cell<1>(dh11, Point(x+lx-0.2, y, z+lz-0.2));
    Dart_handle dh13 = lcc.insert_point_in_cell<1>(dh14, Point(x+0.2, y, z+lz-0.2));
    Dart_handle dh16 = lcc.insert_point_in_cell<1>(dh12, Point(x+0.2, y, z+0.2));
    Dart_handle dh15 = lcc.insert_point_in_cell<1>(dh16, Point(x+lx-0.2, y, z+0.2));

    // on relie ces quatres nouveaux points.
    Dart_handle dh17 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh11,2), lcc.beta(dh16,2));
    Dart_handle dh18 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh12,2), lcc.beta(dh14, 2));

}

// Fonction d'initialisation de la face du haut
// c'est exactement le même principe que pour la face du bas
void immeuble::Haut(LCC& lcc, Dart_handle D){
    // création de 4 points sur les arrètes
    Point p1 = lcc.point(D);
    Point p2 = lcc.point(lcc.other_extremity(D));
    Point p3 = lcc.point(lcc.beta(lcc.beta(D, 1), 1));

    float x = p1.x();
    float y = p1.y();
    float z = p3.z();

    float lx = p3.x() - p2.x();
    float lz = p1.z() - p2.z();

    // on fait la même chose pour la face du haut :

    // création de 4 points sur la face du haut, 2 points sur deux arrêtes :
    Dart_handle dh19 = lcc.insert_point_in_cell<1>(lcc.beta(lcc.beta(D, 1), 1), Point(x+lx, y, z+0.2));
    Dart_handle dh20 = lcc.insert_point_in_cell<1>(dh19, Point(x + lx, y, z +lz-0.2));
    Dart_handle dh21 = lcc.insert_point_in_cell<1>(D, Point(x, y, z+lz-0.2));
    Dart_handle dh22 = lcc.insert_point_in_cell<1>(dh21,Point(x, y, z + 0.2));

    // on relie ces quatres points 2 à 2
    Dart_handle dh23 = lcc.insert_cell_1_in_cell_2(dh22, dh19);
    Dart_handle dh24 = lcc.insert_cell_1_in_cell_2(dh20, dh21);

    // on créé 4 points sur ces nouveaux traits
    Dart_handle dh25 = lcc.insert_point_in_cell<1>(dh23, Point(x+lx-0.2, y, z+0.2));
    Dart_handle dh26 = lcc.insert_point_in_cell<1>(dh25, Point(x + 0.2, y, z +0.2));
    Dart_handle dh27 = lcc.insert_point_in_cell<1>(dh24, Point(x+0.2, y, z+lz-0.2));
    Dart_handle dh28 = lcc.insert_point_in_cell<1>(dh27,Point(x+lx-0.2, y, z +lz- 0.2));

    // on relie ces 4 nouveaux points 2 à 2
    Dart_handle dh29 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh24,2), lcc.beta(dh25,2));
    Dart_handle dh30 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh23,2), lcc.beta(dh27, 2));

}

// Fonction d'initialisation du mur du fond
Dart_handle immeuble::murFond(LCC& lcc, Dart_handle D1, Dart_handle D2, Dart_handle D3, Dart_handle D4){
    // On découpe le volume en deux pour créer le mur :
    std::vector<Dart_handle> path;

    path.push_back(lcc.beta(D1, 1, 1));
    path.push_back(lcc.beta(D3, 0,0,0,2,1,1,2,0));
    path.push_back(lcc.beta(D3, 0,0,0,2,1,1,2));
    path.push_back(lcc.beta(D3, 0,0,0,2,1,1,2,1));

    path.push_back(lcc.beta(D2, 1));
    path.push_back(lcc.beta(D4, 1, 1,2,1,1,2,0));
    path.push_back(lcc.beta(D4, 1, 1,2,1,1,2));
    path.push_back(lcc.beta(D4, 1, 1,2,1,1,2,1));

    Dart_handle dh7=lcc.insert_cell_2_in_cell_3(path.begin(),path.end());
    
    lcc.set_attribute<3>(dh7, lcc.create_attribute<3>());
    lcc.info<3>(dh7).type=MUR;
    int a = rand()%50+80;
    lcc.info<3>(dh7).color=CGAL::Color(a, a, a);

    return dh7;
}

// fonction d'initialisation du mur de devant
Dart_handle immeuble::murFace(LCC& lcc, Dart_handle D1, Dart_handle D2, Dart_handle D3, Dart_handle D4){
    // même principe que pour le mur du fond
    std::vector<Dart_handle> path;
    path.push_back(lcc.beta(D1,0,0,2, 3, 2,1,1,2));
    path.push_back(lcc.beta(D4,1));
    path.push_back(lcc.beta(D4,1,1));
    path.push_back(lcc.beta(D4,1,1,1));

    path.push_back(lcc.beta(D2, 1,2, 3, 2,1,1,2));
    path.push_back(lcc.beta(D3, 1,1));
    path.push_back(lcc.beta(D3, 1,1,1));
    path.push_back(lcc.beta(D3, 1,1,1,1));

    Dart_handle dh8=lcc.insert_cell_2_in_cell_3(path.begin(),path.end());

    lcc.set_attribute<3>(dh8, lcc.create_attribute<3>());
    lcc.info<3>(dh8).type=MUR;
    int a = rand()%50+80;
    lcc.info<3>(dh8).color=CGAL::Color(a, a, a);
    return dh8;
}

// Fonction pour distinguer le mur de gauche avec le reste
void immeuble::murGauche(LCC& lcc, Dart_handle D1, Dart_handle D2){

    // après avoir créé le mur, on a besoin de relier la face du haut avec la face du bas pour pouvoir créer le mur
    Dart_handle dh9 = lcc.insert_cell_1_in_cell_2(lcc.beta(D1,1,3), lcc.beta(D1, 0,0,3));

    Dart_handle dh11 = lcc.insert_cell_1_in_cell_2(lcc.beta(D2,1,3), lcc.beta(D2, 0,0,3));



    std::vector<Dart_handle> path;
    path.push_back(dh9);
    path.push_back(lcc.beta(dh9,1,2,1));
    path.push_back(dh11);
    path.push_back(lcc.beta(dh11, 1,2,1));

    Dart_handle dh13=lcc.insert_cell_2_in_cell_3(path.begin(),path.end());
    lcc.set_attribute<3>(dh13, lcc.create_attribute<3>());
    lcc.info<3>(dh13).type=MUR;
    int a = rand()%50+80;
    lcc.info<3>(dh13).color=CGAL::Color(a, a, a);
}

// Fonction pour distinguer le mur de droite avec le reste
void immeuble::murDroite(LCC& lcc, Dart_handle D1, Dart_handle D2){

    // même principe que pour murGauche
    Dart_handle dh10 = lcc.insert_cell_1_in_cell_2(lcc.beta(D1, 0,0,2,0,0,3), lcc.beta(D1,1,1,2,1,3));
    Dart_handle dh12 = lcc.insert_cell_1_in_cell_2( lcc.beta(D2, 0,0,2,0,0,3),lcc.beta(D2,1,1,2,1,3));

    std::vector<Dart_handle> path;
    path.push_back(dh10);
    path.push_back(lcc.beta(dh10,1,2,1));
    path.push_back(dh12);
    path.push_back(lcc.beta(dh12, 1,2,1));

    Dart_handle dh14=lcc.insert_cell_2_in_cell_3(path.begin(),path.end());

    lcc.set_attribute<3>(dh14, lcc.create_attribute<3>());
    lcc.info<3>(dh14).type=MUR;
    int a=rand()%50+80;
    lcc.info<3>(dh14).color=CGAL::Color(a, a, a);
}

// Fonction qu créé une fenêtre dans le mur de face
void immeuble::creerFenetreDevant(LCC& lcc, Dart_handle D){
    // Pour pouvoir appliquer la fonction à d'autres bâtiments, on a une méthode général pour récupérer les coordonnées des points sur lesquels on veut travailler
    Point p = lcc.point(D);
    Point p2 = lcc.point(lcc.other_extremity(D)); // Pour avoir l'extrémité du brin
    Point p3 = lcc.point(lcc.beta(D, 1, 1, 2, 1, 1, 2, 1, 1));
    int nx = p3.x() - p.x();
    float lx = (250 + (rand() % (150)));
    int lx2 = lx;
    lx = 0.35;
    float y = p2.y() +0.5;
    float z = p.z();
    Point p4 = lcc.point(lcc.other_extremity(lcc.beta(D, 2,1)));
    float lz = p4.z() - p.z();
    float x = p.x() +0.5;
    float ly = lx;
    std::cout<< " " << nx << " " << lx << " " << lz;

    Dart_handle dh10 = lcc. beta(D, 2, 0, 0, 2, 0);

    // Ici, on choisit la position de la porte en fonction de la taille et de la position de l'étage.
    // Rez de chaussée = on fait une porte
    if (p2.y()<1){
        if ((nx > 1.2)&&(p2.y()<1)){
            if (nx > 2.2){
                creerPorte(lcc, p.x()+2.2, p2.y(), p4.z(), dh10);

            }
            else {
                creerPorte(lcc, p.x()+1.2, p2.y(), p4.z(), dh10);

            }
        }
        else {
            creerPorte (lcc, p.x()+0.3, p2.y(), p4.z(), dh10);
        }
       }

    // ici on a la création de la fenêtre. Si on a déjà une porte et que la taille de la maison est de 1, on peut créer la fenêtre
    if ((nx > 1.2)|| (p2.y()>1 )){
        Dart_handle dh1=
            lcc.make_hexahedron(Point(x,y,z+lz),  Point(x+lx,y,z+lz), Point(x+lx,y,z), Point(x,y,z),
                           Point(x,y+ly,z), Point(x,y+ly,z+lz), Point(x+lx,y+ly,z+lz), Point(x+lx,y+ly,z));
        Dart_handle dh12 = lcc.insert_cell_1_in_cell_2(lcc.beta(D,2,1,1,2), dh1);
        Dart_handle dh13 = lcc.make_combinatorial_polygon(4);
        lcc.sew<3>(lcc.beta(dh1, 2,1,1,2), dh13);
        Dart_handle dh14 = lcc.insert_cell_1_in_cell_2(lcc.beta(D,1,1,2,1), lcc.beta(dh1, 2,1,1,2,1));

//on met un attribut sur les deux brins qui ont servi à créer la fenêtre pour pouvoir ne pas les afficher
                    lcc.template set_attribute<1>(dh12, lcc.template create_attribute<1>());
                    lcc.template info<1>(dh12).type=FENETRE;
                    lcc.template set_attribute<1>(dh14, lcc.template create_attribute<1>());
                    lcc.template info<1>(dh14).type=FENETRE;
    }

}

// fonction de création d'une porte
void immeuble::creerPorte(LCC& lcc, float x, float y, float z, Dart_handle D){

    // en premier lieu, on dessine la porte des deux côtés du mur
    Dart_handle dh6 = lcc.beta(D, 2, 1, 1, 1, 2);


    Dart_handle dh1 = lcc.insert_point_in_cell<1>(D, Point(x+0.3, y, z));
    Dart_handle dh2 = lcc.insert_point_in_cell<1>(dh1, Point(x, y, z));

    Dart_handle dh3 = lcc.insert_cell_1_in_cell_2(dh1, dh2);
    Dart_handle dh4 = lcc.insert_point_in_cell<1>(dh3, Point(x, y + 0.8, z));
    Dart_handle dh5 = lcc.insert_point_in_cell<1>(dh4, Point(x + 0.3, y + 0.8, z));


    z = z - 0.2;
    Dart_handle dh7 = lcc.insert_point_in_cell<1>(dh6, Point(x, y, z));
    Dart_handle dh8 = lcc.insert_point_in_cell<1>(dh7, Point(x+0.3, y, z));

    Dart_handle dh9 = lcc.insert_cell_1_in_cell_2(dh7, dh8);
    Dart_handle dh10 = lcc.insert_point_in_cell<1>(dh9, Point(x+0.3, y + 0.8, z));
    Dart_handle dh11 = lcc.insert_point_in_cell<1>(dh10, Point(x , y + 0.8, z));

    Dart_handle dh12 = lcc.insert_cell_1_in_cell_2(lcc.beta (dh7, 2), lcc.beta(D, 2));
    Dart_handle dh13 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh1, 2), lcc.beta(dh6, 2));
    // ensuite, on créé une surface qui va entourer la porte
    std::vector<Dart_handle> path;
    path.push_back(dh3);
    path.push_back(dh4);
    path.push_back(dh5);
    path.push_back(dh12);

    path.push_back(dh9);
    path.push_back(dh10);
    path.push_back(dh11);
    path.push_back(dh13);

    Dart_handle dh20=lcc.insert_cell_2_in_cell_3(path.begin(),path.end());

    lcc.set_attribute<2>(dh4, lcc.create_attribute<2>());
    lcc.info<2>(dh4).type=PORTE;

    // ici, on relie le haut de la porte
    Dart_handle dh14 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh4, 2), lcc.beta(dh9,2));
    Dart_handle dh15 = lcc.insert_cell_1_in_cell_2(lcc.beta(dh10,2), lcc.beta(dh3,2));

    // Le bout de code qui suit sert à placer un cube dans la fenêtre

    // Dart_handle dh16 = lcc.make_combinatorial_polygon(4);
    // lcc.sew<3>(dh10, dh16);

    // Dart_handle dh14 = lcc.make_combinatorial_hexahedron();
    // lcc.sew<3>(dh5, dh14);  // face avant
    // lcc.sew<3>(dh9, lcc.beta(dh14, 2,1 , 1, 2)); // derrière
    // lcc.sew<3>(dh12, lcc.beta(dh14, 0, 2, 0)); // dessous
}

//créé 6 surfaces d'un parallélépipède rectangle de coordonnées (x,y,z) et de longueur lx et lz, et 1 en hauteur

// Fonction qui créé un étage complet d'un immeuble
std::vector<Dart_handle> immeuble::etage (float x, float y, float z, float lx, float lz, LCC& lcc) {
    My_linear_cell_complex_incremental_builder_3<LCC> ib(lcc);
    //8 sommets d'un cube
    ib.add_vertex(Point(x , y , z));
    ib.add_vertex(Point(x , y+1 , z));
    ib.add_vertex(Point(x , y , z+lz));
    ib.add_vertex(Point(x , y+1 , z+lz));
    ib.add_vertex(Point(x+lx , y , z));
    ib.add_vertex(Point(x+lx , y+1 , z));
    ib.add_vertex(Point(x+lx , y , z+lz));
    ib.add_vertex(Point(x+lx , y+1 , z+lz));
    ib.begin_surface();
    //on créé les faces du cube
    Dart_handle dh1 = ib.add_facet({0,1,3,2}); // quand on appelle dh1, on est sur l'arrête (0, 1). arrête au même endroit pour les autres dh.
    Dart_handle dh2 = ib.add_facet({4,6,7,5});
    Dart_handle dh3 = ib.add_facet({1,0,4,5});
    Dart_handle dh4 = ib.add_facet({6,2,3,7});
    Dart_handle dh5 = ib.add_facet({2,6,4,0});
    Dart_handle dh6 = ib.add_facet({3,1,5,7});
    ib.end_surface();

    // lcc.set_attribute<3>(dh1, lcc.create_attribute<3>());
    // lcc.info<3>(dh1).type=MUR;
    // int a = rand()%50+80;
    // lcc.info<3>(dh1).color=CGAL::Color(a, a, a);
    std::vector<Dart_handle> tab = {dh1, dh2, dh3, dh4, dh5, dh6};
    return tab;
}

// Fonction qui retourne un immeuble
Dart_handle immeuble::structImmeuble(int etg, float x, float z, float lx, float lz, LCC& lcc) {
    Dart_handle brinretourne;
    Dart_handle brintemp;
    std::vector<std::vector<Dart_handle>> tab(11,std::vector<Dart_handle>(6,NULL));
    std::cout<<"ON RENTRE \n";
    // On fait ça pour chaque étage de l'immeuble
    for (int j=0; j<etg; j++) {
        
        std::vector<Dart_handle> t = etage (x, .2+j*1.2, z, lx, lz, lcc);

        Dart_handle dh1 = t[0];
        Dart_handle dh2 = t[1];
        Dart_handle dh3 = t[2];
        Dart_handle dh4 = t[3];
        Dart_handle dh5 = t[4];
        Dart_handle dh6 = t[5];

        tab[j][0]=dh1;
        tab[j][1]=dh2;
        tab[j][2]=dh3;
        tab[j][3]=dh4;
        tab[j][4]=dh5;
        tab[j][5]=dh6;
        
        Dart_handle dh0 = plancher(x, j*1.2, z, lx, lz, lcc);
        Dart_handle roof;
        // On est en haut de l'immeuble, on mets un toit
        if (j==etg-1) {
            roof = toit(x, (j+1)*1.2, z, lx, .5, lz, lcc);
            lcc.set_attribute<3>(roof, lcc.create_attribute<3>());
            lcc.info<3>(roof).type=TOIT;
            lcc.info<3>(roof).color=CGAL::Color(rand()%128, 0, 0);
        }
        
        lcc.sew<3>(lcc.beta(dh0, 2, 1, 1, 2), dh5);
        if (j==0) { //cas du rez de chaussée, on doit insérer des points sur la face inférieure du plancher pour pouvoir le coudre à la grille
                    //on insère des points à chaque intersection de la grille (1 unité)
            Dart_handle dh02 =lcc.beta(dh0, 1);
            Dart_handle dh03 =lcc.beta(dh02, 1);
            Dart_handle dh01 =lcc.beta(dh03, 1);
            for (int i=1; i<lz; i++) {
                lcc.insert_point_in_cell<1>(dh01, Point(x, 0, z+lz-i));
            }
            for (int i=1; i<lx; i++) {
                lcc.insert_point_in_cell<1>(dh0, Point(x+lx-i, 0, z+lz));
            }
            for (int i=1; i<lz; i++) {
                lcc.insert_point_in_cell<1>(dh02, Point(x+lx, 0, z+i));
            }
            for (int i=1; i<lx; i++) {
                lcc.insert_point_in_cell<1>(dh03, Point(x+i, 0, z));
            }
            brinretourne = dh01;
        }
        else if (j==etg-1) { //cas du dernier étage, on doit coudre la face supérieur à un toit
            lcc.sew<3>(tab[etg-1][5], roof);
        }
        else lcc.sew<3>(lcc.beta(brintemp, 0), dh0);

        brintemp = dh6;
    }
    // On créé ensuite chaque étage en ajoutant murs, portes et fenêtres
    for (int j=0; j<etg; j++) {
        Dart_handle dh1 = tab[j][0];
        Dart_handle dh2 = tab[j][1];
        Dart_handle dh3 = tab[j][2];
        Dart_handle dh4 = tab[j][3];
        Dart_handle dh5 = tab[j][4];
        Dart_handle dh6 = tab[j][5];
        Bas(lcc, dh5);
        Haut(lcc, dh6);

        murCote (lcc, lcc.beta(dh1, 0, 0, 0));
        murCote (lcc, dh2);

        Dart_handle dh7 = murFond (lcc, dh1, dh2, dh5, dh6);
        Dart_handle dh8 = murFace (lcc, dh1, dh2, dh5, dh6);

        murGauche(lcc, dh7, dh8);
        murDroite(lcc, dh7, dh8);

        creerFenetreDevant(lcc, dh8);
        std::cout<<j << "\n";
    }
    return brinretourne;
}

// Fonction qui retourne une maison
Dart_handle immeuble::structMaison(float x, float y, float z, float lx, float lz, LCC& lcc) {

    // même principe que pour structImmeuble, mais on le fait qu'une seule fois.
    std::vector<Dart_handle> tab = etage (x, y+.2, z, lx, lz, lcc);
    Dart_handle dh1 = tab[0];
    Dart_handle dh2 = tab[1];
    Dart_handle dh3 = tab[2];
    Dart_handle dh4 = tab[3];
    Dart_handle dh5 = tab[4];
    Dart_handle dh6 = tab[5];


    Dart_handle dh0 = plancher(x, y, z, lx, lz, lcc);
    Dart_handle roof = toit(x, y+1.2, z, lx, .5, lz, lcc);
    lcc.set_attribute<3>(roof, lcc.create_attribute<3>());
    lcc.info<3>(roof).type=TOIT;
    lcc.info<3>(roof).color=CGAL::Color(rand()%128, 0, 0);

    lcc.sew<3>(lcc.beta(dh6, 1), roof);
    lcc.sew<3>(lcc.beta(dh0, 2, 1, 1, 2), dh5);

    Dart_handle dh02 =lcc.beta(dh0, 1);
    Dart_handle dh03 =lcc.beta(dh02, 1);
    Dart_handle dh01 =lcc.beta(dh03, 1);
    for (int i=1; i<lz; i++) {
        lcc.insert_point_in_cell<1>(dh01, Point(x, y, z+lz-i));
        std::cout<<i <<" \n";
    }
    for (int i=1; i<lx; i++) {
        lcc.insert_point_in_cell<1>(dh0, Point(x+lx-i, y, z+lz));
        std::cout<<i <<" \n";
    }
    for (int i=1; i<lz; i++) {
        lcc.insert_point_in_cell<1>(dh02, Point(x+lx, y, z+i));
        std::cout<<i <<" \n";
    }
    for (int i=1; i<lx; i++) {
        lcc.insert_point_in_cell<1>(dh03, Point(x+i, y, z));
        std::cout<<i <<" \n";
    } 


    Bas(lcc, dh5);
    Haut(lcc, dh6);

    murCote (lcc, lcc.beta(dh1, 0, 0, 0));
    murCote (lcc, dh2);

    Dart_handle dh7 = murFond (lcc, dh1, dh2, dh5, dh6);

    Dart_handle dh8 = murFace (lcc, dh1, dh2, dh5, dh6);

    murGauche(lcc, dh7, dh8);
    murDroite(lcc, dh7, dh8);

    creerFenetreDevant(lcc, dh8);
    return dh01;
}

// Fonction qui créé le plancher
Dart_handle immeuble::plancher(float x, float y, float z, float lx, float lz, LCC& lcc) {
    My_linear_cell_complex_incremental_builder_3<LCC> ib(lcc);
    //8 sommets d'un cube
    ib.add_vertex(Point(x , y , z));
    ib.add_vertex(Point(x , y+.2 , z));
    ib.add_vertex(Point(x , y , z+lz));
    ib.add_vertex(Point(x , y+.2 , z+lz));
    ib.add_vertex(Point(x+lx , y , z));
    ib.add_vertex(Point(x+lx , y+.2 , z));
    ib.add_vertex(Point(x+lx , y , z+lz));
    ib.add_vertex(Point(x+lx , y+.2 , z+lz));
    ib.begin_surface();
    //on créé les faces du cube
    ib.add_facet({3,2,0,1}); // quand on appelle dh1, on est sur l'arrête (0, 1). arrête au même endroit pour les autres dh.
    ib.add_facet({7,5,4,6});
    ib.add_facet({4,5,1,0});
    ib.add_facet({6,2,3,7});
    Dart_handle dh1 = ib.add_facet({2,6,4,0});
    ib.add_facet({3,1,5,7});
    ib.end_surface();

    lcc.set_attribute<3>(dh1, lcc.create_attribute<3>());
    lcc.info<3>(dh1).type=SOL;
    lcc.info<3>(dh1).color=CGAL::blue();
    return dh1;
}


//Créé les 4 pentes d'un toit de type pyramide aux coordonnées (x,y,z), de longueur lx, lz, et de hauteur à la pointe ly

Dart_handle immeuble::toit (float x, float y, float z, float lx, float ly, float lz, LCC& lcc) {
    My_linear_cell_complex_incremental_builder_3<LCC> ib(lcc);
    //4 sommets de la base d'une pyramide
    ib.add_vertex(Point(x , y , z));
    ib.add_vertex(Point(x , y , z+lz));
    ib.add_vertex(Point(x+lx , y , z));
    ib.add_vertex(Point(x+lx , y , z+lz));
    //sommet de la pyramide
    ib.add_vertex(Point(x+lx/2 , y+ly , z+lz/2));

    //création des faces de la pyramide
    ib.add_facet({0,1,4});
    ib.add_facet({2,0,4});
    ib.add_facet({3,2,4});
    ib.add_facet({1,3,4});
    ib.add_facet({1,0,2,3});
    //Dart_handle dh2 = lcc.insert_point_in_cell<1>(dh, Point(x, y, z + .2));
    return ib.end_surface();

}
