# Commande pipex à tester
./pipex infile "grep -i hello" "wc -l" outfile
# Affichage du résultat pipex
echo "Résultat pipex :"
cat outfile
# Affichage de la commande shell originale pour comparaison
echo "Résultat shell :"
grep -i hello infile | wc -l
# Test mémoire avec valgrind (analyse complète y compris enfants)
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "grep -i hello" "wc -l" outfile

# Test 1 : commande inexistante (erreur)
./pipex infile "notacommand" "wc -l" outfile
echo "Résultat pipex :"
cat outfile 2>/dev/null || echo "Pas de fichier généré"
echo "Résultat shell :"
notacommand infile | wc -l 2>/dev/null || echo "Commande inexistante"
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "notacommand" "wc -l" outfile

# Test 2 : grep sans résultat
./pipex infile "grep foobar" "wc -l" outfile
echo "Résultat pipex :"
cat outfile
echo "Résultat shell :"
grep foobar infile | wc -l
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "grep foobar" "wc -l" outfile

# Test 3 : commandes lentes (sleep), test attente sans crash
./pipex infile "sleep 1" "sleep 1" outfile
echo "Résultat pipex :"
cat outfile 2>/dev/null || echo "Pas de sortie attendue"
echo "Résultat shell :"
sleep 1 < infile | sleep 1
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "sleep 1" "sleep 1" outfile

# Test 4 : grep -v et sed substitution
./pipex infile "grep -v hello" "sed s/test/TEST/g" outfile
echo "Résultat pipex :"
cat outfile
echo "Résultat shell :"
grep -v hello infile | sed s/test/TEST/g
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "grep -v hello" "sed s/test/TEST/g" outfile

# Test 5 : tr majuscule puis tr minuscule
./pipex infile "tr a-z A-Z" "tr A-Z a-z" outfile
echo "Résultat pipex :"
cat outfile
echo "Résultat shell :"
tr a-z A-Z < infile | tr A-Z a-z
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "tr a-z A-Z" "tr A-Z a-z" outfile

# Test 6 : commandes avec options multiples
./pipex infile "awk '{ print \$1 }'" "sort | uniq" outfile
echo "Résultat pipex :"
cat outfile
echo "Résultat shell :"
awk '{ print $1 }' infile | sort | uniq
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "awk '{ print \$1 }'" "sort" outfile

# Test 7 : commande avec redirection de fichier dans commande (doit échouer)
./pipex infile "cat infile" "wc -l" outfile
echo "Résultat pipex :"
cat outfile
echo "Résultat shell :"
cat infile infile | wc -l
echo "Lancement valgrind :"
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./pipex infile "cat infile
