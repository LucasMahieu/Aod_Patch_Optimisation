# TP Algorithmique et Optimisation Discrète

En informatique, un patch est une suite d’instructions de transformation appliquées à un flot en entrée (par exemple un fichier) et dont le résultat est écrit sur le flot de sortie. Le flot est ici traité ligne par ligne, de la première à la dernière1. Une ligne se termine par le caractère NEWLINE (\n). Chaque ligne a un numéro à partir de 1 pour la première ligne ; le début du fichier (avant la première ligne) correspond par convention à une ligne virtuelle de numéro 0.

Un patch est une suite d’instructions, chacune écrite sur une ou deux lignes. La première ligne d’une ins- truction de patch est composée d’un caractère définissant l’instruction (+, =, d ou D), suivi d’un caractère espace puis d’un entier k Ø 0 indiquant le numéro de la ligne du flot d’entrée a ectée par l’instruction. Cette première ligne de l’instruction peut être :

“+ k\n” Ajout : la ligne suivante dans le patch est ajoutée sur le flot de sortie après la ligne k du flot d’entrée. Si k = 0, la ligne est insérée avant la première ligne (numérotée 1) du flot d’entrée.

“= k\n” Substitution : la ligne k du flot d’entrée est remplacée sur le flot de sortie par la ligne suivante dans le patch.

“d k\n” Destruction : la ligne k du flot d’entrée n’est pas recopiée sur le flot de sortie.

“D k m\n” Multi-destruction : les m lignes de la ligne k à la ligne k + m ≠ 1 du flot d’entrée ne sont pas recopiées
sur le flot de sortie.

Le fonctionnement est le suivant :
  
  — Toutes les lignes du flot d’entrée qui ne sont pas détruites et dont le numéro n’apparait pas dans une instruction de substitution sont recopiées sur le flot de sortie.
  
  — Les numéros de lignes k dans les instructions du patch apparaissent par ordre croissant.
  
  — Lesinstructionssontexécutéesdansl’ordredudébutàlafin;ainsideuxcommandesd’ajoutconsécutives avec un même numéro de ligne entraînent l’ajout consécutif des deux lignes opérandes dans l’ordre sur la sortie.
  
  — Uneinstructiondesubstitutionaunnumérodelignestrictementsupérieuràl’instructionquilaprécède.
  
  — Toutes les instructions qui suivent une instruction de destruction "d k" (resp. "D k m") ont un numéro de ligne supérieur ou égal à k + 1 (resp. k + m). De plus, une instruction de destruction (d ou D) ne peut pas porter sur la même ligne qu’une instruction de substitution.
