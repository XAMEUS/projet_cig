# projet_cig
Projet C - Interfaces Graphiques

Pierre Bouvier, Maxime Gourgoulhon, Julie Saouli

## Utilisation :

Pour tout compiler :
```
make
```

Pour avoir la documentation :
```
make doc
```

## Tests

### minimal.c
Il s'agit juste d'une simple interface vide avec un fond blanc.

### lines.c
Teste les primitives de base de dessin : `ei_fill`, `ei_text`, `ei_polyline`, `ei_polygon`. Nous avons ajouté les tests de dessin de point, de cercle, de button et de radiobutton.

### frame.c
C'est un test du placer (positionnement relatif, ancre). Une frame principale contenant 9 frames, chacune à une position particulière (nw, n, ne, w, c, e, sw, s, se). Il a permit une première analyse des performances en forçant le redessin de toute l'interface.

### button.c
Affiche un simple bouton cliquable en relief et affiche le message "Click!" lorsqu'on qu'il est cliqué.

### hello_world
Un `top_level` contenant un bouton, de taille relative, ancré en bas à droite.

### top_level_ception_simple
Test de toplevels simple avec deux toplevel imbriquées et un bouton.

### top_level_ception_position
Test de toplevels et de position de boutons relatif.

### top_level_ception_advanced
Test de toplevel avec deux enfants toplevels.

### puzzle
Super puzzle avec de jolies images.

### two048
Super jeu très amusant.

### test_new_class
Super extension dont on a pas le code qui ne marche pas (mais c'est de notre faute, hein :p), nous avons essayé sur un exemple minimal, le `register` fonctionne, tout est bien "linké" mais la `drawfunc` de testclass ne dessine rien.

## Extensions

### text.c
Une extension qui permet de dessiner du texte en gérant les `\n` et en allant automatiquement à la ligne pour continuer la suite du texte.
À utiliser ensuite par des widgets.

### radiobutton.c
Un (début) d'extension permettant d'avoir des radiobutton.
Le système de dessin fonctionne parfaitement avec de "magnifiques" losanges,
une couleur de fond plus foncée pour celui selectionné, normaux avec une bordure en relief pour les autres.
L'implémentation n'est pas complètement finie.

## Utilisation avancée

On peut utiliser `#include "ei_debug.h"` dans le fichier `ei_application.c` et modifier le Makefile en passant `debug` à `true` pour affichier les fps.
N.B. : cela fait une boucle infinie d'affichage -> pas de capture d'évènements.
