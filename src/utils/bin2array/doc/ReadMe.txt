bin2array : binary to C array converter

Utilit� : ouvre n'importe quel fichier et le transforme en tableau d'octets qui est
ensuite plac� dans un fichier header C/C++ avec protection du header.
C'est un moyen de mettre une ressource binaire (image, dll) directement dans l'ex�cutable final, de cette fa�on on ne
distribue au client qu'un unique fichier au lieu d'utiliser un installateur, etc.

1er parametre : nom du fichier � transformer en tableau d'octets
2eme parametre : nom du header dans lequel placer le tableau d'octets
3eme parametre : nom du tableau C/C++ afin de donner une signification � la ressource
